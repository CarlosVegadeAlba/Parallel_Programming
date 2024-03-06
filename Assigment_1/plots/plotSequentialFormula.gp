cd ".."

# Set the filename of the data
dataFilemane="data/radix_sequential_b=16.txt"

set terminal png
set output 'plots/radix_sequential_Formula.png'

set title "Radix Sort Sequential Formula"
set xlabel "Size (millions)"
set ylabel "Time (Seconds)"
set grid
set key outside

set style data linespoints

f(x) = a * x * log(x) + b

fit f(x) dataFilemane using 1:2 via a, b

plot dataFilemane using 1:2 title "Sequential b=16" with lines, f(x) title "n log n" with lines
