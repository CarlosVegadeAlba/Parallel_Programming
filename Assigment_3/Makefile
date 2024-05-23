CC=gcc
CFLAGS=-Wall -fopenmp -O3
LIBS=-lm

# Definiendo dos objetivos separados
TARGET1=evenOddMergeSortPar
TARGET2=evenOddMergeSortSeq

# Archivos objeto por cada objetivo
OBJ1=evenOddMergeSortPar.o
OBJ2=evenOddMergeSortSeq.o

all: $(TARGET1) $(TARGET2) clean_o

# Reglas específicas para cada programa
$(TARGET1): $(OBJ1)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(TARGET2): $(OBJ2)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Regla general para compilar objetos desde código fuente
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET1) $(TARGET2) $(OBJ1) $(OBJ2) *.o

clean_o:
	rm *.o

.PHONY: all clean run1 run2
