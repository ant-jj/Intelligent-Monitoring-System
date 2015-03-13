#ifndef DETECTFIRSTFRAMEOBJECTTOINITRECTWORKER_H
#define DETECTFIRSTFRAMEOBJECTTOINITRECTWORKER_H
#include "worker.h"
#include "tracktype.h"

class DetectFirstFrameObjectToInitRectWorker : public Worker
{
public:
    DetectFirstFrameObjectToInitRectWorker();
    ~DetectFirstFrameObjectToInitRectWorker();
    cv::VideoCapture video;
    QString videoName;
    TrackMethods trackmethods;
    TrackType tracktype;
public slots:
    virtual void doWork();
};

#endif // DETECTFIRSTFRAMEOBJECTTOINITRECTWORKER_H
