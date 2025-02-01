#!/bin/bash

total_time=0  # Initialize total time counter
file_count=0  # Initialize file count
puzzle_size=16  # Set puzzle size
N_threads=4  # Set number of threads

# Print all filenames and calculate total time
for ((i=1; i<=10; i++)); do
    file="input/txt$puzzle_size/${i}x$puzzle_size.txt"  # Construct filename with number
    if [ ! -f "$file" ]; then
        echo "File not found: $file"
        continue
    fi

    output=$(./sudoku "$file" $puzzle_size $N_threads)  # Run sudoku and capture output
    echo "$output"  # Print output
    time=$(echo "$output" | grep -oP '(?<=Time: )[0-9.]+' | awk '{print $1}')  # Extract time from output
    if [ ! -z "$time" ]; then  # Check if time is not empty
        total_time=$(awk "BEGIN {print $total_time + $time}")  # Add time to total_time
        ((file_count++))  # Increment file count
    fi
done

if [ "$file_count" -gt 0 ]; then
    average_time=$(awk "BEGIN {print $total_time / $file_count}")  # Calculate average time
    echo "Average time for ${file_count} files: ${average_time}s"
else
    echo "No files with time information found."
fi
