#ifndef __SEM_HPP
#define __SEM_HPP

#include "../h/list.hpp"
#include "../h/ccb.hpp"

class _sem
{
public:
    _sem(int init){
        value = init;
        active = true;
    }

    int wait();

    int signal();

    int close();

    ~_sem();

    void *operator new(size_t size) {
        size = (size % MEM_BLOCK_SIZE > 0) ? (size / MEM_BLOCK_SIZE + 1) : (size / MEM_BLOCK_SIZE);
        return MemoryAllocator::alloc(size);
    }

    void operator delete(void *ptr) { MemoryAllocator::dealloc(ptr); }

private:
    bool active;
    int value;
    List<CCB> blockedQueue;

};


#endif //__SEM_HPP
