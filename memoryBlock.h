#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <stdlib.h>
#include <pthread.h>

typedef char ALIGN[16];

                                                        /*
                                                            * MemoryBlock is a union that contains 
                                                                -> Struct or META_DATA of the memory block 
                                                                    * Including nextBlock (pointer to the next block of memory)
                                                                -> Padding or ALIGN
                                                            * Union allows the memory block to either be used as struct or as a padding(ALGIN)
                                                            * We would never need more than MemoryBlock access at a time (We might however need to store different types of datas)
                                                                * So, we use union to store the data in the same memory location
                                                        */

union MemoryBlock {
    struct{
        size_t block_size;
        unsigned is_free;
        union MemoryBlock* nextBlock;
    } mem;
    ALIGN stub;
};

typedef union MemoryBlock MemoryBlock;
extern union MemoryBlock* head;
extern union MemoryBlock* tail;

extern pthread_mutex_t global_malloc_lock;

MemoryBlock* find_free_memory_block(size_t size);


#endif // MEMORY_BLOCK_H

/*

        Diagram of the Memory Block
+-----------------+-----------------+-------------------------------------+--------------------------------------------------+           
| MemoryBlock0    |  Memory Block 1 |  Memory Block 2    | Memory Block 3 | Memory Block 4     | Memory Block 5     |
+-----------------+-----------------+-------------------------------------+--------------------------------------------------+
| size            |                 | size               |                |                    |                    |
| is_free         |                 | is_free            |                |                    |                    |
| next----------->|next------------>| next-------------->|next----------->| next-------------->| next-------------->| TAIL
+-----------------+-----------------+-------------------------------------+--------------------------------------------------+
| ALIGN           | ALIGN           | ALIGN              |                | ALIGN              | ALIGN              |
+-----------------+-----------------+-------------------------------------+--------------------------------------------------+


*/