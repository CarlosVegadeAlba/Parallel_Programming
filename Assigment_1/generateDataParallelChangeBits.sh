#!/bin/bash

# File to store the results
output_file="data/radix_parallelChangeBits.txt"
touch data/radix_parallelChangeBits.txt

# Ensure the output file is empty at the start
> "$output_file"

# Start and end values

threads=(8 16 32)
bits=(2 8 16)
strForDecimals="0"
n=20 #number of times for average
x=10000000


# Function to extract the time from the output
extract_time() {
    echo "$1" | grep 'Time elapsed' | awk '{print $3}'
}

for bit in "${bits[@]}" 
do
    echo "Running bits=$bit"
    currentLine=""
    currentLine+="$bit "

    # First run the sequential algorithm
    sum=0
    echo " Sequential running"
    for ((i=1; i<=n; i++)); do
        output=$(./radix_sequential $x $bit)
        time_elapsed=$(extract_time "$output")
        sum=$(echo "scale=6; $sum + $time_elapsed" | bc)
    done
    average=$(echo "scale=6; $sum / $n" | bc)

    # For printing the numbers less than 1 as 0.5 not like .5
    if [ "$(echo "$average < 1" | bc)" -eq 1 ]; then
        average="0${average}"
    fi
    echo "  Average for bits=$bit and threads=Sequential is $average"

    # Append the size of x and the average runtime to the file
    currentLine+="$average "
    
    for thread in "${threads[@]}"
    do
        echo " $thread threads running"
        sum=0
        for ((i=1; i<=n; i++)); do
            output=$(./radix_parallel $x $bit $thread)
            time_elapsed=$(extract_time "$output")
            sum=$(echo "scale=6; $sum + $time_elapsed" | bc)
        done
        average=$(echo "scale=6; $sum / $n" | bc)

        # For printing the numbers less than 1 as 0.5 not like .5
        if [ "$(echo "$average < 1" | bc)" -eq 1 ]; then
            average="0${average}"
        fi
        echo "  Average for bits=$bit and thread=$thread is $average"

        # Append the size of x and the average runtime to the file
        currentLine+="$average "

    done
    echo "$currentLine" >> "$output_file"
done




