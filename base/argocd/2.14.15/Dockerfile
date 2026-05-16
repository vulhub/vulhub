FROM rancher/k3s:v1.29.1-k3s1

ARG TARGETARCH
ARG ARGOCD_VERSION=v2.14.15

WORKDIR /opt/k3s

ADD --chmod=755 https://github.com/argoproj/argo-cd/releases/download/${ARGOCD_VERSION}/argocd-linux-${TARGETARCH} /usr/local/bin/argocd

COPY argocd-install-v2.14.15.yaml ./argocd-install.yaml
COPY manifests ./manifests
COPY bootstrap.sh /usr/local/bin/bootstrap.sh
COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/bin/sh", "/entrypoint.sh"]

CMD ["k3s", "server", "--disable=traefik", "--disable=servicelb", "--disable=metrics-server", "--disable-network-policy"]
