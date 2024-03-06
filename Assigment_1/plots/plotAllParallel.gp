cd ".."

set terminal png
set output 'plots/radix_parallel_approaches.png'

set title "Radix Sort Parallel Performance changing Numbers with different approaches"
set xlabel "Size (millions)"
set ylabel "Time (Seconds)"
set grid
set key outside

set style data linespoints

plot 'data/radix_sequential_b=16.txt' using 1:2 title 'Sequential 16 bits' with linespoints, \
     'data/radix_sequential_b=8.txt' using 1:2 title 'Sequential 8 bits' with linespoints, \
     'data/radix_parallel(Normal)t=8_b=8.txt' using 1:2 title 'Parallel_1 t=8 b=8' with linespoints, \
     'data/radix_parallel(Normal)t=16_b=16.txt' using 1:2 title 'Parallel_1 t=8 b=8' with linespoints, \
     'data/radix_parallel2(NewParalelize)t=8_b=8.txt' using 1:2 title 'Parallel_2 t=8 b=8' with linespoints, \
     'data/radix_parallel2(NewParalelize)t=12_b=6.txt' using 1:2 title 'Parallel_2 t=12 b=6' with linespoints, \
     'data/radix_parallel2(NewParalelize)t=16_b=4.txt' using 1:2 title 'Parallel_2 t=16 b=4' with linespoints, \
     'data/radix_parallel3(NewDobleParallelize)t=6_b=12.txt' using 1:2 title 'Parallel_3 t=6 b=12' with linespoints, \
     'data/radix_parallel3(NewDobleParallelize)t=8_b=4.txt' using 1:2 title 'Parallel_3 t=8 b=4' with linespoints, \
     'data/radix_parallel3(NewDobleParallelize)t=8_b=8.txt' using 1:2 title 'Parallel_3 t=8 b=8' with linespoints, \
     'data/radix_parallel3(NewDobleParallelize)t=16_b=4.txt' using 1:2 title 'Parallel_3 t=16 b=4' with linespoints