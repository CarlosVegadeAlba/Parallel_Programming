#include "mt19937-64.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
#define MAX_NUMBER 16 //Only for developing 

int check_number_of_elements(int number);
unsigned long long max(unsigned long long list[], int lenght);
int getNumberOfIterationsFromMax(unsigned long long maxNum, int numBits);
int countBits(unsigned long long number);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        // At least 3 arguments
        printf("Usage: %s number_of_elements number_of_bits\n", argv[0]);
        return 1;
    }

    // Convertir los argumentos de entrada de string a int
    int number_of_elements = atoi(argv[1]);
    if (number_of_elements<2){
        printf("\tERROR, the array needs to have at least 2 elements\n");
    }

    int number_of_bits = atoi(argv[2]);
    if (number_of_bits<1){
        printf("\tERROR, the minimum number of bits is 1\n");
    }

    if(check_number_of_elements(number_of_elements)==-1)
        printf("\tERROR, Please the number of elements have to be a power of 2\n");
    else
        printf("\tRadix Sequential Sorting with %d elements and %d bits\n", number_of_elements, number_of_bits);

    unsigned long long rawArray [number_of_elements];
    unsigned long long outputArray [number_of_elements];
    unsigned long long maxNum = 0;
    int numIterations = 0;

    // Random seed
    /* srand((unsigned) time(NULL));
    int randomValue = rand();
    init_genrand64(randomValue); */
    
    // Create the list 
    printf("List of elements: \n\t[");
    for (int i=0; i<number_of_elements-1; i++){
        rawArray[i] = genrand64_int64() % (MAX_NUMBER+1); // REMEMBER TO REMOVE 
        printf("%llu, ", rawArray[i]);
    }
    rawArray[number_of_elements-1] = genrand64_int64() % (MAX_NUMBER+1); // REMEMBER TO REMOVE 
    printf("%llu]\n", rawArray[number_of_elements-1]);

    // Get the max number
    maxNum = max(rawArray, number_of_elements);
    printf("Maximum: %llu\n", maxNum);
    
    numIterations = getNumberOfIterationsFromMax(maxNum, number_of_bits);
    
    
    int numKeys = pow(number_of_bits,2);
    int keys[numKeys];

    for(int i=0; i<numIterations; i++){

        // Reset the key array
        for(int j=0; j<numKeys; j++){
            keys[j]=0;
        }

        // Fill the key array
        for(int j=0; j<number_of_elements; j++){
            
        }

        // Fill the output array
    }

    return 0;
}

/*
    This function checks if the input is a power of 2
*/
int check_number_of_elements(int number){
    if (number < 2) return -1;

    while ((number>2) && (number % 2)==0)
    {
        number = number/2;
    }
    
    if((number%2) != 0) return -1;

    return 0;
}

/*
    This function gets the Maximum value of the list
*/
unsigned long long max(unsigned long long *list, int lenght){
    unsigned long long maximum;
    
    if (list == NULL || lenght<0){
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
    printf("Number of bitsMaximum: %d, Number of Bits per number: %d\n", numberOfBitsMaximum, numBits);
    int numberOfIterations = ((numberOfBitsMaximum-1) / numBits) +1;
    printf("Number of iterations: %d\n", numberOfIterations);

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
    int bits = 64;
    int aux, currentBit;
    char numberInBits[65]="\0";

    /* printf("Loop:\t"); */
    for (int i=0; i<64; i++){
        currentBit = number & 1;
        number >>= 1; //Shift the number 1 bit
        /* printf("%d", currentBit); */
        numberInBits[i]= '0'+currentBit;
    }
    int i = 63;
    for(; i>=0; i--){
        if(numberInBits[i] == '1'){
            break;
        }
    }
    printf("\nString:\t%s\n", numberInBits);
    printf("The maximum bit set to 1 is at position %d\n", i+1); 

    return i+1;
} 