from flask import Flask, request
from jinja2 import Template

app = Flask(__name__)

@app.route("/")
def index():
    name = request.get.get('name')

    t = Template("Hello {{ %s }} " % name)
    return t.render()

if __name__ == "__main__":
    app.run()