# examples/server_simple.py
from aiohttp import web

app = web.Application()
app.router.add_routes([
    web.static("/static", "static/", follow_symlinks=True),  # Remove follow_symlinks to avoid the vulnerability
])

if __name__ == '__main__':
    web.run_app(app)

# 访问 https://www.jetbrains.com/help/pycharm/ 获取 PyCharm 帮助
