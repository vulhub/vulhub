# Nginx Configuration Error Causes Vulnerability

## Running the test environment

```
Docker-compose up -d
```

After running successfully, Nginx will listen to the three ports 8080/8081/8082, corresponding to three vulnerabilities.

## Mistake 1. CRLF Injection Vulnerability

Nginx will decode `$uri`, which will cause a newline character to be introduced by passing %0a%0d, causing a CRLF injection vulnerability.

Wrong configuration file example (original purpose is to make http request jump to https):

```
Location / {
Return 302 https://$host$uri;
}
```

Payload: `http://your-ip:8080/%0a%0dSet-Cookie:%20a=1`, can be injected into the Set-Cookie header.

An XSS vulnerability can be constructed using the techniques in [Bottle HTTP Header Injection Exploitation] (https://www.leavesongs.com/PENETRATION/bottle-crlf-cve-2016-9964.html):

![](1.png)

## Mistake 2. Directory Traversal Vulnerability

When Nginx configures an alias (Alias), if you forget to add `/`, it will cause a directory traversal vulnerability.

Wrong configuration file example (originally intended to give users access to files in the /home/ directory):

```
Location /files {
Alias ​​/home/;
}
```

Payload: `http://your-ip:8081/files../` , successfully traversed to the root directory:

![](2.png)

## Mistake 3. add_header is overwritten

The `add_header` in the Nginx configuration file sub-block (server, location, if) will overwrite the HTTP header added by `add_header` in the parent block, causing some security risks.

As the following code, the entire station (in the parent block) has added a CSP header:

```
Add_header Content-Security-Policy "default-src 'self'";
Add_header X-Frame-Options DENY;

Location = /test1 {
    Rewrite ^(.*)$ /xss.html break;
}

Location = /test2 {
    Add_header X-Content-Type-Options nosniff;
    Rewrite ^(.*)$ /xss.html break;
}
```

However, the `X-Content-Type-Options` header has been added to the location of `/test2`, causing all the `add_header`s in the parent block to be invalid:

![](3.png)

XSS can be triggered:

![](4.png)