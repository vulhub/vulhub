import java.rmi.Naming;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.List;

public class RemoteRMIServer {
    private void start() throws Exception {
        if (System.getSecurityManager() == null) {
            System.out.println("setup SecurityManager");
            System.setSecurityManager(new SecurityManager());
        }

        Calc h = new Calc();
        LocateRegistry.createRegistry(1099);
        Naming.rebind("refObj", h);
    }

    public static void main(String[] args) throws Exception {
        new RemoteRMIServer().start();
    }
}