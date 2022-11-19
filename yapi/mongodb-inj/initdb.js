const { MongoClient } = require("mongodb");

const url = `mongodb://root:root@${process.env.MONGO_ADDR}/?authSource=admin`;
MongoClient.connect(url, async function(err, client) {
    const database = client.db("yapi");

    const user = await database.collection("user").findOne();
    const temp = await database.collection("project").findOne();
    if (temp) {
        console.log("database has already been initialized");
        client.close();
        return
    }

    const baseid = 66;
    await database.collection("group").insertOne({
        "_id": baseid,
        "custom_field1": {
          "enable": false
        },
        "type": "private",
        "uid": user._id,
        "group_name": "User-11",
        "add_time": parseInt(Date.now()/1000),
        "up_time": parseInt(Date.now()/1000),
        "members": [],
        "__v": 0
    });
    await database.collection("project").insertOne({
        "_id": baseid,
        "switch_notice": true,
        "is_mock_open": false,
        "strice": false,
        "is_json5": false,
        "name": "vulhub",
        "basepath": "",
        "members": [],
        "project_type": "private",
        "uid": user._id,
        "group_id": baseid,
        "icon": "code-o",
        "color": "purple",
        "add_time": parseInt(Date.now()/1000),
        "up_time": parseInt(Date.now()/1000),
        "env": [
          {
            "header": [],
            "name": "local",
            "domain": "http://127.0.0.1",
            "global": []
          }
        ],
        "tag": [],
        "__v": 0
    });
    await database.collection("interface_cat").insertOne({
        "_id": baseid,
        "index": 0,
        "name": "公共分类",
        "project_id": baseid,
        "desc": "公共分类",
        "uid": user._id,
        "add_time": parseInt(Date.now()/1000),
        "up_time": parseInt(Date.now()/1000),
        "__v": 0,
    })
    await database.collection("interface_col").insertOne({
        "_id": baseid,
        "checkResponseField": {
          "name": "code",
          "value": "0",
          "enable": false
        },
        "checkScript": {
          "enable": false
        },
        "index": 0,
        "test_report": "{}",
        "checkHttpCodeIs200": false,
        "checkResponseSchema": false,
        "name": "公共测试集",
        "project_id": baseid,
        "desc": "公共测试集",
        "uid": user._id,
        "add_time": parseInt(Date.now()/1000),
        "up_time": parseInt(Date.now()/1000),
        "__v": 0,
    })
    await database.collection("interface").insertOne({
        "_id": baseid,
        "edit_uid": 0,
        "status": "undone",
        "type": "static",
        "req_body_is_json_schema": false,
        "res_body_is_json_schema": false,
        "api_opened": false,
        "index": 0,
        "tag": [],
        "method": "GET",
        "catid": baseid,
        "title": "sample",
        "path": "/",
        "project_id": baseid,
        "req_params": [],
        "res_body_type": "json",
        "query_path": {
          "path": "/",
          "params": []
        },
        "uid": user._id,
        "add_time": parseInt(Date.now()/1000),
        "up_time": parseInt(Date.now()/1000),
        "req_query": [],
        "req_headers": [],
        "req_body_form": [],
        "__v": 0,
    })
    await database.collection("interface_case").insertOne({
        "_id": baseid,
        "index": 0,
        "mock_verify": false,
        "enable_script": false,
        "uid": 11,
        "add_time": parseInt(Date.now()/1000),
        "up_time": parseInt(Date.now()/1000),
        "project_id": baseid,
        "col_id": baseid,
        "interface_id": baseid,
        "casename": "sample",
        "req_params": [],
        "req_headers": [],
        "req_query": [],
        "req_body_form": [],
        "__v": 0
    })
    await database.collection("token").insertOne({
        "_id": baseid,
        "project_id": baseid,
        "token": "1cae15606ea4b223b01a",
        "__v": 0,
    })
    await database.collection("identitycounters").updateMany({field: "_id"}, {$set: {count: baseid}})

    console.log("finish database initialization");
    client.close()
})
