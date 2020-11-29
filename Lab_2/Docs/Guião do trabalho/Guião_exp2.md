# Step 1
Fazer as ligaçoes dos cabo

Ligar Cabos
```
TUX23E0  -> Switch Porta 1 (Verificar que luz acende no Switch para saber porta)
TUX24E0  -> Switch Porta 2
TUX22E0  -> Switch Porta 3
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

tux2:
```
> ifconfig eth0 up
> ifconfig eth0 172.16.21.1/24
> ifconfig eth0 
```

tux2
| IP | MAC |
|--------|-------|
| 172.16.21.1  | ??? |

# Step 2
Ligar Cabo 
```
TUX23S0  -> Switch Porta 4
```

Abrir terminal de teste (gkterm) da porta série (usado no Lab anterior)

Dar Enter e verificar se entrou na consola do Switch

(?) Pode ser preciso dar login no switch (slide 48)
```
»enable
»password: ****** (deve ser 8nortel, olha para a esquerda e confirma)
```

Criar VLAN (vlan20) (Slide 44):
```
»configure terminal
»vlan 20
»end
»show vlan id 20
```

Adicionar ports, não sei quais são os valores das ports :/,  mas é preciso adicionar aqui o eth0 do tux23 e do tux24.

Ao fazeres as ligações fisicas ao switch acho que aparecem umas luzinhas com números, pergunta se as ports são isso, se for, então ligas as ports 1 e 2 á VLAN

 Add port 1 to vlan 20 (Slide 44)
```
»configure terminal
»interface fastethernet 0/1             (x/y ; se o y é a port não sei o que é o x, pergunta aí, maybe 0 de eth0?)
»switchport mode access
»switchport access vlan 20
»end
»show running-config interface fastethernet 0/1
»show interfaces fastethernet 0/1 switchport
```

# Step 3

Criar outra VLAN (vlan21):
```
»configure terminal
»vlan 21
»end
»show vlan id 21
```
Adiciona a porta do tux22, porta 3 em principio.

# Step 4

> Start capture at tuxy3.eth0
> 
Deve ser para para ligar o wireshark e começar a capturar pacotes, verifica antes que os pings que se vão fazer em baixo funcionam, depois podes começar a capturar pacotes

# Step 5

No tux23, pingar o tux4:
```
ping 172.16.20.254
(deixa dar 10 pings e depois dá CTRL+C)
```
pingar o tux22:
```
ping 172.16.21.1
(deixa dar 10 pings e depois dá CTRL+C)
```

# Step 6

Parar a captura e guardar o ficheiro. Renomear para `exp2_step5.pcapng`

# Step 7

- Em tux23 começar captura em eth0
- Passar para tux24
- Em tux24 começar captura em eth0
- Passar para tux22
- Em tux22 começar captura em eth0
  
# Step 8

No tux23, pingar em broadcast:
```
ping -b 172.16.20.255
(deixa dar 10 pings e depois dá CTRL+C)
```

# Step 9

- Parar a captura em tux23
- Parar a captura em tux24
- Parar a captura em tux22
- Guardar o ficheiro de tux23 como `exp2_step7_tux23.pcapng`
- Guardar o ficheiro de tux24 como `exp2_step7_tux24.pcapng`
- Guardar o ficheiro de tux22 como `exp2_step7_tux22.pcapng`

# Step 10

- Em tux23 começar captura em eth0
- Passar para tux24
- Em tux24 começar captura em eth0
- Passar para tux22
- Em tux22 começar captura em eth0

No tux22, pingar em broadcast:
```
ping -b 172.16.21.255
(deixa dar 10 pings e depois dá CTRL+C)
```

- Parar a captura em tux23
- Parar a captura em tux24
- Parar a captura em tux22
- Guardar o ficheiro de tux23 como `exp2_step10_tux23.pcapng`
- Guardar o ficheiro de tux24 como `exp2_step10_tux24.pcapng`
- Guardar o ficheiro de tux22 como `exp2_step10_tux22.pcapng`