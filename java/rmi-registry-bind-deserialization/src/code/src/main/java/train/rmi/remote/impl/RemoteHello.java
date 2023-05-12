package train.rmi.remote.impl;

import train.rmi.remote.IRemoteHello;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

/**
 * @author wh1t3P1g
 * @since 2020/1/9
 */
public class RemoteHello extends UnicastRemoteObject implements IRemoteHello {
    public RemoteHello() throws RemoteException {
        super();
    }

    @Override
    public String sayHello() throws RemoteException {
        return "hello from RMI Server";
    }
}
