# Screenshot Guide

Every environment needs screenshots showing the exploit in action. This skill bundles scripts in `scripts/` to make capturing screenshots easy on GNOME Wayland (where standard screenshot tools often don't work from a terminal/SSH session).

## Browser Screenshots

For web-based vulnerabilities (the most common case), use `scripts/browser-screenshot` in this skill directory. It opens a URL in Chrome via Xwayland, waits for the page to load, and captures the full browser window including the address bar.

```bash
# Basic usage — captures the page and saves as 1.png
bash <skill-dir>/scripts/browser-screenshot -o <software>/<CVE-ID>/1.png http://localhost:8080

# Wait longer for slow-loading pages
bash <skill-dir>/scripts/browser-screenshot -w 15 -o 1.png http://localhost:8080/admin

# Custom window size
bash <skill-dir>/scripts/browser-screenshot -s 1920,1080 -o 1.png http://localhost:8080
```

Replace `<skill-dir>` with the actual path to this skill directory (typically `.claude/skills/add-vulhub-env`).

Dependencies: `google-chrome`, `xwininfo` (from `x11-utils`), `import` (from `imagemagick`). The script auto-detects the GNOME Wayland session environment.

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

## Which Script to Use

| Scenario | Script | Notes |
|----------|--------|-------|
| Web page screenshot | `browser-screenshot` | Best for web vulns — includes address bar, clean profile |
| Xwayland app window | `window-screenshot` | For apps using X11 compatibility |
| Pure Wayland app / full screen | `gnome-screenshot` | Uses portal API, sees all windows |
| Need to select a region interactively | `gnome-screenshot -i` | Opens GNOME's screenshot UI |

## Environment Requirements

All three scripts require a GNOME Wayland session with `mutter-x11-frames` running. They will NOT work in:

- Headless servers or Docker containers (no display)
- Non-GNOME desktop environments (KDE, Sway, etc.) — adapt the D-Bus/Xwayland environment detection
- Pure X11 sessions — use `import` or `scrot` directly instead
