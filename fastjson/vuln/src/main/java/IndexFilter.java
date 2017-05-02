import spark.servlet.SparkApplication;
import static spark.Spark.*;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.parser.Feature;
import com.alibaba.fastjson.parser.ParserConfig;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;


public class IndexFilter implements SparkApplication {
    public void init()
    {
        get("/hello", (req, res) -> "Hello World");

        post("/", (request, response) -> {
            String data = request.queryParams("data");
            ParserConfig config = new ParserConfig();

            Object obj = JSON.parseObject(data, Object.class, config, Feature.SupportNonPublicField);
            return "Hello" ;
        });
    }
}
