#ifndef MEMORYALLOCATOR_HPP
#define MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct Node{
    Node *next;
    size_t size;
};

class MemoryAllocator
{
public:
    static Node *free;
    static Node *busy;

    static void *alloc(size_t size);
    static int dealloc(void* ptr);
    static void initialize();
};


#endif //MEMORYALLOCATOR_HPP
