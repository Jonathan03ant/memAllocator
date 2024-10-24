# Memory Allocator

## Overview
This project is a custom memory allocator implemented in C for educational purposes aiming to explore operating system (OS) concepts related to memory management and the system calls needed for low-level memory handling. By recreating the functionality of malloc, calloc, realloc, and free, this library provides insight into how memory is dynamically allocated and managed at the system level.

## Educational Focus
- **System Calls**: Demonstrates how system calls like `sbrk()` are used to request memory from the operating system's heap, simulating the behavior of `malloc()`.
- **Linked List Memory Management**: Shows how memory blocks are tracked using a simple linked list structure, demonstrating OS concepts of memory allocation and deallocation.
- **Thread Safety**: Highlights the use of `pthread_mutex_lock()` and `pthread_mutex_unlock()` to ensure memory allocation is thread-safe in multi-threaded environments.

The project serves as a learning tool for studying operating systems, memory management, and low-level programming concepts.

## Features

- **Custom dynamic memory allocation** using `jmalloc`.
- **Memory reallocation** using `jrealloc` 
- **Zero-initialized memory allocation** (`jcalloc`), similar to the standard `calloc()`.
- **Safe memory deallocation** (`jfree`), tracking memory blocks with a linked list.
- **Thread-safe memory allocation** with mutexes.

## Files in the Repository

- **`allocator.h`**: Header file with function declarations.
- **`allocator.c`**: Contains the core logic for memory allocation and management.
- **`MemoryBlock.c`**: Defines the memory block structure, simulating a linked list of memory blocks.
- **`build.sh`**: Script for compiling the library and cleaning up object files.
- **`test.c`**: A sample test file demonstrating the use of custom allocator functions.

## Installation

### Step 1: Clone the Repository

```bash
git clone https://github.com/your_username/your_repository.git
cd your_repository
```

### Step 2: Build the Static Library

Run the provided `build.sh` script to compile the library:
```bash
./build.sh
```
To clean up object files and the static library:

```bash
./build.sh clean
```

### Step 3: Linking the Library

To use the custom allocator, link the static library when compiling a program. For example:

1. Write the program that uses the allocator
```c
#include "allocator.h"   //Our Allocator
#include <stdio.h>

int main() {
    void* ptr = jmalloc(100);
    if (ptr) {
        printf("Memory allocated successfully.\n");
        jfree(ptr);
        printf("Memory freed.\n");
    }
    return 0;
}
```

2. Compile the program and link it with the static library:
```bash
gcc -o my_program my_program.c -L. -l:jonathanmemoryallocator.a
```

3. Run the program:
```bash
./my_program
```

## System Calls and OS Concepts

### sbrk(): Requesting Memory From the OS

- The `sbrk()` system call is used to increase the program's heap by a specified number of bytes, simulating how `malloc()` requests memory from the operating system when it runs out of space.
- The allocator tracks the allocated memory blocks using a linked list, similar to how the kernel manages memory for processes.

### Mutexes and Thread Safety

- In multi-threaded environments, race conditions can occur if multiple threads attempt to allocate memory simultaneously.
- The library uses mutex locks (`pthread_mutex_lock()` and `pthread_mutex_unlock()`) to ensure thread-safe memory management.

### Block Management Using a Linked List

- Each memory block is represented by a header that stores metadata about the block's size, whether it is free or allocated, and a pointer to the next block.
- This structure mimics how operating systems track memory allocations, forming a linked list of memory blocks.

### Example Usage

```c
#include "allocator.h"
#include <stdio.h>

int main() {
    // Allocate 100 bytes using jmalloc
    void* ptr = jmalloc(100);
    if (ptr) {
        printf("Memory allocated at address: %p\n", ptr);
    }

    // Resize the block to 200 bytes using jrealloc
    ptr = jrealloc(ptr, 200);
    if (ptr) {
        printf("Memory resized at address: %p\n", ptr);
    }

    // Free the memory block
    jfree(ptr);
    printf("Memory freed.\n");

    return 0;
}
```

## Testing 
- The provided `test.c` program can be used to verify the behavior of the custom memory allocator.
sbrk(): Requesting Memory From the OS

## Compile the test program
```bash
gcc -o test test.c -L. -l:jonathanmemoryallocator.a
```

## Run the test
```bash
./test
```

## Issues
For issues + contribution, make a PR request.

Thanks!