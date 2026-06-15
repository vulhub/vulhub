#!/bin/sh
# Bootstrap Argo CD on first container start. Assumes k3s is already
# running (entrypoint.sh starts it via the container CMD) and the
# Kubernetes API is reachable.
set -eu

log() { echo "[bootstrap] $*"; }

ARGOCD_INSTALL_URL="https://raw.githubusercontent.com/argoproj/argo-cd/v2.14.15/manifests/install.yaml"

ADMIN_PASSWORD="${ARGOCD_ADMIN_PASSWORD:-password22}"
DEVELOPER_PASSWORD="${ARGOCD_DEVELOPER_PASSWORD:-hunter22}"

log "Installing Argo CD v2.14.15 from upstream: $ARGOCD_INSTALL_URL"
kubectl create namespace argocd 2>/dev/null || true
kubectl apply -n argocd -f "$ARGOCD_INSTALL_URL"

log "Exposing argocd-server via NodePort 30443 (HTTPS)"
kubectl apply -f /opt/k3s/manifests/01-argocd-server-svc-nodeport.yaml

log "Waiting for argocd-server deployment to appear"
until kubectl -n argocd get deploy argocd-server >/dev/null 2>&1; do sleep 1; done

log "Waiting for argocd-server and argocd-repo-server to become available"
DEADLINE=$(( $(date +%s) + 900 ))
while :; do
  NOW=$(date +%s)
  if [ "$NOW" -ge "$DEADLINE" ]; then
    log "ERROR: timeout waiting for argocd deployments"
    kubectl -n argocd get pods -o wide
    exit 1
  fi

  SERVER_READY=$(kubectl -n argocd get deploy argocd-server -o jsonpath='{.status.availableReplicas}' 2>/dev/null || echo 0)
  REPO_READY=$(kubectl -n argocd get deploy argocd-repo-server -o jsonpath='{.status.availableReplicas}' 2>/dev/null || echo 0)
  if [ "${SERVER_READY:-0}" -ge 1 ] && [ "${REPO_READY:-0}" -ge 1 ]; then
    log "argocd-server and argocd-repo-server are available"
    break
  fi

  log "--- pods @ $(( DEADLINE - NOW ))s remaining ---"
  kubectl -n argocd get pods --no-headers 2>/dev/null | \
    awk '{printf "    %-50s %s %s\n", $1, $2, $3}'
  sleep 15
done

log "Waiting for initial admin secret"
until kubectl -n argocd get secret argocd-initial-admin-secret >/dev/null 2>&1; do sleep 1; done
INITIAL_ADMIN_PASS=$(kubectl -n argocd get secret argocd-initial-admin-secret -o jsonpath='{.data.password}' | base64 -d)

log "Waiting for argocd-server to accept logins"
i=0
until argocd login 127.0.0.1:30443 \
    --username admin --password "$INITIAL_ADMIN_PASS" \
    --insecure >/dev/null 2>&1; do
  i=$((i + 1))
  if [ "$i" -ge 60 ]; then
    log "ERROR: argocd-server never came up"
    kubectl -n argocd get pods
    kubectl -n argocd logs deploy/argocd-server --tail=80 || true
    exit 1
  fi
  sleep 2
done
log "Logged in as admin (initial random password)"

log "Resetting admin password to configured value"
argocd account update-password \
  --account admin \
  --current-password "$INITIAL_ADMIN_PASS" \
  --new-password "$ADMIN_PASSWORD"
echo "$ADMIN_PASSWORD" > /etc/argocd-admin-password

log "Re-logging in as admin with configured password"
argocd login 127.0.0.1:30443 \
  --username admin --password "$ADMIN_PASSWORD" \
  --insecure >/dev/null

log "Creating AppProject 'gitops-production' and repository Secrets"
kubectl apply -f /opt/k3s/manifests/02-app-project.yaml
kubectl apply -f /opt/k3s/manifests/03-repos.yaml

log "Creating demo Applications (tiered visibility for admin vs developer)"
kubectl apply -f /opt/k3s/manifests/04-applications.yaml

log "Generating CI sync-bot token (proj:gitops-production:ci-sync-bot)"
argocd proj role create-token gitops-production ci-sync-bot -t > /etc/argocd-ci-sync-bot-token
log "Token length: $(wc -c < /etc/argocd-ci-sync-bot-token) bytes"

log "Enabling local user 'developer' (read-only)"
kubectl -n argocd patch configmap argocd-cm --type merge \
  -p '{"data":{"accounts.developer":"login"}}'
kubectl apply -f /opt/k3s/manifests/05-developer-rbac.yaml

log "Restarting argocd-server so the new account is picked up"
kubectl -n argocd rollout restart deploy/argocd-server
kubectl -n argocd rollout status deploy/argocd-server --timeout=180s

log "Re-logging in as admin after restart"
i=0
until argocd login 127.0.0.1:30443 \
    --username admin --password "$ADMIN_PASSWORD" \
    --insecure >/dev/null 2>&1; do
  i=$((i + 1))
  if [ "$i" -ge 60 ]; then
    log "ERROR: argocd-server never recovered after restart"
    exit 1
  fi
  sleep 2
done

log "Setting password for local user 'developer'"
argocd account update-password \
  --account developer \
  --current-password "$ADMIN_PASSWORD" \
  --new-password "$DEVELOPER_PASSWORD"
echo "$DEVELOPER_PASSWORD" > /etc/argocd-developer-password

log "Lab credentials summary:"
log "  admin / $ADMIN_PASSWORD          (also written to /etc/argocd-admin-password)"
log "  developer / $DEVELOPER_PASSWORD  (also written to /etc/argocd-developer-password)"
log "  ci-sync-bot token                (written to /etc/argocd-ci-sync-bot-token)"

log "bootstrap.sh complete"
