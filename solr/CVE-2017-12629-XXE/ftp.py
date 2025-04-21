#!/usr/bin/env python3
import socketserver


class MyTCPHandler(socketserver.BaseRequestHandler):

    def handle(self):
        # self.request is the TCP socket connected to the client
        self.request.send(b'220 xxe-ftp-server\r\n')
        self.communicating = True
        while self.communicating:
            cmd = self.request.recv(1024)
            if len(cmd) == 0:
                break

            cmd = cmd.decode().rstrip()
            print("> " + cmd)
            if cmd.split(' ', 1)[0] == 'USER':
                self.request.send(b'331 password please - version check\r\n')
            else:
                self.request.send(b'230 more data please!\r\n')
        

if __name__ == "__main__":
    HOST, PORT = "0.0.0.0", 2121

    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)
    server.serve_forever()
