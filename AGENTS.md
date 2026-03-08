# Vulhub — AI Agent Guide

Vulhub is an open-source collection of pre-built vulnerable Docker environments for security education.

## Project Structure

```
base/<software>/<version>/Dockerfile      # Base Docker images for vulnerable software
<software>/<CVE-ID or name>/              # Vulnerability environments
├── docker-compose.yml
├── README.md                             # English documentation
├── README.zh-cn.md                       # Chinese documentation
└── 1.png, 2.png, ...                     # Screenshots
environments.toml                          # Registry of all environments
```

## Adding a New Environment

Use the `add-vulhub-env` skill — it contains the complete step-by-step guide, Dockerfile templates, docker-compose patterns, documentation writing rules, and CI check requirements.

## Key Conventions

- Software directories: **lowercase** (`grafana`, `apache-cxf`)
- CVE directories: **UPPERCASE** (`CVE-2024-9264`)
- Non-CVE directories: **lowercase** (`admin-ssrf`, `weak_password`)
- Compose file: must be `docker-compose.yml` (not `.yaml`)
- Docker command: `docker compose up -d` (not `docker-compose up -d`)
- All text files must use **LF** line endings
