const fs = require("fs");
const path = require("path");
const { spawnSync } = require("child_process");

const basedir = path.resolve(__dirname, "..", "..");
const patchPath = path.join(basedir, "chinese-copywriting.patch");
const summaryPath = process.env.GITHUB_STEP_SUMMARY;
const args = new Set(process.argv.slice(2));
const spacingChar = /[\p{Script=Han}\p{Script=Hiragana}\p{Script=Katakana}\p{Script=Hangul}\p{Script=Latin}\p{Number}]/u;
const inlineToken = /(!?)\[([^\]\n]+)\]\(([^)\n]+)\)|(!?)\[([^\]\n]+)\]\[([^\]\n]+)\]|\*\*[^*\n]+?\*\*|__[^_\n]+?__|\*[^*\n]+?\*|<https?:\/\/[^>\n]+>/g;

if (args.size === 0 || args.has("--help")) {
  console.log(`Usage: node tests/tools/check_chinese_copywriting.js [--check|--fix|--patch]

Options:
  --check  Check README.zh-cn.md files and report GitHub annotations in CI.
  --fix    Rewrite README.zh-cn.md files with safe automatic fixes.
  --patch  Generate chinese-copywriting.patch without keeping file changes.`);
  process.exit(args.has("--help") ? 0 : 1);
}

const modes = ["--check", "--fix", "--patch"].filter((mode) => args.has(mode));
if (modes.length !== 1) {
  console.error("Choose exactly one mode: --check, --fix, or --patch.");
  process.exit(1);
}

function collectReadmes(dir) {
  const entries = fs.readdirSync(dir, { withFileTypes: true });
  const files = [];

  for (const entry of entries) {
    if (
      entry.name === ".git" ||
      entry.name === ".pytest_cache" ||
      entry.name === "node_modules"
    ) {
      continue;
    }

    const fullpath = path.join(dir, entry.name);
    if (entry.isDirectory()) {
      files.push(...collectReadmes(fullpath));
    } else if (entry.isFile() && entry.name === "README.zh-cn.md") {
      files.push(fullpath);
    }
  }

  return files.sort();
}

function run(command, commandArgs, options = {}) {
  return spawnSync(command, commandArgs, {
    cwd: basedir,
    encoding: "utf8",
    maxBuffer: 1024 * 1024 * 64,
    ...options,
  });
}

function runForStatus(command, commandArgs, options = {}) {
  return spawnSync(command, commandArgs, {
    cwd: basedir,
    encoding: "utf8",
    maxBuffer: 1024 * 1024 * 64,
    ...options,
  });
}

function relative(file) {
  return path.relative(basedir, file).split(path.sep).join("/");
}

function escapeWorkflow(value) {
  return String(value)
    .replace(/%/g, "%25")
    .replace(/\r/g, "%0D")
    .replace(/\n/g, "%0A");
}

function escapeAnnotationProperty(value) {
  return escapeWorkflow(value).replace(/,/g, "%2C").replace(/:/g, "%3A");
}

function truncate(value, maxLength = 220) {
  const text = String(value).replace(/\s+/g, " ").trim();
  if (text.length <= maxLength) return text;
  return `${text.slice(0, maxLength - 3)}...`;
}

function markdownEscape(value) {
  return String(value).replace(/\\/g, "\\\\").replace(/`/g, "\\`");
}

function markdownCode(value) {
  const text = String(value);
  const maxBackticks = Math.max(0, ...[...text.matchAll(/`+/g)].map((match) => match[0].length));
  const marker = "`".repeat(maxBackticks + 1);
  const needsPadding = text.startsWith("`") || text.endsWith("`");
  const padding = needsPadding ? " " : "";
  return `${marker}${padding}${text}${padding}${marker}`;
}

function parseJson(stdout) {
  const start = stdout.indexOf("{");
  const end = stdout.lastIndexOf("}");
  if (start === -1 || end === -1 || end < start) {
    throw new Error("AutoCorrect did not return JSON output.");
  }

  return JSON.parse(stdout.slice(start, end + 1));
}

function flattenMessages(result) {
  const problems = [];
  for (const message of result.messages || []) {
    for (const line of message.lines || []) {
      problems.push({
        file: message.filepath.replace(/^\.\//, ""),
        line: line.l || 1,
        column: line.c || 1,
        message: "中文排版建议",
        oldText: line.old || "",
        newText: line.new || "",
        fixable: true,
        source: "autocorrect",
        severity: line.severity || 1,
      });
    }
  }
  return problems;
}

function codeSpanRanges(line) {
  const ranges = [];
  let index = 0;

  while (index < line.length) {
    const start = line.indexOf("`", index);
    if (start === -1) break;

    let markerEnd = start + 1;
    while (markerEnd < line.length && line[markerEnd] === "`") markerEnd++;

    const marker = line.slice(start, markerEnd);
    const end = line.indexOf(marker, markerEnd);
    if (end === -1) {
      ranges.push([start, line.length, false]);
      break;
    }

    ranges.push([start, end + marker.length, true]);
    index = end + marker.length;
  }

  return ranges;
}

function inRanges(position, ranges) {
  return ranges.some(([start, end]) => position >= start && position < end);
}

function applyInsertions(line, insertions) {
  let fixed = line;
  for (const position of [...insertions].sort((a, b) => b - a)) {
    fixed = `${fixed.slice(0, position)} ${fixed.slice(position)}`;
  }
  return fixed;
}

function literalBacktickFix(line) {
  return line.replace(/`{5}/g, "`` ` ``");
}

function inlineSpacingFix(line, codeRanges) {
  const insertions = new Set();

  for (const [start, end, closed] of codeRanges) {
    if (!closed) continue;

    const prev = start > 0 ? line[start - 1] : "";
    const next = end < line.length ? line[end] : "";

    if (prev && spacingChar.test(prev)) insertions.add(start);
    if (next && spacingChar.test(next)) insertions.add(end);
  }

  inlineToken.lastIndex = 0;
  let match;
  while ((match = inlineToken.exec(line)) !== null) {
    if (inRanges(match.index, codeRanges)) continue;
    if (match[1] === "!" || match[4] === "!") continue;

    const start = match.index;
    const end = start + match[0].length;
    const prev = start > 0 ? line[start - 1] : "";
    const next = end < line.length ? line[end] : "";

    if (prev && spacingChar.test(prev)) insertions.add(start);
    if (next && spacingChar.test(next)) insertions.add(end);
  }

  if (insertions.size === 0) return null;
  return applyInsertions(line, insertions);
}

function checkMarkdownInlineSpans(files) {
  const problems = [];

  for (const file of files) {
    const relpath = relative(file);
    const lines = fs.readFileSync(file, "utf8").split(/\n/);
    let inFence = false;

    lines.forEach((line, index) => {
      if (/^\s*(```|~~~)/.test(line)) {
        inFence = !inFence;
        return;
      }
      if (inFence) return;

      const literalBacktickFixed = literalBacktickFix(line);
      if (literalBacktickFixed !== line) {
        const fixedCodeRanges = codeSpanRanges(literalBacktickFixed);
        const fixed = inlineSpacingFix(literalBacktickFixed, fixedCodeRanges) || literalBacktickFixed;
        problems.push({
          file: relpath,
          line: index + 1,
          column: line.indexOf("`````") + 1,
          message: "连续反引号应改写为 Markdown 行内反引号字面量",
          oldText: line,
          newText: fixed,
          fixable: true,
          source: "markdown-inline",
        });
        return;
      }

      const codeRanges = codeSpanRanges(line);
      const unclosed = codeRanges.find(([, , closed]) => !closed);
      if (unclosed) {
        problems.push({
          file: relpath,
          line: index + 1,
          column: unclosed[0] + 1,
          message: "请闭合 Markdown 行内代码片段，或改写文本中的反引号",
          oldText: line.slice(unclosed[0]),
          newText: "闭合行内代码片段，或将文本中的反引号改写为普通说明",
          fixable: false,
          source: "markdown-inline",
        });
        return;
      }

      const fixed = inlineSpacingFix(line, codeRanges);
      if (fixed && fixed !== line) {
        problems.push({
          file: relpath,
          line: index + 1,
          column: 1,
          message: "Markdown 行内元素两侧需要添加空格",
          oldText: line,
          newText: fixed,
          fixable: true,
          source: "markdown-inline",
        });
      }
    });
  }

  return problems;
}

function fixMarkdownInlineSpans(files) {
  let changed = false;

  for (const file of files) {
    const lines = fs.readFileSync(file, "utf8").split(/\n/);
    let inFence = false;
    let fileChanged = false;
    const fixedLines = lines.map((line) => {
      if (/^\s*(```|~~~)/.test(line)) {
        inFence = !inFence;
        return line;
      }
      if (inFence) return line;

      const literalBacktickFixed = literalBacktickFix(line);
      if (literalBacktickFixed !== line) {
        const fixedCodeRanges = codeSpanRanges(literalBacktickFixed);
        const fixed = inlineSpacingFix(literalBacktickFixed, fixedCodeRanges) || literalBacktickFixed;
        fileChanged = true;
        return fixed;
      }

      const codeRanges = codeSpanRanges(line);
      if (codeRanges.some(([, , closed]) => !closed)) return line;

      const fixed = inlineSpacingFix(line, codeRanges);
      if (fixed && fixed !== line) {
        fileChanged = true;
        return fixed;
      }
      return line;
    });

    if (fileChanged) {
      changed = true;
      fs.writeFileSync(file, fixedLines.join("\n"));
    }
  }

  return changed;
}

function writeSummary(problems) {
  if (!summaryPath) return;

  const files = new Map();
  const manualCount = problems.filter((problem) => !problem.fixable).length;
  for (const problem of problems) {
    files.set(problem.file, (files.get(problem.file) || 0) + 1);
  }

  const lines = [
    "## Chinese Copywriting Check",
    "",
    `Found ${problems.length} Chinese copywriting issue${problems.length === 1 ? "" : "s"} in ${files.size} file${files.size === 1 ? "" : "s"}.`,
    "",
    "Run this locally to fix the issues:",
    "",
    "```bash",
    "node tests/tools/check_chinese_copywriting.js --fix",
    "```",
    "",
    "In CI, download `chinese-copywriting.patch` and apply it with `git apply chinese-copywriting.patch`.",
    manualCount > 0 ? `${manualCount} issue${manualCount === 1 ? "" : "s"} require manual edits because they are not safe to rewrite automatically.` : "",
    "",
    "The check follows `.autocorrectrc` and [sparanoid/chinese-copywriting-guidelines](https://github.com/sparanoid/chinese-copywriting-guidelines).",
    "",
    "| File | Issues |",
    "| --- | ---: |",
  ];

  for (const [file, count] of [...files.entries()].sort((a, b) => a[0].localeCompare(b[0]))) {
    lines.push(`| \`${markdownEscape(file)}\` | ${count} |`);
  }

  const sample = problems.slice(0, 20);
  if (sample.length > 0) {
    lines.push("", "### Suggested Changes", "");
    for (const problem of sample) {
      lines.push(
        `- \`${markdownEscape(problem.file)}:${problem.line}:${problem.column}\``,
        `  - Current: ${markdownCode(truncate(problem.oldText))}`,
        `  - Suggested: ${markdownCode(truncate(problem.newText))}`,
      );
    }
  }

  if (problems.length > sample.length) {
    lines.push("", `Showing ${sample.length} of ${problems.length} issues. See annotations for the full list.`);
  }

  fs.appendFileSync(summaryPath, `${lines.join("\n")}\n`);
}

function reportAnnotations(problems) {
  for (const problem of problems) {
    const title = escapeAnnotationProperty(problem.message || "中文文档规范建议");
    const file = escapeAnnotationProperty(problem.file);
    const message = escapeWorkflow(
      `${problem.message || "中文文档规范建议"}：${truncate(problem.oldText)} -> ${truncate(problem.newText)}`,
    );
    console.error(
      `::error file=${file},line=${problem.line},col=${problem.column},title=${title}::${message}`,
    );
  }
}

function ensureAutocorrect() {
  const result = run("autocorrect", ["--version"]);
  if (result.status !== 0) {
    console.error("AutoCorrect is required. Install it or run the GitHub Action installer first.");
    process.exit(127);
  }
}

function check(files) {
  const result = run("autocorrect", [
    "--lint",
    "--format",
    "json",
    "--no-diff-bg-color",
    ...files.map(relative),
  ]);

  if (result.error && result.error.code === "ENOENT") {
    console.error("AutoCorrect is required. Install it or run the GitHub Action installer first.");
    process.exit(127);
  }

  let problems;
  try {
    problems = flattenMessages(parseJson(result.stdout));
  } catch (error) {
    if (result.status === 0) {
      console.error(result.stdout);
      console.error(error.message);
      return 1;
    }

    console.error(result.stdout);
    console.error(result.stderr);
    console.error(error.message);
    return result.status || 1;
  }
  problems.push(...checkMarkdownInlineSpans(files));
  problems.sort((a, b) => (
    a.file.localeCompare(b.file) ||
    a.line - b.line ||
    a.column - b.column
  ));

  if (problems.length === 0) {
    if (summaryPath) {
      fs.appendFileSync(
        summaryPath,
        [
          "## Chinese Copywriting Check",
          "",
          "No Chinese copywriting issues found.",
          "",
        ].join("\n"),
      );
    }
    return 0;
  }

  reportAnnotations(problems);
  writeSummary(problems);
  console.error(
    `Chinese copywriting check failed: ${problems.length} issue${problems.length === 1 ? "" : "s"} found.`,
  );
  console.error("Run: node tests/tools/check_chinese_copywriting.js --fix");
  return 1;
}

function fix(files) {
  const result = run("autocorrect", ["--fix", "--quiet", ...files.map(relative)], { stdio: "inherit" });
  if (result.status !== 0) return result.status || 1;
  fixMarkdownInlineSpans(files);
  return 0;
}

function patch(files) {
  if (fs.existsSync(patchPath)) fs.unlinkSync(patchPath);

  const originals = new Map(files.map((file) => [file, fs.readFileSync(file)]));
  const fixStatus = fix(files);
  if (fixStatus !== 0) {
    for (const [file, content] of originals) fs.writeFileSync(file, content);
    return fixStatus;
  }

  let patch = "";
  for (const file of files) {
    const before = originals.get(file);
    const after = fs.readFileSync(file);
    if (Buffer.compare(before, after) === 0) continue;

    const relpath = relative(file);
    const tmpdir = fs.mkdtempSync(path.join(require("os").tmpdir(), "copywriting-"));
    const beforePath = path.join(tmpdir, "before");
    const afterPath = path.join(tmpdir, "after");
    fs.writeFileSync(beforePath, before);
    fs.writeFileSync(afterPath, after);

    const result = runForStatus("diff", [
      "-u",
      "--label",
      `a/${relpath}`,
      "--label",
      `b/${relpath}`,
      beforePath,
      afterPath,
    ]);
    fs.rmSync(tmpdir, { recursive: true, force: true });

    if (result.status !== 1) {
      for (const [restoreFile, content] of originals) fs.writeFileSync(restoreFile, content);
      console.error(result.stderr || result.stdout || `Failed to generate patch for ${relpath}.`);
      return result.status || 1;
    }

    patch += result.stdout;
  }

  for (const [file, content] of originals) fs.writeFileSync(file, content);

  if (patch.trim().length === 0) {
    console.log("No Chinese copywriting patch needed.");
  } else {
    fs.writeFileSync(patchPath, patch);
    console.log(`Generated ${relative(patchPath)}.`);
  }

  return 0;
}

ensureAutocorrect();

const files = collectReadmes(basedir);
if (files.length === 0) {
  console.log("No README.zh-cn.md files found.");
  process.exit(0);
}

if (args.has("--check")) {
  process.exit(check(files));
}

if (args.has("--fix")) {
  process.exit(fix(files));
}

process.exit(patch(files));
