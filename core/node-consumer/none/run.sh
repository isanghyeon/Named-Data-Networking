#!/bin/bash

source ./build.sh

source ./.env
source ./setting.sh

echo "Environment :: " "$ENV"
echo "Path        :: " "$Path"
echo "Build       :: " "$TIMESTAMP-$ENV"

sleep 3

RESULT=$(docker images | grep $Image | awk '{print $2}' | grep $TIMESTAMP-$ENV)

if [ -z $RESULT -o $RESULT != $TIMESTAMP-$ENV ]; then
  echo "[-] Not Found Image"
fi

if [ $RESULT = $TIMESTAMP-$ENV ]; then
  echo "[+] Image import completed..."

  sleep 3

  # apt-get update; apt-get install -y docker-compose

  mv shared/log/$ENV.log shared/log/"$ENV-$(date +%Y.%m.%dT%H:%M:%S)".log

  docker-compose stop
  docker-compose down
  docker-compose rm -f
  docker-compose build
  docker-compose up -d
  docker-compose logs -f >>shared/log/$ENV.log &

fi

echo "[*] Done Composing..."