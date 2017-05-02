import spark.servlet.SparkApplication;
import static spark.Spark.*;


public class IndexFilter implements SparkApplication {
    public void init()
    {
        get("/hello", (req, res) -> "Hello World");
    }
}
