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

RUN chmod 755 -R /usr/src/app

RUN apt-get update
RUN apt-get install -y build-essential openssl libcurl4-openssl-dev libssl-dev vim net-tools git curl wget

# install essential modules
RUN pip install -r /usr/src/app/requirements.txt

# start api
RUN chmod 777 /usr/src/app/entrypoint.sh
ENTRYPOINT ["/usr/src/app/entrypoint.sh"]