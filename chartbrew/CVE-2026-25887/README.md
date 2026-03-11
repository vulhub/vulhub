# Chartbrew MongoDB Dataset Query Remote Code Execution (CVE-2026-25887)

[中文版本(Chinese version)](README.zh-cn.md)

[Chartbrew](https://github.com/chartbrew/chartbrew) is an open-source reporting platform that connects to databases and APIs to build and share live dashboards.

In Chartbrew version 4.8.0, the `runMongo` function in `ConnectionController.js` passes user-supplied MongoDB dataset queries directly into a JavaScript `Function()` constructor without proper validation or sanitization. An authenticated user with permission to create datasets can inject arbitrary JavaScript code through the MongoDB query input, achieving remote code execution on the server in the context of the Node.js process. This vulnerability is fixed in version 4.8.1, which introduces AST-based query validation.

References:

- <https://github.com/chartbrew/chartbrew/security/advisories/GHSA-x4r6-prmw-7wvw>
- <https://nvd.nist.gov/vuln/detail/CVE-2026-25887>

## Environment Setup

Execute the following command to start Chartbrew 4.8.0:

```
docker compose up -d
```

After the server starts, visit `http://your-ip:4018` to access the Chartbrew web interface. The API server is available at `http://your-ip:4019`.

If you are running the environment on a remote server or VM, set the `CB_HOST` environment variable to the server's IP address so the frontend can reach the API correctly:

```
CB_HOST=your-ip docker compose up -d
```

## Vulnerability Reproduction

First, open the Chartbrew web interface and register a new account. The first registered user will become the administrator. After logging in, follow the setup wizard to create a new project. In "Step 2: Connect to your data source", select MongoDB and enter the connection string `mongodb://mongodb:27017/vulhub` pointing to the MongoDB service included in this environment.

![](1.png)

After saving the connection, navigate to "Datasets" and create a new dataset using the MongoDB connection. In the query editor, enter the following malicious query that breaks out of the intended MongoDB call chain and uses `child_process.execSync` to execute arbitrary commands:

```
version + (function(){ try { const r = global.process.mainModule.require('child_process'); return r.execSync('id').toString(); } catch(e) { return e.toString(); } })()
```

Click "Run query" and the result panel on the right will display the output of the `id` command, confirming that arbitrary commands can be executed on the server as root.

![](2.png)
