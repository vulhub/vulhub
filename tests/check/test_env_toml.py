import os
import glob
import tomllib
import difflib

basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))


def test_toml_format():
    with open(os.path.join(basedir, 'environments.toml'), 'rb') as f:
        data = tomllib.load(f)

    for env in data['environment']:
        assert 'name' in env
        assert 'cve' in env
        assert 'app' in env
        assert 'path' in env
        assert 'tags' in env
        assert len(env) == 5

        assert len(env['tags']) > 0
        assert isinstance(env['name'], str)
        assert isinstance(env['cve'], list)
        assert isinstance(env['app'], str)
        assert isinstance(env['path'], str)
        assert isinstance(env['tags'], list)
        assert os.path.exists(os.path.join(basedir, env['path']))

        blocks = env['path'].split('/')
        assert len(blocks) == 2

        assert len(data['tags']) > 0
        for tag in env['tags']:
            assert tag in data['tags']


def test_environments_files():
    with open(os.path.join(basedir, 'environments.toml'), 'rb') as f:
        data = tomllib.load(f)

    compose_files = [name.replace('\\', '/') for name in sorted(glob.glob("**/docker-compose.yml", recursive=True))]
    env_files = []
    for env in data['environment']:
        files = os.listdir(os.path.join(basedir, env['path']))
        assert 'README.md' in files, f"README.md not found in {env['path']}"
        assert 'README.zh-cn.md' in files, f"README.zh-cn.md not found in {env['path']}"
        assert 'docker-compose.yml' in files, f"docker-compose.yml not found in {env['path']}"
        env_files.append(env['path'] + "/docker-compose.yml")

    assert len(compose_files) == len(env_files), f"Do not forget to add new environment in environments.toml, difference: \n{'\n'.join(difflib.unified_diff(compose_files, env_files))}"
