FROM python:3.13-slim

RUN apt-get update \
    && apt-get install -y git \
    && rm -rf /var/lib/apt/lists/*

RUN git clone --branch v0.9.2 --depth 1 https://github.com/Comfy-Org/ComfyUI.git ComfyUI
RUN git clone --branch 3.37 --depth 1 https://github.com/Comfy-Org/ComfyUI-Manager.git ComfyUI/custom_nodes/ComfyUI-Manager

WORKDIR /ComfyUI

# Install CPU-only PyTorch (much smaller than GPU version)
# This is sufficient for vulnerability testing without running actual AI models
RUN pip install --no-cache-dir \
    --extra-index-url https://download.pytorch.org/whl/cpu \
    torch torchvision torchaudio

RUN pip install --no-cache-dir -r requirements.txt \
    && pip install --no-cache-dir -r custom_nodes/ComfyUI-Manager/requirements.txt \
    && pip install --no-cache-dir requests

CMD ["python", "main.py", "--listen", "0.0.0.0", "--cpu"]
