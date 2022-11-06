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

# install tools
RUN  apt-get update \
     && apt-get install -y net-tools git curl wget vim \
     && apt-get install -y build-essential pkg-config python3-minimal libboost-all-dev libssl-dev libsqlite3-dev python3-sphinx \
     && apt-get install -y doxygen graphviz python3-pip \
     && apt-get install -y gcc g++ clang cmake make software-properties-common libpcap-dev libsystemd-dev valgrind libwebsocketpp-dev libcurl4-openssl-dev \
     && apt-get install -y zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev libsqlite3-dev libbz2-dev sphinxcontrib-doxylink

# install Python3
#RUN mkdir -p /usr/src/app/python@3 \
#    && cd /usr/src/app/python@3 \
#    && wget https://www.python.org/ftp/python/3.9.12/Python-3.9.12.tgz; tar -xf Python-3.9.12.tgz \
#    && cd Python-3.9.12; ./configure --enable-optimizations;  make -j 12; make altinstall \
#    && python3.9 --version; python3.9 -m pip --version \
#    && cd /usr/src/app


# install ndn-cxx
RUN git clone https://github.com/named-data/ndn-cxx.git /usr/src/app/ndn-cxx \
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

CMD /usr/local/bin/nfd -c $CONFIG > $LOG_FILE 2>&1MD /usr/local/bin/nfd -c $CONFIG > $LOG_FILE 2>&1