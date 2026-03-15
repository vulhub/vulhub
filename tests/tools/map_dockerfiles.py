#!/usr/bin/env python3
"""Map each environment in environments.toml to its Dockerfile directory under base/."""

import os
import re
import sys

BASE_DIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# Hardcoded image-to-directory overrides for cases that can't be resolved algorithmically
IMAGE_OVERRIDES = {
    "weblogic:10.3.6.0-2017": "base/weblogic",
    "weblogic:12.2.1.3-2018": "base/weblogic/12.2.1.3",
    "discuz:x3.4": "base/discuzx/3.4-20170801",
    "owncloud:10.12.1": "base/owncloud/10.12",
    "ffmpeg:2.8.4-with-php": "base/ffmpeg/2.8.4-php",
    "ffmpeg:3.2.4-with-php": "base/ffmpeg/3.2.4-php",
    "comfyui:3.37-with-manager": "base/comfyui/with-manager/3.37",
    "erlang:27.3.2-with-ssh": "base/erlang/with-ssh/27.3.2",
    "drupal:7.57": None,  # No Dockerfile exists
    "php:httpoxy": None,   # No Dockerfile exists
    "php:5.6-apache": None,  # No Dockerfile exists
}

# Name prefix mappings: image name -> base directory prefix
NAME_MAPPINGS = {
    "nextjs": "next.js",
    "discuz": "discuzx",
    "aiohttp": "python/aiohttp",
}


def has_dockerfile(rel_dir):
    """Check if a Dockerfile exists at the given relative directory."""
    return os.path.isfile(os.path.join(BASE_DIR, rel_dir, "Dockerfile"))


def resolve_image_to_dockerfile(image_ref):
    """Resolve a vulhub image reference to its Dockerfile directory.

    Args:
        image_ref: e.g. "activemq:5.11.1" (without "vulhub/" prefix)

    Returns:
        Dockerfile directory path relative to BASE_DIR, or None if not found.
    """
    # Check hardcoded overrides first
    if image_ref in IMAGE_OVERRIDES:
        return IMAGE_OVERRIDES[image_ref]

    name, tag = image_ref.split(":", 1)

    # Strategy 1: Direct match
    direct = f"base/{name}/{tag}"
    if has_dockerfile(direct):
        return direct

    # Strategy 2: Name prefix mappings
    mapped_name = None

    # Check exact name mappings
    if name in NAME_MAPPINGS:
        mapped_name = NAME_MAPPINGS[name]
    # Check spring-with-* pattern (maps to spring/with-*)
    elif name.startswith("spring-with-"):
        suffix = name[len("spring-"):]  # "with-h2database" etc.
        mapped_name = f"spring/{suffix}"
    # Check spring-* pattern (maps to spring/spring-*)
    elif name.startswith("spring-"):
        mapped_name = f"spring/{name}"
    # Check uwsgi-* pattern
    elif name.startswith("uwsgi-"):
        suffix = name[len("uwsgi-"):]
        mapped_name = f"uwsgi/{suffix}"

    if mapped_name:
        mapped = f"base/{mapped_name}/{tag}"
        if has_dockerfile(mapped):
            return mapped

    # Strategy 3: Split tag on "-with-" -> base/<name>/<ver>/with-<suffix>/
    if "-with-" in tag:
        parts = tag.split("-with-", 1)
        ver, suffix = parts
        candidate = f"base/{name}/{ver}/with-{suffix}"
        if has_dockerfile(candidate):
            return candidate
        if mapped_name:
            candidate = f"base/{mapped_name}/{ver}/with-{suffix}"
            if has_dockerfile(candidate):
                return candidate

    # Strategy 4: Split tag on last dash -> base/<name>/<prefix>/<suffix>/
    if "-" in tag:
        last_dash = tag.rfind("-")
        prefix = tag[:last_dash]
        suffix = tag[last_dash + 1:]
        candidate = f"base/{name}/{prefix}/{suffix}"
        if has_dockerfile(candidate):
            return candidate
        if mapped_name:
            candidate = f"base/{mapped_name}/{prefix}/{suffix}"
            if has_dockerfile(candidate):
                return candidate

    # Strategy 5: Tomcat legacy pattern -> base/tomcat/tomcat<tag>
    if name == "tomcat":
        candidate = f"base/tomcat/tomcat{tag}"
        if has_dockerfile(candidate):
            return candidate

    # Strategy 6: Fallback - recursive search under base/<name>/
    search_name = mapped_name or name
    base_search = os.path.join(BASE_DIR, "base", search_name)
    if os.path.isdir(base_search):
        for root, dirs, files in os.walk(base_search):
            if "Dockerfile" in files:
                rel = os.path.relpath(root, BASE_DIR)
                # Check if tag appears in the path
                if tag.replace("-", "") in rel.replace("-", "").replace("/", ""):
                    return rel

    return None


def extract_vulhub_images_from_compose(compose_path):
    """Extract all vulhub/* image references from a docker-compose.yml."""
    images = []
    with open(compose_path) as f:
        content = f.read()

    # Find all image: vulhub/... references
    for match in re.finditer(r'image:\s*vulhub/(\S+)', content):
        images.append(match.group(1))

    return images


def extract_vulhub_from_dockerfile(dockerfile_path):
    """Extract FROM vulhub/* references from a Dockerfile."""
    images = []
    if not os.path.isfile(dockerfile_path):
        return images
    with open(dockerfile_path) as f:
        for line in f:
            match = re.match(r'\s*FROM\s+vulhub/(\S+)', line, re.IGNORECASE)
            if match:
                images.append(match.group(1))
    return images


def has_build_directive(compose_path):
    """Check if any service in docker-compose.yml uses build:."""
    with open(compose_path) as f:
        content = f.read()
    return bool(re.search(r'^\s*build:', content, re.MULTILINE))


def parse_environments_toml(toml_path):
    """Parse environments.toml and return list of environment dicts with path and line info."""
    envs = []
    with open(toml_path) as f:
        lines = f.readlines()

    current_env = None
    for i, line in enumerate(lines):
        if line.strip() == "[[environment]]":
            if current_env:
                envs.append(current_env)
            current_env = {"line": i, "fields": {}}
        elif current_env and "=" in line and not line.strip().startswith("#"):
            key = line.split("=", 1)[0].strip()
            val = line.split("=", 1)[1].strip()
            current_env["fields"][key] = val
            if key == "path":
                # Extract path value (remove quotes)
                current_env["path"] = val.strip('"').strip("'")
            if key == "tags":
                current_env["tags_line"] = i

    if current_env:
        envs.append(current_env)

    return envs


def resolve_env_dockerfiles(path):
    """Resolve all Dockerfile directories for an environment path.

    Returns:
        (mapping, unresolved) where mapping is an OrderedDict of
        "vulhub/<name>:<tag>" -> "base/..." and unresolved is a list of
        image refs that couldn't be resolved.
    """
    compose_path = os.path.join(BASE_DIR, path, "docker-compose.yml")
    if not os.path.isfile(compose_path):
        return {}, []

    all_image_refs = set()

    compose_images = extract_vulhub_images_from_compose(compose_path)
    all_image_refs.update(compose_images)

    if has_build_directive(compose_path):
        local_dockerfile = os.path.join(BASE_DIR, path, "Dockerfile")
        from_images = extract_vulhub_from_dockerfile(local_dockerfile)
        all_image_refs.update(from_images)

    mapping = {}
    unresolved = []
    for img_ref in sorted(all_image_refs):
        resolved = resolve_image_to_dockerfile(img_ref)
        full_image = f"vulhub/{img_ref}"
        if resolved is not None:
            if has_dockerfile(resolved):
                mapping[full_image] = resolved
            else:
                unresolved.append(img_ref)
        elif img_ref not in [k for k, v in IMAGE_OVERRIDES.items() if v is None]:
            unresolved.append(img_ref)

    return mapping, unresolved


def format_dockerfile_field(mapping):
    """Format the dockerfile field as a TOML inline table."""
    if not mapping:
        return "dockerfile = {}\n"
    pairs = ", ".join(f'"{k}" = "{v}"' for k, v in mapping.items())
    return f"dockerfile = {{{pairs}}}\n"


def update_environments_toml():
    """Read environments.toml, add/replace dockerfile field, and write back."""
    toml_path = os.path.join(BASE_DIR, "environments.toml")

    with open(toml_path) as f:
        lines = f.readlines()

    # First pass: remove existing dockerfile lines
    lines = [line for line in lines if not line.strip().startswith("dockerfile = ")]

    # Re-parse after removing old dockerfile lines
    # Write temp content for parsing
    import tempfile
    with tempfile.NamedTemporaryFile(mode="w", suffix=".toml", delete=False) as tmp:
        tmp.writelines(lines)
        tmp_path = tmp.name

    envs = parse_environments_toml(tmp_path)
    os.unlink(tmp_path)

    # Build path -> mapping
    path_to_mapping = {}
    all_unresolved = []
    for env in envs:
        path = env.get("path", "")
        if path:
            mapping, unresolved = resolve_env_dockerfiles(path)
            path_to_mapping[path] = mapping
            for img in unresolved:
                all_unresolved.append((path, img))

    # Insert dockerfile lines right before tags line (in reverse order)
    insertions = []
    for env in envs:
        path = env.get("path", "")
        tags_line = env.get("tags_line")
        if path and tags_line is not None:
            mapping = path_to_mapping.get(path, {})
            dockerfile_line = format_dockerfile_field(mapping)
            insertions.append((tags_line, dockerfile_line))

    insertions.sort(key=lambda x: x[0], reverse=True)

    for line_num, text in insertions:
        lines.insert(line_num, text)

    with open(toml_path, "w") as f:
        f.writelines(lines)

    print(f"Updated {len(insertions)} environments in environments.toml")

    if all_unresolved:
        print(f"\n{len(all_unresolved)} unresolved images:")
        for path, img in all_unresolved:
            print(f"  {path}: vulhub/{img}")

    return len(insertions), all_unresolved


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--update":
        count, unresolved = update_environments_toml()
        return

    # Default: dry-run mode - just print the mapping
    toml_path = os.path.join(BASE_DIR, "environments.toml")
    envs = parse_environments_toml(toml_path)

    for env in envs:
        path = env.get("path", "")
        if not path:
            continue
        mapping, unresolved = resolve_env_dockerfiles(path)
        if mapping:
            pairs = ", ".join(f"{k} -> {v}" for k, v in mapping.items())
            print(f"{path} -> {pairs}")
        else:
            print(f"{path} -> (none)")
        for img in unresolved:
            print(f"  UNRESOLVED: vulhub/{img}", file=sys.stderr)


if __name__ == "__main__":
    main()
