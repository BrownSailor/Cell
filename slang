#!/bin/bash

./slangc $1 | clang -x ir -
