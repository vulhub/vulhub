"use strict";

const SPACING_CHAR = /[\p{Script=Han}\p{Script=Hiragana}\p{Script=Katakana}\p{Script=Hangul}\p{Script=Latin}\p{Number}]/u;
const INLINE_TYPES = new Set([
  "codeText",
  "link",
  "autolink",
  "literalAutolink",
  "emphasis",
  "strong",
  "strikethrough",
]);

function walk(token, lines, onError) {
  if (INLINE_TYPES.has(token.type)) {
    const { startLine, startColumn, endLine, endColumn } = token;

    if (startColumn > 1) {
      const prev = lines[startLine - 1][startColumn - 2];
      if (prev && SPACING_CHAR.test(prev)) {
        onError({
          lineNumber: startLine,
          detail: "需要在 Markdown 行内元素前添加空格",
          range: [startColumn - 1, 2],
          fixInfo: { editColumn: startColumn, insertText: " " },
        });
      }
    }

    const endLineText = lines[endLine - 1];
    if (endLineText && endColumn - 1 < endLineText.length) {
      const next = endLineText[endColumn - 1];
      if (next && SPACING_CHAR.test(next)) {
        onError({
          lineNumber: endLine,
          detail: "需要在 Markdown 行内元素后添加空格",
          range: [endColumn - 1, 2],
          fixInfo: { editColumn: endColumn, insertText: " " },
        });
      }
    }
  }

  for (const child of token.children || []) walk(child, lines, onError);
}

module.exports = {
  names: ["cjk-inline-spacing"],
  description: "Markdown 行内元素与中文 / 拉丁字符之间需要空格",
  tags: ["whitespace", "cjk"],
  parser: "micromark",
  function: function rule(params, onError) {
    if (!params.name.endsWith("README.zh-cn.md")) return;
    for (const token of params.parsers.micromark.tokens) {
      walk(token, params.lines, onError);
    }
  },
};
