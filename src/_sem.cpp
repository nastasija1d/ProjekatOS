#include "../h/_sem.hpp"

int _sem::wait() {
    if (!active) return -1;
    if (value > 0){
        value--;
    }else{
        blockedQueue.addLast(CCB::running);
        CCB::block();
    }
    return 0;
}

int _sem::signal() {
    if (!active) return  -1;
    if (blockedQueue.isEmpty()){
        value++;
    }else{
        CCB *ccb = blockedQueue.removeFirst();
        ccb->unblock();
    }
    return 0;
}

_sem::~_sem() {
    close();
}

int _sem::close() {
    if (!active) return -1;
    active = false;
    while(!blockedQueue.isEmpty()) {
        CCB* ccb= blockedQueue.removeFirst();
        ccb->unblock();
    }
    return 0;
}