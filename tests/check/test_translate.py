import os

# Directories that should be skipped as they are not vulnerability documentation
SKIP_DIRS = {'tests', 'base', '.git', '.github', '.cursor'}


def test_translate():
    """Test if every README.md has a corresponding README.zh-cn.md file"""
    missing_translations = []
    basedir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..'))
    
    # Walk through all directories
    for root, dirs, files in os.walk(basedir):
        # Skip directories that should be ignored
        dirs[:] = [d for d in dirs if d not in SKIP_DIRS]
        
        if 'README.md' in files:
            # Skip root README which doesn't need translation checking
            if os.path.samefile(root, basedir):
                continue
                
            # Check if translation exists
            if 'README.zh-cn.md' not in files:
                rel_path = os.path.relpath(root, basedir)
                missing_translations.append(rel_path)
    
    # Assert no missing translations
    assert len(missing_translations) == 0, f"Missing README.zh-cn.md translations in: {', '.join(missing_translations)}"

if __name__ == '__main__':
    test_translate()
