import os
import re
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
        assert 'dockerfile' in env
        assert len(env) == 6

        assert len(env['tags']) > 0
        assert isinstance(env['name'], str)
        assert isinstance(env['cve'], list)
        assert isinstance(env['app'], str)
        assert isinstance(env['path'], str)
        assert isinstance(env['tags'], list)
        assert isinstance(env['dockerfile'], dict)
        assert os.path.exists(os.path.join(basedir, env['path']))

        for image, dockerfile_dir in env['dockerfile'].items():
            assert image.startswith('vulhub/'), f"Image {image} should start with 'vulhub/'"
            assert os.path.isfile(os.path.join(basedir, dockerfile_dir, 'Dockerfile')), f"Dockerfile not found at {dockerfile_dir}"

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


def test_dockerfile_covers_all_vulhub_images():
    """Every vulhub/* image in docker-compose.yml must have a corresponding entry in dockerfile field."""
    with open(os.path.join(basedir, 'environments.toml'), 'rb') as f:
        data = tomllib.load(f)

    for env in data['environment']:
        compose_path = os.path.join(basedir, env['path'], 'docker-compose.yml')
        with open(compose_path) as f:
            content = f.read()

        images_in_compose = set(re.findall(r'image:\s*(vulhub/\S+)', content))
        images_in_dockerfile = set(env.get('dockerfile', {}).keys())

        # Also check local Dockerfile for FROM vulhub/* (build: type envs)
        local_dockerfile = os.path.join(basedir, env['path'], 'Dockerfile')
        if os.path.isfile(local_dockerfile):
            with open(local_dockerfile) as f:
                for line in f:
                    m = re.match(r'\s*FROM\s+(vulhub/\S+)', line, re.IGNORECASE)
                    if m:
                        images_in_compose.add(m.group(1))

        missing = images_in_compose - images_in_dockerfile
        assert not missing, f"{env['path']}: vulhub images {missing} in docker-compose.yml but not in dockerfile field"
