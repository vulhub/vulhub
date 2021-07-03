import os
import re


ARCHIVE_FILE_PATTERN = re.compile(r'^.*\.(tar\.gz|zip|7z|rar|exe|jar|xz|gz|tar|war)$', re.I)
ARCHIVE_EXCEPTED = re.compile(r'[/\\](struts2|CVE-2016-9086|weblogic[/\\]weak_password)[/\\]')
LOWERCASE_EXT_EXCEPTED = re.compile(r'[/\\].pytest_cache[/\\]')


def test_filename():
    """
    We are not allowed uppercase software directory name
    """
    basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))
    for name in os.listdir(basedir) + os.listdir(os.path.join(basedir, 'base')):
        if not os.path.isdir(name):
            continue

        assert name.islower()

    for (now_dir, dirs, files) in os.walk(basedir):
        for name in files:
            fullname = os.path.join(now_dir, name)

            if LOWERCASE_EXT_EXCEPTED.search(fullname) is None:
                _, ext = os.path.splitext(name)
                assert ext == ext.lower(), 'file extension must be lowercase, not %r' % fullname

            if name.lower() == 'readme.md' or name.lower() == 'readme.zh-cn.md':
                assert name == 'README.md' or name == 'README.zh-cn.md', "README filename must be 'README.md' or 'README.zh-cn.md', not %r" % fullname

            if ARCHIVE_EXCEPTED.search(fullname) is None:
                assert ARCHIVE_FILE_PATTERN.match(name) is None, "You should not upload a archive file like %r" % fullname
