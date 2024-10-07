#include "MemoryBlock.c"
#include "allocator.h"
#include <sys/mman.h>
#include <string.h>

//Global Variables
extern MemoryBlock* head;
extern MemoryBlock* tail;
extern pthread_mutex_t global_memory_lock;
extern MemoryBlock* find_free_memory_block(size_t size);

/*
    *IMPLEMENTING MALLOC*
        ** Size is passed, this is the amount of memory block we need to allocate
        ** Returns a pointer to void* memory block
*/
void* jmalloc (size_t size){
    pthread_mutex_lock(&global_memory_lock);                                                                /* Locking the mutex */  
    MemoryBlock* block = find_free_memory_block(size);                                                      /* Find a free memory block */  

    if (block){
        block->memory.is_free = 0;                                                                          /* Mark the block as not free */  
        pthread_mutex_unlock(&global_memory_lock);                                                          /* Unlock the mutex */  
        return (void*)(block + 1);                                                                          /* Return the pointer to the block */  
    }

                                                                                                            /* At thsi point, we have no free memory block */  
                                                                                                            /* We need to allocate a new block of memory using sbrk */  
    size_t total_size = sizeof(MemoryBlock) + size;                                                         /* Total size of the memory block */  
    block = sbrk(total_size);                                                                               /* Allocate the memory block from the OS */  
    if (block == (void*)-1){
        printf("srbk failed!\n");                                                                           
        pthread_mutex_unlock(&global_memory_lock);                                                          /* Unlock the mutex */  
        return NULL;
    }
                                                                                                            /* Now initialize the memory block */  
                                                                                                            /* Connect it to the linked list */  
    block->memory.block_size = size;
    block->memory.is_free = 0;
    block->memory.nextBlock = NULL;

    if (head == NULL){                                                                                      /* If the head is NULL, set the head to the block */  
        head = block;
    }
    if (tail){                                                                                              /* If the tail is not NULL means MB is not empty */  
        tail->memory.nextBlock = block;
    }
    tail = block;
    pthread_mutex_unlock(&global_memory_lock);        
    return (void*)(block + 1);
} 


void jfree(void* block_pointer){
    if (!block_pointer) {
        printf("Block is NULL\n");
        return;
    }

    pthread_mutex_lock(&global_memory_lock);
    MemoryBlock* block = (MemoryBlock*)block_pointer -1;
    block->memory.is_free = 1;
                                                                                                            /* If the block is the tail, release it to the OS */  
                                                                                                            /* Otherwise, mark it as free cause it causes overhead */  
    /*
    head -> block1 -> block2 -> NULL
                            ^
                            |
                          tail (last block)
    */
   if (block->memory.nextBlock == NULL){
        if (brk(block) == -1){
            printf("brk failed\n");
            pthread_mutex_unlock(&global_memory_lock);  
            return;
        }
        if (head == block){                                                                                  /* If the block is the only block */             
            head = NULL;
            tail = NULL;
        } else {
            MemoryBlock* current = head;
            while (current->memory.nextBlock != block) {
                current = current->memory.nextBlock;
            }
            current->memory.nextBlock = NULL;
            tail = current;
        }
    }
    pthread_mutex_unlock(&global_memory_lock);
}

/*
    *Impelementation CALLOC
        *takes in num, and nsize to allocate n*size blocks of memory
*/

void* jcalloc(size_t num, size_t nsize) {
    if (!num || !nsize)
        return NULL;

    size_t total_size;
    void* block;

    total_size = num * nsize;

                                                                                                            
    if (nsize != 0 && total_size / nsize != num)                                                            /* Check for multiplication overflow */
        return NULL;

                                                                                                            
    block = jmalloc(total_size);                                                                            /* Use jmalloc to allocate the memory */
    if (!block)
        return NULL;

                                                                                                            
    memset(block, 0, total_size);                                                                           /* Initialize the allocated memory to zero */

    return block;
}


/*
    *Realloc Implementation
        *takes in a block of memory, Changes a block of memory by "size" pointed by this block
        *returns a header to the new memory Block 
*/
void* jrealloc(void* block, size_t size) {
    MemoryBlock* header;
    void* new_block;

    
    if (!block)                                                                                             /* If the block is NULL, allocate a new block */
        return jmalloc(size);
    
    if (size == 0) {
        jfree(block);
        return NULL;
    }

    header = (MemoryBlock*)block - 1;                                                                       /* Retrieve the block's metadata (header) */

    
    if (header->memory.block_size >= size)                                                                  /* If the block has enough space, return the block */
        return block;

    
    new_block = jmalloc(size);                                                                              /* Allocate a new block of memory */
    if (new_block) {
        memcpy(new_block, block, (header->memory.block_size < size) 
        ? header->memory.block_size : size);                                                                /* Copy The Old Data */ 
        jfree(block); 
    }

    return new_block;
}