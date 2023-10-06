#!/bin/bash

# Specify the list of C files you want to compile and run
c_files=("server.c" "auth.c" "dns.c" "user.c")

# Specify the corresponding list of arguments for each C file
# Use empty string ("") if no arguments are needed for a particular file
c_args=("" "" "" "")

# Check if the lengths of the two lists are equal
if [ "${#c_files[@]}" -ne "${#c_args[@]}" ]; then
    echo "Error: Number of source files and argument lists do not match."
    exit 1
fi

# Create an array to hold the PIDs of the background processes
pids=()

# Loop through the list of C files
for i in "${!c_files[@]}"; do
  file="${c_files[$i]}"
  args="${c_args[$i]}"

  # Compile the C file using gcc
  gcc -o "${file%.c}" "$file"

  # Check if the compilation was successful
  if [ $? -eq 0 ]; then
    # Set execute permissions for the compiled program
        sudo chmod +x "./${file%.c}"

    # Run the compiled program with arguments in the background
    if [ -z "$args" ]; then
      xterm -e "./${file%.c}" &
    else
      xterm -e "./${file%.c} $args" &
    fi

    # Store the PID of the background process
    pids+=($!)
  else
    echo "Compilation of $file failed."
  fi
done

# Wait for all background processes to finish
for pid in "${pids[@]}"; do
  wait "$pid"
done

echo "All processes have completed. "
