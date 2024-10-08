#include <stdio.h>
//#include <malloc.h>
#include "[Mei] sorting.h"
#include <stdlib.h>
#include <time.h>

//! This is for Part Ci). This is for array sizes increment in the value we have provided.
//! WARNING: The current csv file will be overwrite with new values if you don't rename the csv file in line 35.

//For generating random numbers in the array
void gen_random_numbers(int *array, int len){
    for (int i = 0; i < len; i++) {    
        unsigned long x;
        x = rand() + 1; // Generates between 1 to 32767
        x <<= 15; // Shifts bits by 15 places to the left, aka 2^15. Increases number of bits, hence increasing range of values
        //availble to be output. 

        x ^= rand(); //Used to add more randomness when generating numbers. 
        // Without XOR, it is limited to its randomness due to the range of rand().
        // With XOR, there is a greater variety of values created.
        // This also avoids any biased patterns in random numbers. 

        x %= 10000001; //Change this value for MAX Size. This generates up to number 10 million (10000001). 
        //Limits the range to be 1 to 10 million. 
        
        array[i] = x;
    }
        
}

int main() {
    srand(time(0)); //Generate different results each time the program runs

    int Size = 1000;
    int increment = 1000;  //! Increment size by 1000 each iteration. Change this value or value of Size and adjust accordingly to our needs.
    int max_size = 20000;  
    int threshold = 50;  //! Fixed threshold S value for part Ci). Once find optimal value of S, can replace this value with that.
    clock_t start, end;
    double cpu_time_used;

    // Open CSV file
    FILE *file = fopen("Increment_Array_Sizes_Part_Ci.csv", "w");

    if (file == NULL) {
        printf("Cannot open.\n");
        return 1;
    }

    // Write header row to the CSV file
    fprintf(file, "Size,Threshold,Comparisons,CPU Time (ms)\n");

    while (Size <= max_size) {

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

        // Record CPU time for mergeSortWithInsertionSort().
        start = clock();
        // Call mergeSortWithInsertionSort to start the hybrid algorithm
        total_comparisons totalComparisons = mergeSortWithInsertionSort(array, array + Size, threshold);
        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

        printf("After sorting:\n");
        for (int i = 0; i < 10; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        printf("Total comparisons: %llu\n", totalComparisons);
        printf("Total time taken in ms: %.3f milliseconds\n", cpu_time_used);

        // Write results to csv
        fprintf(file, "%d,%d,%llu,%.3f\n", Size, threshold, totalComparisons, cpu_time_used);

        free(array);

        Size += increment;
    }

    fclose(file);

    printf("\nFinal Size = %d\n", Size); //Displays if it has hit 10 million in range.

    return 0;
}