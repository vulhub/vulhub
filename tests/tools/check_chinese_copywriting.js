const fs = require("fs");
const path = require("path");
const { spawnSync } = require("child_process");

const basedir = path.resolve(__dirname, "..", "..");
const args = new Set(process.argv.slice(2));
const mode = args.has("--fix") ? "fix" : args.has("--lint") ? "lint" : null;

if (!mode) {
  console.error("Usage: node tests/tools/check_chinese_copywriting.js [--lint|--fix]");
  process.exit(1);
}

const spacingChar = /[\p{Script=Han}\p{Script=Hiragana}\p{Script=Katakana}\p{Script=Hangul}\p{Script=Latin}\p{Number}]/u;
const inlineToken = /(!?)\[([^\]\n]+)\]\(([^)\n]+)\)|\*\*[^*\n]+?\*\*|__[^_\n]+?__|\*[^*\n]+?\*|<https?:\/\/[^>\n]+>/g;

function collectReadmes(dir) {
  const files = [];
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    if (entry.name === ".git" || entry.name === "node_modules" || entry.name === ".pytest_cache") continue;
    const full = path.join(dir, entry.name);
    if (entry.isDirectory()) files.push(...collectReadmes(full));
    else if (entry.name === "README.zh-cn.md") files.push(full);
  }
  return files;
}

function codeSpanRanges(line) {
  const ranges = [];
  let i = 0;
  while (i < line.length) {
    const start = line.indexOf("`", i);
    if (start === -1) break;
    let end = start + 1;
    while (end < line.length && line[end] === "`") end++;
    const marker = line.slice(start, end);
    const close = line.indexOf(marker, end);
    if (close === -1) { ranges.push([start, line.length, false]); break; }
    ranges.push([start, close + marker.length, true]);
    i = close + marker.length;
  }
  return ranges;
}

function inRanges(pos, ranges) {
  return ranges.some(([s, e]) => pos >= s && pos < e);
}

function inlineSpacingFix(line, codeRanges) {
  const insertions = new Set();
  const consider = (start, end) => {
    const prev = start > 0 ? line[start - 1] : "";
    const next = end < line.length ? line[end] : "";
    if (prev && spacingChar.test(prev)) insertions.add(start);
    if (next && spacingChar.test(next)) insertions.add(end);
  };

  for (const [start, end, closed] of codeRanges) {
    if (closed) consider(start, end);
  }

  inlineToken.lastIndex = 0;
  let m;
  while ((m = inlineToken.exec(line)) !== null) {
    if (inRanges(m.index, codeRanges)) continue;
    if (m[1] === "!") continue;
    consider(m.index, m.index + m[0].length);
  }

  if (insertions.size === 0) return null;
  let out = line;
  for (const pos of [...insertions].sort((a, b) => b - a)) {
    out = `${out.slice(0, pos)} ${out.slice(pos)}`;
  }
  return out;
}

function processFile(file, doFix) {
  const lines = fs.readFileSync(file, "utf8").split(/\n/);
  const relpath = path.relative(basedir, file);
  let inFence = false;
  let problems = 0;
  const out = lines.map((line, idx) => {
    if (/^\s*(```|~~~)/.test(line)) { inFence = !inFence; return line; }
    if (inFence) return line;
    const ranges = codeSpanRanges(line);
    if (ranges.some(([, , c]) => !c)) return line;
    const fixed = inlineSpacingFix(line, ranges);
    if (fixed && fixed !== line) {
      problems++;
      if (!doFix) console.error(`${relpath}:${idx + 1}: Markdown 行内元素两侧需要添加空格`);
      return fixed;
    }
    return line;
  });
  if (doFix && problems > 0) fs.writeFileSync(file, out.join("\n"));
  return problems;
}

const files = collectReadmes(basedir);
const fileArgs = files.map((f) => path.relative(basedir, f));

const ac = spawnSync("autocorrect", [mode === "fix" ? "--fix" : "--lint", ...fileArgs], {
  cwd: basedir, stdio: "inherit",
});
if (ac.error && ac.error.code === "ENOENT") {
  console.error("AutoCorrect 未安装：https://github.com/huacnlee/autocorrect");
  process.exit(127);
}

let total = 0;
for (const file of files) total += processFile(file, mode === "fix");

if (mode === "lint" && (ac.status !== 0 || total > 0)) {
  if (total > 0) {
    console.error(`\n发现 ${total} 处 Markdown 行内元素空格问题，运行 \`node tests/tools/check_chinese_copywriting.js --fix\` 修复。`);
  }
  process.exit(1);
}
process.exit(0);
