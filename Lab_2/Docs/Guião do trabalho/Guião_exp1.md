# Step 2 & 3
Ligar Cabos
```
TUX23E0  -> Switch
TUX24E0 -> Switch
```

tux3:
```
> ifconfig eth0 up
> ifconfig eth0 172.16.20.1/24
> ifconfig eth0 
```
tux3
| IP | MAC |
|--------|-------|
| 172.16.20.1  | 00:21:5a:5a:7d:12 |
tux3


tux4:
```
> ifconfig eth0 up
> ifconfig eth0 172.16.20.254/24
> ifconfig eth0 
```
tux4
| IP | MAC |
|--------|-------|
| 172.16.20.254  | 00:08:54:50:3f:2c |

# Step 4

tux3:
```
> ping 172.16.20.254 
```
recebe pacotes de 64 bytes desse endereço

tux4:
```
> ping 172.16.20.1
```
recebe pacotes de 64 bytes desse endereço
# Step 5

tux3:
```
> route -n
```
Associa endereço IP a porta para saber por onde mandar 

Neste pc:
- Destination 172.16.20.0 
- Iface eth0

```
> arp -a
```
Associa Endereço IP a um Endereço MAC 
```
< ? (172.16.20.254) at 00:08:54:50:3f:2c [ether] on eth0
```
# Step 6

tux3:
```
> arp -d 172.16.20.254
> arp -a [retorna nada]
```