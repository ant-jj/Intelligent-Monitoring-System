#ifndef GETNETFRAMEWORKER_H
#define GETNETFRAMEWORKER_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

#include "worker.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>


#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;
using namespace cv;
using namespace std;


class getNetFrameWorker : public Worker
{
public:
    getNetFrameWorker();
    ~getNetFrameWorker();
public slots:
    void doWork();
};

#endif // GETNETFRAMEWORKER_H
