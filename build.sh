#!/bin/bash

# step one
# compile c file in to object code
echo "Compiling allocator.c in to allocator.o ..."

gcc -C allocator.c -o allocator.o

#check if compilation  was successfull

if [ $? -ne 0 ]; then
  echo "Compilation failed!"
  exit 1
fi

# Step 2: Create the static library (jonathanmemoryallocator.a)
echo "Creating static library jonathanmemoryallocator.a..."
ar rcs jonathanmemoryallocator.a allocator.o

# Check if the library creation was successful
if [ $? -ne 0 ]; then
  echo "Failed to create the static library!"
  exit 1
fi

echo "Static library jonathanmemoryallocator.a created successfully!"

