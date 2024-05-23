#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

int isSorted(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;  // No está ordenado
        }
    }
    return 1;  // Está ordenado
}

// Función de comparación y posible intercambio
void compare(int *arr, int a, int b) {
    if (arr[a] > arr[b]) {
        int tmp = arr[a];
        arr[a] = arr[b];
        arr[b] = tmp;
    }
}

void oddEvenSplit(int *arr, int *odd, int *even, int n) {
    // #pragma omp parallel for
    for (int i = 0; i < n/2; i++) {
        odd[i] = arr[2 * i + 1];
        even[i] = arr[2 * i];
    }
}

void oddEvenJoin(int *arr, int *odd, int *even, int n) {
    // #pragma omp parallel for
    for (int i = 0; i < n/2; i++) {
        arr[2 * i + 1] = odd[i];
        arr[2 * i] = even[i];
    }
}

// Función de fusión de elementos pares e impares
void oddEvenMerge(int *arr, int n) {
    if (n == 1) return;
    if (n == 2) {
        compare(arr, 0, 1);
        return;
    }
    else {
        int *odd = (int *)malloc(sizeof(int) * (n / 2));
        int *even = (int *)malloc(sizeof(int) * (n / 2));
        oddEvenSplit(arr, odd, even, n);
        
        // #pragma omp task shared(odd, even)
        oddEvenMerge(odd, n / 2);

        // #pragma omp task shared(odd, even)
        oddEvenMerge(even, n / 2);

        oddEvenJoin(arr, odd, even, n);

        for (int i = 1; i < n / 2; i++) {
            compare(arr, 2 * i - 1, 2 * i);
        }

        free(odd);
        free(even);
    }
}

// Función recursiva para ordenar
void oddEvenMergeSort(int *arr, int n, int *level, int limit) {
    if (n > 1) {
        int m = n / 2;
        printf("id: \n", getpid());

        #pragma omp parallel
        {
            #pragma omp single
            {
                if (*level < limit)
                {
                    *level += 1;
                    #pragma omp task shared(arr) // Crea una tarea para la primera mitad
                    oddEvenMergeSortRecursive(arr, m, level, limit);

                    #pragma omp task shared(arr) // Crea una tarea para la segunda mitad
                    oddEvenMergeSortRecursive(arr + m, n - m, level, limit);

                    #pragma omp taskwait // Espera a que ambas tareas anteriores completen
                    oddEvenMerge(arr, n);
                }
                else
                {
                    oddEvenMergeSortSequential(arr, m);

                    oddEvenMergeSortSequential(arr + m, n - m);

                    oddEvenMerge(arr, n);
                }
            }
        }   
        
    }
}

// Función recursiva para ordenar
void oddEvenMergeSortRecursive(int arr, int n, int level, int limit) {
    if (n > 1) {
        int m = n / 2;
        printf("id: \n", getpid());

        if (*level < limit)
        {
            *level += 1;
            #pragma omp task shared(arr) // Crea una tarea para la primera mitad
            oddEvenMergeSortRecursive(arr, m, level, limit);

            #pragma omp task shared(arr) // Crea una tarea para la segunda mitad
            oddEvenMergeSortRecursive(arr + m, n - m, level, limit);

            #pragma omp taskwait // Espera a que ambas tareas anteriores completen
            oddEvenMerge(arr, n);
        }
        else
        {
            oddEvenMergeSortSequential(arr, m);

            oddEvenMergeSortSequential(arr + m, n - m);

            oddEvenMerge(arr, n);
        }
    }
}

// Función recursiva para ordenar
void oddEvenMergeSortSequential(int *arr, int n) {
    if (n > 1) {
        int m = n / 2;

        oddEvenMergeSortSequential(arr, m);

        oddEvenMergeSortSequential(arr + m, n - m);

        oddEvenMerge(arr, n);
        
    }
}
// amMgjafs/f11lfd$



void sort(int *arr, int n, int limit) {
    int newSize = pow(2, ceil(log2(n))); // Tamaño ajustado a la próxima potencia de 2
    int *newArr = (int *)malloc(sizeof(int) * newSize);
    int *level = (int)malloc(sizeof(int));
    *level = 0;

    for (int i = 0; i < newSize; i++) {
        newArr[i] = (i < n) ? arr[i] : INT_MAX; // Rellena con INT_MAX
    }
    
    // Crear semaforo con valor = level
    oddEvenMergeSort(newArr, newSize, level, limit);

    // Copia de vuelta solo los elementos originales
    for (int i = 0; i < n; i++) {
        arr[i] = newArr[i];
    }
    free(newArr);
}

// Función principal para probar el algoritmo
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <número de elementos>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int limit = atoi(argv[2]);
    if (n <= 0) {
        fprintf(stderr, "Ingrese un número de elementos válido. Debe ser mayor que 0.\n");
        return 1;
    }

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Error al asignar memoria.\n");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100; // Números aleatorios entre 0 y 999999
    }

    // printf("Procesando %d elementos...\n", n);

    // if (n <= 100)
    // {
    //     printf("Array sin ordenar: ");
    //     for (int i = 0; i < n; i++) {
    //             printf("%d ", arr[i]);
    //         }
    //         printf("\n");
    // }

    double start_time = omp_get_wtime();
    sort(arr, n, limit);
    double end_time = omp_get_wtime();

    if (isSorted(arr, n) == 1)
    {
        printf("Tiempo total: %f \n", end_time - start_time);
        if (n <= 100) {
        printf("\n");
        }
    }
    // printf("Array ordenado: ");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");


    return 0;
}