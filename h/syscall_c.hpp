#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "ccb.hpp"
#include "../h/_sem.hpp"

class _sem;

typedef CCB* thread_t;
typedef _sem* sem_t;

void* mem_alloc (size_t size);

int mem_free (void*);

int thread_initialize(thread_t *handle, void(*start_routine)(void*), void* arg);

int thread_start(thread_t); //ove f-je postoje u C-API da bi C++ API mogo da ih "obmota"

int thread_create(thread_t *handle, void(*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

int sem_open (sem_t* handle, unsigned init);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

char getc ();

void putc (char c);

#endif //SYSCALL_C_HPP