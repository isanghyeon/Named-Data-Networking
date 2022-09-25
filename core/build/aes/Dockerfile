FROM ubuntu:20.04
LABEL maintainer "isanghyeon <cpd4268@sch.ac.kr>"
ARG VERSION_CXX=master
ARG VERSION_NFD=master
ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /usr/src/app

# install tools
RUN  apt-get update \
     && apt-get install -y net-tools git curl wget build-essential

# install ndn-cxx and NFD dependencies
RUN apt-get install -y pkg-config libboost-all-dev libssl-dev \
    && apt-get install -y doxygen graphviz python3-pip python3-sphinx software-properties-common libsqlite3-dev libboost-all-dev libpcap-dev \
    && apt-get install -y libpcap-dev libsystemd-dev valgrind libwebsocketpp-dev libcurl4-openssl-dev

# source copy
COPY . /usr/src/app
RUN chmod -R 777 /usr/src/app

# install Python3
RUN mkdir -p /usr/src/app/python@3 \
    && cd /usr/src/app/python@3 \
    && wget https://www.python.org/ftp/python/3.9.12/Python-3.9.12.tgz; tar -xf Python-3.9.12.tgz \
    && cd Python-3.9.12; ./configure --enable-optimizations;  make -j 12; make altinstall \
    && python3.9 --version; python3.9 -m pip --version \
    && cd /usr/src/app

# install ndn-cxx
RUN git clone https://github.com/named-data/ndn-cxx.git /usr/src/app/ndn-cxx \
    && mkdir -p /usr/src/app/ndn-cxx/ndn-cxx/security/transform/curl \
    && cp -rp /usr/src/app/src/curl/* /usr/src/app/ndn-cxx/ndn-cxx/security/transform/curl \
    && cp -rp /usr/src/app/src/key/* /usr/src/app/ndn-cxx/ndn-cxx/security/transform/curl \
    && cp -rp /usr/src/app/src/aes-cipher.* /usr/src/app/ndn-cxx/ndn-cxx/security/transform \
    && cp -rp /usr/src/app/src/name.* /usr/src/app/ndn-cxx/ndn-cxx \
    && cp -rp /usr/src/app/src/wscript /usr/src/app/ndn-cxx \
    && cp -rp /usr/src/app/src/conf-cxx.py /usr/src/app/ndn-cxx/docs/conf.py \
    && cd /usr/src/app/ndn-cxx \
    && ./waf configure \
    && ./waf \
    && ./waf install \
    && ldconfig \
    && cd ..

# install NFD
RUN git clone --recursive https://github.com/named-data/NFD.git /usr/src/app/NFD \
    && cp -rp /usr/src/app/src/conf-nfd.py /usr/src/app/NFD/docs/conf.py \
    && cd /usr/src/app/NFD \
    && ./waf configure \
    && ./waf \
    && ./waf install \
    && cd ..

# install ndn-tools
RUN git clone https://github.com/named-data/ndn-tools.git /usr/src/app/ndn-tools \
    && cp -rp /usr/src/app/src/conf-tools.py /usr/src/app/ndn-tools/manpages/conf.py \
    && cd /usr/src/app/ndn-tools \
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

CMD /usr/local/bin/nfd -c $CONFIG > $LOG_FILE 2>&1MD /usr/local/bin/nfd -c $CONFIG > $LOG_FILE 2>&1