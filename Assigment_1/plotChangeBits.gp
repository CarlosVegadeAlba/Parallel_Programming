set terminal png
set output 'plots/radix_parallel_changeBits.png'

set title "Radix Sort Parallel Performance with 10000000 numbers changing bits"
set xlabel "Bits"
set ylabel "Time (Seconds)"
set grid
set key outside

set style data linespoints

plot 'data/radix_parallelChangeBits.txt' using 1:2 title 'Sequential' with linespoints, \
     'data/radix_parallelChangeBits.txt' using 1:3 title '8 threads' with linespoints, \
     'data/radix_parallelChangeBits.txt' using 1:4 title '16 threads' with linespoints, \
     'data/radix_parallelChangeBits.txt' using 1:5 title '32 threads' with linespoints

