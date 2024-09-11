#include <stdio.h>
//#include <malloc.h>
//#include <sorting.h>
#include <stdlib.h>
#include <time.h>


void gen_random_numbers(int *array, int len, int min, int max){
    for (int i = 0; i < len; i++)
        array[i] = rand() % (max - min + 1) + min;
}

int main() {
    srand(time(0));
    int Size = 100;
    int *array = (int *) malloc(Size * sizeof(int));
    gen_random_numbers(array,Size,1,100000);

    for(int i=0;i<Size;i++){
        printf("%d ", array[i]);
    }
    free(array);
    return 0;
}
