#!/bin/bash

start=1000000
increment=1500000
end=43000000
threads=16
bits=16
strForDecimals="0"

programToRun="./radix_parallel"
filename="data/radix_parallel(Normal)t=${threads}_b=${bits}.txt"
output_file=$filename
touch $filename
# Ensure the output file is empty at the start
> "$output_file"


# Current value of x
current=$start

# Function to extract the time from the output
extract_time() {
    echo "$1" | grep 'Time elapsed' | awk '{print $3}'
}

# Loop until current is less than or equal to end
while [ $(echo "$current <= $end" | bc) -eq 1 ]; do
    # Sum of all runs for the current x
    sum=0

    # Run the program 10 times for the current x
    echo "Running x=$current"
    for i in {1..10}; do
        # Execute the program and capture the output
        # Assuming the program's output is the runtime in a format that can be summed
        output=$($programToRun $current $bits $threads)
        time_elapsed=$(extract_time "$output")
        # Add the output to sum
        sum=$(echo "scale=6; $sum + $time_elapsed" | bc)
    done

    # Calculate the average for the current x
    average=$(echo "scale=6; $sum / 10" | bc)

    # For printing the numbers less than 1 as 0.5 not like .5
    if [ "$(echo "$average < 1" | bc)" -eq 1 ]; then
        average="0${average}"
    fi
    echo "  Average for x=$current is $average"

    # Append the size of x and the average runtime to the file
    xValue=$(echo "scale=6; $current / 1000000" | bc) # Save the x as millions for making easier to plot
    # For printing the numbers less than 1 as 0.5 not like .5
    if [ "$(echo "$xValue < 1" | bc)" -eq 1 ]; then
        xValue="0${xValue}"
    fi

    echo "$xValue $average" >> "$output_file"

    # Increment x exponentially by a factor of 1.2
    # current=$(echo "$current * 1.2" | bc)
    ((current += increment))
done

#Once the number is bigger than 40 million just do one more time for 40 million

: '
current=$end
sum=0
echo "Running x=$current"
for i in {1..10}; do
    output=$(./radix_parallel $current 16 16)
    time_elapsed=$(extract_time "$output")
    sum=$(echo "scale=6; $sum + $time_elapsed" | bc)
done

average=$(echo "scale=6; $sum / 10" | bc)

if [ "$(echo "$average < 1" | bc)" -eq 1 ]; then
    average="0${average}"
fi
echo "  Average for x=$current is $average"

xValue=$(echo "scale=6; $current / 1000000" | bc)
if [ "$(echo "$xValue < 1" | bc)" -eq 1 ]; then
    xValue="0${xValue}"
fi
echo "$xValue $average" >> "$output_file"
'


