#!/usr/bin/env bash

cd usr/src/app/src
python3 -m uvicorn main:app --host 0.0.0.0 --port 20100 --reload