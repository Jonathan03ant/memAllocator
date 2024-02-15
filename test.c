#include <stdio.h>
#include "j_allocator.h"

int main() {
    printf("-------*********Starting test**********--------\n\n");
    printf("TEST ONE\n");
    printf("********** Allocating 10 bytes of memory **********\n");
    void* block = jalloc(10);
    if (block == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }
    printf("Allocated memory at **Pointer Address** %p\n\n", block);

    // Reallocate memory
    printf("********** Reallocating 20 bytes of memory **********\n");
    block = jrealloc(block, 20);
    if (block == NULL) {
        printf("Failed to reallocate memory\n");
        return 1;
    }
    printf("Reallocated memory at **Pointer Address** %p\n\n", block);

    // Free memory
    jfree(block);
    printf("Freed memory\n\n");

    printf("TEST TWO\n");
    int* ptr;
    int n, i;

    printf("Number of elements to be entered:");
    scanf("%d", &n);

    ptr = (int*)jalloc(n * sizeof(int));
    if (ptr == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    } else{
        printf("Allocated memory successfully \n\n");
        for (i = 0; i < n; i++) {
            printf("Memory Address: %p\n", ptr + i);
            ptr[i] = i + 1;
        }

        printf("****The elements of the array are****:\n ");
        for (i = 0; i < n; i++) {
            printf("%d, ", ptr[i]);
        }
        return 0;
    }
    
    printf("\n\n");



    return 0;
}