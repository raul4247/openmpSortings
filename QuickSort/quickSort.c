#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

#define INPUT_SIZE 1000000
#define RANGES 6
const int TEST_RANGES[RANGES] = {10000, 50000, 100000, 300000, 500000, 1000000};

void parallelQuicksort(int *arr, int size);
int* fillArray(int *array, int size);
bool isSorted(int* arr, int n);
void copyArr(int* arr, int n, int* newArr);

void quicksort(int *arr, int size);
void quickSort_internal(int* arr, int left, int right);
void quickSort_parallel(int* array, int lenArray, int numThreads);
void quickSort_parallel_internal(int* array, int left, int right, int cutoff) ;

int main(void){
 	int *bigArr, *testArr;
    int i, input_range;
    time_t begin, end;
    float seq, par, speedup;

    bigArr  = (int*) malloc(INPUT_SIZE*sizeof(int));
    testArr = (int*) malloc(INPUT_SIZE*sizeof(int));

    fillArray(bigArr, INPUT_SIZE);
    
    for (i = 0; i < RANGES; i++)
    {
        input_range = TEST_RANGES[i];

        printf("Testing array[%i]\n", input_range);
        
        copyArr(bigArr, input_range, testArr);

        begin = clock();
        // Sequential quicksort
        quicksort(testArr, input_range);
        end = clock();

        seq = (double)(end - begin) / CLOCKS_PER_SEC;

        printf("\tSequential:\t%fs\t", seq);
        printf("-- %s --\n", (isSorted(testArr, input_range) ? "In order!" : "Not in order!"));

        copyArr(bigArr, input_range, testArr);
        begin = clock();
        // Parallel quicksort
        quickSort_parallel(testArr, input_range, omp_get_max_threads());
        end = clock();

        par = (double)(end - begin) / CLOCKS_PER_SEC;
        
        printf("\tParallel:\t%fs\t", par);
        printf("-- %s --\n", (isSorted(testArr, input_range) ? "In order!" : "Not in order!"));

        speedup = (par == 0) ? 0 : ((seq/par)*100)-100;

        printf("\tSpeedup:\t%.2f%%\n", speedup);
    }

    free(bigArr);
    free(testArr);

 	return 0;
}

int* fillArray(int *array, int size){
    int i;
    for(i=0;i<size;i++)
        scanf("%i", &array[i]);
}

// A utility function to swap two elements 
void swap(int* arr, int a, int b) 
{ 
    int tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
} 

void quicksort(int* arr, int n)
{
    quickSort_internal(arr, 0, n-1);
}

void quickSort_internal(int* arr, int left, int right) 
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

  	/* PARTITION PART */
	while (i <= j)
    {
		while (arr[i] < pivot) i++;

		while (arr[j] > pivot) j--;

		if (i <= j)
        {
			swap(arr, i, j);

			i++;
			j--;
		}
	}

	/* RECURSION PART */
	if (left < j) quickSort_internal(arr, left, j);
	if (i< right) quickSort_internal(arr, i, right);
}

void quickSort_parallel(int* array, int lenArray, int numThreads)
{
	int cutoff = 1000;

	#pragma omp parallel num_threads(numThreads)
	{	
		#pragma omp single nowait
		{
			quickSort_parallel_internal(array, 0, lenArray-1, cutoff);	
		}
	}
}

void quickSort_parallel_internal(int* arr, int left, int right, int cutoff) 
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	/* PARTITION PART */
	while (i <= j)
    {
		while (arr[i] < pivot) i++;

		while (arr[j] > pivot) j--;

		if (i <= j)
        {
			swap(arr, i, j);

			i++;
			j--;
		}
	}

	if ( right - left < cutoff )
    {
		if (left < j)  quickSort_parallel_internal(arr, left, j,  cutoff);			
		if (i < right) quickSort_parallel_internal(arr, i, right, cutoff);
	}
    else
    {
		#pragma omp task 	
		quickSort_parallel_internal(arr, left, j, cutoff);

		#pragma omp task 	
		quickSort_parallel_internal(arr, i, right, cutoff);
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