#include <stdio.h>
//#include <malloc.h>
#include "[Mei] sorting.h"
#include <stdlib.h>
#include <time.h>

//For generating random numbers in the array
void gen_random_numbers(int *array, int len, int min, int max){
    for (int i = 0; i < len; i++)
        array[i] = rand() % (max - min + 1) + min;
}

int main() {
    srand(time(0));//Generate different results each time the program runs

    // Legit code for generating random sizes
    unsigned long x;
    x = rand();
    x <<= 15;
    x ^= rand();
    x %= 10000001; //Change this value for MAX Size. This generates up to number 10 million 

    int Size = 1000 + (int) x; //Generates a size between 1000 to 10 million. The addition of 1000 shifts the range from 1000 instead of 0.
    printf("Generated Size: %d\n", Size);

    int threshold = 1; //! Find the sweet spot for the threshold

    // Dynamically allocate memory for an array of size 'Size'
    int *array = (int *) malloc(Size * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    gen_random_numbers(array,Size,1,10000000);

    printf("Before sorting:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Call mergeSortWithInsertionSort to start the hybrid algorithm
    comparison_count comparisons = mergeSortWithInsertionSort(array, array + Size, threshold);

    printf("After sorting:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    printf("Total comparisons: %llu\n", comparisons);\

    free(array);
    return 0;
}