FROM python:3.10-buster

LABEL author="Sanghyeon Lee <cpd4268@sch.ac.kr>"
LABEL author_email="cpd4268@sch.ac.kr"
LABEL subAuthor="Dawit Jeong"
LABEL subAuthor_email="djung0605@sch.ac.kr"
LABEL version="2022.11"

ENV TZ=Asia/Seoul
ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /usr/src/app
COPY . /usr/src/app

# src
# Dockerfile
# entrypoint.sh
# requirements.txt
RUN apt-get update
#RUN apt-get install software-properties-common -y
#RUN add-apt-repository ppa:deadsnakes/ppa
#RUN apt-get install -y build-essential libcurl4-openssl-dev libssl-dev vim net-tools git curl wget
#RUN apt-get install -y zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libreadline-dev libffi-dev libsqlite3-dev wget libbz2-dev

RUN chmod 755 -R /usr/src/app

# install essential modules
RUN python3.10 -m pip install -r /usr/src/app/requirements.txt

# start api
# RUN chmod 777 /usr/src/app/entrypoint.sh
# ENTRYPOINT /usr/src/app/entrypoint.sh && /bin/bash
CMD cd /usr/src/app/src; python3.10 -m uvicorn main:app --host 0.0.0.0 --port 20100 --reload