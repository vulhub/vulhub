FROM python:3.13-slim

RUN apt-get update && apt-get install -y git && rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 https://github.com/Comfy-Org/ComfyUI.git ComfyUI
RUN git clone --branch 3.37 --depth 1 https://github.com/Comfy-Org/ComfyUI-Manager.git ComfyUI/custom_nodes/ComfyUI-Manager

WORKDIR /ComfyUI

RUN pip install --no-cache-dir -r requirements.txt

CMD ["python", "main.py", "--listen", "0.0.0.0", "--cpu"]
