#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

#define INPUT_SIZE 1000000
#define RANGES 6
const int TEST_RANGES[RANGES] = {10000, 50000, 100000, 300000, 500000, 1000000};

void shellSortParallel(int *arr, int size);
void InsertSort(int *arr, int i, int size,  int half);
void shellSortSequential(int *arr, int size);
int* fillArray(int *arr, int size);
bool isSorted(int* arr, int n);
void copyArr(int* arr, int n, int* newArr);

int main(void){
 	int *bigArr, *testArr;
    int i, input_range;
    time_t ini, fim;
    float seq, par, speedup;

    bigArr = (int*) malloc(INPUT_SIZE*sizeof(int));
    testArr = (int*) malloc(INPUT_SIZE*sizeof(int));
    fillArray(bigArr, INPUT_SIZE);
    
    for (i = 0; i < RANGES; i++) {
        input_range = TEST_RANGES[i];
        printf("Testing array[%i]\n", input_range);
        
        copyArr(bigArr, input_range, testArr);
        ini = clock();
        // ALGORITMO SEQUENCIAL
        shellSortSequential(testArr, input_range);
        fim = clock();

        seq = (double)(fim - ini) / CLOCKS_PER_SEC; 
        printf("\tSequential:\t%.10fs\t", seq);
        printf("-- %s --\n", (isSorted(testArr, input_range) ? "In order!" : "Not in order!"));

        copyArr(bigArr, input_range, testArr);
        ini = clock();
        // ALGORITMO PARALELO
        shellSortParallel(testArr, input_range);
        fim = clock();

        par = (double)(fim - ini) / CLOCKS_PER_SEC; 
        printf("\tParallel:\t%.10fs\t", par);
        printf("-- %s --\n", (isSorted(testArr, input_range) ? "In order!" : "Not in order!"));

        speedup = (par == 0) ? 0 : ((seq/par)*100)-100;
        printf("\tSpeedup:\t%.2f%%\n", speedup);
    }

 	return 0;
}

int* fillArray(int *array, int size){
    int i;

    int ret = 1;

    for(i=0;i<size;i++)
    {
        ret = scanf("%i", &array[i]);

        if (ret != 1) break;
    }
}

void shellSortParallel (int *arr, int size) {
	int h, j = 0, i = 0, tmp = 0;

	for(h = size / 2; h > 0; h = h/2){
		#pragma omp parallel for shared(arr, size, h, i) default(none)
		for(i = 0; i < h; i++)
			InsertSort(arr, i, size, h);
	}
}
void InsertSort(int *arr, int i, int size, int half) {
	int tmp = 0;
	int f, j = 0;

	for (f = half + i; f < size; f += half) {
		j = f;
		while(j > i && arr[j-half] > arr[j]) {
			tmp = arr[j];
			arr[j] = arr[j-half];
			arr[j-half] = tmp;
			j = j - half;
		}
	}
}

void shellSortSequential(int *arr, int size) {
	int count = 0, j = 0, tmp = 0, h = 0;
	int i, f;
	
	for(h = size / 2; h > 0; h = h / 2) {	
		for(i = 0; i < h; i++) {
			for (f = h + i; f < size; f = f + h) {
				j = f;
				while(j > i && arr[j-h] > arr[j]) {
					tmp = arr[j];
					arr[j] = arr[j-h];
					arr[j-h] = tmp;
					j = j - h;
				}
			}
		}
	}
}

bool isSorted(int* arr, int n){
    int i;

    for(i = 1; i < n; i++)
        if (arr[i] < arr[i - 1]) return false;

    return true;    
}

void copyArr(int* arr, int n, int* newArr){
    int i;

    for(i = 0; i < n; i++) newArr[i] = arr[i];
}