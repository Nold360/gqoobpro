FROM debian:stretch
RUN apt-get update && apt-get install -y build-essential gcc automake libusb-dev libtool pkg-config

COPY . /src
WORKDIR /src
RUN for dir in libqoob qoob-flasher ; do cd $dir  && ./autogen.sh && ./configure --prefix=/usr && make && make install && cd .. ; done
