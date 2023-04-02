#!/bin/sh

asm="asm"
lib="lib"

mkdir $lib

for file in $asm/*
do
    nasm -felf64 $file -o $lib/${file:4:-4}.o
done

ld -o lib.o -r $lib/*.o
rm -rf $lib

nasm -felf64 main.asm
ld -o main main.o lib.o

