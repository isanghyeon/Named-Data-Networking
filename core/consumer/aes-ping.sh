#!/bin/bash

# consumer container
# Consumer IP : 192.168.0.142
# Producer IP : 192.168.0.141

IP="192.168.0.141"
Name="/sch.ac.kr/calab/research.file"
Protocol="tcp4"

echo "=================== http server ==================="
docker-compose exec aes ndnping $Name -c 1000 -n 1 > aes-result.txt