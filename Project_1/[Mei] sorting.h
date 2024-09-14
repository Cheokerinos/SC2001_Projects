#ifndef Sorting_H
#define Sorting_H

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef uint64_t total_comparisons;

typedef total_comparisons (*SortingFunction)(int *begin, const int *end, ...);

total_comparisons insertionSort(int *begin, const int *end) {
    total_comparisons totalComparisons = 0;
    for (int *now = begin + 1; now != end; ++now) { // starts from the 2nd element in the array
        int key = *now; // Current element to be inserted
        int *p = now - 1; // Pointer to the previous element
        while (p >= begin && ++totalComparisons && *p > key) { // If key is smaller than current p, shift p to the right
            *(p + 1) = *p; // Move the element at p one position to the right
            --p; // Move the pointer to the previous element
        }
        *(p + 1) = key; // Insert the key in its correct position
    }
    return totalComparisons; // Return the total number of comparisons made
}

total_comparisons merge(int *begin, int *middle, const int *end) {
    total_comparisons totalComparisons = 0;
    size_t size = end - begin;
    int *temp = (int *) malloc(size * sizeof(int));
    if (!temp) {
        return 0;
    }

    int *left = begin; // left subarray
    int *right = middle;  // right subarray
    int *p = temp;     // pointer to temp array

    while (left < middle && right < end) {
        if (*left <= *right) {
            *p++ = *left++;
        } else {
            *p++ = *right++;
        }
        totalComparisons++;
    }

    while (left < middle) { // if elements still exist in left subarray
        *p++ = *left++;
    }

    while (right < end) { // if elements still exist in right subarray
        *p++ = *right++;
    }

    memcpy(begin, temp, sizeof(int) * size);
    free(temp);
    return totalComparisons;
}

total_comparisons mergeSort(int *begin, const int *end) { //* For part D when comparing the hybrid algorithm to original mergeSort algo
    total_comparisons totalComparisons = 0;
    size_t size = end - begin;
    if (size == 1) {
        return 0;
    }
    int *middle = begin + size / 2;
    totalComparisons += mergeSort(begin, middle);
    totalComparisons += mergeSort(middle, end);
    totalComparisons += merge(begin, middle, end);
    return totalComparisons;
}


total_comparisons mergeSortWithInsertionSort(int *begin, const int *end, int threshold) {
    total_comparisons totalComparisons = 0;
    size_t arraySize = end - begin;
    if (arraySize <= threshold) {
        totalComparisons += insertionSort(begin, end); //If size is below threshold, will start using insertionSort
    } else { //If not, use mergeSort until the size decreases below or equivalent to the threshold
        int *middle = begin + arraySize / 2;
        totalComparisons += mergeSortWithInsertionSort(begin, middle, threshold);
        totalComparisons += mergeSortWithInsertionSort(middle, end, threshold);
        totalComparisons += merge(begin, middle, end);
    }
    return totalComparisons;
}

#endif 