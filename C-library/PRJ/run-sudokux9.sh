#!/bin/bash

# List all files in the directory and store them in an array
files=(input/txt9/*)
N_threads=4 # Set number of threads

# Check if there are any files in the directory
if [ ${#files[@]} -eq 0 ]; then
    echo "No files found in $1"
    exit 0
fi

total_time=0  # Initialize total time counter
file_count=0  # Initialize file count

# Print all filenames and calculate total time
for file in "${files[@]}"; do
    output=$(./sudoku "$file" 9 $N_threads)  # Run sudoku and capture output
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