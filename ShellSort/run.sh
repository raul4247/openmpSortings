#!/bin/bash

gcc shellSort.c -o shellSort.out -O3 -fopt-info-vec-optimized -fopenmp

./shellSort.out < ../input.txt > output.txt
echo "log on output.txt"