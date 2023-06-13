#!/bin/bash

if [ ! -f slangc ]; then
    make
fi

./slangc $1 | clang -O0 -x ir -
