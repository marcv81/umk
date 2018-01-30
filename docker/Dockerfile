FROM debian:stretch

RUN apt-get update && apt-get install --no-install-recommends -y \
gcc-avr \
binutils-avr \
avr-libc \
python3 \
&& rm -rf /var/lib/apt/lists/*

VOLUME /umk
WORKDIR /umk
CMD /bin/bash
