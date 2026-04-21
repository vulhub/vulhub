#!/bin/bash
set -e

GATEWAY_TOKEN="${OPENCLAW_GATEWAY_TOKEN:-d4b7c7689b82981ff86c735a9f8f616b310491b0d334659a1491c55a13353e66}"

mkdir -p /root/.config/openclaw
cat > /root/.config/openclaw/openclaw.json5 <<EOF
{
  gateway: {
    mode: "local",
    bind: "lan",
    port: 18790,
    auth: {
      mode: "token",
      token: "$GATEWAY_TOKEN"
    },
    controlUi: {
      allowInsecureAuth: true
    }
  }
}
EOF

CONTROL_UI_INDEX="/usr/local/lib/node_modules/clawdbot/dist/control-ui/index.html"
if [ -f "$CONTROL_UI_INDEX" ] && ! grep -q 'vulhub-prefill' "$CONTROL_UI_INDEX"; then
    echo "[*] Injecting localStorage prefill into Control UI index.html..."
    TOKEN_ESCAPED=$(printf '%s' "$GATEWAY_TOKEN" | sed 's/[&/\`'"'"']/\\&/g')
    PREFILL_SNIPPET="<script id=\"vulhub-prefill\">(function(){try{var K='clawdbot.settings.v1';var c={};try{c=JSON.parse(localStorage.getItem(K)||'{}')}catch(e){}if(!c.token){c.token='${TOKEN_ESCAPED}';c.gatewayUrl=c.gatewayUrl||('ws://'+location.host);localStorage.setItem(K,JSON.stringify(c))}}catch(e){}})();</script>"
    sed -i "s#</head>#${PREFILL_SNIPPET}</head>#" "$CONTROL_UI_INDEX"
fi

echo "[*] Starting OpenClaw Gateway..."
node /usr/local/lib/node_modules/clawdbot/dist/index.js gateway --port 18790 --allow-unconfigured &
GATEWAY_PID=$!

echo "[*] Waiting for gateway to be ready..."
until curl -sf http://127.0.0.1:18790/healthz > /dev/null 2>&1; do
    sleep 2
done

echo "[*] Bridging 18789 -> 18790 for host access..."
socat TCP-LISTEN:18789,bind=0.0.0.0,reuseaddr,fork TCP:127.0.0.1:18790 &

echo "[+] OpenClaw Gateway is ready on port 18789"
echo "[+] Gateway token: $GATEWAY_TOKEN"
echo "[+] Control UI: http://your-ip:18789 (auto-login via localStorage prefill)"

wait $GATEWAY_PID
