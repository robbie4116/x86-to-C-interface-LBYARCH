#!/bin/bash
set -e # for errors lang

# For mac, I have to use Docker to build and run the program in an x86_64 Linux container.

docker run --rm --platform=linux/amd64 -v "$PWD":/src -w /src ubuntu:22.04 bash -c "
apt-get update -qq && apt-get install -y -qq build-essential nasm > /dev/null 2>&1
nasm -f elf64 math.asm -o math.o
gcc -O2 -msse2 -c math.c -o math_c.o
gcc -O2 -c main.c -o main.o
gcc main.o math_c.o math.o -lm -o saxpy
./saxpy
"

