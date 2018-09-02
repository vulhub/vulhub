# Jupyter Notebook Unauthorized Access Vulnerability

Jupyter Notebook (formerly known as IPython notebook) is an interactive notebook that supports more than 40 programming languages.

If the administrator does not configure a password for Jupyter Notebook, it will result in an unauthorized access vulnerability where visitors can create a console and execute arbitrary Python code and commands.

## Environment running

Run the test environment:

```
Docker-compose up -d
```

After running, visit `http://your-ip:8888` and you will see the web management interface of Jupyter Notebook. There is no password required.

## Vulnerability recurrence

Select new -> terminal to create a console:

![](1.png)

Execute any command directly:

![](2.png)