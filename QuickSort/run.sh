#!/bin/bash

gcc quickSort.c -o quickSort.out -O3 -fopt-info-vec-optimized -fopenmp

./quickSort.out < ../input.txt > output.txt
echo "log on output.txt"