# Vulhub Vulnerability Documentation

## Your Primary Task

Write bilingual vulnerability documentation for the Vulhub project:
- English documentation in `README.md`
- Chinese documentation in `README.zh-cn.md`

Each documentation should enable users to start a vulnerable Docker environment and reproduce the vulnerability by following the steps.

## Critical Writing Rules

Follow these rules strictly for all documentation:

**Version Specification**
- ALWAYS specify the affected software version range when describing vulnerabilities
- Example: "affects versions 1.2.x through 1.4.5" or "versions prior to 2.0.1"

**Hyperlink Format**
- Use ONLY `<https://example.com>` or `[text](https://example.com)` format
- Never use bare URLs without angle brackets

**References Section**
- Use plain text "References:" followed by a blank line and bullet points
- Do NOT use heading format (`##` or `###`)
- List up to 5 reference links maximum

**Reproduction Steps**
- Write in natural language paragraphs
- Do NOT use numbered lists (1, 2, 3...)
- Do NOT use bullet points for step-by-step instructions
- Describe the process as flowing narrative text

**Language-Specific Rules**
- English README.md: Add `[中文版本(Chinese version)](README.zh-cn.md)` link below the title
- Chinese README.zh-cn.md: Do NOT add any link to English version
- Chinese text: Do NOT add spaces between Chinese characters and English letters/numbers

## Document Structure Template

Every vulnerability documentation must follow this exact structure:

```markdown
# [Vulnerability Title] (CVE-XXXX-XXXXX if exists)

[中文版本(Chinese version)](README.zh-cn.md)

[Software Name] is a [brief one-sentence description of what the software does].

[One paragraph describing: the vulnerability principle, how it works, affected versions, and potential security impact. Use technical but accessible language.]

References:

- <https://example.com/reference1>
- <https://example.com/reference2>
- <https://example.com/reference3>

## Environment Setup

Execute the following command to start [Software Name] v[X.Y.Z]:

\```
docker compose up -d
\```

After the server starts, [describe how users access the service - e.g., "visit `http://your-ip:8080`" or "the service listens on port 3306"].

## Vulnerability Reproduction

[Describe the reproduction process in natural flowing paragraphs. DO NOT use numbered steps. Connect actions with words like "first," "then," "next," "after that," "finally." Include at least one image to illustrate key steps.]

![screenshot description](1.png)

[Continue with more paragraphs describing the exploitation process and expected results.]
```

## Reference Examples

When writing new documentation, READ these example files first to match the writing style:

**For pre-authentication RCE vulnerabilities:**
- `langflow/CVE-2025-3248/README.md` - Clean structure, clear exploitation flow

**For authentication bypass vulnerabilities:**
- `xxl-job/unacc/README.md` - Good unauthorized access documentation pattern

**For deserialization RCE:**
- `hertzbeat/CVE-2024-42323/README.md` - Excellent technical explanation of deserialization

**For property evaluation RCE:**
- `geoserver/CVE-2024-36401/README.md` - Well-balanced technical depth

**For configuration-based RCE:**
- `craftcms/CVE-2024-56145/README.md` - Clear configuration manipulation steps

Use the Read tool to examine these files before writing new documentation to ensure consistent style and quality.

## Project Structure Reference

Understanding the project layout:

**Base Images Directory:**
- `base/[software]/[version]/Dockerfile` - Contains Docker build files for vulnerable software versions
- Example: `base/activemq/5.17.3/Dockerfile` builds ActiveMQ 5.17.3

**Vulnerability Directories:**
- `[software]/[CVE-ID or name]/` - Each vulnerability gets its own directory
- Contains: `docker-compose.yml`, `README.md`, `README.zh-cn.md`, POC scripts
- Example: `activemq/CVE-2023-46604/`

**Project Root:**
- `README.md` and `README.zh-cn.md` - Main project documentation
- `tests/` - CI/CD testing scripts

## Image Requirements

Documentation must include visual proof:
- Include at least one screenshot showing critical exploitation steps
- Images should be named sequentially: `1.png`, `2.png`, `3.png`, etc.
- Use clear, readable screenshots that demonstrate successful exploitation
- Add descriptive alt text: `![vulnerability exploitation result](1.png)`

## Before Submitting Documentation

After writing any vulnerability documentation, verify by reading the complete file:

1. **Check version specification** - Affected versions are clearly stated
2. **Verify hyperlinks** - All URLs use `<>` or `[]()` format
3. **Confirm References format** - Plain text "References:", NOT a heading
4. **Review reproduction steps** - Written in paragraphs, NOT numbered/bulleted lists
5. **Validate language links**:
   - English doc has Chinese version link below title
   - Chinese doc has NO English version link
6. **Check Chinese spacing** - No forced spaces between Chinese and English text
7. **Ensure images present** - At least one screenshot included with proper alt text

## Common Mistakes to Avoid

**Instead of numbered steps:**
```markdown
❌ Wrong:
1. First, send a POST request
2. Then, check the response
3. Finally, verify the exploit
```

**Use flowing narrative:**
```markdown
✅ Correct:
First, send a POST request to the `/api/endpoint` with the malicious payload. 
After receiving the response, check that the server returns a 200 status code 
with the injected command output. Finally, verify the exploit succeeded by 
observing the system command execution result in the response body.
```

**Instead of heading for References:**
```markdown
❌ Wrong:
## References

- link1
```

**Use plain text:**
```markdown
✅ Correct:
References:

- <https://example.com/advisory>
```

## Working with Docker Compose

When describing environment setup:
- Always use `docker compose up -d` (not `docker-compose up -d`)
- Specify exact version numbers in the description
- Explain what ports/services become available after startup
- Mention any default credentials if applicable

## Technical Writing Guidelines

**Vulnerability descriptions should include:**
- Root cause (e.g., "improper input validation", "unsafe deserialization")
- Attack vector (e.g., "unauthenticated attacker can...", "requires authenticated access")
- Impact (e.g., "leading to remote code execution", "resulting in data disclosure")
- Affected versions range

**Keep language:**
- Technical but accessible - avoid unnecessary jargon
- Concise - one paragraph for vulnerability description
- Precise - use exact technical terms for exploit techniques
- Neutral - describe facts without sensationalism

## When You Need More Context

If the vulnerability details are unclear or you need more information:
- Read the reference links provided by the user
- Search for CVE details if a CVE ID is mentioned
- Examine similar vulnerability documentation in the project using Read/Grep tools
- Ask clarifying questions about the exploitation method or affected versions

Remember: The goal is to help users understand and reproduce the vulnerability in a controlled environment for learning purposes. Your documentation should be clear enough that someone with basic security knowledge can follow along successfully.