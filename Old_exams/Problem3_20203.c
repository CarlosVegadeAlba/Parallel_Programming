#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h> // Necesario para memcpy

int main (){

    double **A, **At, **B;
    double count = 0;
    int n = 4;
    int eps = 10;
    int i, j, flag;

    // Allocate normal matrix
    A = (double**)malloc(sizeof(double*) * n);
    for(i = 0; i < n; i++){
        A[i] = (double*) malloc(sizeof(double) * n);
        if (!A[i]){
            perror("Memory allocation failed");
            return -1;
        }
    }
    if(!A){
        perror("Memory allocation failed");
        return -1;
    }

    // Allocate transpose matrix
    At = (double**)malloc(sizeof(double*) * n);
    for(i = 0; i < n; i++){
        At[i] = (double*) malloc(sizeof(double) * n);
        if (!At[i]){
            perror("Memory allocation failed");
            return -1;
        }
    }
    if(!At){
        perror("Memory allocation failed");
        return -1;
    }

    // Allocate result matrix
    B = (double**)malloc(sizeof(double*) * n);
    for(i = 0; i < n; i++){
        B[i] = (double*) malloc(sizeof(double) * n);
        if (!B[i]){
            perror("Memory allocation failed");
            return -1;
        }
    }
    if(!B){
        perror("Memory allocation failed");
        return -1;
    }

    // Initialize
    count = 0;
    for(i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            A[i][j] = count;
            B[i][j] = count;
            count++;
        }
    }

    // Show that the matrix is well created
    printf("Matrix A:\n");
    for(i = 0; i < n; i++){
        printf("(");
        for (j = 0; j < n; j++){
            printf("%4.2lf, ", A[i][j]);
        }
        printf(")\n");
    }

    // Transpose the matrix using memcpy
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            At[j][i] = A[i][j];
        }
    }

    // Show the transposed matrix
    printf("\nMatrix At (transposed):\n");
    for(i = 0; i < n; i++){
        printf("(");
        for (j = 0; j < n; j++){
            printf("%4.2lf, ", At[i][j]);
        }
        printf(")\n");
    }


    // Calculate the average
    flag=0;

     

    #pragma omp parallel for private(i,j)
    for(i=1; i<(n-1); i++){
        for(j=1; j<(n-1); j++){
            B[i][j] = (A[i-1][j] + A[i+1][j] + At[i-1][j] + At[i+1][j])/4;
        }
    }
    

    // Show result matrix
    printf("\nMatrix Average Neighors:\n");
    for(i = 0; i < n; i++){
        printf("(");
        for (j = 0; j < n; j++){
            printf("%4.2lf, ", B[i][j]);
        }
        printf(")\n");
    }


    // Free allocated memory
    for(i = 0; i < n; i++){
        free(A[i]);
        free(At[i]);
        free(B[i]);
    }
    free(A);
    free(At);
    free(B);

    return 0;
}
