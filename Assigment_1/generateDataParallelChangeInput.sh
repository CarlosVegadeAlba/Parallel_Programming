#!/bin/bash

# File to store the results
output_file="data/radix_parallelChangeInput.txt"
touch data/radix_parallelChangeInput.txt

# Ensure the output file is empty at the start
> "$output_file"

# Start and end values
start=1000000
end=40000000
threads=(2 8 16 32 80)
bits=8
strForDecimals="0"
n=10 #number of times for average

# Current value of x
current=$start

# Function to extract the time from the output
extract_time() {
    echo "$1" | grep 'Time elapsed' | awk '{print $3}'
}

# Loop until current is less than or equal to end
while [ $(echo "$current <= $end" | bc) -eq 1 ]; do

    # Run the program for each thread n times for the current x
    echo "Running x=$current"
    # Append the size of x and the average runtime to the file
    currentLine=""
    currentLine+=$(echo "scale=6; $current / 1000000" | bc) # Save the x as millions for making easier to plot
    currentLine+=" "

    # First run the sequential algorithm
    sum=0
    for ((i=1; i<=n; i++)); do
        # Execute the program and capture the output
        # Assuming the program's output is the runtime in a format that can be summed
        output=$(./radix_sequential $current $bits)
        time_elapsed=$(extract_time "$output")
        # Add the output to sum
        sum=$(echo "scale=6; $sum + $time_elapsed" | bc)
        # Calculate the average for the current x
    done
    average=$(echo "scale=6; $sum / $n" | bc)

    # For printing the numbers less than 1 as 0.5 not like .5
    if [ "$(echo "$average < 1" | bc)" -eq 1 ]; then
        average="0${average}"
    fi
    echo "  Average for x=$current and threads=Sequential is $average"

    # Append the size of x and the average runtime to the file
    currentLine+="$average "
    
    for thread in "${threads[@]}"
    do
        echo " Threads: $thread"
        # Sum of all runs for the current x
        sum=0
        for ((i=1; i<=n; i++)); do
            # Execute the program and capture the output
            # Assuming the program's output is the runtime in a format that can be summed
            output=$(./radix_parallel $current $bits $thread)
            time_elapsed=$(extract_time "$output")
            # Add the output to sum
            sum=$(echo "scale=6; $sum + $time_elapsed" | bc)
            # Calculate the average for the current x
        done
        average=$(echo "scale=6; $sum / $n" | bc)

        # For printing the numbers less than 1 as 0.5 not like .5
        if [ "$(echo "$average < 1" | bc)" -eq 1 ]; then
            average="0${average}"
        fi
        echo "  Average for x=$current and thread=$thread is $average"

        # Append the size of x and the average runtime to the file
        currentLine+="$average "

    done
    echo "$currentLine" >> "$output_file"
    # Increment x exponentially by a factor of 1.2
    current=$(echo "$current * 1.4" | bc)

done

#Once the number is bigger than 40 million just do one more time for 40 million

current=$end
echo "Running x=$current"
# Append the size of x and the average runtime to the file
currentLine=""
currentLine+=$(echo "scale=6; $current / 1000000" | bc)
currentLine+=" "

# First run the sequential algorithm
sum=0
for ((i=1; i<=n; i++)); do
    # Execute the program and capture the output
    # Assuming the program's output is the runtime in a format that can be summed
    output=$(./radix_sequential $current $bits)
    time_elapsed=$(extract_time "$output")
    # Add the output to sum
    sum=$(echo "scale=6; $sum + $time_elapsed" | bc)
    # Calculate the average for the current x
done
average=$(echo "scale=6; $sum / $n" | bc)

# For printing the numbers less than 1 as 0.5 not like .5
if [ "$(echo "$average < 1" | bc)" -eq 1 ]; then
    average="0${average}"
fi
echo "  Average for x=$current and threads=Sequential is $average"

# Append the size of x and the average runtime to the file
currentLine+="$average "

for thread in "${threads[@]}"
do
    echo " Threads: $thread"
    # Sum of all runs for the current x
    sum=0
    for ((i=1; i<=n; i++)); do
        # Execute the program and capture the output
        # Assuming the program's output is the runtime in a format that can be summed
        output=$(./radix_parallel $current $bits $thread)
        time_elapsed=$(extract_time "$output")
        # Add the output to sum
        sum=$(echo "scale=6; $sum + $time_elapsed" | bc)
        # Calculate the average for the current x
    done
    average=$(echo "scale=6; $sum / $n" | bc)

    # For printing the numbers less than 1 as 0.5 not like .5
    if [ "$(echo "$average < 1" | bc)" -eq 1 ]; then
        average="0${average}"
    fi
    echo "  Average for x=$current and thread=$thread is $average"

    # Append the size of x and the average runtime to the file
    currentLine+="$average "

done
echo "$currentLine" >> "$output_file"


