import os

# Directories that should be skipped as they are not vulnerability documentation
SKIP_DIRS = {'.pytest_cache', 'gitlist/0.6.0-rce', 'struts2', 'fastjson/vuln', 'openssl/heartbleed', 'base/weblogic/12.2.1.3', 'base/httpd/bad-http'}


def is_skip_dir(dir_path):
    dir_path = dir_path.replace('\\', '/')
    for skip in SKIP_DIRS:
        if dir_path.endswith("/" + skip):
            return True

    return False


def test_translate():
    """Test if every README.md has a corresponding README.zh-cn.md file"""
    missing_translations = []
    basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))
    
    # Walk through all directories
    for root, _, files in os.walk(basedir):
        if 'README.md' in files:
            if is_skip_dir(root):
                continue
                
            # Check if translation exists
            if 'README.zh-cn.md' not in files:
                rel_path = os.path.relpath(root, basedir)
                missing_translations.append(rel_path)
    
    # Assert no missing translations
    assert len(missing_translations) == 0, f"Missing README.zh-cn.md translations in: {', '.join(missing_translations)}"

if __name__ == '__main__':
    test_translate()
