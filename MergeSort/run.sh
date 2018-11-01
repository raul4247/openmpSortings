#!/bin/bash

gcc mergeSort.c -o mergeSort.out -O3 -fopt-info-vec-optimized -fopenmp

./mergeSort.out < ../input.txt > output.txt
echo "log on output.txt"