#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <stdlib.h>
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

extern memory_block* head;
extern memory_block* tail;

extern pthread_mutex_t global_malloc_lock;

memory_block* get_free_block(size_t size);

#endif // MEMORY_BLOCK_H