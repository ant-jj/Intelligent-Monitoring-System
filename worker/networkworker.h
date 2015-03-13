#ifndef NETWORKWORKER_H
#define NETWORKWORKER_H
#include "worker.h"




class netWorkWorker : public Worker
{
public:
    netWorkWorker();
    ~netWorkWorker();
public slots:
    void doWork();
};

#endif // NETWORKWORKER_H
