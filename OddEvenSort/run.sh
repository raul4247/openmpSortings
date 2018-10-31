#!/bin/bash

gcc oddEvenSort.c -o oddEvenSort.out -O3 -fopt-info-vec-optimized -fopenmp
echo "This sorting usually takes a lot of time to run..."

./oddEvenSort.out < ../input.txt > output.txt
echo "log on output.txt"