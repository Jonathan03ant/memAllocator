#include "memoryBlock.h"
#include "allocator.h"
#include <sys/mman.h>
#include <string.h>

/*
    *Implementing malloc
        *size is passed, this is the amount of memory block we need to allocate
*/
void* jalloc (size_t size){

    memory_blockHeader* header;

    /* If size is zero, return null */

    if (!size)
        return NULL;
    /* *LOCK */

    pthread_mutex_lock(&global_malloc_lock);

        /* 
            *If a block of memory of size >= size is found, mark it as not free 
             *Takes in a size to allocate a memory for
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

    total_size = sizeof(memory_blockHeader) + size;
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