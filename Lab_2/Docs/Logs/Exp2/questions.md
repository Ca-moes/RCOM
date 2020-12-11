## How to configure vlany0?

Ligar Cabo 
```powershell
TUXyzS0  -> Switch Porta 4
```
| --- | --- | --- |
|--------|-------|----|
| TUX23E0 (1) | TUX22E0 (3) | empty (5) |
| TUX24E0 (2) | TUX23S0 (4) | empty (6) |

Abrir terminal de teste (gkterm) da porta série (usado no Lab anterior)

Dar Enter e verificar se entrou na consola do Switch

(?) Pode ser preciso dar login no switch (slide 48)
```powershell
»enable
»password: ****** (deve ser 8nortel, olha para a esquerda e confirma)
```

Criar VLAN (vlan20) (Slide 44):
```powershell
»configure terminal
»vlan 20
»end
»show vlan id 20
```

Adicionar ports, não sei quais são os valores das ports :/,  mas é preciso adicionar aqui o eth0 do tux23 e do tux24.

Ao fazeres as ligações fisicas ao switch acho que aparecem umas luzinhas com números, pergunta se as ports são isso, se for, então ligas as ports 1 e 2 á VLAN

Add port 1 to vlan 20 (Slide 44):
```powershell
»configure terminal
»interface fastethernet 0/1             (x/y ; se o y é a port não sei o que é o x, pergunta aí, maybe 0 de eth0?)
»switchport mode access
»switchport access vlan 20
»end
»show running-config interface fastethernet 0/1
»show interfaces fastethernet 0/1 switchport
```


## How many broadcast domains are there? How can you conclude it from the logs?

what's a broadcast domain?

---

Tantos logs para só haver 2 perguntas <_>
