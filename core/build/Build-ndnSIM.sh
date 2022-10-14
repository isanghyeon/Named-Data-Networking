#!/bin/bash

echo "[*] Install Dependencies for NS-3"
sudo apt update
sudo apt install build-essential libsqlite3-dev libboost-all-dev libssl-dev git python3-setuptools castxml -y

sudo apt install gir1.2-goocanvas-2.0 gir1.2-gtk-3.0 libgirepository1.0-dev python3-dev python3-gi python3-gi-cairo python3-pip python3-pygraphviz python3-pygccxml -y
sudo pip3 install kiwi

echo  "[*] Cloning ndnSIM"
mkdir ndnSIM; cd ndnSIM
git clone https://github.com/named-data-ndnSIM/ns-3-dev.git ns-3
git clone https://github.com/named-data-ndnSIM/pybindgen.git pybindgen
git clone --recursive https://github.com/named-data-ndnSIM/ndnSIM.git ns-3/src/ndnSIM

echo "[*] ndnSIM Installation"
cd ns-3
./waf configure --enable-examples
./waf

echo "[+] Done!"