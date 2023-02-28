//#include "../lib/mem.h"
#include "../h/syscall_cpp.hpp"

//              MEMORIJA
using size_t = decltype(sizeof(0));

void *operator new  (size_t n){ return mem_alloc(n); }

void *operator new[](size_t n){ return mem_alloc(n); }

void operator delete  (void *p) {  mem_free(p); }

void operator delete[](void *p) {  mem_free(p); }





//              THREAD
void Thread::wrapper(void* thread) {
    if(thread) {
        ((Thread*)thread)->run();
    }
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->myHandle = nullptr;
    thread_create(&this->myHandle, body, arg);
}

Thread::Thread() {
    //this->myHandle = nullptr;
    thread_initialize(&this->myHandle, Thread::wrapper, this);
}

int Thread::start() {
    return thread_start(this->myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::~Thread() { delete myHandle; }





//              SEMAPHORE
Semaphore::Semaphore(unsigned init)
{
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore()
{
    sem_close(myHandle);
}

int Semaphore::wait()
{
    return sem_wait(myHandle);
}

int Semaphore::signal()
{
    return sem_signal(myHandle);
}




//              CONSOLE
char Console::getc()
{
    return ::getc();
}

void Console::putc(char c)
{
    ::putc(c);
}