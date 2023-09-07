import requests
import os
import sys
import pathlib
import logging
import yaml
from typing import Mapping, Iterable
from collections import defaultdict


logging.basicConfig(stream=sys.stdout, level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
auth_token = os.environ.get('TOKEN', '')
session = requests.session()
session.headers = {
    'Authorization': f'Bearer {auth_token}',
    'Origin': 'https://hub.docker.com',
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36',
    'X-DOCKER-API-CLIENT': 'docker-hub/v3012.0.0',
}
base = pathlib.Path(__file__).parent.parent.parent.absolute()
template = '''# Vulhub image for {placeholder_name}

## Image Description

Vulhub is an open-source collection of pre-built vulnerable docker environments. No pre-existing knowledge of docker is required, just execute two simple commands and you have a vulnerable environment.

This image is one of the environment of Vulhub project.

Please see the [USER MANUAL](https://github.com/vulhub/vulhub) from the Vulhub project to see more detail.

## Usage

Do not use this image directly, use it through [docker-compose](https://docs.docker.com/compose/).

```
docker-compose up -d
```

Following environments are using this image, you can find the `docker-compose.yml` file on these folders:

{placeholder_vulns_block}

## Quick reference

- **Maintained by** <br>[phith0n](https://github.com/phith0n) and other contributors from [Vulhub](https://github.com/vulhub)
- **Where to get help:** <br>[Github Issues](https://github.com/vulhub/vulhub/issues)
- **Which environments do this image be used:**<br>{placeholder_vulns_inline}

## License

Because Vulhub is packaged with other software, please refer to the software license for the software inside the Vulhub image.

Vulhub's own code is open source based on the [MIT license](https://github.com/vulhub/vulhub/blob/master/LICENSE).

As for any pre-built image usage, it is the image user's responsibility to ensure that any use of this image complies with any relevant licenses for all software contained within.
'''


def prepare_vulhub() -> Mapping:
    vulhub = defaultdict(list)
    for f in base.rglob("docker-compose.yml"):
        start = len(str(base.absolute())) + 1
        end = len(str(f.absolute())) - 18 - 1
        vuln_path = str(f.absolute())[start:end].replace('\\', '/')

        compose = yaml.safe_load(f.read_bytes())
        for service_name, array in compose['services'].items():
            if 'image' not in array:
                continue

            image_name = array['image']
            if ':' in image_name:
                image_name, _ = image_name.split(':')

            if image_name.startswith('vulhub/'):
                vulhub[image_name].append(vuln_path)

    return vulhub


def build_readme(name, vulns: Iterable):
    envs = []
    for vuln in vulns:
        envs.append(f'[{vuln}](https://github.com/vulhub/vulhub/tree/master/{vuln})')

    return template\
            .replace('{placeholder_name}', name)\
            .replace('{placeholder_vulns_block}', '\n'.join([f'- {v}' for v in envs]))\
            .replace('{placeholder_vulns_inline}', ', '.join(envs))


def list_all_repository():
    response = session.get('https://hub.docker.com/v2/repositories/vulhub?page_size=200&ordering=last_updated')
    data = response.json()
    if response.status_code != 200 or data.get('error', None):
        raise Exception('authentication error')

    for obj in data['results']:
        yield f"{obj['namespace']}/{obj['name']}"


def update_description(name, vulns):
    response = session.patch(f'https://hub.docker.com/v2/repositories/{name}/', json={
        'full_description': build_readme(name, vulns)
    }, headers={
        'Content-Type': 'application/json'
    })
    if response.status_code != 200:
        raise Exception(f'update readme for {name} failed, status code = {response.status_code}, response text = {response.text}')


def main():
    try:
        vuln = prepare_vulhub()
        for name in list_all_repository():
            update_description(name, vuln.get(name, []))
            logging.info("Success to update readme for %s", name)
    except Exception as e:
        logging.error("error: %r", e)


if __name__ == '__main__':
    main()
