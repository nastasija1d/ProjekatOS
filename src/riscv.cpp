#include "../h/riscv.hpp"
#include "../h/ccb.hpp"
#include "../lib/console.h"
#include "../h/operations.hpp"
#include "../h/syscall_c.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    long a0 = arg0();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) //ovo je prekid usled poziva instrukcije ECALL, 8-korisnicki, 9-sistemski rezim
    {
        uint64 a1 = arg1();     //argumenti f-je upisani u registre
        uint64 a2 = arg2();
        uint64 a3 = arg3();
        uint64 a4 = arg4();

        using Body = void (*)(void *);
        thread_t * h;
        sem_t * st;
        sem_t s;

        uint64 volatile sepc = r_sepc() + 4;    //u registar SEPC se prilikom skoka na prekidnu rutinu upisuje adresa povratka (PC)
                                                //posto PC ukazuje na poziv ECALL, uvecamo za 4 i sad pokazuje na prvu sledecu instrukciju
        uint64 volatile sstatus = r_sstatus();
        switch (a0)
        {
            case MEM_ALLOC:
                w_retVal((uint64) MemoryAllocator::alloc((size_t)a1));
                break;
            case MEM_FREE:
                w_retVal((uint64) MemoryAllocator::dealloc((void *)a1));
                break;
            case GETC:
                w_retVal((uint64)__getc());
                break;
            case PUTC:
                __putc((char) a1);
                break;
            case THREAD_DISPATCH:
                CCB::dispatch();
                break;
            case THREAD_EXIT:
                w_retVal(CCB::exitCoroutine());
                break;
            case THREAD_CREATE:
                h= (thread_t*) a1;
                *h = CCB::createCoroutine((Body)a2, (void*) a3, (uint64*)a4);
                w_retVal(0);
                break;
            case THREAD_INIT:
                h = (thread_t*) a1;
                *h = CCB::initializeCoroutine((Body)a2, (void*) a3, (uint64*)a4);
                w_retVal(0);
                break;
            case THREAD_START:
                CCB::startCoroutine((CCB*)a1);
                break;
            case SEM_OPEN:
                st = (sem_t*) a1;
                *st = new _sem((int)a2);
                w_retVal(0);
                break;
            case SEM_CLOSE:
                s = (sem_t)a1;
                w_retVal(s->close());
                break;
            case SEM_SIGNAL:
                s = (sem_t)a1;
                w_retVal(s->signal());
                break;
            case SEM_WAIT:
                s = (sem_t)a1;
                w_retVal(s->wait());
                break;
        }
        w_sstatus(sstatus);
        w_sepc(sepc); //upisujemo adresu na koju se vracamo kad izadjemo iz prekidne rutine

    } else if (scause == 0x8000000000000001UL) // ovo je prekid iz 3. najprivilegovanijeg rezima rada -> u ovom projektu je to TAJMER
    {
        mc_sip(SIP_SSIP); //registar SIP sadrzi informaciju o tome koji zahtevi za prekid su trenutno aktivni
        // bit SSIP(1 bit) oznacava da postoji zahtev za softverskim prekidom, ako u njega upisemo 0, znaci da nema zahteva (obradjen je)
    } else if (scause == 0x8000000000000009UL) //ovo je spoljasnji hardverski prekid -> KONZOLA
    {
        console_handler();
    } else  //ako nije nista od navedenog, znaci da je doslo do neke greske i ispisujemo je
    {
        // unexpected trap cause
    }

}

/*
CCB::timeSliceCounter++;
if (CCB::timeSliceCounter >= CCB::running->getTimeSlice()){
    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();
    CCB::timeSliceCounter = 0;
    CCB::dispatch();
    w_sstatus(sstatus);
    w_sepc(sepc);
}
 */
