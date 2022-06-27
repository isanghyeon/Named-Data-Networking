#!/bin/bash

# consumer container
# 192.168.0.142

echo "=================== http server ==================="
echo "[*] FPE IP"
echo $(docker-compose exec fpe ifconfig | grep 172 | awk '{print $2}')
docker-compose exec fpe nfd-status-http-server -a $(docker-compose exec fpe ifconfig | grep 172 | awk '{print $2}') -p 8080
