# Build environment setup

    docker build docker -t ergodox-builder
    docker run -it --rm -v `pwd`:/ergodox ergodox-builder
