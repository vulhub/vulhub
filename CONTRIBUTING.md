# Contributing to Vulhub

This guide walks you through the full process of contributing a new vulnerability environment. It is written for both human contributors and AI agents.

## Before You Start

Pick a vulnerability that meets these criteria:

- **Impactful** — The vulnerability has a high CVSS score or was widely exploited in the wild.
- **Open source** — The affected software is open source or has a freely available edition.
- **Docker-friendly** — The software can run in a Linux Docker container. Avoid vulnerabilities that only affect Windows, proprietary firmware, or require specialized hardware.
- **Reproducible** — The exploit is reliable and deterministic. Race conditions or timing-dependent bugs are harder to demonstrate in a lab setting.
- **Not already covered** — Check existing directories and `environments.toml` to avoid duplicates.

## Project Structure

```
vulhub/
  base/<software>/<version>/Dockerfile    # Base image Dockerfiles
  <software>/<CVE-ID>/                    # Vulnerability environment
    docker-compose.yml                    #   Required
    README.md                             #   Required (English)
    README.zh-cn.md                       #   Required (Chinese)
    1.png, 2.png, ...                     #   Screenshots
  environments.toml                       # Registry of all environments
  AGENTS.md                               # Documentation style guide
  tests/check/                            # CI checks
```

### Naming Conventions

- Software directories: **lowercase** (e.g., `grafana`, `apache-cxf`)
- CVE directories: **UPPERCASE** (e.g., `CVE-2024-9264`)
- Non-CVE directories: **lowercase** (e.g., `admin-ssrf`, `weak_password`)
- File extensions: **lowercase** (e.g., `.yml`, `.md`, `.png`)
- Use `docker-compose.yml`, NOT `docker-compose.yaml`

## Step-by-Step Workflow

### 1. Create a Base Image (if needed)

If the software version does not already exist under `base/`, create a Dockerfile:

```
base/<software>/<version>/Dockerfile
```

The Dockerfile should build a minimal image of the vulnerable software version. Include any dependencies required to make the vulnerability exploitable. For example, if the vulnerability requires a specific binary in PATH, install it here.

**Guidelines:**

- Use official upstream images as the base when available.
- If the upstream image is Alpine-based and you need glibc binaries, use the `-ubuntu` variant instead.
- Clean up package manager caches to keep image size small.
- Switch back to a non-root user (e.g., `USER grafana`) at the end if the upstream image expects it.
- Run `hadolint` on your Dockerfile — the CI will check this.

**Example:**

```dockerfile
FROM grafana/grafana:11.0.0-ubuntu

LABEL maintainer="phithon <root@leavesongs.com>"

USER root

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget unzip \
    && wget -q -O /tmp/duckdb.zip https://github.com/duckdb/duckdb/releases/download/v1.0.0/duckdb_cli-linux-amd64.zip \
    && unzip /tmp/duckdb.zip -d /usr/local/bin/ \
    && chmod +x /usr/local/bin/duckdb \
    && rm /tmp/duckdb.zip \
    && apt-get purge -y wget unzip \
    && apt-get autoremove -y \
    && rm -rf /var/lib/apt/lists/* \
    && mkdir -p /usr/share/grafana/.duckdb \
    && chown grafana:root /usr/share/grafana/.duckdb

USER grafana
```

### 2. Create the Vulnerability Directory

```
<software>/CVE-XXXX-XXXXX/
```

### 3. Write docker-compose.yml

Keep it minimal. Reference the pre-built image from Docker Hub under the `vulhub/` namespace:

```yaml
services:
  grafana:
    image: vulhub/grafana:11.0.0
    ports:
      - "3000:3000"
```

**Notes:**

- Do NOT include `version: '2'` — newer environments omit it.
- Only expose ports that users need to interact with.
- Add environment variables only if required for the vulnerability to work.

### 4. Register in environments.toml

Add a `[[environment]]` block in **alphabetical order** among the same software's entries:

```toml
[[environment]]
name = "Grafana SQL Expressions Remote Code Execution"
cve = ["CVE-2024-9264"]
app = "Grafana"
path = "grafana/CVE-2024-9264"
tags = ["RCE", "SQL Injection"]
```

**Field requirements:**

| Field  | Type       | Description                                         |
|--------|------------|-----------------------------------------------------|
| `name` | string     | Human-readable vulnerability title                  |
| `cve`  | list       | CVE IDs (empty list `[]` if no CVE assigned)        |
| `app`  | string     | Software name                                       |
| `path` | string     | Relative path with exactly 2 components             |
| `tags` | list       | At least one tag from the global `tags` list at top |

Available tags are defined at the top of `environments.toml`. Use existing tags when possible.

### 5. Write Documentation

Create both `README.md` (English) and `README.zh-cn.md` (Chinese). See `AGENTS.md` for the full style guide. The key rules are:

**Structure:**

```markdown
# Vulnerability Title (CVE-XXXX-XXXXX)

[中文版本(Chinese version)](README.zh-cn.md)    <!-- English only -->

[Software] is a [one-sentence description].

[Vulnerability description paragraph: root cause, affected versions, impact.]

References:

- <https://example.com/advisory>

## Environment Setup

Execute the following command to start [Software] v[X.Y.Z]:

​```
docker compose up -d
​```

After the server starts, visit `http://your-ip:PORT` to access the service.

## Vulnerability Reproduction

[Flowing narrative paragraphs with HTTP requests and results.]

![description](1.png)
```

**Hard rules:**

- References: plain text `References:`, NOT a `##` heading. URLs in `<>` brackets. Max 5 links.
- Reproduction steps: narrative paragraphs, NOT numbered lists or bullet points.
- English README: include `[中文版本(Chinese version)](README.zh-cn.md)` below the title.
- Chinese README: do NOT link to the English version. Do NOT add spaces between Chinese characters and English/numbers.
- Always specify affected version ranges.
- Always use `docker compose up -d` (not `docker-compose up -d`).
- Include at least one screenshot (`1.png`, `2.png`, ...) showing the exploit result.

**Reproduction approach:**

- Vulhub is an educational project, NOT an attack tool. Demonstrate impact clearly but safely.
- Prefer showing command output (e.g., `id`) over reverse shells or destructive payloads.
- Use `curl` or raw HTTP requests that users can easily reproduce.
- Mention default credentials if they are needed.

### 6. Build and Test

Before submitting, verify the full workflow locally:

```bash
# Build base image (if you created one)
cd base/<software>/<version>
docker build -t vulhub/<software>:<version> .

# Start the environment
cd <software>/CVE-XXXX-XXXXX
docker compose up -d

# Wait for the service to be ready, then test the exploit
curl ...

# Clean up
docker compose down
```

Confirm that:

- The container starts without errors.
- The vulnerability can be reproduced exactly as documented.
- The exploit output matches what the README describes.

### 7. Submit a Pull Request

```bash
git checkout -b add-<software>-cve-xxxx-xxxxx    # lowercase branch name
git add base/<software>/<version>/Dockerfile \
         <software>/CVE-XXXX-XXXXX/ \
         environments.toml
git commit -m "Add <Software> CVE-XXXX-XXXXX <vulnerability type> environment"
git push -u origin add-<software>-cve-xxxx-xxxxx
gh pr create --title "Add <Software> CVE-XXXX-XXXXX environment" --base master
```

**Important:** Use a lowercase branch name. Mixed-case branch names cause issues on macOS and Windows.

## CI Checks

Every PR runs the following checks automatically. Make sure they pass before requesting review.

| Check            | What It Validates                                                       |
|------------------|-------------------------------------------------------------------------|
| `format-check`   | Line endings (LF only), Dockerfile linting (hadolint), `environments.toml` completeness, file naming conventions |
| `markdown-check` | Markdown formatting                                                     |

Common failure causes:

- **Forgot `environments.toml`** — Every new `docker-compose.yml` must have a matching entry.
- **CRLF line endings** — All text files must use LF.
- **Archive files too large** — Compressed files must be under 4096 bytes.
- **Wrong casing** — Software directories must be lowercase; CVE directories must be uppercase.

## Checklist

Use this checklist before submitting:

- [ ] Base Dockerfile created (if the software version is new)
- [ ] Base Dockerfile passes `hadolint`
- [ ] `docker-compose.yml` references the correct `vulhub/<software>:<version>` image
- [ ] `README.md` follows the template in `AGENTS.md`
- [ ] `README.zh-cn.md` follows the template in `AGENTS.md`
- [ ] Screenshots included (`1.png`, `2.png`, ...)
- [ ] Entry added to `environments.toml` with valid tags
- [ ] Environment tested: `docker compose up -d` works, exploit reproduces successfully
- [ ] All text files use LF line endings
- [ ] Branch name is lowercase
