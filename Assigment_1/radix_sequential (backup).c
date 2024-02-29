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

    if (!rawArray || !outputArray) {
        fprintf(stderr, "Memory allocation failed\n");
        free(rawArray); // It's safe to call free on NULL
        free(outputArray);
        return -1;
    }

    unsigned long long randomNum=0, maxNum=0, aux1=0, aux2=0, numKeys=1;
    int numIterations;

    // Random seed
    /* srand((unsigned) time(NULL));
    int randomValue = rand();
    init_genrand64(randomValue); */
    
    // Create the list 
    /* printf("List of elements: \n\t["); */
    for (int i=0; i<number_of_elements-1; i++){
        randomNum = genrand64_int64(); 
        rawArray[i] = randomNum; 
        /* printf("%llu, ", rawArray[i]); */
    }
    randomNum = genrand64_int64();
    rawArray[number_of_elements-1] = randomNum;
    /* printf("%llu]\n", rawArray[number_of_elements-1]); */

    /* printf("Raw Array: \n[");
    for(int j=0; j< number_of_elements-1; j++){
        printf("%llu, ", rawArray[j]);
    }
    printf("%llu]\n", rawArray[number_of_elements-1]); */

    // Get the max number
    maxNum = max(rawArray, number_of_elements);
    /* printf("Maximum: %llu\n", maxNum); */
    
    numIterations = getNumberOfIterationsFromMax(maxNum, number_of_bits);
    
    for(int i=1; i<=number_of_bits; i++){
        numKeys = numKeys*2;
    }

    /* printf("If there are %d bits there %llu possible numbers\n", number_of_bits, numKeys); */
    unsigned long long* countKeys = malloc(numKeys * sizeof(unsigned long long));
    if(!countKeys){
        fprintf(stderr, "Memory allocation failed\n");
        free(rawArray);
        free(outputArray);
        free(countKeys);
        return -1;
    }

    unsigned int bits = (1U << number_of_bits) - 1;
    /* char numberInBits[MAXNBITS+1]="\0";
    getStringFromNumber(bits, numberInBits);
    printf("AUXILIAR VALUE FOR AND OPERATION %s\n", numberInBits); */

    // Start timing
    clock_t start = clock();
    for(int i=0; i<numIterations; i++){
       /*  printf("\nIteration %d\n", i); */

        // Reset the key array
        for(int j=0; j<numKeys; j++){
            countKeys[j]=0;
        }
        /* printf("Hola1?\n"); */

        // Fill the key array
        for(int j=0; j<number_of_elements; j++){
            aux2 = rawArray[j];
            aux1 = (aux2 >> (i*number_of_bits)) & bits; //aux1 has the bits we are studing in this iteration

            /* char numberInBits1[MAXNBITS+1]="\0";
            char numberInBits2[MAXNBITS+1]="\0";
            getStringFromNumber(aux2, numberInBits1);
            getStringFromNumber(aux1, numberInBits2); 
            printf("\t-The number %llu: %s\n\thas a value of %llu\n", rawArray[j], numberInBits1, aux1); */
            countKeys[aux1]++;
        }
        /* printf("Hola2?\n"); */
        
        // Show Count Array
        /* printf("Raw Count Array\n");
        printf("[");
        for(int j=0; j<numKeys; j++){
            printf("%llu,", countKeys[j]);
        }
        printf("]\n");  */

        // Do cumulative sum
        for(int j=1; j<numKeys; j++){
            countKeys[j]= countKeys[j-1] + countKeys[j];
        }
        /* printf("Hola3?\n"); */

        // Show Count Array
        /* printf("Accumulative sum Count Array\n");
        printf("[");
        for(int j=0; j<numKeys; j++){
            printf("%llu,", countKeys[j]);
        }
        printf("]\n"); */

        // Fill the output array
        for(int j=number_of_elements-1; j>=0; j--){
            /* printf("\tHola3.5?\n"); */
            aux2 = rawArray[j];
            aux1 = (aux2 >> (i*number_of_bits)) & bits; //aux1 has the bits we are studing in this 
            countKeys[aux1]--;
            /* printf("%llu\n", countKeys[aux1]);
            printf("\t\tThe value %llu has to go to the position %llu\n", rawArray[j], countKeys[aux1]); */
            outputArray[countKeys[aux1]] = rawArray[j];
            
        }
        /* printf("Hola4?\n"); */

        // update rawArray
        for(int j=0; j< number_of_elements; j++){
            rawArray[j] = outputArray[j];
        }
        /* printf("Hola5?\n"); */
        // Result of this iteration
        /* printf("Result of this iteration: ");
        for(int j=0; j< number_of_elements; j++){
            printf("%llu, ", rawArray[j]);
        }
        printf("\n"); */
    }
    // End timing
    clock_t end = clock();

    /* printf("Final Sorted Array: \n[");
    for(int j=0; j< number_of_elements-1; j++){
        printf("%llu, ", rawArray[j]);
    }
    printf("%llu]\n", rawArray[number_of_elements-1]); */

    // Check if the array is sorted well
    for(int j=0; j< number_of_elements-1; j++){
        if(rawArray[j] > rawArray[j+1]){
            printf("ERROR, the array is not well sorted\n");
            break;
        }
    }

    printf("Success!, Array Sorted succesfully :)\n");
     // Calculate the time elapsed
    double time_elapsed = (double)(end - start) / CLOCKS_PER_SEC;

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
    
    if (list == NULL || lenght<=0){
        return 0;
    }

    maximum = list[0];

    for (int i=0; i<lenght; i++){
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
    /* printf("Number of bitsMaximum: %d, Number of Bits per number: %d\n", numberOfBitsMaximum, numBits); */
    int numberOfIterations = ((numberOfBitsMaximum-1) / numBits) +1;
    /* printf("Number of iterations: %d\n", numberOfIterations); */

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
    int aux, currentBit;
    char numberInBits[MAXNBITS+1]="\0";

    /* printf("Loop:\t"); */
    for (int i=0; i<MAXNBITS; i++){
        currentBit = number & 1;
        number >>= 1; //Shift the number 1 bit
        /* printf("%d", currentBit); */
        numberInBits[i]= '0'+currentBit;
    }
    int i = MAXNBITS-1;
    for(; i>=0; i--){
        if(numberInBits[i] == '1'){
            break;
        }
    }
    /* printf("\nString:\t%s\n", numberInBits);
    printf("The maximum bit set to 1 is at position %d\n", i+1);  */

    return i+1;
}


void getStringFromNumber(unsigned long long number, char * numberInBits){
    int aux, currentBit;

    for (int i=0; i<MAXNBITS; i++){
        currentBit = number & 1;
        number >>= 1;
        numberInBits[i]= '0'+currentBit;
    }

}