# Memory Allocator

## Overview
This project is a custom memory allocator implemented in C for educational purposes to explore operating system (OS) concepts related to memory management and the system calls needed for low-level memory handling. By recreating the functionality of malloc, calloc, realloc, and free, this library provides insight into how memory is dynamically allocated and managed at the system level.

## Educational Focus
- **System Calls**: Demonstrates how system calls like `sbrk()` are used to request memory from the operating system's heap, simulating the behavior of `malloc()`.
- **Linked List Memory Management**: Shows how memory blocks are tracked using a simple linked list structure, demonstrating OS concepts of memory allocation and deallocation.
- **Thread Safety**: Highlights the use of `pthread_mutex_lock()` and `pthread_mutex_unlock()` to ensure memory allocation is thread-safe in multi-threaded environments.

The project serves as a learning tool for studying operating systems, memory management, and low-level programming concepts.

## Features

- **Custom dynamic memory allocation** using `sbrk()` (`jmalloc`).
- **Memory reallocation** (`jrealloc`) with `memcpy()` for copying data.
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







## Instalation Setup
To instal the memory allocator, clone the repository and navigate to the file containing the repository, and the src file.
```bash
git clone https://github.com/Jonathan03ant/memAlocator.git
cd memAlocator
cd src
```

Copy everything inside the src file and paste it in to the root of your own working directory where you are writing your codes. The files include three header files, and a static library.
```bash

j_allocator.h
allocator.h
memoryBlock.h
libj_allocator.a
```
## Usage
To use memAllocator, include the J_allocator.h header in your C source files and link against the static library libj_allocator.a:

```c
#include "J_allocator.h"

// ...

void *ptr = jalloc(size);
// use ptr...
jfree(ptr);
```

After including the header file in your source files, simply compile your code and link the static library like this

```bash
gcc -o my_program my_program.c -L. -lj_allocator
```

If you are using a Makefile to build your project, include the static library as LDFLAG and link the LDFLAG in to the executable.

```bash
EXECS = program 
LDFLAGS =  -L. -lj_allocator

all: $(EXECS)

program: $(OBJS)
	gcc $(OBJS) $(LDFLAGS) -o program
```
## Note
To make your own library from the source code, compile the allocator.c and the memoryblock.c code in to an object code first
```bash
gcc -c allocator.c -o allocator.o
gcc -c memoryBlock.c -o memoryBlock.o
```
Next, make your own static library (.a) with the following command
```bash
ar rcs libj_allocator.a allocator.o memoryBlock.o
```

### Documentation on progress....



  
