#!/bin/bash

# Copyright (c) 2022 Sang-Hyeon Lee All Rights Reserved.
# Email: cpd4268@sch.ac.kr
# Web: http://logos.sch.ac.kr


#echo "============== Python v3.10.6 Build Initializing ================="
#mkdir -p /opt/python@3/
#cd /opt/python@3/; wget https://www.python.org/ftp/python/3.10.6/Python-3.10.6.tgz; tar -xf Python-3.10.6.tgz
#
#ls -al /opt/python@3
#
#echo "============== Python v3.10.6 Build Start ================="
#cd /opt/python@3/Python-3.10.6; ./configure --enable-optimizations;  make -j 12; make altinstall
#
#echo "============== Python v3.10.6 Build Done... ================="
#python3.10 --version
#python3.10 -m pip --version
#
#
#echo "============== Python v3.10.6 Essential Module install ================="
#python3.10 -m pip install -r /usr/src/app/requirements.txt

echo "============== REST API (FastAPI) Start ================="
cd /usr/src/app/src; python3.10 -m uvicorn main:app --host 0.0.0.0 --port 20100 --reload