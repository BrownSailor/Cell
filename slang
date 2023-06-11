#!/bin/bash

if [ ! -f slangc ]; then
    make
fi

./slangc $1 | clang -x ir -
