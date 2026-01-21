import org.apache.cxf.jaxws.JaxWsServerFactoryBean;

public class ServerStarter {
    public static void main(String[] args) {
        JaxWsServerFactoryBean factory = new JaxWsServerFactoryBean();
        factory.setServiceClass(Test.class);
        factory.setAddress("http://0.0.0.0:8080/test");
        factory.setServiceBean(new TestImpl());
        factory.create();
        System.out.println("Webservice: http://localhost:8080/test");
        System.out.println("Webservice WSDL: http://localhost:8080/test?wsdl");
    }
}