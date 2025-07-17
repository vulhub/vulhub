from aiohttp import web                        
async def index(request):             
    return web.Response(text="Hello, World!")             
             
app = web.Application()             
app.router.add_routes([             
    web.static("/static", "static/", follow_symlinks=True),             
])             
app.router.add_get('/', index)             
             
if __name__ == '__main__':             
    web.run_app(app, host='0.0.0.0', port=8080)   

