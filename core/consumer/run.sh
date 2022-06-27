#!/bin/bash

# consumer container
# Consumer IP : 192.168.0.142
# Producer IP : 192.168.0.141

IP="192.168.0.141"
Name="/sch.ac.kr/calab/research.file"
Protocol="tcp4"

echo "=================== running ==================="
sleep 3
echo "=================== compose ==================="
docker-compose up -d --build
sleep 20
echo "=================== initialize ==================="
echo "[*] FPE"
docker-compose exec fpe nfdc face create $Protocol://$IP:40012
docker-compose exec fpe nfdc route add $Name $Protocol://$IP:40012
sleep 3

echo "[*] AES"
docker-compose exec aes nfdc face create $Protocol://$IP:40010
docker-compose exec aes nfdc route add $Name $Protocol://$IP:40010
sleep 3

echo "[*] None"
docker-compose exec none nfdc face create $Protocol://$IP:40014
docker-compose exec none nfdc route add $Name $Protocol://$IP:40014
sleep 3

echo "=================== Done ==================="