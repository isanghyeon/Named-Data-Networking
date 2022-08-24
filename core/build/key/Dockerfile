FROM ubuntu:20.04
LABEL maintainer "isanghyeon <cpd4268@sch.ac.kr>"
ENV TZ=Asia/Seoul
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y

WORKDIR /usr/src/app
COPY . /usr/src/app

RUN apt-get update
RUN apt-get install -y python-pip python3-dev build-essential libcurl4-openssl-dev libssl-dev
RUN apt-get install -y net-tools git curl wget build-essential

RUN python3 pip install -r  /usr/src/app/requirements.txt

ENTRYPOINT /usr/src/app/entrypoint.sh && /bin/bash
CMD ["true"]