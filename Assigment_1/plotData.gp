set terminal png
set output 'radix_parallel_performance.png'

set title "Radix Sort Parallel Performance"
set xlabel "Size (millions)"
set ylabel "Time (Seconds)"
set grid
set key outside

set style data linespoints

plot 'data/radix_parallel.txt' using 1:2 title 'Sequential' with linespoints, \
     'data/radix_parallel.txt' using 1:3 title '2 threads' with linespoints, \
     'data/radix_parallel.txt' using 1:4 title '8 threads' with linespoints, \
     'data/radix_parallel.txt' using 1:5 title '16 threads' with linespoints, \
     'data/radix_parallel.txt' using 1:6 title '24 threads' with linespoints, \
     'data/radix_parallel.txt' using 1:7 title '32 threads' with linespoints, \
     'data/radix_parallel.txt' using 1:8 title '50 threads' with linespoints, \
     'data/radix_parallel.txt' using 1:9 title '80 threads' with linespoints
