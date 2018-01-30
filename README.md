# Build environment setup

    docker build docker -t umk-builder
    docker run -it --rm -v `pwd`:/umk umk-builder
