#心 bleeding loophole (CVE-2014-0160)

Compile and run:

```
Docker-compose build
Docker-compose up -d
```

Visit `https://filippo.io/Heartbleed` for online testing:

![](1.png)

Python2 runs [ssltest.py](ssltest.py) and gets sensitive data (cookies):

![](2.png)