#!/bin/bash

set -euo pipefail

# If there is a parameter, make it the entry point
if ! test -z "${1-}"; then
    entrypoint=(--entrypoint "$1")
fi;

# Build and run the container
docker build docker -t umk-builder
docker run -it --rm \
    -u "$(id -u)":"$(id -g)" \
    -v "$(pwd)":/umk \
    -v /etc/passwd:/etc/passwd \
    -v /etc/group:/etc/group \
    "${entrypoint[@]}" \
    umk-builder
