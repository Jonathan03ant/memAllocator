#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef char ALIGN[16];

/*
    *Header is a representation of the block of memory that is to be allocated
    *It contains 
        --> the size of the block of memory
        --> a pointer to the next block of memory (header itself)
        --> memory marker to check if the block of memory is free or not
*/

union header {
    struct {
        size_t size;
        unsigned is_free;
        union header* next;
    } mem;
    ALIGN stub;
};

typedef union header memory_block;

memory_block* head;
memory_block* tail;

pthread_mutex_t global_malloc_lock;

/*
    *function to get a block of memory
    *Traverses through the LL Mem blocks and 
        --> returns header_t* if the block is free and size is sufficient
        --> returns null if no block is free or size is not sufficient
*/

memory_block* get_free_block(size_t size) {
    memory_block* curr = head;

    while (curr){
        if (curr->mem.is_free && curr->mem.size >= size)
            return curr;
        curr = curr->mem.next;
    }

    return NULL;
}

/*
    *Debugging
*/
int main() {
    size_t size = 10;

    memory_block* debug_block = malloc(sizeof(memory_block));
    debug_block->mem.size = size;
    debug_block->mem.is_free = 1;
    debug_block->mem.next = NULL;

    head = debug_block;


    memory_block* block = get_free_block(size);
    if (block == NULL) {
        printf("No free block found.\n");
    } else {
        printf("Free block of size %zu found.\n", block->mem.size);
    }

    free(block);
    return 0;
}