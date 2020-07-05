FROM ubuntu:20.04

RUN apt-get update && apt-get install --no-install-recommends -y \
gcc-avr \
binutils-avr \
avr-libc \
python3 \
black \
clang-format \
shellcheck \
&& rm -rf /var/lib/apt/lists/*

VOLUME /umk
WORKDIR /umk
CMD /bin/bash
