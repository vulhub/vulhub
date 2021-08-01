import smtpd
import asyncore,sys,time

class CustomSMTPServer(smtpd.SMTPServer):
    
    def process_message(self, peer, mailfrom, rcpttos, data, **kwargs):
        r = data.decode("utf-8").split("\n")
        for l in r:
            if l.startswith("Subject:"):
                sys.stdout.write("[{0}] {1}\n".format(time.time(),l))
        sys.stdout.flush()
        return


# server = smtpd.DebuggingServer(('0.0.0.0', 1025), None)
server = CustomSMTPServer(('0.0.0.0', 1025), None)

sys.stdout.write("[+] Start SMTPServer on 0.0.0.0:1025\n")
sys.stdout.flush()

asyncore.loop()