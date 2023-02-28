#ifndef CCB_HPP
#define CCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

// Coroutine Control Block
class CCB
{
public:
    ~CCB() { delete[] stack; }

    bool isFinished() const { return finished; }
    void setFinished(bool value);

    bool isInitialized() const { return initialized; }
    void setInitialized(bool value) { initialized = value; }

    bool isWorking() const { return working; }
    void setWorking(bool value) { working = value; }

    bool isReady() const { return ready; }
    void setReady(bool value);

    bool isBlocked() const {return blocked;}
    void setBlocked(bool value);

    using Body = void (*)(void*);

    static CCB *createCoroutine(Body body, void *arg, uint64 *stack); //pravi nit i pokrece je

     static CCB *initializeCoroutine(Body body, void *arg, uint64 *stack); //samo pravi nit

    static int startCoroutine(CCB *ccb); //napravljenu nit dodaje u sceduler (pokrece je)

    static int exitCoroutine(); //gasi tekucu nit

    static void yield();

    static CCB *running; //tekuca nit koja se izvrsava

    static int ID;

    void *operator new(size_t size) {
        size = (size % MEM_BLOCK_SIZE > 0) ? (size / MEM_BLOCK_SIZE + 1) : (size / MEM_BLOCK_SIZE);
        return MemoryAllocator::alloc(size);
    }

    void operator delete(void *ptr) { MemoryAllocator::dealloc(ptr); }

private:
     explicit CCB(Body body, void *arg, uint64 *stack) :
            body(body),
            arg(arg),
            stack(body != nullptr ? stack : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
                    })
    {
        initialized = true;
        ready = false;
        working = true;
        finished = false;
        blocked = false;
        id = ID++;
        if (body != nullptr) working = false;
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };


    int id;
    Body body;          //telo niti - funkcija nad kojom se nit pokrece
    void *arg;          //pokazivac na argumente
    uint64 *stack;      //stek koji je dodeljen niti
    Context context;    //kontekst niti - ukoliko se izvrsavanje niti prekine, ovde se cuva adresa povratka i sp

    bool initialized;   //da li je napravljena
    bool ready;         //da li je pokrenuta (da li je pozvana f-ja start i nit dodata u scheduler)
    bool working;       //da li se trenutno izvrsava
    bool finished;      //da li je zavrsila
    bool blocked;       //da li je blokirana na nekom semaforu

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();  //metoda kojom se nit "odrice svog rada" i ustupa procesor drugoj

    static void threadWrapper();  //funkcija omotac koja samo sluzi da zavrsi nit

    static void block();    //blokira tekucu nit na semaforu

    void unblock();        //odblokira datu nit i stavlja je u scheduler

    friend class Riscv;
    friend class _sem;

};

#endif //CCB_HPP
