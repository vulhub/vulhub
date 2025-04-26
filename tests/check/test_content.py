import re
import os

FILE_EXCLUDE_PATTERN = re.compile(r'[/\\]\.(git|idea|vscode|pytest_cache)[/\\]')


def is_textplain(data: bytes):
    return b'\x00' not in data


def test_content():
    basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))
    for (now_dir, dirs, files) in os.walk(basedir):
        for name in files:
            filename = os.path.join(now_dir, name)
            if FILE_EXCLUDE_PATTERN.search(filename):
                continue

            with open(filename, 'rb') as f:
                data = f.read()

            if not is_textplain(data):
                continue

            assert b'\r\n' not in data, f'CRLF must be convert to LF for Vulhub files, but {filename} did not'
