#ifndef MILWORKER_H
#define MILWORKER_H
#include "worker.h"
#include "miltracker/object_tracker.h"
#include "miltracker/cv_onlineboosting.h"
#include "miltracker/cv_onlinemil.h"
#include "miltracker/Window.h"

class MilWorker : public Worker
{
public:
    MilWorker();
    ~MilWorker();

public slots:
    void doWork();
};

#endif // MILWORKER_H
