import os
import re


basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))
ARCHIVE_FILE_PATTERN = re.compile(r'^.*\.(tar\.gz|zip|7z|rar|exe|jar|xz|gz|tar|war)$', re.I)
ARCHIVE_EXCEPTED = re.compile(r'[/\\](struts2|weblogic[/\\]weak_password)[/\\]')


def test_dir_islower():
    for name in os.listdir(basedir) + os.listdir(os.path.join(basedir, 'base')):
        if not os.path.isdir(name):
            continue

        assert name.islower()


def test_filename_format():
    """
    We are not allowed uppercase software directory name
    """    
    for (root, _, files) in os.walk(basedir):
        if os.path.basename(root).startswith('.'):
            continue

        for name in files:
            # check if extension is lowercase
            fullname = os.path.join(root, name)
            _, ext = os.path.splitext(name)
            assert ext == ext.lower(), 'file extension must be lowercase, not %r' % name

            # check if docker-compose.yaml is used
            assert name != "docker-compose.yaml", "docker-compose.yaml is not allowed, use docker-compose.yml instead"

            # check if readme file name is correct
            if name.lower() == 'readme.md':
                assert name == 'README.md', "README filename must be 'README.md', not %r" % name

            # check if readme.zh-cn.md file name is correct
            if name.lower() == 'readme.zh-cn.md':
                assert name == 'README.zh-cn.md', "README.zh-cn filename must be 'README.zh-cn.md', not %r" % name

            if os.path.isdir(fullname) and (name.lower().startswith('cve-') or name.lower().startswith('cnvd-') or name.lower().startswith('cnnvd-')):
                assert name == name.upper(), "CVE/CNVD/CNNVD directory name must be uppercase, not %r" % name

            # check if archive file size is lower than 4096 bytes
            if ARCHIVE_FILE_PATTERN.match(name) is not None and ARCHIVE_EXCEPTED.search(fullname) is None:
                assert os.path.getsize(fullname) <= 4096, "You should not upload a archive file larger than 4096 bytes"
