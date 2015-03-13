#ifndef CTWORKER_H
#define CTWORKER_H
#include "worker.h"
#include <QObject>
#include <QMutex>
#include <QThread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "CompressiveTracking/CompressiveTracker.h"
class CTWorker : public Worker
{
public:
    CTWorker();
public slots:
    void doWork();
};

#endif // CTWORKER_H
