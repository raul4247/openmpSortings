#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

#define INPUT_SIZE 1000000
#define RANGES 6
const int TEST_RANGES[RANGES] = {10000, 50000, 100000, 300000, 500000, 1000000};

void parallelMerge(int arr[], int l, int m, int r);
void sequentialMerge(int arr[], int l, int m, int r);
void sequentialMergeSort(int *arr, int n);
void sequentialInternalMergeSort(int arr[], int l, int r);
void parallelMergeSort(int *arr, int n);
void parallelInternalMergeSort(int arr[], int l, int r);
int* fillArray(int *array, int size);
bool isSorted(int* arr, int n);
void copyArr(int* arr, int n, int* newArr);

int main(int argc, char const *argv[]){
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
        sequentialMergeSort(testArr, input_range);
        fim = clock();

        seq = (double)(fim - ini) / CLOCKS_PER_SEC; 
        printf("\tSequential:\t%f\t", seq);
        printf("-- %s --\n", (isSorted(testArr, input_range) ? "In order!" : "Not in order!"));

        copyArr(bigArr, input_range, testArr);
        ini = clock();
        // ALGORITMO PARALELO
        parallelMergeSort(testArr, input_range);
        fim = clock();

        par = (double)(fim - ini) / CLOCKS_PER_SEC; 
        printf("\tParallel:\t%f\t", par);
        printf("-- %s --\n", (isSorted(testArr, input_range) ? "In order!" : "Not in order!"));

        speedup = (par == 0) ? 0 : ((seq/par)*100)-100;
        printf("\tSpeedup:\t%.2f%%\n", speedup);
    }

 	return 0;
}


int* fillArray(int *array, int size){
    int i;
    for(i=0;i<size;i++)
        scanf("%i", &array[i]);
}

void parallelMerge(int arr[], int l, int m, int r){
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    int L[n1], R[n2]; 

    #pragma omp simd
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i]; 
    
    #pragma omp simd
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2){ 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } 
        else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
}

void sequentialMerge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    int *L, *R;
	L = (int*) malloc(n1*sizeof(int)); 
	R = (int*) malloc(n2*sizeof(int)); 
	
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } 
        else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
}

void parallelMergeSort(int *arr, int n){
    parallelInternalMergeSort(arr, 0, n-1); 
}

void sequentialMergeSort(int *arr, int n){
	sequentialInternalMergeSort(arr, 0, n-1);
}

void sequentialInternalMergeSort(int arr[], int l, int r) { 
    if (l < r) { 
        int m = l+(r-l)/2; 
  
        sequentialInternalMergeSort(arr, l, m); 
        sequentialInternalMergeSort(arr, m+1, r); 
        sequentialMerge(arr, l, m, r); 
    } 
} 

void parallelInternalMergeSort(int arr[], int l, int r) { 
    if (l < r) { 
        int m = l+(r-l)/2; 
  
        parallelInternalMergeSort(arr, l, m); 
        parallelInternalMergeSort(arr, m+1, r); 
        parallelMerge(arr, l, m, r); 
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