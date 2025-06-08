import javax.jws.WebService;

@WebService(name = "Test", targetNamespace = "http://service.namespace/")
public interface Test {
    String test(Model model);
}