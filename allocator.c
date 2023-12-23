#include "memoryBlock.h"
#include "allocator.h"
#include <sys/mman.h>

void* jalloc (size_t size){

    memory_block* header;

    /* If size is zero, return null */

    if (!size)
        return NULL;
    /* *LOCK */

    pthread_mutex_lock(&global_malloc_lock);

        /* 
            *If a block of memory of size >= size is found, mark it as not free 
             *Return a pointer to the block of memory
        */
    header = get_free_block(size);
    if (header){
        header->mem.is_free = 0;

        /* *UNLOCK */
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }

    /* 
        *If no such block is found, allocate a new block of memory
            *Calling the OS mmap (MEMORY MAP)--> System Call
    */
    size_t total_size;
    void* block;

    total_size = sizeof(memory_block) + size;
    block = mmap(0, total_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (block == MAP_FAILED){

        /* *UNLOCK */
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = block;
    header->mem.size = size;
    header->mem.is_free = 0;
    header->mem.next = NULL;

    /*
        *Manage the linked list of memory blocks
    */
    if (!head)
        head = header;
    if (tail)
        tail->mem.next = header;
    tail = header;

    /* *UNLOCK */
    pthread_mutex_unlock(&global_malloc_lock);
    return (void*)(header + 1);
}