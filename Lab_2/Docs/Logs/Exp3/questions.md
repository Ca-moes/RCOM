## What routes are there in the tuxes? What are their meaning?
No tux23 adiciona-se uma rota para dizer que para aceder á vlan com endereços `172.16.21.0/24` usa-se como *gateway* o IP `172.16.20.254`, correspondente á porta eth0 do tux24. Este comando é para ser feito na consola:
```powershell
route add -net 172.16.21.0/24 gw 172.16.20.254
```
Da mesma forma, no tux22 faz-se:
```powershell
route add -net 172.16.20.0/24 gw 172.16.21.253
```
Para aceder á vlan com endereços `172.16.20.0/24` usa-se como *gateway* o IP `172.16.20.253`, correspondente á porta eth1 do tux24.

## What information does an entry of the forwarding table contain (routing table)? 

**tux22**:
| Destination | Gateway       | Genmask       | Flags | Metric | Ref | Use | Iface |
|-------------|---------------|---------------|-------|--------|-----|-----|-------|
| 172.16.20.0 | 172.16.21.253 | 255.255.255.0 | UG    | 0      | 0   | 0   | eth0  |
| 172.16.21.0 | 0.0.0.0       | 255.255.255.0 | U     | 0      | 0   | 0   | eth0  |

**tux23**:
| Destination | Gateway       | Genmask       | Flags | Metric | Ref | Use | Iface |
|-------------|---------------|---------------|-------|--------|-----|-----|-------|
| 172.16.20.0 | 0.0.0.0       | 255.255.255.0 | U     | 0      | 0   | 0   | eth0  |
| 172.16.21.0 | 172.16.21.254 | 255.255.255.0 | UG    | 0      | 0   | 0   | eth0  |

**tux24**:
| Destination | Gateway       | Genmask       | Flags | Metric | Ref | Use | Iface |
|-------------|---------------|---------------|-------|--------|-----|-----|-------|
| 172.16.20.0 | 0.0.0.0       | 255.255.255.0 | U     | 0      | 0   | 0   | eth0  |
| 172.16.21.0 | 0.0.0.0       | 255.255.255.0 | U     | 0      | 0   | 0   | eth1  |


## What ARP messages, and associated MAC addresses, are observed and why?



## What ICMP packets are observed and why? 


## What are the IP and MAC addresses associated to ICMP packets and why?