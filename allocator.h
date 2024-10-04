#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

void* jmalloc(size_t size);
void jfree(void* block);
void* jcalloc(size_t num, size_t nsize);
void* jrealloc(void* block, size_t size);




#endif 