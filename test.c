#include "allocator.h"
#include <stdio.h>

int main() {
    // Allocate a block of memory using jmalloc
    printf("Allocating 100 bytes using jmalloc...\n");
    void* ptr1 = jmalloc(100);
    if (ptr1) {
        printf("Memory allocated at address: %p\n", ptr1);
    } else {
        printf("Memory allocation failed!\n");
    }

    // Allocate another block of memory
    printf("\nAllocating 200 bytes using jmalloc...\n");
    void* ptr2 = jmalloc(200);
    if (ptr2) {
        printf("Memory allocated at address: %p\n", ptr2);
    } else {
        printf("Memory allocation failed!\n");
    }

    // Free the first block
    printf("\nFreeing the first block (100 bytes)...\n");
    jfree(ptr1);
    printf("First block freed.\n");

    // Allocate memory using jcalloc
    printf("\nAllocating 5 blocks of 50 bytes using jcalloc...\n");
    void* ptr3 = jcalloc(5, 50);
    if (ptr3) {
        printf("Memory allocated at address: %p\n", ptr3);
    } else {
        printf("Memory allocation failed!\n");
    }

    // Reallocate the second block to a larger size
    printf("\nReallocating the second block (200 bytes) to 300 bytes using jrealloc...\n");
    void* ptr4 = jrealloc(ptr2, 300);
    if (ptr4) {
        printf("Memory reallocated at address: %p\n", ptr4);
    } else {
        printf("Memory reallocation failed!\n");
    }

    // Free the remaining blocks
    printf("\nFreeing the remaining blocks...\n");
    jfree(ptr3);
    jfree(ptr4);

    printf("All memory blocks freed.\n");

    return 0;
}
