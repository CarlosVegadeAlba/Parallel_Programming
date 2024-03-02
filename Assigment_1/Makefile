Flags=gcc -c -Wall -O3 -fopenmp

# Radix Sequential
radix_sequential.o: radix_sequential.c
	$(Flags) radix_sequential.c
radix_sequential: radix_sequential.o
	gcc -o radix_sequential radix_sequential.o
RadixSequential=radix_sequential.o radix_sequential

# Radix Parallel
radix_parallel.o: radix_parallel.c
	$(Flags) radix_parallel.c
radix_parallel: radix_parallel.o
	gcc -o radix_parallel radix_parallel.o
RadixParallel=radix_parallel.o radix_parallel

# Redo
redo:
	rm radix_parallel.c radix_sequential.c
	touch radix_parallel.c radix_sequential.c

# All
all: $(RadixSequential) $(RadixParallel)

# Clean
clean:
	rm *.o radix_sequential radix_parallel