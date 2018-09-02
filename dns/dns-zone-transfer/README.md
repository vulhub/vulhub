#DNS Domain Delivery Vulnerability

The DNS protocol supports the use of axfr type records for area transfer to solve the problem of master-slave synchronization. If the administrator does not restrict the source of the record when configuring the DNS server, it will cause a DNS domain transfer vulnerability.

## Environment Building

Vulhub uses bind9 to build a dns server, but it does not mean that only bind9 supports axfr records. Run the DNS server:

```
Docker-compose up -d
```

After the environment is running, it will listen to port 53 of TCP and UDP. The DNS protocol also supports data transmission from these two ports. Because it involves a port number below 1024, running the above command may require root privileges.

## Vulnerability recurrence

Under Linux, we can use the dig command to send dns requests. For example, we can use `dig @your-ip www.vulhub.org` to get the A record for the domain name `www.vulhub.org` on the target dns server:

![](1.png)

Send a dns request of type axfr: `dig @your-ip -t axfr vulhub.org`

![](2.png)

Visible, I got all the subdomain records of `vulhub.org`, there is a DNS domain delivery vulnerability.

We can also use the nmap script to scan for this vulnerability: `nmap --script dns-zone-transfer.nse --script-args "dns-zone-transfer.domain=vulhub.org" -Pn -p 53 your-ip`

![](3.png)