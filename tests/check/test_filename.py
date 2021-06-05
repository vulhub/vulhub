import os


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
            if name.lower() == 'readme.md' or name.lower() == 'readme.zh-cn.md':
                assert name == 'README.md' or name == 'README.zh-cn.md', f"README filename must be 'README.md' or 'README.zh-cn.md', not {now_dir} / {name}"
