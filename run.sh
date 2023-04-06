#!/bin/sh

asm="std/asm"
lib="std/lib"

make > /dev/null
mkdir $lib

./slang test.sl > main.ll

for dir in $asm/*
do
    for file in $dir/*
    do
        nasm -felf64 $file -o $lib${file:10:-4}.o
    done
done

ld -o lib.o -r $lib/*.o

clang -c -o main.o main.ll
clang -o main main.o lib.o

./main

make clean > /dev/null
rm -rf $lib
rm lib.o
rm main.o
