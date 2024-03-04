set terminal png
set output 'plots/radix_sequential_Formula.png'

set title "Radix Sort Sequential Formula"
set xlabel "Size (millions)"
set ylabel "Time (Seconds)"
set grid
set key outside

set style data linespoints

f(x) = a * x * log(x) + b

fit f(x) 'data/radix_sequential.txt' using 1:2 via a, b

plot 'data/radix_sequential.txt' using 1:2 title "Sequential" with lines, f(x) title "n log n" with lines
