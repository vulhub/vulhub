import spark.servlet.SparkApplication;
import static spark.Spark.*;

import com.alibaba.fastjson.*;
import com.alibaba.fastjson.parser.Feature;
import com.alibaba.fastjson.parser.ParserConfig;


public class IndexFilter implements SparkApplication {
    public void init()
    {
        get("/", (req, res) -> "Hello World");

        post("/", (request, response) -> {
            String data = request.body();

            JSONObject obj = JSON.parseObject(data, Feature.SupportNonPublicField);

            JSONObject ret = new JSONObject();
            ret.put("success", 200);
            ret.put("data", "Hello " + obj.get("name") + ", Your age is " + obj.get("age"));

            response.status(200);
            response.type("application/json");
            return ret.toJSONString();
        });
    }

    public static void main(String[] args)
    {
        IndexFilter i = new IndexFilter();
        i.init();
    }
}
