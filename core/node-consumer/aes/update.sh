#! /bin/bash

source ./.env

echo "[*] TimeStamp   :: " "$(date +%Y.%m.%dT%H:%M:%SZ)"
echo "[*] Environment :: " $ENV
echo "[*] Path        :: " $Path
echo "[*] Build       :: " $TIMESTAMP-$ENV

BUILDPATH="/usr/src/app/src/ndn-cxx"

sleep 3

cd $BUILDPATH; ./waf configure; ./waf; ./waf install

sleep 3

echo "[*] Done ..."