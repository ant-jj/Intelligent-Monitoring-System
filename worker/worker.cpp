/*
   Copyright 2013 Fabien Pierre-Nicolas.
      - Primarily authored by Fabien Pierre-Nicolas

   This file is part of simple-qt-thread-example, a simple example to demonstrate how to use threads.
   This example is explained on http://fabienpn.wordpress.com/qt-thread-simple-and-stable-with-sources/

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This progra is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "worker.h"
#include <QTimer>
#include <QEventLoop>

#include <QThread>
#include <QDebug>


typedef unsigned int uint;

const char* WINDOW  = "Particle Tracker";

const Scalar RED = Scalar(0, 0, 255);
const Scalar BLUE = Scalar(255, 0, 0);
const Scalar GREEN = Scalar(0, 255, 0);
const Scalar CYAN = Scalar(255, 255, 0);
const Scalar MAGENTA = Scalar(255, 0, 255);
const Scalar YELLOW = Scalar(0, 255, 255);
const Scalar WHITE = Scalar(255, 255, 255);
const Scalar BLACK = Scalar(0, 0, 0);

const uint NUM_PARTICLES = 200;

bool showimage =true;


inline void update_target_histogram(Mat& image, Mat& lbp_image, Rect& selection, Mat& histogram, Mat& target, bool use_lbp)
{
    Mat roi(image, selection), lbp_roi(lbp_image, selection);
    roi.copyTo(target);
    Mat new_hist;
    float alpha = 0.2;

    calc_hist(roi, lbp_roi, new_hist, use_lbp);
    normalize(new_hist, new_hist);

    if(histogram.empty())
    {
        histogram = new_hist;
    }
    else
    {
        // TODO - support for adaptive updates not fully implemented.
        histogram = ((1.f - alpha) * histogram) + (alpha * new_hist);
        normalize(histogram, histogram);
    }
    cout << "Target updated" << endl;
}

struct StateData
{
    StateData(int num_particles, bool use_lbp_):
        image(),
        lbp(),
        target(),
        target_histogram(),
        selector(WINDOW),
        selection(),
        use_lbp(use_lbp_),
        paused(false),
        draw_particles(false),
        filter(num_particles)
    {};
    Mat image;
    Mat lbp;
    Mat target;
    Mat target_histogram;
    Selector selector;
    Rect selection;
    bool use_lbp;
    bool paused;
    bool draw_particles;
    ParticleFilter filter;
};

State_ state_start(StateData& d)
{

    if( d.selector.selecting() )
    {
        cout << "state_selecting" << endl;

        return state_selecting;
    }
    else
    {
        return state_start;
    }
}

State_ state_selecting(StateData& d)
{
    if( d.selector.valid() )
    {
        cout << "state_initializing: (" << d.selection.x << ", " << d.selection.y << ", " << d.selection.width << ", " << d.selection.height  << ")" << endl;
        d.selection = d.selector.selection();
        cout << "selection: (" << d.selection.x << ", " << d.selection.y << ", " << d.selection.width << ", " << d.selection.height  << ")" << endl;

//        destroyAllWindows();

        return state_initializing(d); // Call with current frame
    }
    else
    {
        Mat roi(d.image, d.selector.selection());
        bitwise_not(roi, roi);
        return state_selecting;
    }
}

State_ state_initializing(StateData& d)
{
    if( d.selector.selecting() )
    {
        cout << "state_selecting" << endl;
        return state_selecting;
    }

    // Generate initial target histogram
    update_target_histogram(d.image, d.lbp, d.selection, d.target_histogram, d.target, d.use_lbp);

    // Initialize condensation filter with center of selection
    d.filter.init(d.selection);

    // Start video running if paused
    d.paused = true;

    cout << "state_tracking" << endl;
    return state_tracking(d); // Call with current frame
}

State_ state_tracking(StateData& d)
{


    if( d.selector.selecting() )
    {
        cout << "state_selecting" << endl;
        return state_selecting;
    }

    // Update particle filter
    d.filter.update(d.image, d.lbp, d.selection.size(), d.target_histogram, d.use_lbp);

    Size target_size(d.target.cols, d.target.rows);

    // Draw particles
    if( d.draw_particles )
        d.filter.draw_particles(d.image, target_size, WHITE);

    // Draw estimated state with color based on confidence
    float confidence = d.filter.confidence();

    // TODO - Make these values not arbitrary
    if( confidence > 0.1 )
    {
        d.filter.draw_estimated_state(d.image, target_size, GREEN);
    }
    else if( confidence > 0.025 )
    {
        d.filter.draw_estimated_state(d.image, target_size, YELLOW);
    }
    else
    {
        d.filter.draw_estimated_state(d.image, target_size, RED);
    }

    return state_tracking;
}

struct Options
{
    Options()
        :num_particles(NUM_PARTICLES),
          use_lbp(false),
          infile(),
          outfile()
    {}

    int num_particles;
    bool use_lbp;
    string infile;
    string outfile;
};

void parse_command_line(Options& o)
{
    o.num_particles=200;
//    o.infile = file2Track;
}


Worker::Worker(QObject *parent) :
    QObject(parent)
{
    _working =false;
    _abort = false;
}


void Worker::requestWork()
{
    mutex.lock();
    _working = true;
    _abort = false;
    qDebug()<<"Request worker start in Thread "<<thread()->currentThreadId();
    mutex.unlock();

    emit workRequested();
}

void Worker::abort()
{
    mutex.lock();
    if (_working) {
        _abort = true;
        qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    }
    mutex.unlock();
}

Rect Worker::DetectFirstFrameToInitRect(Mat &image, TrackType tracktype, TrackMethods trackmethod)
{

    return Rect(10,10,10,10);
}


void Worker::doWork()
{
    qDebug()<<"Starting worker process in Thread "<<thread()->currentThreadId();
    //init particle
    Options o;
    parse_command_line(o);
    o.infile=this->file2Track;
    VideoCapture cap(o.infile.c_str ());

//    if( !o.infile.empty() )
//    {
////        cap.open(o.infile);
//    }

    namedWindow(WINDOW, CV_WINDOW_FREERATIO | CV_GUI_NORMAL);
    StateData d(o.num_particles, o.use_lbp);
    State state = state_start;
    Mat frame, gray;

    lbp_init();

    for(;;)
    {

        mutex.lock();
        bool abort = _abort;
        mutex.unlock();
        if (abort) {
            qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
            break;
        }

        //if the particle checkbox is checked ,then start particle track



        // Capture frame
        if( !d.paused)
        {
            cap >> frame;
            if(frame.empty())
            {
                cerr << "Error reading frame" << endl;
                break;
            }
        }
        frame.copyTo(d.image);

        // Set up all the image formats we'll need
        if(d.use_lbp)
        {
            cvtColor(d.image, gray, CV_BGR2GRAY);
            lbp_from_gray(gray, d.lbp);
        }
        else
        {
            if( d.lbp.empty() )
                d.lbp = Mat::zeros(d.image.rows, d.image.cols, CV_8UC1);
        }
        // Handle keyboard input
        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch(c)
        {
        case 'p':
            d.paused = !d.paused;

            break;

        case 'c':
            cout << "Tracking cancelled." << endl;
            state = state_start;
            break;

        case 'd':
            d.draw_particles = !d.draw_particles;
            cout << "Draw particles: " << d.draw_particles << endl;
            break;
        }

        // Process frame in current state
        state = state(d);

        Mat target_display_area(d.image, Rect(d.image.cols - d.selection.width, 0, d.selection.width, d.selection.height));
        d.target.copyTo(target_display_area);
        if(showimage ){
            imshow(WINDOW, d.image);

        }

        Mat labelImg;
        d.image.copyTo (labelImg);

        emit imageChanged (labelImg);
    }

    mutex.lock();
    _working = false;
    mutex.unlock();

    qDebug()<<"Worker Track[ Particle Filter Tracker ] process finished in Thread "<<thread()->currentThreadId();

    emit finished();
}



