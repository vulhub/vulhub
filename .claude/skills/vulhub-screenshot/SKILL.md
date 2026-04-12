---
name: vulhub-screenshot
description: "Capture screenshots for Vulhub vulnerability environments on GNOME Wayland. Use this skill whenever you need to take screenshots of web pages, browser windows, Xwayland application windows, or full-screen captures for vulnerability documentation. Also trigger when you need to interact with a web page via Chrome DevTools Protocol (CDP) before capturing (e.g., filling login forms, clicking buttons, navigating multi-step flows). This skill provides scripts for browser screenshots, window captures, and GNOME portal-based screenshots. Use it for any screenshot task in a GNOME Wayland session, especially when documenting exploit reproduction steps."
---

# Vulhub Screenshot Tool

This skill provides scripts for capturing screenshots on GNOME Wayland sessions, designed for documenting vulnerability reproduction in Vulhub environments.

## Quick Reference

| Scenario | Script | Notes |
|----------|--------|-------|
| Web page screenshot | `browser-screenshot` | Includes address bar, clean Chrome profile |
| Interact with page first (login, click) | `browser-screenshot -d 9222` + CDP | See "DOM Interaction" below |
| Xwayland app window | `window-screenshot` | Match by title or WM_CLASS |
| Pure Wayland app / full screen | `gnome-screenshot` | Uses portal API, sees all windows |
| Select a region interactively | `gnome-screenshot -i` | Opens GNOME's screenshot UI |

## Browser Screenshots

For web-based vulnerabilities (the most common case), use `scripts/browser-screenshot` in this skill directory. It opens a URL in Chrome via Xwayland, waits for the page to load, and captures the full browser window including the address bar.

```bash
# Basic usage — captures the page and saves as 1.png
bash <skill-dir>/scripts/browser-screenshot -o <software>/<CVE-ID>/1.png http://localhost:8080

# Wait longer for slow-loading pages
bash <skill-dir>/scripts/browser-screenshot -w 15 -o 1.png http://localhost:8080/admin

# Custom window size
bash <skill-dir>/scripts/browser-screenshot -s 1920,1080 -o 1.png http://localhost:8080

# Enable remote debugging (for CDP-based DOM interaction, see below)
bash <skill-dir>/scripts/browser-screenshot -d 9222 -w 0 -o 1.png http://localhost:8080
```

Replace `<skill-dir>` with the actual path to this skill directory (typically `.claude/skills/vulhub-screenshot`).

The script includes Chrome flags to prevent common screenshot interference: extensions are disabled, password save prompts are suppressed, and the search engine choice screen is skipped.

Dependencies: `google-chrome`, `xwininfo` (from `x11-utils`), `import` (from `imagemagick`).

## Non-Browser Screenshots (Xwayland windows)

For capturing Xwayland windows (apps running in X11 compatibility mode) by title or WM_CLASS, use `scripts/window-screenshot`:

```bash
# Capture a window by title substring
bash <skill-dir>/scripts/window-screenshot -o 2.png "Terminal"

# Capture by WM_CLASS
bash <skill-dir>/scripts/window-screenshot -c -o 2.png "gnome-terminal"
```

Dependencies: same as browser-screenshot (`xwininfo`, `imagemagick`).

Note: this only works for X11/Xwayland windows. Pure Wayland windows (most native GNOME apps) are invisible to `xwininfo`. Use `gnome-screenshot` below for those.

## Full Screen / Pure Wayland Screenshots

For capturing pure Wayland windows (native GNOME apps that don't run through Xwayland), use `scripts/gnome-screenshot`. It uses the `xdg-desktop-portal` Screenshot API, which captures at the compositor level and can see ALL windows regardless of display protocol.

```bash
# Full-screen capture (non-interactive, no user action needed)
python3 <skill-dir>/scripts/gnome-screenshot -o 1.png

# Interactive mode — opens GNOME's built-in screenshot UI to select window or area
python3 <skill-dir>/scripts/gnome-screenshot -i -o 1.png
```

Dependencies: `python3`, `python3-dbus`, `python3-gi` (all pre-installed on Ubuntu with GNOME).

The non-interactive mode captures the full screen. If you need just a specific window or region, use `-i` to open the GNOME screenshot selection UI, or capture full screen and crop with ImageMagick:

```bash
# Capture full screen then crop to a region
python3 <skill-dir>/scripts/gnome-screenshot -o /tmp/fullscreen.png
convert /tmp/fullscreen.png -crop 1280x900+100+200 1.png
```

## DOM Interaction via Remote Debugging (CDP)

When a screenshot requires user interaction (e.g. filling a login form, clicking buttons, navigating through multi-step flows), use Chrome's remote debugging protocol (CDP) to manipulate the DOM programmatically. **Always prefer CDP over tools like `xdotool`** — xdotool operates at the X11 window level and is fragile (wrong coordinates, focus issues, no access to shadow DOM), while CDP operates directly on the page DOM and is reliable.

### Workflow

1. Launch browser-screenshot with `--debug-port` and `--wait 0` (so it doesn't capture immediately):

```bash
bash <skill-dir>/scripts/browser-screenshot -d 9222 -w 0 -o 2.png http://localhost:8080/login
```

2. Connect to Chrome via CDP (Python example with `websocket-client` and `requests`):

```python
import json, requests, websocket

# Find the page target (skip chrome-extension:// background pages)
targets = requests.get("http://localhost:9222/json").json()
ws_url = next(t["webSocketDebuggerUrl"] for t in targets
              if t["type"] == "page" and "chrome-extension" not in t["url"])

ws = websocket.create_connection(ws_url, timeout=15)
msg_id = 0

def evaluate(expr):
    global msg_id
    msg_id += 1
    ws.send(json.dumps({"id": msg_id, "method": "Runtime.evaluate",
                         "params": {"expression": expr, "returnByValue": True}}))
    while True:
        data = json.loads(ws.recv())
        if data.get("id") == msg_id:
            return data

# Fill form and submit
evaluate("document.getElementById('user').value = 'admin'")
evaluate("document.getElementById('password').value = 'secret123'")
evaluate("document.getElementById('submit').click()")
```

3. Wait for the page to load, then capture the window using `xwininfo` + `import` (same method as browser-screenshot uses internally).

### When to use CDP

- Filling login forms with leaked credentials (common for demonstrating impact)
- Navigating multi-step exploit flows
- Dismissing popups or dialogs before taking screenshots
- Any scenario where you need to interact with page elements

### Important notes

- **Do NOT use xdotool** for typing or clicking inside browser pages. It is unreliable for form input and breaks easily with different window managers, focus states, and coordinate systems.
- When connecting via CDP `/json` endpoint, filter targets by `type == "page"` and skip `chrome-extension://` URLs to find the actual page target.
- The `--disable-extensions` flag is already included in browser-screenshot, but some Chrome built-in extensions may still appear as targets.

## Important: No Fake Screenshots

**NEVER fabricate screenshots by constructing fake HTML pages, images, or any other artificial representation to simulate tool output.** If a vulnerability can only be reproduced via command-line tools (e.g., `curl --request-target`) and you cannot capture a real terminal screenshot, do NOT create an HTML page styled to look like terminal output and screenshot that. This is dishonest and misleading.

When you cannot take a genuine screenshot of the exploit result, simply skip the screenshot and clearly tell the user that this screenshot needs to be added manually. It is always better to have no screenshot than a fake one.

## Environment Requirements

All three scripts require a GNOME Wayland session with `mutter-x11-frames` running. They will NOT work in:

- Headless servers or Docker containers (no display)
- Non-GNOME desktop environments (KDE, Sway, etc.) — adapt the D-Bus/Xwayland environment detection
- Pure X11 sessions — use `import` or `scrot` directly instead
