FROM ubuntu:20.04
LABEL maintainer "isanghyeon <cpd4268@sch.ac.kr>"
ARG VERSION_CXX=master
ARG VERSION_NFD=master
ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /usr/src/app

# source copy
COPY . /usr/src/app
RUN chmod -R 755 /usr/src/app

# install tools
RUN  apt-get update \
     && apt-get install -y net-tools git curl wget build-essential

# install tools
RUN apt-get update \
    && apt-get install -y wget net-tools vim git curl build-essential gcc g++ clang cmake \
    && apt-get install -y zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev libsqlite3-dev libbz2-dev

# install Python3
RUN mkdir -p /usr/src/app/python@3 \
    && cd /usr/src/app/python@3 \
    && wget https://www.python.org/ftp/python/3.9.12/Python-3.9.12.tgz; tar -xf Python-3.9.12.tgz \
    && cd Python-3.9.12; ./configure --enable-optimizations;  make -j 12; make altinstall \
    && python3.9 --version; python3.9 -m pip --version \
    && cd /usr/src/app

# install ndn-cxx and NFD dependencies
RUN apt-get install -y pkg-config libboost-all-dev python3-sphinx \
    && apt-get install -y doxygen graphviz software-properties-common libboost-all-dev libpcap-dev \
    && apt-get install -y libsystemd-dev valgrind libwebsocketpp-dev libcurl4-openssl-dev

# install ndn-cxx
RUN git clone https://github.com/named-data/ndn-cxx.git /usr/src/app/ndn-cxx \
    && cp -rp /usr/src/app/src/curl /usr/src/app/ndn-cxx/ndn-cxx/security/transform/curl \
    && cp -rp /usr/src/app/src/fpe-cipher.* /usr/src/app/ndn-cxx/ndn-cxx/security/transform \
    && cp -rp /usr/src/app/src/name.* /usr/src/app/ndn-cxx/ndn-cxx \
    && cp -rp /usr/src/app/src/wscript /usr/src/app/ndn-cxx \
    && cd /usr/src/app/ndn-cxx \
    && ./waf configure --with-tests \
    && ./waf \
    && ./waf install \
    && ldconfig \
    && cd ..

# install NFD
RUN git clone --recursive https://github.com/named-data/NFD.git /usr/src/app/NFD \
    && cd /usr/src/app/NFD \
    && ./waf configure \
    && ./waf \
    && ./waf install \
    && cd ..

# install ndn-tools
RUN git clone https://github.com/named-data/ndn-tools.git /usr/src/app/ndn-tools \
    && cd /usr/src/app/ndn-tools \
    && cp -rp /usr/src/app/src/conf.py /usr/src/app/ndn-tools/manpages/conf.py \
    && ./waf configure \
    && ./waf \
    && ./waf install \
    && cd ..

# initial configuration
RUN cp /usr/local/etc/ndn/nfd.conf.sample /usr/local/etc/ndn/nfd.conf \
    && ndnsec-keygen /`whoami` | ndnsec-install-cert - \
    && mkdir -p /usr/local/etc/ndn/keys \
    && ndnsec-cert-dump -i /`whoami` > default.ndncert \
    && mv default.ndncert /usr/local/etc/ndn/keys/default.ndncert

RUN mkdir /share \
    && mkdir /logs

EXPOSE 6363/tcp
EXPOSE 6363/udp

ENV CONFIG=/usr/local/etc/ndn/nfd.conf
ENV LOG_FILE=/logs/nfd.log

CMD /usr/local/bin/nfd -c $CONFIG > $LOG_FILE 2>&1