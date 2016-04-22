#!/bin/bash

echo "start"
echo $1

#./main --server $1 --port 443 &
#./main --server $1 --port 80 --proxy &
#./main --server $1 --port 443 --tls &
./main --server $1 --port 443 --proxy --tls &

sleep 2

ip addr add dev vpn0 192.168.66.66/24
ip link set up dev vpn0

bash
