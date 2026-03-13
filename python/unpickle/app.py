import pickle
import base64
from flask import Flask, request

app = Flask(__name__)

@app.route("/")
def index():
    try:
        user_data = request.cookies.get('user')
        user = base64.b64decode(user_data)
        user = pickle.loads(user)
        username = user
        # print(username)
    except Exception as e:
        print(e)
        username = "Guest"

    return "Hello %s" % username

if __name__ == "__main__":
    app.run()