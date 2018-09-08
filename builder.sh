#!/bin/sh

# If there is a parameter, make it the entry point
if ! test -z "$1"; then
    ENTRYPOINT="--entrypoint $1"
    echo $ENTRYPOINT
fi;

# Build and run the container
docker build docker -t umk-builder
docker run -it --rm \
    -u `id -u`:`id -g` \
    -v `pwd`:/umk \
    -v /etc/passwd:/etc/passwd \
    -v /etc/group:/etc/group \
    $ENTRYPOINT \
    umk-builder
