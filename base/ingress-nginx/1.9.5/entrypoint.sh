#!/bin/sh

# Start setup operations in the background
(
  echo "[INFO] Waiting for Kubernetes API to be ready..."
  until kubectl get nodes > /dev/null 2>&1; do
    sleep 1
    echo "[INFO] Waiting for Kubernetes API to be ready..."
  done

  echo "[INFO] Setting ingress-nginx as default namespace..."
  kubectl config set-context --current --namespace=ingress-nginx

  echo "[INFO] Waiting for ingress-nginx controller..."
  kubectl wait --for=condition=available --timeout=600s deployment/ingress-nginx-controller

  kubectl get pods
) &

# Execute the command passed to the entrypoint
exec "$@"
