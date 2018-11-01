#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

#define INPUT_SIZE 1000000
#define RANGES 6
const int TEST_RANGES[RANGES] = {10000, 50000, 100000, 300000, 500000, 1000000};

void sequentialOddEven(int *arr, int size);
void parallelOddEven(int *arr, int size);
int* fillArray(int *array, int size);
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
        sequentialOddEven(testArr, input_range);
        fim = clock();

        seq = (double)(fim - ini) / CLOCKS_PER_SEC; 
        printf("\tSequential:\t%fs\t", seq);
        printf("-- %s --\n", (isSorted(testArr, input_range) ? "In order!" : "Not in order!"));

        copyArr(bigArr, input_range, testArr);
        ini = clock();
        // ALGORITMO PARALELO
        parallelOddEven(testArr, input_range);
        fim = clock();

        par = (double)(fim - ini) / CLOCKS_PER_SEC; 
        printf("\tParallel:\t%fs\t", par);
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

void sequentialOddEven(int *arr, int size){
    int i, tmp;
    bool isSorted = false;

    while (!isSorted) {
        isSorted = true;
        for (i=0; i<=size-2; i=i+2) { 
            if (arr[i] > arr[i+1]) { 
                tmp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1]=tmp;
                isSorted = false;
            } 
        }


        for (i=1; i<=size-2; i=i+2) {
            if (arr[i] > arr[i+1]) {
                tmp = arr[i+1];
                arr[i+1] = arr[i];
                arr[i]=tmp;
                isSorted = false; 
            }
        } 
        if(isSorted)
            break;
    } 
}

void parallelOddEven (int *arr, int size) {
    int tmp, i, j;

    for ( i = 0 ; i < size ; i++) {
        if ( i % 2 == 0 ) {
            #pragma omp parallel for private(tmp, j) shared(arr)
            for ( j = 0 ; j < size - 1 ; j += 2) {
                if ( arr[j] > arr[j + 1] ) {
                    tmp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = tmp;
                }
            }
        }
        else {
            #pragma omp parallel for private(tmp, j) shared(arr)
            for ( j = 1 ; j < size - 1 ; j += 2) {
                if ( arr[j] > arr[j + 1] ) {
                    tmp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = tmp;
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