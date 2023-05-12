const Koa = require('koa')
const { Client } = require('pg')

const app = new Koa()
const client = new Client({
    user: "postgres",
    password: "postgres",
    database: "example",
    host: "db",
    port: 5432
})
client.connect()

app.use(async ctx => {
    ctx.response.type = 'html'

    let id = ctx.request.query.id || 1
    let sql = `SELECT * FROM "user" WHERE "id" = ${id}`
    const res = await client.query(sql)

    ctx.body = `<html>
                    <body>
                        <table>
                            <tr><th>id</th><td>${res.rows[0].id}</td></tr>
                            <tr><th>name</th><td>${res.rows[0].name}</td></tr>
                            <tr><th>score</th><td>${res.rows[0].score}</td></tr>
                        </table>
                    </body>
                </html>`
})

app.listen(3000)