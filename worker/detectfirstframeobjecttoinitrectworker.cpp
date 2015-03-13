#include "detectfirstframeobjecttoinitrectworker.h"

DetectFirstFrameObjectToInitRectWorker::DetectFirstFrameObjectToInitRectWorker()
{

}

DetectFirstFrameObjectToInitRectWorker::~DetectFirstFrameObjectToInitRectWorker()
{

}

void DetectFirstFrameObjectToInitRectWorker::doWork()
{
    mutex.lock();
    bool abort = _abort;
    mutex.unlock();
/*
    video.open (videoName.toStdString ().c_str ());
    if(!video.isOpened ()){
        emit finished ();
        return;
    }
    Mat frame;
    video>>frame;
    if(frame.empty ()){
        emit finished ();
        return;
    }
    //detect
*/

/*
 * 使用何种方法检测何种东西
 */
    if(trackmethods == HOG ){
        qDebug()<<"use HOG method";
    }

    if(trackmethods == HAAR ){
        qDebug()<<"use HAAR method";
    }

    if(trackmethods == LBP ){
        qDebug()<<"use LBP method";
    }

    if(tracktype == People ){
        qDebug()<<"detect people";
    }
    if(tracktype == Car ){
        qDebug()<<"detect car";
    }
    if(tracktype == FootBall ){
        qDebug()<<"detect football";
    }


    //for david.mpg 返回一个随机的Rect
    cv::Rect rect(165,93,51,54);
    emit RectInited(rect);

    video.release ();
    mutex.lock();
    _working = false;
    mutex.unlock();
    emit finished ();
    return;
}

