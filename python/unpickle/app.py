import pickle
import base64
from flask import Flask, request

app = Flask(__name__)

@app.route("/")
@app.route("/<int:version>")
def index(version=3):
    try:
        user = base64.b64decode(request.cookies.get('user'))
        user = pickle.loads(user, protocol=version)
        username = user["username"]
    except:
        username = "Guest"

    return "Hello %s" % username

if __name__ == "__main__":
    app.run()