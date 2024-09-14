#include <stdio.h>
//#include <malloc.h>
#include "[Mei] sorting.h"
#include <stdlib.h>
#include <time.h>

//! This is for Part Cii). This is for fixed array sizes while using different values of S.
//! WARNING: The current csv file will be overwrite with new values if you don't rename the csv file in line 32.

//For generating random numbers in the array
void gen_random_numbers(int *array, int len){
    for (int i = 0; i < len; i++) {
        //array[i] = rand() % (max - min + 1) + min;
        
        // Legit code for generating random sizes
        unsigned long x;
        x = rand();
        x <<= 15;
        x ^= rand();
        x %= 10000001; //Change this value for MAX Size. This generates up to number 10 million (10000001)
        array[i] = x;
    }
}

int main() {
    srand(time(0));  // Generate different results each time the program runs

    int fixed_size = 10000;  //! Can change the fixed sizes of array accordingly. 
    int thresholds[] = {10, 16, 32, 50, 100, 200, 500, 1000};  // Set of threshold we can use to test on. Generally people use between 16 to 32 but we can try others.
    int num_thresholds = sizeof(thresholds) / sizeof(thresholds[0]);  // Number of thresholds we are testing
    clock_t start, end;
    double cpu_time_used;

    FILE *file = fopen("Different_S_Values_Part_Cii.csv", "w");

    if (file == NULL) {
        printf("Cannot open CSV file.\n");
        return 1;
    }

    fprintf(file, "Size,Threshold,Comparisons,CPU Time (ms)\n");

    for (int i = 0; i < num_thresholds; i++) {
        int threshold = thresholds[i];

        printf("\nTesting with Size = %d and Threshold = %d\n", fixed_size, threshold);

        int *array = (int *)malloc(fixed_size * sizeof(int));
        if (array == NULL) {
            printf("Memory allocation failed for Size = %d\n", fixed_size);
            return 1;
        }

        gen_random_numbers(array, fixed_size);

        start = clock();
        // Call mergeSortWithInsertionSort to sort the array and get the comparison count
        total_comparisons totalComparisons = mergeSortWithInsertionSort(array, array + fixed_size, threshold);
        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

        printf("Total comparisons with Threshold = %d: %llu\n", threshold, totalComparisons);

        fprintf(file, "%d,%d,%llu,%.3f\n", fixed_size, threshold, totalComparisons, cpu_time_used);

        free(array);
    }

    fclose(file);

    printf("\nDone\n");

    return 0;
}
