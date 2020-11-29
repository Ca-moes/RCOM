![Exp4](./imgs/exp4.png)

# Steps
1. Configure commercial router RCand connect it (no NAT) to the  lab network  (172.16.1.0/24)&ast;
2. Verify routes
    - tuxy4 as default router of tuxy3; Rcas default router for tuxy2 and tuxy4
    - Routes for 172.16.y0.0/24 in tuxy2 and Rc
3. Using ping commands and wireshark, verify if tuxy3 can ping all the network interfaces of tuxy4, tuxy2 and Rc
4. In  tuxy2
    - Do:  echo 0 > /proc/sys/net/ipv4/conf/eth0/accept_redirects and  echo 0 > /proc/sys/net/ipv4/conf/all/accept_redirects
    - remove the route to 172.16.y0.0/24 via tuxy4
    - In tuxy2, ping  tuxy3
    - Using capture at tuxy2, try to understand the path followed by ICMP ECHO and  ECHO-REPLY packets  (look at MAC addresses)
    - In tuxy2, do traceroute tuxy3
    - In tuxy2, add again the route to 172.16.y0.0/24 via tuxy4 and  do  traceroute tuxy3
    - Activate the acceptance of  ICMP redirect at tuxy2 when there is no route to 172.16.y0.0/24 via tuxy4 and try to understand  what happens
5. In tuxy3, ping the router of the lab I.321 (172.16.1.254)** and try to understand what  happens
6. Add NAT functionality to router Rc
7. In tuxy3 ping 172.16.1.254&ast;&ast;, verify if there is connectivity, and try to understand what happens
   
&ast; In room I320 the lab network address is 172.16.2.0/24

&ast;&ast; Router of  lab I320 has IP address 172.16.2.254
