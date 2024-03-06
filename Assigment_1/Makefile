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
	gcc -o radix_parallel radix_parallel.o -fopenmp
RadixParallel=radix_parallel.o radix_parallel

# Radix Parallel
radix_parallel2.o: radix_parallel2.c
	$(Flags) radix_parallel2.c
radix_parallel2: radix_parallel2.o
	gcc -o radix_parallel2 radix_parallel2.o -fopenmp
RadixParallel2=radix_parallel2.o radix_parallel2

# Radix Parallel
radix_parallel3.o: radix_parallel3.c
	$(Flags) radix_parallel3.c
radix_parallel3: radix_parallel3.o
	gcc -o radix_parallel3 radix_parallel3.o -fopenmp
RadixParallel3=radix_parallel3.o radix_parallel3


# Redo
redo_all:
	rm radix_parallel.c radix_sequential.c
	touch radix_parallel.c radix_sequential.c
redo_sequential:
	rm radix_sequential.c
	touch radix_sequential.c
	nano radix_sequential.c
redo_parallel:
	rm radix_parallel.c
	touch radix_parallel.c
	nano radix_parallel.c

# All
all: $(RadixSequential) $(RadixParallel) $(RadixParallel2) $(RadixParallel3)

# Clean
clean:
	rm *.o radix_sequential radix_parallel radix_parallel2 radix_parallel3