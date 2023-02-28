#include "../h/ccb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"

extern void userMain();

void user_wrapper(void *sem)
{
    userMain();
    sem_signal((sem_t)sem);
}

int main()
{
    MemoryAllocator::initialize();

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap); //u registar STVEC upisujemo adresu funkcije supervisorTrap jer zelimo da nju izvrsava kad se desi prekid
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE); //u bit SIE se upisuje 1 cime se dozvoljavaju prekidi, ili 0 cie se zabranjuju


    thread_t main;
    main = CCB::createCoroutine(nullptr, nullptr, nullptr);
    CCB::running = main;


    sem_t semafor;
    sem_open(&semafor,0);

    thread_t user;
    thread_create(&user, user_wrapper, semafor);

    sem_wait(semafor);
    sem_close(semafor);

    return 0;

}