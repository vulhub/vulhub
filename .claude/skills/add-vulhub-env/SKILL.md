---
name: add-vulhub-env
description: "Guide for adding new vulnerability reproduction environments to the Vulhub project. Use this skill whenever the user wants to add a new CVE environment, create a vulnerability Docker lab, contribute a new vulnerability to vulhub, write vulnerability documentation for vulhub, or create docker-compose.yml / Dockerfile / README for a vulnerability environment. Also trigger when the user mentions a specific CVE they want to add, asks about vulhub contribution workflow, or wants to set up a reproducible exploit environment."
---

# Adding a New Vulhub Environment

Vulhub is an open-source collection of pre-built vulnerable Docker environments for security education. Each environment lets users reproduce a real-world vulnerability by running `docker compose up -d`.

This skill guides you through creating a complete environment from scratch.

## Prerequisites

Before starting, verify the vulnerability meets these criteria:

- **High impact** — high CVSS score or widely exploited in the wild
- **Open source** — the affected software is freely available
- **Docker-friendly** — runs in a Linux Docker container (no Windows-only or proprietary firmware)
- **Reproducible** — the exploit is reliable and deterministic
- **Not already covered** — check existing directories and `environments.toml`

## Required Deliverables

Every new environment requires these files:

```
base/<software>/<version>/Dockerfile          # Only if this version doesn't exist
<software>/<CVE-ID>/docker-compose.yml        # Required
<software>/<CVE-ID>/README.md                 # Required (English)
<software>/<CVE-ID>/README.zh-cn.md           # Required (Chinese)
<software>/<CVE-ID>/1.png, 2.png, ...         # Screenshots
environments.toml                              # Add entry
```

If the vulnerability is not a CVE, the directory name should be lowercase vulnerability name and the CVE-ID should be replaced with the directory name.

## Naming Conventions

| Item | Rule | Example |
|------|------|---------|
| Software directory | lowercase | `grafana`, `apache-cxf` |
| CVE directory | UPPERCASE | `CVE-2024-9264` |
| Non-CVE directory | lowercase | `admin-ssrf`, `weak_password` |
| File extensions | lowercase | `.yml`, `.md`, `.png` |
| Compose file | must be `.yml` | `docker-compose.yml` (NOT `.yaml`) |
| Branch name | lowercase | `add-grafana-cve-2024-9264` |

## Step 1: Research the Vulnerability

Gather this information before writing any files:

- Official advisory or CVE description
- Affected software version ranges (both start and end)
- Root cause (e.g., deserialization, injection, path traversal)
- Exploit method and proof-of-concept
- Default credentials (if relevant)
- Network ports the service uses

## Step 2: Create Base Dockerfile (if needed)

Check if `base/<software>/<version>/` already exists. If it does, skip this step.

If creating a new base image:

```dockerfile
FROM <official-upstream-image>:<version>

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends <packages> \
    && <install vulnerability dependencies> \
    && apt-get purge -y <build-only-packages> \
    && apt-get autoremove -y \
    && rm -rf /var/lib/apt/lists/*
```

Guidelines:

- Use official upstream images as the base when available
- Clean up package manager caches (`rm -rf /var/lib/apt/lists/*`)
- Prefer Debian-based images, do not choose `-alpine` variants when available
- Must pass `hadolint` linting (the CI runs this automatically)

## Step 3: Write docker-compose.yml

Keep it minimal. Reference pre-built images from the `vulhub/` Docker Hub namespace:

```yaml
services:
  <service-name>:
    image: vulhub/<software>:<version>
    ports:
      - "<host-port>:<container-port>"
```

Rules:

- Do NOT include `version: '2'` or any version header — newer environments omit it
- Only expose ports users need to interact with
- Use default port if possible, do not change the port number
- Add environment variables only if required for the vulnerability
- For multi-service setups (e.g., app + database), use `depends_on`

Multi-service example:

```yaml
services:
  web:
    image: vulhub/craftcms:5.6.16
    depends_on:
      - db
    ports:
      - "8088:80"
  db:
    image: mysql:8.4
    environment:
      - MYSQL_ROOT_PASSWORD=root
      - MYSQL_DATABASE=craftcms
```

## Step 4: Register in environments.toml

Add an `[[environment]]` block in **alphabetical order** among the same software's entries:

```toml
[[environment]]
name = "Human-readable Vulnerability Title"
cve = ["CVE-XXXX-XXXXX"]
app = "Software Name"
path = "<software>/<CVE-ID>"
dockerfile = {"vulhub/<software>:<version>" = "base/<software>/<version>"}
tags = ["RCE"]
```

Field requirements:

| Field | Type | Description |
|-------|------|-------------|
| `name` | string | Human-readable vulnerability title |
| `cve` | list | CVE IDs; use `[]` if no CVE assigned |
| `app` | string | Software name with proper casing (e.g., "Grafana") |
| `path` | string | Relative path with exactly 2 path components |
| `dockerfile` | inline table | Maps each `vulhub/*` image name to its Dockerfile directory under `base/`; use `{}` if no vulhub image |
| `tags` | list | At least one tag from the available tags |

Available tags (defined at the top of `environments.toml`):

```
# Vulnerability type
"Auth Bypass", "Backdoor", "Deserialization", "DoS", "Environment Injection",
"Expression Injection", "File Upload", "File Deletion", "Hard Coding",
"Info Disclosure", "Path Traversal", "Privilege Escalation", "RCE",
"SQL Injection", "SSRF", "SSTI", "XXE", "XSS"

# Application type
"CMS", "Database", "Framework", "Message Queue", "Webserver", "LLM"

# Other
"Other"
```

## Step 5: Write Documentation

Create both `README.md` (English) and `README.zh-cn.md` (Chinese).

**Read `references/readme-writing-guide.md` in this skill directory for the complete writing guide with templates.**

Also read `AGENTS.md` in the project root for additional style guidance and reference examples of well-written READMEs.

Critical rules summary:

- ALWAYS specify affected version ranges
- `References:` section uses plain text (NOT a `##` heading), URLs in `<>` brackets, max 5 links
- Reproduction steps must be **narrative paragraphs** (NEVER use numbered lists or bullet points for reproduction steps)
- Use `docker compose up -d` (NOT `docker-compose up -d`)
- English README: add `[中文版本(Chinese version)](README.zh-cn.md)` below the title
- Chinese README: do NOT link to English version; do NOT add spaces between Chinese characters and English/numbers
- Include at least one screenshot (`1.png`, `2.png`, ...)
- Prefer safe demonstration payloads (e.g., `id` command output over reverse shells)

## Taking Screenshots

Every environment needs screenshots, but not all screenshots can be taken automatically. Before attempting to capture screenshots, decide whether automation is feasible:

- **Can automate**: The exploit result is visible in a browser page or an Xwayland GUI window (e.g., a web response showing command output, an admin panel, an error page with version info). Use the `vulhub-screenshot` skill which provides browser-screenshot, window-screenshot, and gnome-screenshot scripts.
- **Cannot automate — leave for humans**: The screenshot requires interactive tools that an LLM cannot operate (e.g., Burp Suite, a native desktop app, a complex multi-step GUI interaction), or the exploit output is only visible in a terminal session that you are running yourself (terminal screenshots of your own shell are not meaningful). In this case, skip the screenshot step and note in your output that screenshots need to be added manually. **NEVER fabricate screenshots** by creating fake HTML pages styled to look like terminal output or tool output — this is dishonest. No screenshot is always better than a fake screenshot.

## Step 6: Test Locally

```bash
# Build base image (if you created one)
cd base/<software>/<version>
docker build -t vulhub/<software>:<version> .

# Start the environment
cd <software>/CVE-XXXX-XXXXX
docker compose up -d

# Wait for service readiness, then test the exploit
# Verify output matches what README describes

# Clean up
docker compose down -v
```

Confirm:

- Container starts without errors
- Vulnerability reproduces exactly as documented
- Exploit output matches README descriptions

## Step 7: Submit Pull Request

```bash
git checkout -b add-<software>-cve-xxxx-xxxxx    # lowercase branch name
git add base/<software>/<version>/Dockerfile \
         <software>/CVE-XXXX-XXXXX/ \
         environments.toml
git commit -m "Add <Software> CVE-XXXX-XXXXX <vulnerability type> environment"
git push -u origin add-<software>-cve-xxxx-xxxxx
gh pr create --title "Add <Software> CVE-XXXX-XXXXX environment" --base master
```

## CI Checks

Every PR runs these checks automatically:

| Check | What It Validates |
|-------|-------------------|
| `format-check` | LF line endings, hadolint, `environments.toml` completeness, naming conventions |
| `markdown-check` | Markdown formatting |

Common failures:

- Missing `environments.toml` entry for new `docker-compose.yml`
- Missing or incorrect `dockerfile` field in `environments.toml` entry
- CRLF line endings (all text files must use LF)
- Archive files > 4096 bytes
- Wrong directory casing (software dirs lowercase, CVE dirs UPPERCASE)
- Using `docker-compose.yaml` instead of `docker-compose.yml`

## Pre-submission Checklist

- [ ] Base Dockerfile created (if the software version is new)
- [ ] Base Dockerfile passes `hadolint`
- [ ] `docker-compose.yml` references the correct `vulhub/<software>:<version>` image
- [ ] `README.md` follows the style guide
- [ ] `README.zh-cn.md` follows the style guide
- [ ] Screenshots included (`1.png`, `2.png`, ...)
- [ ] Entry added to `environments.toml` with valid tags and correct `dockerfile` mapping
- [ ] Environment tested: `docker compose up -d` works, exploit reproduces
- [ ] All text files use LF line endings
- [ ] Branch name is lowercase
