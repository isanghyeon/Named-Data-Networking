#!/bin/bash

# producer container
# Consumer IP : 192.168.0.142
# Producer IP : 192.168.0.141

IP="192.168.0.141"
Name="/sch.ac.kr/calab/research.file"
Protocol="tcp4"

echo "=================== http server ==================="
docker-compose exec aes ndnpingserver $Name -p 1000 -s 1000