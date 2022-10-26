#!/bin/bash
# Copyright (c) 2022 Sang-hyeon Lee All Rights Reserved.
# Email: cpd4268@sch.ac.kr
# Web: http://logos.sch.ac.kr


echo "============== Packages Update ================="
apt-get update


echo "============== Python v3.9.12 Environment ================="

echo Dependency package install
echo ==============1==============
apt-get install -y build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev libsqlite3-dev wget libbz2-dev

echo Python v3.9.12 install
echo ==============2==============
cd /opt
pwd
wget https://www.python.org/ftp/python/3.9.12/Python-3.9.12.tgz
tar -xf Python-3.9.12.tgz
cd Python-3.9.12
./configure --enable-optimizations

echo Python v3.9.12 build process start
echo ==============3==============
make -j 12

echo Python v3.9.12 install
echo ==============4=============
make altinstall

echo Done...
python3.9 --version
python3.9 -m pip --version


echo "============== Docker Environment ================="

apt-get install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
add-apt-repository \
"deb [arch=amd64] https://download.docker.com/linux/ubuntu \
$(lsb_release -cs) \
stable"
apt-get update && apt-get install -y docker-ce docker-ce-cli containerd.io
systemctl enable docker && service docker start
service docker status
