FROM debian:stretch

RUN apt-get update && apt-get install --no-install-recommends -y \
gcc-avr \
binutils-avr \
avr-libc \
&& rm -rf /var/lib/apt/lists/*

VOLUME /ergodox
WORKDIR /ergodox
CMD /bin/bash
