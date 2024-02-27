#include "mt19937-64.c"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        // Si hay menos de 3 argumentos, muestra un mensaje de error.
        // Recuerda que el primer argumento (argv[0]) es el nombre del programa,
        // por lo que necesitas al menos 3 para incluir tus dos entradas.
        printf("Usage: %s number_of_elements number_of_bits\n", argv[0]);
        return 1;
    }

    // Convertir los argumentos de entrada de string a int
    int number_of_elements = atoi(argv[1]);
    int number_of_bits = atoi(argv[2]);

    printf("Radix Sequential Sorting with %d elements and %d bits\n", number_of_elements, number_of_bits);

    return 0;
}