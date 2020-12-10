#!/bin/bash

# configuração de IP's
ifconfig eth0 up
ifconfig eth0 172.16.20.254/24
ifconfig eth0 

ifconfig eth1 up
ifconfig eth1 172.16.21.253/24
ifconfig eth1 

# Enable IP forwarding
echo 1 > /proc/sys/net/ipv4/ip_forward
# Disable ICMP echo-ignore-broadcast
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

# route para aceder a internet a partir de eth0 de router
route add -net 172.16.1.0/24 gw 172.16.21.254

# ver routes
routes -n