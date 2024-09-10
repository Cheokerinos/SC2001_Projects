#ifndef Sort_H
#define Sort_H

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>

typedef uint64_t comparison_count;

typedef comparison_count (*SortingFunction)(int *begin, const int *end, ...);

comparison_count insertionSort(int *begin, const int *end) {
    comparison_count compareCount = 0;
    for (int *now = begin + 1; now != end; ++now) {
        int key = *now;
        int *p = now - 1;
        while (p >= begin && ++compareCount && *p > key) {
            *(p + 1) = *p;
            --p;
        }
        *(p + 1) = key;
    }
    return compareCount;
}

comparison_count mergeWithExistingTemp(int *begin, int *mid, const int *end, int *temp) {
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
}

comparison_count merge(int *begin, int *mid, const int *end) {
    comparison_count compareCount = 0;
    size_t size = end - begin;
    int *temp = (int *) malloc(size * sizeof(int));
    compareCount += mergeWithExistingTemp(begin, mid, end, temp);
    free(temp);
    return compareCount;
}

comparison_count mergeSort(int *begin, const int *end) {
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
        compareCount += insertionSort(begin, end);
    } else {
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

#endif //PROJECT1_SORT_H