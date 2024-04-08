import os
import tomllib


def test_environments_toml():
    basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))
    with open(os.path.join(basedir, 'environments.toml'), 'rb') as f:
        data = tomllib.load(f)

    for env in data['environment']:
        assert 'name' in env
        assert 'cve' in env
        assert 'app' in env
        assert 'path' in env
        assert len(env) == 4

        assert isinstance(env['name'], str)
        assert isinstance(env['cve'], list)
        assert isinstance(env['app'], str)
        assert isinstance(env['path'], str)

        blocks = env['path'].split('/')
        assert len(blocks) == 2
