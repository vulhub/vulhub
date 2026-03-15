import os
import subprocess

import pytest


def collect_dockerfiles():
    basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))

    dockerfiles = []
    for (now_dir, dirs, files) in os.walk(basedir):
        for name in files:
            if name in ('oracle-java', ):
                continue

            if name == 'Dockerfile':
                dockerfiles.append(os.path.join(now_dir, name))

    return dockerfiles


@pytest.mark.parametrize('dockerfile', collect_dockerfiles(), ids=lambda p: os.path.relpath(p))
def test_dockerfile_lint(dockerfile):
    basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))
    config = os.path.join(basedir, 'tests', 'hadolint.yaml')
    subprocess.run(['hadolint', '--config', config, '--failure-threshold', 'error', dockerfile], check=True)
