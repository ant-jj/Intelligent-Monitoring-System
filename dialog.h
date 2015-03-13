#ifndef DIALOG_H
#define DIALOG_H

//include qt head

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QTimer>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPoint>
#include <QRect>
#include <QDebug>
#include <QThread>
#include <QDir>
#include <QIcon>
#include <QTcpSocket>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <vector>

//include cpp head

#include <cstdlib>
#include <string>

//include opencv head
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "tracktype.h"
//include my head
//#include "mytcpserver.h"

//background substract
#include "package_bgs/MixtureOfGaussianV2BGS.h"
#include "package_bgs/FrameDifferenceBGS.h"
#include "package_bgs/pt/PixelBasedAdaptiveSegmenter.h"

//blob tracking
#include "package_analysis/VehicleCouting.h"
#include "package_tracking/BlobTracking.h"


//include workers


#include "worker/worker.h"
#include "worker/ctworker.h"
#include "worker/networkworker.h"
#include "worker/milworker.h"
#include "worker/detectfirstframeobjecttoinitrectworker.h"


//include panels
#include "panel/firstpaneldialog.h" //手机报警网络端口设置
#include "panel/datasourcedialog.h"
#include "panel/detectmethoddialog.h"
#include "panel/carsettingpaneldialog.h"
#include "panel/trakerselectdialog.h"
#include "panel/whattotrackselectdialog.h"
#include "panel/firstframedetectmethodssetdialog.h"


//scene
#include "scene/scene1/formscene1.h"
#include "scene/scene2/formscene2.h"
#include "scene/scene3/formscene3.h"

using namespace cv;
using namespace std;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
//    Mat showimg;                                /* 可以去掉 */
//    MyTcpServer *server;
    /* 服务器端 */
    /*
     * hogpeople >> HOG检测
     */
    Mat hogpeople(Mat image);

    void setlablepic(QLabel *lable, Mat image);                 //label显示Mat图像
    void setLablePicAutoRefresh(QLabel * lable, Mat image);
    bool hogflag;
    bool havepeople;

    void init();
    void initUi();

    //背景建模车辆统计使用
    IBGS *bgs;
    int carnum;
    Rect rect;
    QString dir;                      //车辆文件夹
    QString fileName;                 //datalist.txt
    Mat subImg;


    //多物体检测
    bool multiflag;
    double MHI_DURATION;
    int N;
    int CONTOUR_MAX_AERA;
    IplImage **buf;
    int last;
    IplImage *mhi;
    IplImage* motion;
    void update_mhi(IplImage* img, IplImage* dst);
    Mat multitrack(Mat image);
    IplImage* image;

    //车速检测
    bool carspeedflag;
    CvCapture *capture;
    int resize_factor;
    VehicleCouting* vehicleCouting;

    cv::Mat img_blob;
    BlobTracking* blobTracking;
    IplImage *frame_aux;
    IplImage *frame;
    QTimer *timerforcarspeed;
    IBGS *bgscarspeed;


    VideoCapture localhogavi;
    bool uselocal;
    bool useseq;

    //车速测量
    QString AVIfilename;

    void workerAndThreadInit();
signals:
    void getlocalFrameSignal(); //获取车辆图像
    void carSpeedFileNameSetSignal();
    void sendtoandroid(const cv::Mat &img);
    void UpdateImageOnLabel(const cv::Mat &img);

private:
    Ui::Dialog *ui;

    //检测入侵,写入视频和发消息到android
    VideoWriter writer; //摄像头每次抓取的图像为一帧，使用该指针指向一帧图像的内存空间
    Size videoSize;

    int PORT;
    QString IP;
    QTcpSocket *socket; //SEND IMG TO ANDROID

    //getlocalimage
    QTimer *timer;                              /* 定时器，更新界面 */
    QTimer *localhogtimer;                              /* 定时器，更新界面 */



    //global file name or seq dir name
    QString globalFileName;
    QString globalDirName;
    VideoCapture globalvideocaption;


    /*
particle tracker timer ----------> get image
*/
    QTimer *timer4tracker;


    QThread *thread;
    QThread *ctthread;
    QThread * networkthread;
    QThread * milthread;
    QThread * trackerrectinitthread;

    Worker *worker;
    CTWorker *ctworker;
    netWorkWorker *networkworker;
    MilWorker *milworker;
    DetectFirstFrameObjectToInitRectWorker *detectfirstframeobjecttoinitrectworker;

//    getNetFrameWorker *getnetframeworker;
    //各种基于对话框的控制面板设计
    firstPanelDialog *firstpanel;
    DataSourceDialog *datasourcepanel; //设置数据源面板
    DetectMethodDialog *detectmethodpanel;
    CarSettingPanelDialog *carsettingpanel;
    TrakerSelectDialog *trakerselectpanel;
    WhatToTrackSelectDialog *whattotrackselectpanel;
    FirstFrameDetectMethodsSetDialog * firstframedetectmethodssetpanel;

    //tracker flag

    bool ctFlag;
    bool particleFlag;
    bool milFlag;

    //设置跟踪时检测第一帧initRect
    TrackMethods trackmethods;

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void selfupdateLabel(const cv::Mat &img);
    void setTrackerFirstFrameRect(const cv::Rect &rect);

private slots:
    void getFrame();
    void getlocalFrame(); //车辆用
    void carSpeedFileNameSetProcess();
    void on_bindIP_clicked();
    void on_pushButton_start_tracker_clicked();
    void on_pushButton_set_file2track_clicked();

    void on_pushButton_set_data_source_clicked();
    void on_pushButton_set_detec_method_clicked();
    void on_pushButton_apply_detect_clicked();
    void sendtoandroidprocess(const cv::Mat &img);
    void updateImage(const cv::Mat &img);
    void netWorkupdateImage(const cv::Mat &img);

    void on_pushButton_car_setting_panel_clicked();
    void on_pushButton_test_function_clicked();
    void on_pushButton_tracker_panel_clicked();
    void on_pushButton_selectWhatToTrack_clicked();
    void on_pushButton_firstFrameDetectMethod_clicked();

    /**
     * @brief on_pushButton_EXIT_SYSTEM_clicked
     * 无边框时退出程序的按钮，结束程序
     */
    void on_pushButton_EXIT_SYSTEM_clicked();


    void on_Scene1_clicked();

    void on_Scene2_clicked();

    void on_Scene3_clicked();

protected:

    //+++++++++++++++++++++++++++   设置无边框可拖拽属性  BEGIN +++++++++++++++++
    /**
     * @brief 鼠标按下，准备拖动窗体
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);
    /**
    * @brief 鼠标移动，处理窗体拖动
    * @param event
    */
    void mouseMoveEvent(QMouseEvent *event);
    /**
    * @brief 释放鼠标
    * @param event
    */
    void mouseReleaseEvent(QMouseEvent *event);
    /**
     * @brief 获取可拖动控件，必须由子类指定
     * @return
     */
    QWidget*getDragnWidget();
    /**
     * @brief 判断鼠标点击的位置是否进入可拖动区域
     * @param widget 可拖动控件位置
     * @param point  鼠标点击位置
     * @return
     */
    bool isPointInDragnWidget(const QWidget*widget,const QPoint &point);
    /**
      * @brief 标志是否移动窗体
      */
    bool isMove;
    /**
       * @brief 鼠标按下去的点
       */
    QPoint pressedPoint;

    //-------------------------   设置无边框可拖拽属性 END ----------------------



public:
    FormScene1 *scene1;
    FormScene2 *scene2;
    FormScene3 *scene3;

};

#endif // DIALOG_H
