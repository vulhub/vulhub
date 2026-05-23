# README Writing Guide for Vulhub Environments

This is the detailed reference for writing `README.md` (English) and `README.zh-cn.md` (Chinese) documentation for Vulhub vulnerability environments.

## English README Template

```markdown
# Vulnerability Title (CVE-XXXX-XXXXX)

[中文版本(Chinese version)](README.zh-cn.md)

[Software Name] is a [one-sentence description of what the software does].

[Paragraph explaining the vulnerability: root cause, how it can be exploited, affected version ranges, and impact. Be specific about versions — e.g., "affects versions 1.2.x through 1.4.5".]

References:

- <https://example.com/advisory>
- <https://example.com/cve-details>

## Environment Setup

Execute the following command to start [Software Name] [version]:

​```
docker compose up -d
​```

After the server starts, visit `http://your-ip:PORT` to access [the service / the admin panel / etc.].

## Vulnerability Reproduction

[Write flowing narrative paragraphs describing the exploitation steps. Use connecting words like "first", "then", "next" to guide the reader through the process. Do NOT use numbered lists or bullet points.]

[Include HTTP requests as code blocks where relevant:]

​```
POST /api/endpoint HTTP/1.1
Host: your-ip:PORT
Content-Type: application/json

{"payload": "value"}
​```

[Describe the expected response and what it demonstrates.]

![description of what the screenshot shows](1.png)

[Continue with additional steps if needed, always in paragraph form.]

![another description](2.png)
```

## Chinese README Template

```markdown
# 漏洞标题（CVE-XXXX-XXXXX）

[软件名称] 是一个[软件用途的一句话描述]。

[描述漏洞的段落：根本原因、利用方式、受影响版本范围、影响。中文和拉丁字母 / 阿拉伯数字之间需要加空格，例如 `影响 Grafana 11.0.0 至 11.2.1 版本`。]

参考链接：

- <https://example.com/advisory>
- <https://example.com/cve-details>

## 环境搭建

执行如下命令启动 [软件名称] [版本号]：

​```
docker compose up -d
​```

服务启动后，访问 `http://your-ip:端口` 即可看到[服务描述]。

## 漏洞复现

[用自然段落描述漏洞利用步骤。使用 “首先”、“然后”、“接下来” 等连接词。不要使用编号列表或项目符号。]

![截图描述](1.png)
```

## Hard Rules

### References Section

- Use plain text `References:` (English) or `参考链接：` (Chinese) — this is NOT a heading
- Each URL must be wrapped in angle brackets: `<https://...>`
- Maximum 5 reference links
- Place after the vulnerability description paragraph, before `## Environment Setup`

### Reproduction Steps

- Write in natural language **paragraphs** — this is the most important rule
- NEVER use numbered lists (`1. 2. 3.`) or bullet points (`- - -`) for reproduction steps
- Use transitional phrases: "first", "then", "next", "after that" / "首先", "然后", "接下来", "之后"
- Include raw HTTP requests in code blocks
- Show expected responses or command output

### English-Specific Rules

- Add `[中文版本(Chinese version)](README.zh-cn.md)` on the line immediately after the title
- Use `your-ip` as placeholder for the target IP address
- Write version ranges explicitly (e.g., "affects versions 11.0.0 through 11.2.1")

### Chinese-Specific Rules

The Chinese README follows the [中文文案排版指北 (sparanoid/chinese-copywriting-guidelines)](https://github.com/sparanoid/chinese-copywriting-guidelines). The rules below apply that style to vulhub documentation — they exist to make Chinese-mixed-with-English text more legible, which is exactly the situation every CVE write-up is in.

**Structure**

- Do NOT include a link to the English version (the English README links to the Chinese one, not the other way around)

**Spacing**

- Add a space between Chinese characters and adjacent Latin letters or Arabic numerals — this is the single most important rule
  - Correct: `Grafana 是一个开源的数据可视化平台，影响版本 11.0.0 至 11.2.1`
  - Wrong: `Grafana是一个开源的数据可视化平台，影响版本11.0.0至11.2.1`
- Add a space between numbers and units (e.g., `10 Gbps`, `监听 8088 端口`), with two exceptions: `°` and `%` stay tight against the number (`90°`, `15%`)
- Do NOT add extra spaces around full-width punctuation — full-width characters already carry visual whitespace
  - Correct: `刚刚购入了一台 iPhone，非常开心！`
  - Wrong: `刚刚购入了一台 iPhone ，非常开心 ！`
- Add a space between Chinese characters and inline code spans (backticks behave like Latin content)
  - Correct: `访问 \`http://your-ip:8088\` 即可看到登录页面`
  - Wrong: `访问\`http://your-ip:8088\`即可看到登录页面`

**Punctuation**

- Use full-width Chinese punctuation throughout Chinese sentences: `，` `。` `：` `；` `（` `）` `！` `？` `、` `——` `……` (not `, . : ; ( ) ! ? -- ...`)
- Inside a complete English sentence, proper noun, code span, or URL, keep half-width punctuation — e.g., `引用乔布斯的名言："Stay hungry, stay foolish."`
- Do NOT repeat punctuation for emphasis: write `！` not `！！！`, `？` not `？？？`
- Use full-width Chinese quotes `“”` for quoted speech; the original guideline recommends corner brackets `「」` `『』` but vulhub READMEs consistently use `“”` so stay consistent with the existing corpus

**Proper nouns**

- Use the canonical casing for product / project / company names: `GitHub`, `iOS`, `macOS`, `JavaScript`, `TypeScript`, `Grafana`, `CraftCMS`, `Apache`, `Spring`, `Tomcat`, `JDK`
- Do NOT write `github` / `GITHUB` / `Github`, `javascript`, `jdk`, etc.
- Do NOT invent abbreviations the upstream doesn't use (`TS` for TypeScript, `RJS` for React, `h5` for HTML5)

**Section headings and fixed phrases**

- Section headings stay in Chinese: `参考链接：`, `环境搭建`, `漏洞复现` (not the English equivalents)
- Transitional words for the reproduction narrative: `首先`、`然后`、`接下来`、`之后`、`最后`

### Screenshots

**Do NOT capture or generate screenshot files yourself** — the human author will fill in the actual `.png` files after reviewing the PR. Your job is to leave correctly-placed image references so the human knows what to shoot.

- Name sequentially: `1.png`, `2.png`, `3.png`, ...
- Reference at least one screenshot showing the exploit result (e.g., successful command execution, admin panel access, data exfiltration)
- Place the image reference at the exact paragraph it illustrates, so the reviewer can tell which step each number corresponds to
- Use descriptive alt text (English README) or 描述性文字 (Chinese README) so the human knows what the image should depict
- Do NOT fabricate fake screenshots (no rendered HTML, no synthetic terminal images) — leaving the `.png` missing is intentional

### Docker Commands

- Always use `docker compose up -d` (with a space)
- NEVER use `docker-compose up -d` (with a hyphen)

### Payload Guidelines

Vulhub is an educational project, not an attack tool:

- Prefer safe, demonstrative payloads (e.g., `id`, `whoami`, reading `/etc/passwd`)
- Avoid reverse shells or destructive payloads unless necessary to demonstrate impact
- Use `curl` or raw HTTP requests that users can easily reproduce
- Mention default credentials if they are needed to access the service

## Reference Examples

These existing READMEs in the project exemplify the correct style:

- `grafana/CVE-2024-9264/README.md` — RCE via SQL injection
- `craftcms/CVE-2024-56145/README.md` — Configuration-based RCE
- `hertzbeat/CVE-2024-42323/README.md` — Deserialization RCE
- `geoserver/CVE-2024-36401/README.md` — Property evaluation RCE
- `langflow/CVE-2025-3248/README.md` — Code execution RCE

When in doubt, read one of these examples to see the correct format in practice.
