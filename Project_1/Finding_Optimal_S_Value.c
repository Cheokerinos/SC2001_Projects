#include <stdio.h>
//#include <malloc.h>
#include "[Mei] sorting.h"
#include <stdlib.h>
#include <time.h>

//! This is for Part Ciii). Finding optimal S value using a range of array sizes and a range of S values and output them accordingly.
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
    srand(time(0)); 

    int sizes[] = {1000, 5000, 10000, 50000, 100000, 200000, 500000, 750000, 1000000, 2000000, 5000000, 7500000, 10000000}; //! Can change to whatever values we want
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int thresholds[] = {10, 20, 50, 100, 200, 500, 1000}; //! Can change this too
    int num_thresholds = sizeof(thresholds) / sizeof(thresholds[0]);

    FILE *file = fopen("Optimal_S_Part_Ciii.csv", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(file, "Size,Threshold,Comparisons\n");

    // Loops through each array size and S value given. 
    for (int i = 0; i < num_sizes; i++) {
        int Size = sizes[i];
        
        for (int j = 0; j < num_thresholds; j++) {
            int threshold = thresholds[j];

            //For checking purposes only
            printf("\nAllocating memory for array size = %d with threshold = %d\n", Size, threshold);

            int *array = (int *) malloc(Size * sizeof(int));
            if (array == NULL) {
                perror("Memory allocation failed");
                return 1;
            }

            gen_random_numbers(array, Size);

            comparison_count comparisons = mergeSortWithInsertionSort(array, array + Size, threshold);

            fprintf(file, "%d,%d,%llu\n", Size, threshold, comparisons);

            free(array);
        }
    }

    fclose(file);

    printf("\nDone\n");

    return 0;
}
