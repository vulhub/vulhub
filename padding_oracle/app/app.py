
from flask import Flask, render_template, request
from os import urandom
from Crypto.Cipher import DES

# author:https://github.com/bongtrop

app = Flask(__name__)

BLOCK_SIZE = 8

IV = "12344321"#urandom(BLOCK_SIZE)
KEY = "juytgdf2"#urandom(BLOCK_SIZE)

WL = ["dog.html", "cat.html", "rat.html"]#, "flagnaja.html"]

class PKCS7:
    def __init__(self, blocksize):
        self.bs = blocksize

    def pad(self, m):
        length = self.bs - (len(m) % self.bs)
        m += chr(length)*length
        return m

    def unpad(self, m):
        length = ord(m[-1])
        if not length: return None
        for i in range(1, length+1):
            if ord(m[-i]) != length:
                return None

        return m[:-length]

pkcs7 = PKCS7(BLOCK_SIZE)

def encrypt(m, iv=IV):
    cipher = DES.new(KEY, DES.MODE_CBC, iv)
    return iv + cipher.encrypt(pkcs7.pad(m))

def decrypt(c):
    #import pdb;pdb.set_trace()
    if len(c) % BLOCK_SIZE != 0 :
        return None

    iv = c[:BLOCK_SIZE]
    c = c[BLOCK_SIZE:]
    cipher = DES.new(KEY, DES.MODE_CBC, iv)
    return pkcs7.unpad(cipher.decrypt(c))

@app.route("/<c>")
def oracle(c):
    m = decrypt(c.decode("hex"))

    if m == None:
        return "decrypt error naja", 500
    if m not in WL:
        return "not found naja", 404

    return render_template(m)

@app.route("/")
def home():
    a = {l:encrypt(l).encode("hex") for l in WL}
    return render_template('index.html', a=a)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=1337)
