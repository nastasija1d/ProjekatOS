#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

enum Opcodes: uint64 {
    MEM_ALLOC       = 0x01,
    MEM_FREE        = 0x02,
    THREAD_CREATE   = 0x11,
    THREAD_EXIT     = 0x12,
    THREAD_DISPATCH = 0x13,
    THREAD_START    = 0x14,
    THREAD_INIT     = 0x15,
    SEM_OPEN        = 0x21,
    SEM_CLOSE       = 0x22,
    SEM_WAIT        = 0x23,
    SEM_SIGNAL      = 0x24,
    TIME_SLEEP      = 0x31,
    GETC            = 0x41,
    PUTC            = 0x42,
};

#endif //OPERATIONS_HPP
