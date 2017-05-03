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
            ParserConfig config = new ParserConfig();

            Record obj = JSON.parseObject(data, Record.class, config, Feature.SupportNonPublicField);

            response.status(200);
            response.type("application/json");
            return "{\"status\": 200, \"info\": \"Hello " + obj.name + ", Your age is " + obj.age + "\"}";
        });
    }

    public static void main(String[] args)
    {
        IndexFilter i = new IndexFilter();
        i.init();
    }
}
