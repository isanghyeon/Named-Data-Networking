#!/bin/bash

source ./.env

echo "[*] Environment :: " "$ENV"
echo "[*] Path        :: " "$Path"
echo "[*] Build       :: " "$TIMESTAMP-$ENV"
check="N"

sleep 3

apt-get install -y docker-compose

sleep 3

docker-compose stop
docker-compose down
docker-compose rm -f

sleep 3

read -p "Delete legacy environment or docker image? [y/N] " check

sleep 3

if [ ${check^^} == "Y" ]; then
    docker rmi $Image:$TIMESTAMP-$ENV
    docker build -t $Image:$TIMESTAMP-$ENV $Path

    if [ $? -eq 1 ]; then
        echo "[-] Docker image build failed..."
    else
        echo "[+] Docker image build completed..."
    fi
fi

echo "[+] Docker image build completed..."

sleep 3

echo "[*] Done building..."
