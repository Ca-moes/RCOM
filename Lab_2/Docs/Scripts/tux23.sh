#!/bin/bash

# configuração do IP
ifconfig eth0 up
ifconfig eth0 172.16.20.1/24
ifconfig eth0 

# route para aceder a vlan 1 a partir de eth0 de tux24
route add -net 172.16.21.0/24 gw 172.16.20.254
# definir tux24 como default router
route add default gw 172.16.20.254
# ver routes
routes -n