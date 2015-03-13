#include "milworker.h"
MilWorker::MilWorker()
{

}

MilWorker::~MilWorker()
{

}

void MilWorker::doWork()
{
    //mil worker
    cv::ObjectTrackerParams params;
    params.algorithm_ = cv::ObjectTrackerParams::CV_ONLINEMIL;
    params.num_classifiers_ = 50;
    params.num_features_ = 250;
    cv::ObjectTracker tracker(params);
    const char* prefix = "img";
    const char* suffix = "png";
    char filename[1024];
    // Some book-keeping
    bool is_tracker_initialized = false;
    cv::VideoCapture capture;
    capture.open(this->file2Track);
    if (!capture.isOpened()) {
        std::cout << "camera not found..." << std::endl;
        return;
    }
    window selectWin = window("select rectangle to track");
    selectWin.initialize(&capture);
    selectWin.run(true);
    cvDestroyWindow("select rectangle to track");
    CvRect init_bb =selectWin.rect;
    cv::Rect theTrack;
    bool tracker_failed = false;
//    cv::namedWindow("Tracker Display", cv::WINDOW_NORMAL);
    for(;;)
    {
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();
        if (abort) {
            qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
            break;
        }
        //add your code here
        cv::Mat image ;
        capture >> image;
        int width = image.cols;
        int height = image.rows;
        if (image.empty())
        {
            break;
        }
        // Initialize/Update the tracker
        if (!is_tracker_initialized)
        {
            // Initialize the tracker
            if (!tracker.initialize(image, init_bb))
            {
                // If it didn't work for some reason, exit now
                std::cerr << "\n\nCould not initialize the tracker!  Exiting early...\n" << std::endl;
                break;
            }
            // Store the track for display
            theTrack = init_bb;
            tracker_failed = false;
            // Now it's initialized
            is_tracker_initialized = true;
            std::cout << std::endl;
            continue;
        }
        else
        {
            // Update the tracker
            if (!tracker.update(image, theTrack))
            {
                //        std::cerr << "\rCould not update tracker (" << frame << ")";
                tracker_failed = true;
            }
            else
            {
                tracker_failed = false;
            }
        }
        // Display the tracking box
        CvScalar box_color;
        if (tracker_failed)
        {
            box_color = cv::Scalar(255, 0, 0);
        }
        else
        {
            box_color = cv::Scalar(255, 255, 0);
        }
        cv::rectangle(image, cvPoint(theTrack.x, theTrack.y),
                      cvPoint(theTrack.x + theTrack.width - 1, theTrack.y + theTrack.height - 1), box_color, 2);
        // Display the new image
        Mat result;
        cv::resize (image,result,Size(width,height));
//        cv::imshow("result Display", result);
//        // Check if the user wants to exit early
//        int key = cv::waitKey(10);
//        if (key == 'q' || key == 'Q')
//        {
//            break;
//            //        return 0;
//        }
        Mat lableimage ;
        result.copyTo (lableimage);
        emit imageChanged (lableimage);
    }
    mutex.lock();
    _working = false;
    mutex.unlock();
    qDebug()<<"Worker process finished in Thread "<<thread()->currentThreadId();
    emit finished();
}