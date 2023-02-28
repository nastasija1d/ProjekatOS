#include "../h/scheduler.hpp"

List<CCB> Scheduler::readyCoroutineQueue;

CCB *Scheduler::get()
{
    return readyCoroutineQueue.removeFirst();
}

void Scheduler::put(CCB *ccb)
{
    readyCoroutineQueue.addLast(ccb);
}