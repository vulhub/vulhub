#!/bin/sh
# Bootstrap Argo CD on first container start. Assumes k3s is already
# running (entrypoint.sh starts it via the container CMD) and the
# Kubernetes API is reachable.
set -eu

log() { echo "[bootstrap] $*"; }

log "Installing Argo CD v2.14.15 manifests"
kubectl create namespace argocd 2>/dev/null || true
kubectl apply -n argocd -f /opt/k3s/argocd-install.yaml

log "Exposing argocd-server via NodePort 30443 (HTTPS)"
kubectl apply -f /opt/k3s/manifests/02-argocd-server-svc-nodeport.yaml

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
ADMIN_PASS=$(kubectl -n argocd get secret argocd-initial-admin-secret -o jsonpath='{.data.password}' | base64 -d)
echo "$ADMIN_PASS" > /etc/argocd-admin-password

log "Waiting for argocd-server to accept logins"
i=0
until argocd login 127.0.0.1:30443 \
    --username admin --password "$ADMIN_PASS" \
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
log "Logged in as admin"

log "Creating AppProject 'myproject' and helm repository Secret"
kubectl apply -f /opt/k3s/manifests/03-app-project.yaml
kubectl apply -f /opt/k3s/manifests/04-helm-repo-secret.yaml

log "Generating project role token (project-automation-role)"
argocd proj role create-token myproject project-automation-role -t > /etc/argocd-demo-token
log "Token length: $(wc -c < /etc/argocd-demo-token) bytes"

log "bootstrap.sh complete"
