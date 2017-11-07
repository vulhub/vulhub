const express = require('express')
const app = express()
const path = require('path')

app.get('/', (req, res) => {
    res.send(`<html>
                <head>
                    <meta charset="utf-8">
                    <title>Hello vulhub!</title>
                </head>
                <body>
                    <div id="app">
                        <input v-model="name">
                        <p>Hello {{ name }}</p>
                    </div>
                <script src="//cdn.bootcss.com/vue/2.4.4/vue.min.js"></script>
                <script src="/static/main.js"></script>
                </body>
            </html>`)
})

app.use('/static', express.static(path.join(__dirname, 'static')));

app.listen(3000, () => console.log('Example app listening on port 3000!'))