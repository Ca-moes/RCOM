#!/bin/bash

# configuração do IP
ifconfig eth0 up
ifconfig eth0 172.16.21.1/24
ifconfig eth0 

# route para aceder a vlan 0 a partir de eth1 de tux24
route add -net 172.16.20.0/24 gw 172.16.21.253

# route para aceder a internet a partir de eth0 de router
route add -net 172.16.1.0/24 gw 172.16.21.254

# ver routes
routes -n