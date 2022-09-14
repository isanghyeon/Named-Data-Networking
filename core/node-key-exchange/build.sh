#!/bin/bash

source .env

echo "[*] Environment :: " "$ENV"
echo "[*] Path        :: " "$Path"
echo "[*] Build       :: " "$TIMESTAMP-$ENV"

sleep 3

docker-compose stop
docker-compose down
docker-compose rm -f

docker rmi $Image:$TIMESTAMP-$ENV

docker build -t $Image:$TIMESTAMP-$ENV $Path

if [ $? -eq 1 ]; then
  echo "[-] Docker image build failed..."
else
  echo "[+] Docker image build completed..."
fi

sleep 3

echo "[*] Done building..."