#include <stdio.h>
//#include <malloc.h>
#include "[Mei] sorting.h"
#include <stdlib.h>
#include <time.h>

//! This is for Part Ci). This is for given array sizes that we have manually coded and selected.
//! WARNING: The current csv file will be overwrite with new values if you don't rename the csv file in line 34.

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
    srand(time(0)); //Generate different results each time the program runs

    //Linear growth
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 200000, 500000, 750000, 1000000, 2000000, 5000000, 7500000, 10000000};  //! Can change the sizes here if you need
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);  // Number of array sizes
    int threshold = 50; //! Fixed threshold S value for part Ci).

    // Open CSV file
    FILE *file = fopen("Selected_Array_Sizes_Part_Ci.csv", "w");

    if (file == NULL) {
        printf("Cannot open.\n");
        return 1;
    }

    // Write header row to the CSV file
    fprintf(file, "Size,Threshold,Comparisons\n");

    for (int i = 0; i < num_sizes; i++) {
        int Size = sizes[i];

        printf("\nAllocating memory for Size = %d\n", Size);

        int *array = (int *) malloc(Size * sizeof(int));

        if (array == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }

        gen_random_numbers(array,Size); //Generates random values in the stated array size

        printf("Before sorting:\n");
        for (int i = 0; i < 10; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        // Call mergeSortWithInsertionSort to start the hybrid algorithm
        total_comparisons totalComparisons = mergeSortWithInsertionSort(array, array + Size, threshold);

        printf("After sorting:\n");
        for (int i = 0; i < 10; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        printf("Total comparisons: %llu\n", totalComparisons);

        // Write results to csv
        fprintf(file, "%d,%d,%llu\n", Size, threshold, totalComparisons);

        free(array);

    }

    fclose(file);

    printf("\nDone\n");

    return 0;
}