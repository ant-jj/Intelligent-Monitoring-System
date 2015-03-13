#include "ctworker.h"
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

using namespace std;
using namespace cv;

Rect box; // tracking object
bool drawing_box = false;
bool gotBB = false;	// got tracking box or not
bool fromfile = false;

void readBB(char* file )	// get tracking box from file
{
    ifstream tb_file (file);
    string line;
    getline(tb_file, line);
    istringstream linestream(line);
    string x1, y1, w1, h1;
    getline(linestream, x1, ',');
    getline(linestream, y1, ',');
    getline(linestream, w1, ',');
    getline(linestream, h1, ',');
    int x = atoi(x1.c_str());
    int y = atoi(y1.c_str());
    int w = atoi(w1.c_str());
    int h = atoi(h1.c_str());
    box = Rect(x, y, w, h);
}

// tracking box mouse callback
void mouseHandler(int event, int x, int y, int flags, void *param)
{
    switch (event)
    {
    case CV_EVENT_MOUSEMOVE:
        if (drawing_box)
        {
            box.width = x - box.x;
            box.height = y - box.y;
        }
        break;
    case CV_EVENT_LBUTTONDOWN:
        drawing_box = true;
        box = Rect(x, y, 0, 0);
        break;
    case CV_EVENT_LBUTTONUP:
        drawing_box = false;
        if (box.width < 0)
        {
            box.x += box.width;
            box.width *= -1;
        }
        if( box.height < 0 )
        {
            box.y += box.height;
            box.height *= -1;
        }
        gotBB = true;
        break;
    default:
        break;
    }
}

void print_help(void)
{
    printf("use:\n     welcome to use CompressiveTracking\n");
    printf("Kaihua Zhang's paper:Real-Time Compressive Tracking\n");
    printf("C++ implemented by yang xian\nVersion: 1.0\nEmail: yang_xian521@163.com\nDate:	2012/08/03\n\n");
    printf("-v    source video\n-b        tracking box file\n");
}

void read_options(VideoCapture& capture,string avifile ,bool initWithMouseFlag,Worker *work)
{

    //select a roi by mouse,if not ,plea uncomment below

    if(!initWithMouseFlag)
    /*
     * 如果不是鼠标选择初始化目标，则运行检测程序初始化检测目标RECT
     */
    {
//        work.DetectFirstFrameToInitRect ()

        box = work->initRect;

//        readBB("C:/Users/leeyoung/Desktop/src/init.txt");
        gotBB = true;
    }
    capture.open(avifile.c_str ());
    fromfile = true;
}


CTWorker::CTWorker()
{
}

void CTWorker::doWork()
{

    VideoCapture capture;
    capture.open (this->file2Track);
    read_options(capture,this->file2Track,this->initWithMouse,this);

    if (!capture.isOpened())
    {
        cout << "capture device failed to open!" << endl;
        emit finished();
        return ;
    }

    namedWindow("CT", CV_WINDOW_AUTOSIZE);
    setMouseCallback("CT", mouseHandler, NULL);
    CompressiveTracker ct;
    Mat frame;
    Mat last_gray;
    Mat first;
    capture >> frame;
    if(frame.empty ()) {
        emit finished();
        return;
    }
    cvtColor(frame, last_gray, CV_RGB2GRAY);
    frame.copyTo(first);

    // Initialization
    while(!gotBB)
    {
        if (!fromfile)
        {
            capture >> frame;
        }
        else
        {
            first.copyTo(frame);
        }
        cvtColor(frame, last_gray, CV_RGB2GRAY);
        rectangle(frame, box, Scalar(0,0,255));
        imshow("CT", frame);
        if (cvWaitKey(33) == 'q') {
            emit finished();
            return ;
        }
    }
    // Remove callback
    setMouseCallback("CT", NULL, NULL);
    cvDestroyWindow("CT");

    printf("Initial Tracking Box = x:%d y:%d h:%d w:%d\n", box.x, box.y, box.width, box.height);
    // CT initialization
    ct.init(last_gray, box);
    // Run-time
    Mat current_gray;

    while(capture.read(frame))
    {
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        if (abort) {
            qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
            break;
        }
        //add your code here
        cvtColor(frame, current_gray, CV_RGB2GRAY);
        ct.processFrame(current_gray, box);
        rectangle(frame, box, Scalar(0,0,255));
        Mat lableimage ;
        frame.copyTo (lableimage);
        emit imageChanged (lableimage);
    }
    mutex.lock();
    _working = false;
    mutex.unlock();
    qDebug()<<"Worker process finished in Thread "<<thread()->currentThreadId();
    emit finished();
}
