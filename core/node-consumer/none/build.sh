#!/bin/bash

source ./.env

sleep 3

docker-compose stop
docker-compose down
docker-compose rm -f

sleep 3

docker rmi $IMAGES:$TIMESTAMP-$ENV
docker build -t $IMAGES:$TIMESTAMP-$ENV $Path

if [ $? -eq 1 ]; then
    echo "[-] Docker image build failed..."
else
    echo "[+] Docker image build completed..."
fi

sleep 3

echo "[*] building Done..."
