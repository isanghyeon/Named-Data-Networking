#!/bin/bash

# consumer container

echo "=================== running ==================="
docker-compose ps
sleep 3

echo "=================== down, stop ==================="
echo "[*] FPE"
echo "[*] AES"
echo "[*] None"
docker-compose stop
docker-compose down

sleep 3

echo "=================== Done ==================="