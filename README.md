# Memory Allocator

## Introduction
#memAllocator is a a custom memory allocation library designed to manage memory efficiently and effectively in your next C and C++ programming project. 
It provides a set of functions that allow you to allocate, deallocate, and free memory dynamically by implementing all memory related functions of the C programming language, giving you more control over your program's memory usage.

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
Please go to this pdf file to read more about the implementation and testing process!



  
