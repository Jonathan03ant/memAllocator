#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

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
    } memory;
    ALIGN stub;
};
typedef union MemoryBlock MemoryBlock;
pthread_mutex_t global_memory_lock = PTHREAD_MUTEX_INITIALIZER;

MemoryBlock* head = NULL;
MemoryBlock* tail = NULL;


/*
    * FUnction to find free memory block
    * size_t size: size of the memory block needed
    * return: MemoryBlock pointer
*/
MemoryBlock* find_free_memory_block(size_t size){
    MemoryBlock* current = head;
    while (current){
        if (current->memory.is_free && current->memory.block_size >= size){
            return current;
        }
        current = current->memory.nextBlock;
    }

    printf("No free memory block found\n");
    return NULL;
}


#endif // MEMORY_BLOCK_H

/*
        Diagram for a single memory block
+-------------------------+-------------------+-------------------+-----------------------+
| block_size (MD)         | is_free (MD)      | nextBlock (MD)    | Actual Usable Memory |
+-------------------------+-------------------+-------------------+-----------------------+
|  64 bytes               |      1 (free)     |    (pointer)      | (block + 1) -> usable memory |
+-------------------------+-------------------+-------------------+-----------------------+


        Diagram of the a typical Memory Block
+-----------------+-----------------+-------------------------------------+--------------------------------------------------+           
| MemoryBlock0    |  Memory Block 1 |  Memory Block 2    | Memory Block 3 | Memory Block 4     | Memory Block 5     |
+-----------------+-----------------+-------------------------------------+--------------------------------------------------+
| size            |                 | size               |                |                    |                    |
| is_free         |                 | is_free            |                |                    |                    |
| next----------->|next------------>| next-------------->|next----------->| next-------------->| next-------------->| TAIL
+-----------------+-----------------+-------------------------------------+--------------------------------------------------+
| ALIGN           | ALIGN           | ALIGN              |                | ALIGN              | ALIGN              |
+-----------------+-----------------+-------------------------------------+--------------------------------------------------+
Rightnow, we have head and tail memory blocks that has this structure but points to NULL
*/