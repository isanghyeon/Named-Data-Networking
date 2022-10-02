#!/bin/bash

source ./.env

check="N"

apt-get install -y docker-compose

sleep 3

docker-compose stop
docker-compose down
docker-compose rm -f

sleep 3

echo -n "Delete legacy environment or docker image? [y/N] "
read check

sleep 3

if [ "${check^^}" == "Y" ]; then
    docker rmi $IMAGES:$TIMESTAMP-$ENV
    docker build -t $IMAGES:$TIMESTAMP-$ENV $Path

    if [ $? -eq 1 ]; then
        echo "[-] Docker image build failed..."
    else
        echo "[+] Docker image build completed..."
    fi
fi

sleep 3

echo "[*] Done building..."
