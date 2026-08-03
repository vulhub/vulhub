#!/bin/sh
set -u

STATE_DIR=/var/lib/vulhub-argocd
INIT_MARKER=$STATE_DIR/initialized

mkdir -p /var/log "$STATE_DIR"

log() { echo "[entrypoint] $*"; }

# Run the bootstrapper in the background once Kubernetes is ready.
# On first start it installs Argo CD and provisions the demo project /
# repo / token. On subsequent starts it is a no-op (marker file present).
(
  if [ -f "$INIT_MARKER" ]; then
    log "Already initialised; skipping bootstrap."
    exit 0
  fi

  log "Waiting for Kubernetes API to be ready..."
  until kubectl get nodes >/dev/null 2>&1; do sleep 1; done
  log "Kubernetes API is ready."

  log "Running first-time Argo CD bootstrap (this takes several minutes)..."
  if /bin/sh /usr/local/bin/bootstrap.sh; then
    touch "$INIT_MARKER"
    log "Bootstrap complete. Credentials in /etc/argocd-*-password and /etc/argocd-ci-sync-bot-token."
  else
    log "ERROR: bootstrap failed; see output above."
  fi
) &

exec "$@"
