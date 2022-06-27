#!/bin/bash

# producer container
# Consumer IP : 192.168.0.142
# Producer IP : 192.168.0.141

IP="192.168.0.142"
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
sleep 3
echo "[*] AES"
docker-compose exec aes nfdc face create $Protocol://$IP:40010
sleep 3
echo "[*] None"
docker-compose exec none nfdc face create $Protocol://$IP:40014c
sleep 3

echo "=================== test ==================="
echo "[*] FPE"
echo "[*] AES"
echo "[*] None"
sleep 3
echo "=================== Done ==================="