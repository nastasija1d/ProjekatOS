#include "../h/ccb.hpp"
#include "../h/riscv.hpp"
#include "../h/operations.hpp"

CCB *CCB::running = nullptr;

int CCB::ID = 0;

CCB *CCB::createCoroutine(Body body, void *arg, uint64 *stack){
    CCB* c = new CCB(body, arg, stack);
    if (c->body != nullptr) {
        c->setReady(true);
        Scheduler::put(c);
    }
    return c;
}

CCB *CCB::initializeCoroutine(CCB::Body body, void *arg, uint64 *stack){
    return new CCB(body, arg, stack);
}

int CCB::startCoroutine(CCB *ccb){
    if(!ccb->isInitialized()) return -1;  //ako nit nije napravljena
    if(ccb->isWorking() || ccb->isReady() || ccb->isFinished() || ccb->isBlocked()) return -1;

    if (ccb->body != nullptr) {
        ccb->setReady(true);
        Scheduler::put(ccb);
    }
    return 0;
}

void CCB::yield(){
    Riscv::w_a0(THREAD_DISPATCH);   //u registar a0 se upisuje kod operacije
    __asm__ volatile ("ecall");
}

void CCB::dispatch(){
    CCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) {
        old->setReady(true);
        Scheduler::put(old); }

    running = Scheduler::get();
    running->setWorking(true);

    CCB::contextSwitch(&old->context, &running->context);
}

int CCB::exitCoroutine(){
    if (!running->isWorking()) {
        return -1;}
    running->setFinished(true);
    dispatch();
    return 0;
}

void CCB::threadWrapper(){
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    yield();
}

void CCB::setReady(bool value) {
    ready = value;
    if (value) {
        working = false;
        finished = false;
    }
}

void CCB::setFinished(bool value) {
    this->finished = value;
    if (value){
        ready = false;
        working = false;
        initialized = false;
        blocked = false;
    }
}

void CCB::setBlocked(bool value) {
    this->blocked = value;
    if (value){
        ready = false;
        working = false;
        finished = false;
    }else{
        ready = true;
    }
}

void CCB::block(){
    //if (!running->isWorking()) { return;}
    running->setBlocked(true);
    dispatch();   //nit se blokirala pa drugim nitima ustupa procesor
}

void CCB::unblock() {
    this->setBlocked(false);
    Scheduler::put(this);
}

