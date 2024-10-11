#!/usr/bin/sh

TARGET=prog3
TIMES=10

gcc -O0 -o $TARGET $TARGET.c

for i in {1..10}; do
    echo "[TASK $i]"
    ./$TARGET
done
