#!/bin/bash

# Initial parameters
x=100000
y=16
factor=2 # Exponential growth factor for initial phase

# Function to extract the time from the output
extract_time() {
    echo "$1" | grep 'Time elapsed' | awk '{print $3}'
}

# Run the program initially to get the output
output=$(./radix_sequential $x $y)
time_elapsed=$(extract_time "$output")

# Start with exponential growth
while [ $(echo "$time_elapsed < 10" | bc) -eq 1 ]; do
    echo "Time elapsed was $time_elapsed seconds, with [x=$x y=$y], which is less than 10. Increasing x exponentially by $factor."
    prev_x=$x # Store current x as previous x before increasing
    x=$(echo "$x * $factor" | bc) # Increase x exponentially
    output=$(./radix_sequential $x $y)
    time_elapsed=$(extract_time "$output")
done

# Once over 10 seconds, revert to the last value of x that was under 10 seconds and increase by 100000
echo "Time elapsed was $time_elapsed seconds, with [x=$x y=$y], which exceed 10 seocnds."

# Continue with lower exponential growth
factor=1.05
x=$prev_x
echo "Reverting to previous x=$x and incrementing by Exponentially by $factor."
time_elapsed=0

while [ $(echo "$time_elapsed < 10" | bc) -eq 1 ]; do
    x=$(echo "$x * $factor" | bc)
    output=$(./radix_sequential $x $y)
    time_elapsed=$(extract_time "$output")
    echo "Time elapsed was $time_elapsed seconds, with [x=$x y=$y], which is less than 10. Increasing x exponentially by $factor."
done

echo "Final configuration reached with x=$x, y=$y, and time elapsed: $time_elapsed seconds."
