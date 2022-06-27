#!/bin/bash

# consumer container
# 192.168.0.142


echo "=================== http server ==================="
echo "[*] AES IP"
echo $(docker-compose exec none ifconfig | grep 172 | awk '{print $2}')
docker-compose exec none nfd-status-http-server -a $(docker-compose exec none ifconfig | grep 172 | awk '{print $2}') -p 8080
