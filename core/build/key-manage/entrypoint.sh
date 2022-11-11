#!/bin/sh

# Copyright (c) 2022 Sang Hyeon Lee All Rights Reserved.
# Email: cpd4268@sch.ac.kr
# Web: http://logos.sch.ac.kr


echo "============== REST API (FastAPI) Start ================="
cd /usr/src/app/src; python3 -m uvicorn main:app --host 0.0.0.0 --port 20100 --reload