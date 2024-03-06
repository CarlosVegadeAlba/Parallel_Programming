cd ".."

dataFilemane="data/radix_parallel3(NewDobleParallelize)t=8_b=4.txt"

set terminal png
set output 'plots/radix_parallel_Formula.png'

set title "Radix Sort Parallel Formula"
set xlabel "Size (millions)"
set ylabel "Time (Seconds)"
set grid
set key outside

set style data linespoints

f(x) = a * x * log(x) + b

fit f(x) dataFilemane using 1:2 via a, b

plot dataFilemane using 1:2 title "Parallel_3 t=8 b=4" with lines, f(x) title "n log n" with lines
