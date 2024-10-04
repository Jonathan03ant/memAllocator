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
    pthread_mutex_lock(&global_memory_lock);                                                                // Locking the mutex
    MemoryBlock* block = find_free_memory_block(size);                                                      // Find a free memory block

    if (block){
        block->memory.is_free = 0;                                                                          // Mark the block as not free
        pthread_mutex_unlock(&global_memory_lock);                                                          // Unlock the mutex
        return (void*)(block + 1);                                                                          // Return the pointer to the block
    }

                                                                                                            // At thsi point, we have no free memory block
                                                                                                            // We need to allocate a new block of memory using sbrk
    size_t total_size = sizeof(MemoryBlock) + size;                                                         // Total size of the memory block
    block = sbrk(total_size);                                                                               // Allocate the memory block from the OS
    if (block == (void*)-1){
        printf("srbk failed!\n");                                                                           
        pthread_mutex_unlock(&global_memory_lock);                                                          // Unlock the mutex
    }
                                                                                                            // Now initialize the memory block
                                                                                                            // Connect it to the linked list
    block->memory.block_size = size;
    block->memory.is_free = 0;
    block->memory.nextBlock = NULL;

    if (head == NULL){                                                                                      // If the head is NULL, set the head to the block
        head = block;
    }
    if (tail){                                                                                              // If the tail is not NULL means MB is not empty
        tail->memory.nextBlock = block;
    }
    tail = block;
    pthread_mutex_unlock(&global_memory_lock);        
    return (void*)(block + 1);
}                                                    



/*
    *Implementing free
        *Block is passed to jfree, it is a pointer to the start of memory to be freed
*/
void jfree(void* block) {
    memory_blockHeader* header;
    memory_blockHeader* curr;

    /*
        *Check if the bloack to be freed is NULL
    */

    if (!block){
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);
    /*
        *Retriving the header to access information about the memory block (UNION HEADER)
    */
    header = (memory_blockHeader*)block - 1;

    /*
        *After retriving the header (MEMORYBLOCK METADATA), we need to check the following
            *If it is the end of our Linked List (header == tail) ---> or points to tail
                --> we can release it to the OS
            *Otherwise, i.e the block is in the middle, we can only mark it Free
                --> Mark it free so we can use it later
    */
    if (header == tail){
        /* 
            Block To be freed is the only Element 
            *Head-->Block-->Tail
        */
        if (header == tail){
            head = tail = NULL;
        }
        /*
            *Is there are other blocks, traverse the LL and find the tail, 
                --> set the tail to NULL
        */
        else{
            curr = head;
            while(curr){
                if(curr->mem.next == tail){
                    curr->mem.next = NULL;
                    tail = curr;
                }
                curr = curr->mem.next;
            }
        }

        /*
            *Finally, system call to actually release the memory
        */
        munmap(header, header->mem.size + sizeof(memory_blockHeader));
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }
    /*
        *If the Block (From header) is not at the end, it is better if we mark it "FREE"
            *So it cn=an be used later
    */

    header->mem.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}

/*
    *Impelementation Calloc
        *takes in num, and nsize to allocate n*size blocks of memory
*/

void* jcalloc(size_t num, size_t nsize){
    if (!num || !nsize)
        return NULL;

    size_t total_size;
    void* block;

    total_size = num * nsize;

    /* check for multiplication overflow */
    if (nsize != total_size / num)
        return NULL;

    /*
        *We can simply call our Jalloc function which grabs a memory,
        *It also sets the appropriate header values
    */
    block = jalloc(total_size);

    /*
        *However, calloc is different from malloc (Jalloc)
            --> we need to initialize the blocks to zero
            --> unlike malloc(jalloc) where they are undefined
    */

    mmap(block, total_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    return block;
}

/*
    *Realloc Implementation
        *takes in a block of memory, Changes a block of memory by "size" pointed by this block
        *returns a header to the new memory Block 
*/
void* jrealloc(void* block, size_t size){
    memory_blockHeader* header;
    void* new_block;

    if (!block || !size)
        return jalloc(size);

    /*
        *Retriving information of the given block
    */

    header = (memory_blockHeader*)block - 1;
    /* Check the size before increasing it */

    if (header->mem.size >= size)
        return block;
    
    /*  
        *If size is not sufficient
            *allocate a new Block 
            *Copy the oldBlock to the new one
            *free the oldOne
    */

    new_block = jalloc(size);
    if (new_block){
        memcpy(new_block, block, header->mem.size);
        jfree(block);
    }
    return new_block;
}