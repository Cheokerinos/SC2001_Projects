#ifndef Sorting_H
#define Sorting_H

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef uint64_t comparison_count;

typedef comparison_count (*SortingFunction)(int *begin, const int *end, ...);

comparison_count insertionSort(int *begin, const int *end) {
    comparison_count compareCount = 0;
    for (int *now = begin + 1; now != end; ++now) { // starts from the 2nd element in the array
        int key = *now; // Current element to be inserted
        int *p = now - 1; // Pointer to the previous element
        while (p >= begin && ++compareCount && *p > key) { // If key is smaller than current p, shift p to the right
            *(p + 1) = *p; // Move the element at p one position to the right
            --p; // Move the pointer to the previous element
        }
        *(p + 1) = key; // Insert the key in its correct position
    }
    return compareCount; // Return the total number of comparisons made
}


/*comparison_count mergeWithExistingTemp(int *begin, int *mid, const int *end, int *temp) {
    comparison_count compareCount = 0;
    int *left = begin;
    int *right = mid;
    int *p = temp;
    while (left != mid && right != end) {
        if (*left <= *right) {
            *p++ = *left++;
        } else {
            *p++ = *right++;
        }
        ++compareCount;
    }
    while (left != mid) {
        *p++ = *left++;
    }
    while (right != end) {
        *p++ = *right++;
    }
    memcpy(begin, temp, sizeof(int) * (end - begin));
    return compareCount;
}*/

comparison_count merge(int *begin, int *mid, const int *end) {
    comparison_count compareCount = 0;
    size_t size = end - begin;
    int *temp = (int *) malloc(size * sizeof(int));
    if (!temp) {
        return 0;
    }

    int *left = begin; // left subarray
    int *right = mid;  // right subarray
    int *p = temp;     // pointer to temp array

    while (left < mid && right < end) {
        if (*left <= *right) {
            *p++ = *left++;
        } else {
            *p++ = *right++;
        }
        compareCount++;
    }

    while (left < mid) { // if elements still exist in left subarray
        *p++ = *left++;
    }

    while (right < end) { // if elements still exist in right subarray
        *p++ = *right++;
    }

    memcpy(begin, temp, sizeof(int) * size);
    free(temp);
    return compareCount;
}

comparison_count mergeSort(int *begin, const int *end) { //* For part D when comparing the hybrid algorithm to original mergeSort algo
    comparison_count compareCount = 0;
    size_t size = end - begin;
    if (size == 1) {
        return 0;
    }
    int *mid = begin + size / 2;
    compareCount += mergeSort(begin, mid);
    compareCount += mergeSort(mid, end);
    compareCount += merge(begin, mid, end);
    return compareCount;
}

comparison_count mergeSortWithInsertionSort(int *begin, const int *end, int threshold) {
    comparison_count compareCount = 0;
    size_t size = end - begin;
    if (size <= threshold) {
        compareCount += insertionSort(begin, end); //If size is below threshold, will start using insertionSort
    } else { //If not, use mergeSort until the size decreases below or equivalent to the threshold
        int *mid = begin + size / 2;
        compareCount += mergeSortWithInsertionSort(begin, mid, threshold);
        compareCount += mergeSortWithInsertionSort(mid, end, threshold);
        compareCount += merge(begin, mid, end);
    }
    return compareCount;
}

/*compare_count_t mergeSortNoRecursion(int *begin, int *end) {
    size_t size = end - begin;
    compare_count_t compareCount = 0;
    int *temp = (int *) malloc(size * sizeof(int));
    for (size_t step = 1; step < size; step <<= 1) {
        for (size_t i = 0; i < size; i += step * 2) {
            int *left = begin + i;
            int *mid = left + step;
            int *right = mid + step;
            if (mid >= end) {
                continue;
            }
            if (right > end) {
                right = end;
            }
            compareCount += mergeWithExistingTemp(left, mid, right, temp);
        }
    }
    free(temp);
    return compareCount;
}*/

/*compare_count_t mergeSortNoRecursionWithInsertionSort(int *begin, int *end, int threshold) {
    size_t size = end - begin;
    compare_count_t compareCount = 0;
    int *temp = (int *) calloc(size, sizeof(int));
    for (int *p = begin; p < end; p += threshold) {
        int *q = p + threshold;
        if (q > end) {
            q = end;
        }
        compareCount += insertionSort(p, q);
    }
    for (size_t step = threshold; step < size; step <<= 1) {
        for (size_t i = 0; i < size; i += step * 2) {
            int *left = begin + i;
            int *mid = left + step;
            int *right = mid + step;
            if (mid >= end) {
                continue;
            }
            if (right > end) {
                right = end;
            }
            compareCount += mergeWithExistingTemp(left, mid, right, temp);
        }
    }
    free(temp);
    return compareCount;
}*/

#endif //PROJECT1_Sorting_H