#include <stdio.h>
//#include <malloc.h>
#include "[Mei] sorting.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

//! This is for Part D). This is for comparsion between MergeSort & Hybrid Algorithm.
//! WARNING: The current csv file will be overwrite with new values if you don't rename the csv file in line 36.

//For generating random numbers in the array
void gen_random_numbers(int *array, int len){
    for (int i = 0; i < len; i++) {
        //array[i] = rand() % (max - min + 1) + min;
        
        // Legit code for generating random sizes
        unsigned long x;
        x = rand(); // Generates between 0 to 32767
        x <<= 15; // Shifts bits by 15 places to the left, aka 2^15. Increases number of bits, hence increasing range of values availble to be output. 
        x ^= rand(); //Used to add more randomness when generating numbers. 
        // Without XOR, it is limited to its randomness due to the range of rand().
        // With XOR, there is a greater variety of values created.
        // This also avoids any biased patterns in random numbers. 
        x %= 10000001; //Change this value for MAX Size. This generates up to number 10 million (10000001). Limits the range to be 0 to 10 million. 
        array[i] = x;
    }
}

int main() {

    int Size = 10000000;
    int threshold = 50;
    clock_t start, end;
    double cpu_time_used;

    FILE *file = fopen("Comparisons_Part_D.csv", "w");

    if (file == NULL) {
        printf("Cannot open CSV file.\n");
        return 1;
    }

    fprintf(file, "Algorithm,Time (seconds),Comparisons\n");

    int *array = (int *) malloc(Size * sizeof(int));

    if (array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Generate random array
    gen_random_numbers(array, Size);

    // Measure time for MergeSort
    start = clock(); //Returns number of clock ticks when program starts
    total_comparisons totalComparisons = mergeSort(array, array + Size);
    end = clock(); // Stops the recording of time taken
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    // end - start is the number of clock ticks from clock(). 
    // CLOCKS_PER_SEC is a constant that represents number of clock ticks per second. This is defined in the function. 
    // Time taken in seconds as cpu time used in double format. 

    printf("MergeSort Time: %f seconds\n", cpu_time_used);
    printf("MergeSort Comparisons: %lld\n", totalComparisons);
    fprintf(file, "MergeSort,%f,%lld\n", cpu_time_used, totalComparisons);

    // Generate a new random array for hybrid sort
    gen_random_numbers(array, Size);

    // Measure time for HybridMergeSort
    start = clock();
    comparisons = mergeSortWithInsertionSort(array, array + Size, threshold);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Hybrid Time: %f seconds\n", cpu_time_used);
    printf("Hybrid Comparisons: %lld\n", totalComparisons);
    fprintf(file, "Hybrid,%f,%lld\n", cpu_time_used, totalComparisons);

    // Free allocated memory
    free(array);

    return 0;
}

