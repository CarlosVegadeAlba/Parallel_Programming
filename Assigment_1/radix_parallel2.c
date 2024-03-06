#include "mt19937-64.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <omp.h>
/* #define MAX_NUMBER 8 */ //Only for developing 
#define MAXNBITS sizeof(unsigned long long) * 8


int check_number_of_elements(int number);
unsigned long long max(unsigned long long list[], int lenght);
int getNumberOfIterationsFromMax(unsigned long long maxNum, int numBits);
int countBits(unsigned long long number);
void getStringFromNumber(unsigned long long number, char * numberInBits);


int main(int argc, char *argv[]) {
    if (argc != 4) {
        // At least 4 arguments
        printf("Usage: %s number_of_elements number_of_bits number_of_threads\n", argv[0]);
        return -1;
    }

    char *endptr;
    unsigned long long number_of_elements = strtoull(argv[1], &endptr, 10);

    // Check if the coversion was done successfully
    if (endptr == argv[1]) {
        printf("La conversión falló. Asegúrate de que ingresaste un número válido.\n");
        return 1;
    }

    int number_of_bits = atoi(argv[2]);
    if (number_of_bits<1){
        printf("\tERROR, the minimum number of bits is 1\n");
        return -1;
    }
    if (number_of_bits>=64){
        printf("\tERROR, the maximum number of bits is 63\n");
        return -1;
    }

    int number_of_threads = atoi(argv[3]);
    if (number_of_threads<1){
        printf("\tERROR, the minimum number of threads is 1\n");
        return -1;
    }
    if (number_of_threads>80){
        printf("\tERROR, the maximum number of threads is 80\n");
        return -1;
    }
    omp_set_num_threads(number_of_threads);

    printf("Radix Parallel Sorting with %llu elements, %d bits, %d threads\n", number_of_elements, number_of_bits, number_of_threads);

    unsigned long long* rawArray = malloc(number_of_elements * sizeof(unsigned long long));
    unsigned long long* outputArray = malloc(number_of_elements * sizeof(unsigned long long));
    unsigned long long** countKeys;

    if (!rawArray || !outputArray) {
        fprintf(stderr, "Memory allocation failed\n");
        free(rawArray); // It's safe to call free on NULL
        free(outputArray);
        return -1;
    }

    unsigned long long randomNum=0, maxNum=0, aux1=0, aux2=0, numKeys=1, index=0;
    unsigned int bits;
    int numIterations, i, j, randomValue;
    double time_elapsed;
    double start, end;

    // Random seed
    srand((unsigned) time(NULL));
    randomValue = rand();
    init_genrand64(randomValue);
    
    for (i=0; i<number_of_elements-1; i++){
        randomNum = genrand64_int64(); 
        rawArray[i] = randomNum;
    }
    randomNum = genrand64_int64();
    rawArray[number_of_elements-1] = randomNum;

    // Get the max number
    maxNum = max(rawArray, number_of_elements);
    numIterations = getNumberOfIterationsFromMax(maxNum, number_of_bits);
    
    for(i=1; i<=number_of_bits; i++){
        numKeys = numKeys*2;
    }

    // A Key array per iteration
    countKeys = (unsigned long long**)malloc(numIterations * sizeof(unsigned long long));
    if(!countKeys){
        fprintf(stderr, "Memory allocation failed\n");
        free(rawArray);
        free(outputArray);
        free(countKeys);
        return -1;
    }

    // Alloc for each iteration
    for(i=0; i<numIterations; i++){
        countKeys[i] = (unsigned long long*)calloc(numKeys, sizeof(unsigned long long));
        if (!countKeys[i]){
            free(rawArray);
            free(outputArray);
            for(j=0; j<i; j++){
                free(countKeys[j]);
            }
            free(countKeys);
            return -1;
        }
    }

    bits = (1U << number_of_bits) - 1;

    // Start timing
    start = omp_get_wtime();
    #pragma omp parallel for
    for(i=0; i<numIterations; i++){
        for(j=0; j<number_of_elements; j++){
            unsigned long long aux2 = rawArray[j];
            unsigned int aux1 = (aux2 >> (i*number_of_bits)) & bits;
            countKeys[i][aux1]++;
        }

        // Do cumulative sum
        // PARALELISE 2 -Possible paralelise 2 (more challenging)
        for(j=1; j<numKeys; j++){
            countKeys[i][j]= countKeys[i][j-1] + countKeys[i][j];
        }

    }
    

    for(i=0; i<numIterations; i++){
    // Fill the output array
        // PARALELISE 3 - Order the elements
        //#pragma omp parallel for
        for(j=number_of_elements-1; j>=0; j--){
            aux2 = rawArray[j];
            aux1 = (aux2 >> (i*number_of_bits)) & bits; //aux1 has the bits we are studing in this 
            index = countKeys[i][aux1];
            countKeys[i][aux1]--;
            outputArray[index-1] = rawArray[j];
        }

        // #pragma omp parallel for
        for(j=0; j< number_of_elements; j++){
            rawArray[j] = outputArray[j];
        }
    }
    
    // End timing
    end = omp_get_wtime();

    // Check if the array is sorted well
    for(j=0; j< number_of_elements-1; j++){
        if(rawArray[j] > rawArray[j+1]){
            printf("ERROR, the array is not well sorted\n");
            free(rawArray);
            free(outputArray);
            free(countKeys);
            return -1;
        }
    }

    printf("Success!, Array Sorted succesfully :)\n");
     // Calculate the time elapsed
    time_elapsed = end - start;

    printf("Time elapsed: %f seconds\n", time_elapsed);

    free(rawArray);
    free(outputArray);
    for(j=0; j<numIterations; j++){
        free(countKeys[j]);
    }
    free(countKeys);
    return 0;
}


/*
    This function gets the Maximum value of the list
*/
unsigned long long max(unsigned long long *list, int lenght){
    unsigned long long maximum;
    int i;
    
    if (list == NULL || lenght<=0){
        return 0;
    }

    maximum = list[0];

    for (i=0; i<lenght; i++){
        if (maximum<list[i]) maximum=list[i];
    }
    return maximum;
}


/*
    This function gets the number of Iterations we will need to sort the elements.
    It would be done by dividing the max number by the number of bits were given
    when running the program
*/
int getNumberOfIterationsFromMax(unsigned long long maxNum, int numBits){

    int numberOfBitsMaximum = countBits(maxNum);
    int numberOfIterations = ((numberOfBitsMaximum-1) / numBits) +1;

    return numberOfIterations;
}


/*
    This function gives the number of bits are actually used in a number
    For example if the number if 9 in decimal, in binary would be represented
    as 000...1001, Therefore this function would return 4 as the maximum 1 in binary 
    is at position 4
*/
int countBits(unsigned long long number) 
{   
    int currentBit, i;
    char numberInBits[MAXNBITS+1]="\0";

    for (i=0; i<MAXNBITS; i++){
        currentBit = number & 1;
        number >>= 1; //Shift the number 1 bit
        numberInBits[i]= '0'+currentBit;
    }

    i = MAXNBITS-1;
    for(; i>=0; i--){
        if(numberInBits[i] == '1'){
            break;
        }
    }

    return i+1;
}


void getStringFromNumber(unsigned long long number, char * numberInBits){
    int currentBit, i;

    for (i=0; i<MAXNBITS; i++){
        currentBit = number & 1;
        number >>= 1;
        numberInBits[i]= '0'+currentBit;
    }
}