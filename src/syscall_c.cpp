#include "../h/syscall_c.hpp"
#include "../h/operations.hpp"
#include "../h/riscv.hpp"

inline void ecall(uint64 o){
    __asm__ volatile ("mv a0, %0" : : "r" (o)); //u registar a0 upisujemo kod sistemskog poziva
    __asm__ volatile ("ecall");
}

inline uint64 eval(){
    uint64 volatile val;
    __asm__ volatile ("mv %0, a0" : "=r" (val)); //u registar a0 sistemski poziv upisuje povratnu vrednost
    return val;
}

void *mem_alloc(size_t size){

    if (!size) { return nullptr; }
    size = (size % MEM_BLOCK_SIZE > 0) ? (size / MEM_BLOCK_SIZE + 1) : (size / MEM_BLOCK_SIZE);
    Riscv::w_a1((uint64)size);
    ecall(MEM_ALLOC);
    return (void*)eval();
}

int mem_free(void *ptr){
    if (!ptr) { return 0; }
    Riscv::w_a1((uint64)ptr);
    ecall(MEM_FREE);
    return (eval() == 0) ? 0 : -1;
}

int thread_create(thread_t *handle, void (*start_routine)(void*), void *arg){
    //if (!handle) { return -1; }
    if (!start_routine) { return -2; }
    void *stack = mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
    if (!stack) { return -3; }

    Riscv::w_a4((uint64)stack);
    Riscv::w_a3((uint64)arg);
    Riscv::w_a2((uint64)start_routine);
    Riscv::w_a1((uint64)handle);

    ecall(THREAD_CREATE);
    return (eval() == 0) ? 0 : -1;
}

int thread_initialize(thread_t *handle, void (*start_routine)(void*), void *arg){
    //if (!handle) { return -1; }
    if (!start_routine) { return -2; }
    void *stack = mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
    if (!stack) { return -3; }

    Riscv::w_a4((uint64)stack);
    Riscv::w_a3((uint64)arg);
    Riscv::w_a2((uint64)start_routine);
    Riscv::w_a1((uint64)handle);

    ecall(THREAD_INIT);
    return (eval() == 0) ? 0 : -1;
}

int thread_start(thread_t t){
    Riscv::w_a1((uint64)t);
    ecall(THREAD_START);
    return (eval() == 0) ? 0 : -1;
}

int thread_exit(){
    ecall(THREAD_EXIT);
    return (eval() == 0) ? 0 : -1;
}

void thread_dispatch(){
    ecall(THREAD_DISPATCH);
}

int sem_open (sem_t* handle, unsigned  init) {
    if (!handle) return -1;
    Riscv::w_a2((uint64)init);
    Riscv::w_a1((uint64)handle);

    ecall(SEM_OPEN);
    return (eval() == 0) ? 0 : -1;
}

int sem_close (sem_t handle) {
    if (!handle) return -1;
    Riscv::w_a1((uint64)handle);
    ecall(SEM_CLOSE);
    return (eval() == 0) ? 0 : -1;
}

int sem_wait (sem_t id) {
    if (!id) return -1;
    Riscv::w_a1((uint64)id);
    ecall(SEM_WAIT);
    return (eval() == 0) ? 0 : -1;
}

int sem_signal (sem_t id) {
    if (!id) return -1;
    Riscv::w_a1((uint64)id);
    ecall(SEM_SIGNAL);
    return (eval() == 0) ? 0 : -1;
}

char getc () {
    ecall(GETC);
    return (char)eval();
}

void putc (char c) {
    Riscv::w_a1((uint64)c);
    ecall(PUTC);
}