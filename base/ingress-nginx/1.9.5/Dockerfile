FROM rancher/k3s:v1.29.1-k3s1

WORKDIR /opt/k3s

COPY ingress-nginx-v1.9.5-deploy.yaml deploy.yaml
COPY entrypoint.sh entrypoint.sh

RUN echo "[INFO] Starting K3s server..." && \
		nohup k3s server  k3s.log 2>&1 & \
		echo "[INFO] Waiting for Kubernetes API to be ready..." && \
		until kubectl get nodes > /dev/null 2>&1; do sleep 1 ;done && \
		echo "[INFO] Installing ingress-nginx..." && \
		kubectl apply -f deploy.yaml &&  \
        echo "[INFO] Waiting for ingress-nginx controller..." && \
		kubectl wait --for=condition=available --timeout=600s deployment/ingress-nginx-controller -n ingress-nginx 

ENTRYPOINT ["/bin/sh", "entrypoint.sh"]

CMD ["k3s", "server"]
