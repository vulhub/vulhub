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
echo "[+] Control UI: http://your-ip:18789 (paste the token above to log in)"

wait $GATEWAY_PID
