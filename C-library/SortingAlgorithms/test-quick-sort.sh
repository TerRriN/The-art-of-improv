#!/bin/bash

# List all files in the directory and store them in an array
files=(unsorted_data/*)

# Check if there are any files in the directory
if [ ${#files[@]} -eq 0 ]; then
    echo "No files found in $1"
    exit 0
fi

total_time=0  # Initialize total time counter
N_values=(10 50 100 250 500 1000 2500 5000 7500 10000 100000)  # Set N values

# Print all filenames and calculate total time
for i in "${!N_values[@]}"; do
    N=${N_values[i]}
    file=${files[i]}
    output=$(./quick "$file" $N)  # Run quick_sort and capture output
    echo "$output"  # Print output
    time=$(echo "$output" | grep -oP '(?<=Time: )[0-9.]+' | awk '{print $1}')  # Extract time from output
    if [ ! -z "$time" ]; then  # Check if time is not empty
        total_time=$(awk "BEGIN {print $total_time + $time}")  # Add time to total_time
        ((file_count++))  # Increment file count
    fi
done

if [ "$file_count" -gt 0 ]; then
    echo "Total time: $total_time seconds"  # Print total time
else
    echo "No files with time information found."
fi