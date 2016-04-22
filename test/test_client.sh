#!/bin/bash

echo "start"
echo $1

#./main --client $1 --port 443 &
#./main --client $1 --port 80 --proxy --ipproxy $2 --portproxy $3 &
#./main --client $1 --port 443 --tls &
./main --client $1 --port 443 --proxy --ipproxy $2 --portproxy $3 --tls &

sleep 2

ip addr add dev vpn0 192.168.66.67/24
ip link set up dev vpn0

bash
