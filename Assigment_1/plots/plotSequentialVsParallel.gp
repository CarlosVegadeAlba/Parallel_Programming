cd ".."

set terminal png
set output 'plots/radix_sequentialVsParallel.png'

set title "Radix Sort best performance of Sequential vs Parallel"
set xlabel "Size (millions)"
set ylabel "Time (Seconds)"
set grid
set key outside

set style data linespoints

plot 'data/radix_sequential_b=16.txt' using 1:2 title 'Sequential 16 bits' with linespoints, \
     'data/radix_parallel3(NewDobleParallelize)t=8_b=4.txt' using 1:2 title 'Parallel_3 t=8 b=4' with linespoints