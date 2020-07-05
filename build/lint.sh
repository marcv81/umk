#!/bin/bash

set -euo pipefail

find firmware -type f -exec clang-format --style=google -i {} \;
find . -name '*.sh' -exec shellcheck {} \;
black keymap
