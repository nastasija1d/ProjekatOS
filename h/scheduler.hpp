#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "list.hpp"

class CCB;

class Scheduler
{
private:
    static List<CCB> readyCoroutineQueue;

public:
    static CCB *get();

    static void put(CCB *ccb);

};

#endif //SCHEDULER_HPP
