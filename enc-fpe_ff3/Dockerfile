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
    && apt-get install -y libpcap-dev libsystemd-dev valgrind libwebsocketpp-dev

# install Python v3.9.12
RUN chmod 777 Automated-Python3.9 \
    && /usr/src/app/Automated-Python3.9

RUN mkdir -p /usr/src/app/tmp
COPY . /usr/src/app/tmp
RUN chmod -R 777 .


# install ndn-cxx
RUN git clone https://github.com/named-data/ndn-cxx.git /usr/src/app/ndn-cxx \
    && cp -rp /usr/src/app/tmp/fpe-cipher.* /usr/src/app/ndn-cxx/ndn-cxx/security/transform \
    && cp -rp /usr/src/app/tmp/name.cpp /usr/src/app/ndn-cxx/ndn-cxx \
    && cd /usr/src/app/ndn-cxx \
    && ./waf configure \
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