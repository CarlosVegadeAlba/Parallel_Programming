#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <limits.h>
#include <math.h>

// Method to check the final array
int isSorted(int *arr, int n) {
    int i;
    for ( i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Method to compare two elements and swap them if necessary
void compare(int *arr, int a, int b) {
    if (arr[a] > arr[b]) {
        int tmp = arr[a];
        arr[a] = arr[b];
        arr[b] = tmp;
    }
}

// Method to split the array in odd and even elements
void oddEvenSplit(int *arr, int *odd, int *even, int n) {
    int i;
    for (i = 0; i < n/2; i++) {
        odd[i] = arr[2 * i + 1];
        even[i] = arr[2 * i];
    }
}

// Method to join the odd and even elements in the array
void oddEvenJoin(int *arr, int *odd, int *even, int n) {
    int i;
    for (i = 0; i < n/2; i++) {
        arr[2 * i + 1] = odd[i];
        arr[2 * i] = even[i];
    }
}

// Method to merge the odd and even elements
void oddEvenMerge(int *arr, int n, int level, int limit) {
    if (n == 1) return;
    if (n == 2) {
        compare(arr, 0, 1);
        return;
    }
    else {
        int *odd = (int *)malloc(sizeof(int) * (n / 2));
        int *even = (int *)malloc(sizeof(int) * (n / 2));
        int i;
        oddEvenSplit(arr, odd, even, n);

        oddEvenMerge(odd, n / 2, level, limit);

        oddEvenMerge(even, n / 2, level, limit);

        oddEvenJoin(arr, odd, even, n);

        for (i = 1; i < n / 2; i++) {
            compare(arr, 2 * i - 1, 2 * i);
        }

        free(odd);
        free(even);
    }
}

// Method 2 to sort the array
void oddEvenMergeSort(int *arr, int n, int *level, int limit) {
    if (n > 1) {
        int m = n / 2;
        int localLevel = *level;

        if (localLevel < limit)
        {
            localLevel += 1;
            #pragma omp task shared(arr)
            oddEvenMergeSort(arr, m, &localLevel, limit);

            #pragma omp task shared(arr)
            oddEvenMergeSort(arr + m, n - m, &localLevel, limit);

            #pragma omp taskwait
            oddEvenMerge(arr, n, localLevel, limit);
        }
        else {
            oddEvenMergeSort(arr, m, &localLevel, limit);

            oddEvenMergeSort(arr + m, n - m, &localLevel, limit);

            oddEvenMerge(arr, n, 0, localLevel);
        }
    }
}

// Method 1 to sort the array
void sort(int *arr, int *newArr, int n, int limit, int newSize) {
    
    #pragma omp parallel
    {
        int i;
        int *level = (int *)malloc(sizeof(int));
        *level = 0;
        #pragma omp single
        oddEvenMergeSort(newArr, newSize, level, limit);

        // Original elements
        #pragma omp for schedule(static)
        for ( i = 0; i < n; i++) {
            arr[i] = newArr[i];
        }
        
    }

}

// Fix the size of the array if it is not a power of 2
int * sizeFix(int * arr, int n, int *newSize){

    *newSize = pow(2, ceil(log2(n)));
    int *newArr = (int *)malloc(sizeof(int) * *newSize);
    int i;
    
    for (i = n; i < *newSize; i++) {
        newArr[i] = INT_MAX; //Fill with INT_MAX
    }

    return newArr;
}

// Main method
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "use: %s <número de elementos> <limit> <threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int limit = atoi(argv[2]);
    int nthreads = atoi(argv[3]);
    omp_set_num_threads(nthreads);
    if (n <= 0) {
        fprintf(stderr, "Invalid number. Write a positive number\n");
        return 1;
    }

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Malloc failed.\n");
        return 1;
    }

    srand(time(NULL));
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = rand() % INT_MAX;
    }

    int * newArr, newSize;
    newArr = sizeFix(arr, n, &newSize);

    double start_time = omp_get_wtime();
    sort(arr, newArr, n, limit, newSize);
    double end_time = omp_get_wtime();

    if (isSorted(newArr, n) == 1)
    {
        printf("Total time: %f \n", end_time - start_time);
        if (n <= 100) {
        printf("\n");
        }
    }

    free(newArr);
    free(arr);
    return 0;
}