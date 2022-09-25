#! /bin/bash

source ./.env

echo "[*] TimeStamp   :: " "$(date +%Y.%m.%dT%H:%M:%SZ)"
echo "[*] Environment :: " $ENV
echo "[*] Path        :: " $Path
echo "[*] Build       :: " $TIMESTAMP-$ENV

BUILDPATH="/usr/src/app/src/ndn-cxx"

cd $BUILDPATH; ./waf --configure; ./waf; ./waf install