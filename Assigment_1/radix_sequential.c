#include "mt19937-64.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int check_number_of_elements(int number);
unsigned long long max(unsigned long long list[], int lenght);

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

    // Random seed
    /* srand((unsigned) time(NULL));
    int randomValue = rand();
    init_genrand64(randomValue); */
    
    printf("List of elements: \n\t[");
    for (int i=0; i<number_of_elements-1; i++){
        rawArray[i] = genrand64_int64();
        printf("%llu, ", rawArray[i]);
    }
    printf("%llu]\n", rawArray[number_of_elements-1]);

    printf("Maximum: %llu\n", max(rawArray, number_of_elements));
    

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