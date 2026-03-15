package train.rmi.remote;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * @author wh1t3P1g
 * @since 2020/1/9
 */
public interface IRemoteHello extends Remote {

    String sayHello() throws RemoteException;
}
