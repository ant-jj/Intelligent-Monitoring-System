#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QDebug>
#include <QThread>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
//include particle
#include <iostream>
#include "particle/filter.h"
#include "particle/lbp.h"
#include "particle/selector.h"
#include "particle/state.h"
#include "particle/hist.h"
#include <cmath>
//#include <sys/time.h>
//#include <unistd.h> // For getopt
#include <cstdlib>
#include <string>
#include <iostream>

//worker  跟踪器处理的目标和处理的方法（第一帧Rect初始化）
#include "tracktype.h"

using namespace cv;
using namespace std;

static bool startParticle = false,startCT =false;


class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);
    void requestWork();
    void abort();
    std::string file2Track;
    bool initWithMouse;
    cv::Rect DetectFirstFrameToInitRect(cv::Mat &image,TrackType tracktype,TrackMethods trackmethod);
    cv::Rect initRect;
public:
    bool _abort;
    bool _working;
    QMutex mutex;

signals:
    void imageChanged(const cv::Mat &img);//发送跟踪的图像给主线程，UI显示
    void workRequested();
    void valueChanged(const QString &value);
    void RectInited(const cv::Rect &rect);//发送RectInit 给 DetectFirstFrameObjectToInitRectWorker
    void finished();

public slots:
    virtual void doWork();
};

#endif // WORKER_H
