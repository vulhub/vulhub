FROM  rancher/k3s:v1.29.1-k3s1

COPY ingress-nginx-v1.9.5-deploy.yaml deploy.yaml

RUN echo "[INFO] Starting K3s server..." && \
		nohup /bin/k3s server  k3s.log 2>&1 & \
		echo "[INFO] Waiting for Kubernetes API to be ready..." && \
		until /bin/kubectl get nodes > /dev/null 2>&1; do sleep 1 ;done && \
		echo "[INFO] Installing ingress-nginx..." && \
		/bin/kubectl apply -f deploy.yaml &&  \
        echo "[INFO] Waiting for ingress-nginx controller..." && \
		/bin/kubectl wait --for=condition=available --timeout=600s deployment/ingress-nginx-controller -n ingress-nginx 



