# Python unpickle Causes arbitrary command execution vulnerabilities

## Principle

Reference article:

  - http://rickgray.me/2015/09/12/django-command-execution-analysis.html
  - https://www.leavesongs.com/PENETRATION/zhangyue-python-web-code-execute.html

## Testing

Compile and run the test environment:

```
Docker-compose build
Docker-compose up -d
```

Visit `http://your-ip:8000` and display `Hello {username}!`. Username is the cookie variable user, the "username" variable in the object restored by base64 decoding + deserialization, the default is "Guest", pseudo code: `pickle_decode(base64_decode(cookie['user']))[ 'username'] or 'Guest'`.

Call exp.py to bounce the shell:

![](1.png)