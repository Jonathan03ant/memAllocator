#include <stdio.h>
#include "j_allocator.h"

int main() {
    // Allocate memory
    void* block = jalloc(10);
    if (block == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }
    printf("Allocated memory at %p\n", block);

    // Reallocate memory
    block = jrealloc(block, 20);
    if (block == NULL) {
        printf("Failed to reallocate memory\n");
        return 1;
    }
    printf("Reallocated memory at %p\n", block);

    // Free memory
    jfree(block);
    printf("Freed memory\n");

    return 0;
}