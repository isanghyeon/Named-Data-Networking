#!/bin/bash

# consumer container
# 192.168.0.142


echo "=================== http server ==================="
echo "[*] AES IP"
echo $(docker-compose exec aes ifconfig | grep 172 | awk '{print $2}')
docker-compose exec aes nfd-status-http-server -a $(docker-compose exec aes ifconfig | grep 172 | awk '{print $2}') -p 8080
