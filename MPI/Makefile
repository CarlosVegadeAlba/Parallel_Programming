# Lista de archivos fuente
SOURCES = prueba1.c prueba2.c matrixMulMPI.c matrixDiv.c

# Derivar nombres de ejecutables a partir de los nombres de los archivos fuente
EXECS = $(SOURCES:.c=)

# Compilador
CC = mpicc

# Opciones del compilador
CFLAGS = -Wall -O2

# Regla por defecto
all: $(EXECS)

# Regla para compilar cada ejecutable a partir de su archivo fuente correspondiente
%: %.o
	$(CC) $(CFLAGS) -o $@ $^

# Regla para compilar archivos fuente en objetos
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Regla para limpiar los archivos compilados
clean:
	rm -f $(EXECS) *.o

# Reglas para ejecutar los programas
run_%: %
	mpirun -np 4 ./$<
