#!/bin/bash

asm="std/asm"
lib="std/lib"

mkdir $lib

echo "Compiling libraries"
for dir in $asm/*
do
    for file in $dir/*
    do
        nasm -felf64 $file -o $lib${file:10:-4}.o
    done
done

ld -o lib.o -r $lib/*.o
