FROM ubuntu:20.04

LABEL author="Sanghyeon Lee <cpd4268@sch.ac.kr>"
LABEL author_email="cpd4268@sch.ac.kr"
LABEL subAuthor="Dawit Jeong"
LABEL subAuthor_email="djung0605@sch.ac.kr"
LABEL version="2022.11"

ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /usr/src/app

# source copy
COPY . /usr/src/app
RUN chmod -R 755 /usr/src/app

# install
RUN  apt-get update \
     && apt-get install -y net-tools git curl wget vim gcc g++ clang cmake make \
     && apt-get install -y build-essential pkg-config python3-minimal libboost-all-dev libssl-dev libsqlite3-dev \
     && apt-get install -y doxygen graphviz python3-pip \
     && apt-get install -y openssl libcurl4-openssl-dev \
     && pip3 install --user sphinx sphinxcontrib-doxylink

# install ndn-cxx
RUN git clone https://github.com/named-data/ndn-cxx.git /usr/src/app/ndn-cxx \
    && /usr/src/app/src/consumer.cpp /usr/src/app/ndn-cxx/example \
    && /usr/src/app/src/producer.cpp /usr/src/app/ndn-cxx/example \
    && cd /usr/src/app/ndn-cxx  \
    && ./waf configure --with-examples \
    && ./waf \
    && ./waf install \
    && ldconfig \
    && echo /usr/local/lib | tee /etc/ld.so.conf.d/ndn-cxx.conf \
    && cd /usr/src/app

# install 2
RUN apt-get install -y libpcap-dev libsystemd-dev

# install NFD
RUN git clone --recursive https://github.com/named-data/NFD.git /usr/src/app/NFD \
    && cd /usr/src/app/NFD \
    && ./waf configure \
    && ./waf \
    && ./waf install \
    && cd /usr/src/app

# install ndn-tools
RUN git clone https://github.com/named-data/ndn-tools.git /usr/src/app/ndn-tools \
    && cd /usr/src/app/ndn-tools \
    && ./waf configure \
    && ./waf \
    && ./waf install \
    && cd /usr/src/app

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
