#include "mt19937-64.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <time.h>
/* #define MAX_NUMBER 8 */ //Only for developing 
#define MAXNBITS sizeof(unsigned long long) * 8


int check_number_of_elements(int number);
unsigned long long max(unsigned long long list[], int lenght);
int getNumberOfIterationsFromMax(unsigned long long maxNum, int numBits);
int countBits(unsigned long long number);
void getStringFromNumber(unsigned long long number, char * numberInBits);


int main(int argc, char *argv[]) {
    if (argc != 3) {
        // At least 3 arguments
        printf("Usage: %s number_of_elements number_of_bits\n", argv[0]);
        return -1;
    }

    // Convertir los argumentos de entrada de string a int
    int number_of_elements = atoi(argv[1]);
    if (number_of_elements<2){
        printf("\tERROR, the array needs to have at least 2 elements\n");
        return -1;
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

    printf("Radix Sequential Sorting with %d elements and %d bits\n", number_of_elements, number_of_bits);

    unsigned long long* rawArray = malloc(number_of_elements * sizeof(unsigned long long));
    unsigned long long* outputArray = malloc(number_of_elements * sizeof(unsigned long long));
    unsigned long long* countKeys;

    if (!rawArray || !outputArray) {
        fprintf(stderr, "Memory allocation failed\n");
        free(rawArray); // It's safe to call free on NULL
        free(outputArray);
        return -1;
    }

    unsigned long long randomNum=0, maxNum=0, aux1=0, aux2=0, numKeys=1;
    unsigned int bits;
    int numIterations, i, j, randomValue;
    double time_elapsed;
    clock_t start, end;

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

    countKeys = malloc(numKeys * sizeof(unsigned long long));
    if(!countKeys){
        fprintf(stderr, "Memory allocation failed\n");
        free(rawArray);
        free(outputArray);
        free(countKeys);
        return -1;
    }

    bits = (1U << number_of_bits) - 1;

    // Start timing
    start = clock();
    for(i=0; i<numIterations; i++){

        // Reset the key array
        for(j=0; j<numKeys; j++){
            countKeys[j]=0;
        }

        // Fill the key array
        for(j=0; j<number_of_elements; j++){
            aux2 = rawArray[j];
            aux1 = (aux2 >> (i*number_of_bits)) & bits; //aux1 has the bits we are studing in this iteration
            countKeys[aux1]++;
        }

        // Do cumulative sum
        for(j=1; j<numKeys; j++){
            countKeys[j]= countKeys[j-1] + countKeys[j];
        }

        // Fill the output array
        for(j=number_of_elements-1; j>=0; j--){
            aux2 = rawArray[j];
            aux1 = (aux2 >> (i*number_of_bits)) & bits; //aux1 has the bits we are studing in this 
            countKeys[aux1]--;
            outputArray[countKeys[aux1]] = rawArray[j];
            
        }

        // update rawArray
        // CHANGE USE MEMCPY IS FASTER
        /* for(j=0; j< number_of_elements; j++){
            rawArray[j] = outputArray[j];
        } */
        memcpy(rawArray, outputArray, number_of_elements * sizeof(unsigned long long));

    }
    // End timing
    end = clock();

    // Check if the array is sorted well
    for(j=0; j< number_of_elements-1; j++){
        if(rawArray[j] > rawArray[j+1]){
            printf("ERROR, the array is not well sorted\n");
            break;
        }
    }

    printf("Success!, Array Sorted succesfully :)\n");
     // Calculate the time elapsed
    time_elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time elapsed: %f seconds\n", time_elapsed);

    free(rawArray);
    free(outputArray);
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