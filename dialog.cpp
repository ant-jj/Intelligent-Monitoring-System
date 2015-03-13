#include "dialog.h"
#include "ui_dialog.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>

static bool stable2;
static QList<QString> imagelist;
static int carframe=0;
static Mat writetoavi;

TrackType currentTrackType = People;
/**
 * @brief Dialog::workerAndThreadInit
 */
void Dialog::workerAndThreadInit()
/*
 *  链接worker和thread
 */
{
    qRegisterMetaType< cv::Mat >("cv::Mat");
    qRegisterMetaType< cv::Rect >("cv::Rect");
    connect(this, SIGNAL(UpdateImageOnLabel(const cv::Mat &)), this,
            SLOT(selfupdateLabel(const cv::Mat &)));
    //tracker  particle
    thread = new QThread();
    worker = new Worker();
    worker->moveToThread(thread);
//    qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(worker, SIGNAL(imageChanged(const cv::Mat &)), this,
            SLOT(updateImage(const cv::Mat &)));
    connect(worker, SIGNAL(workRequested()), thread, SLOT(start()));
    connect(thread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);

    //tracker ct
    ctthread = new QThread();
    ctworker = new CTWorker();
    ctworker->moveToThread(ctthread);
//    qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(ctworker, SIGNAL(imageChanged(const cv::Mat &)), this,
            SLOT(updateImage(const cv::Mat &)));
    connect(ctworker, SIGNAL(workRequested()), ctthread, SLOT(start()));
    connect(ctthread, SIGNAL(started()), ctworker, SLOT(doWork()));
    connect(ctworker, SIGNAL(finished()), ctthread,
            SLOT(quit()), Qt::DirectConnection);

    //networkworker
    networkthread = new QThread();
    networkworker = new netWorkWorker();
    networkworker->moveToThread (networkthread);
//    qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(networkworker, SIGNAL(imageChanged(const cv::Mat &)), this, SLOT(netWorkupdateImage(const cv::Mat &)));
    connect(networkworker, SIGNAL(workRequested()), networkthread, SLOT(start()));
    connect(networkthread, SIGNAL(started()), networkworker, SLOT(doWork()));
    connect(networkworker, SIGNAL(finished()), networkthread, SLOT(quit()), Qt::DirectConnection);

    milthread = new QThread();
    milworker = new MilWorker();
    milworker->moveToThread (milthread);
//    qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(milworker, SIGNAL(imageChanged(const cv::Mat &)), this, SLOT(updateImage(const cv::Mat &)));
    connect(milworker, SIGNAL(workRequested()), milthread, SLOT(start()));
    connect(milthread, SIGNAL(started()), milworker, SLOT(doWork()));
    connect(milworker, SIGNAL(finished()), milthread, SLOT(quit()), Qt::DirectConnection);

    trackerrectinitthread = new QThread();
    detectfirstframeobjecttoinitrectworker = new DetectFirstFrameObjectToInitRectWorker();
    detectfirstframeobjecttoinitrectworker->moveToThread (trackerrectinitthread);
//    qRegisterMetaType< cv::Rect >("cv::Rect");
    connect(detectfirstframeobjecttoinitrectworker, SIGNAL(RectInited(const cv::Rect &)), this, SLOT(setTrackerFirstFrameRect(const cv::Rect &)));
    connect(detectfirstframeobjecttoinitrectworker, SIGNAL(workRequested()), trackerrectinitthread, SLOT(start()));
    connect(trackerrectinitthread, SIGNAL(started()), detectfirstframeobjecttoinitrectworker, SLOT(doWork()));
    connect(detectfirstframeobjecttoinitrectworker, SIGNAL(finished()), trackerrectinitthread, SLOT(quit()), Qt::DirectConnection);

}
Dialog::Dialog(QWidget *parent) :carspeedflag(false),capture(NULL),resize_factor(100),bgscarspeed(new PixelBasedAdaptiveSegmenter),vehicleCouting(new VehicleCouting),blobTracking(NULL),IP("10.104.0.76"),PORT(3200),videoSize(Size(320,240)),hogflag(false),multiflag(false),useseq(false),carnum(0),havepeople(false),socket(NULL),ctFlag(false),particleFlag(false),milFlag(false),
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //设置窗体为无边框
    this->setWindowFlags(Qt::FramelessWindowHint);

//    设置底层背景透明
//    setAttribute(Qt::WA_TranslucentBackground);
    init();
    initUi();
    workerAndThreadInit();
}
/**
 * @brief Dialog::~Dialog
 */
Dialog::~Dialog()
{
    //release particle tracker
    worker->abort();
    thread->wait();
    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    delete ctthread;
    delete worker;

    //release ct tracker
    ctworker->abort();
    thread->wait();
    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    delete thread;
    delete ctworker;

    //release networkworker
    networkworker->abort();
    networkthread->wait();
    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    delete networkthread;
    delete networkworker;

    //release mil  tracker
    milworker->abort();
    milthread->wait();
    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    delete milthread;
    delete milworker;

    detectfirstframeobjecttoinitrectworker->abort();
    trackerrectinitthread->wait();
    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    delete trackerrectinitthread;
    delete detectfirstframeobjecttoinitrectworker;

    delete ui;
}
/**
 * @brief Dialog::getFrame
 */
void Dialog::getFrame()
/* 本地视频检测
 *          不发送到手机端
 */
{
       //get local avi file for hog detect and multi object detect
    if(uselocal){
        Mat localimageforhog;
        localhogavi.read(localimageforhog);
        if(localimageforhog.empty())
            return;
        setlablepic(ui->labelrecv,localimageforhog);
        if(multiflag){
            Mat showmulti = multitrack(localimageforhog);
            setlablepic(ui->labelmult,showmulti);
        }
        if(hogflag){
            Mat peopleimage=hogpeople(localimageforhog);
            setlablepic(ui->labelperson,peopleimage);
            if(havepeople){
                ui->textEdit->setText ("检测结果：有人");

            }else{
                ui->textEdit->setText ("检测结果：无人");
            }
        }
        waitKey(10);
        return;
    }
}
/**
 * @brief Dialog::getlocalFrame
 */
void Dialog::getlocalFrame()
/*
 * 车辆检测用
 */
{
    int key = 0;
    QString imagename;
    if (carframe < imagelist.size()){
        imagename = imagelist.at(carframe);
    }
    Mat img_input =imread(imagename.toStdString().c_str());
    rectangle(img_input, Point( 0, 120 ), Point( 320, 170),Scalar( 0, 255, 255 ), 1, 8, 0);
    cv::Mat img_mask;
    cv::Mat img_bkgmodel;
    bgs->process(img_input, img_mask, img_bkgmodel);
    Mat submask = img_mask(rect);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8, 8));
    cv::erode(submask,submask,element);
    cv::dilate(submask,submask,element);
    int rowsum=0;
    for (int jj=0;jj<submask.cols;jj++)
    {
        if (submask.at<uchar>(25,jj)>125)
        {
            rowsum +=1;
        }
    }
    if (rowsum<17)
    {
        carnum +=0;
    }else
    {
        if (rowsum<40)
        {
            carnum+=1;
        }else
        {
            if(rowsum<200)
            {
                carnum +=2;
            }
        }
    }
    //int2string
    QString text = QString::number(carnum);
    int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    double fontScale = 1;
    int thickness = 2;
    cv::Point textOrg(0, 50);
    cv::putText(img_input, text.toStdString().c_str(), textOrg, fontFace, fontScale, Scalar(255,0,0,0), thickness,8);
    setlablepic(ui->labelcar,img_input);
    carframe++;
}
/**
 * @brief Dialog::setlablepic
 * @param lable ui中的lable 使用image显示
 * @param image 用于显示的Mat图像
 */
void Dialog::setlablepic(QLabel * lable, Mat image)
{
    QImage labelimage= QImage((uchar*) image.data, image.cols, image.rows, image.step, QImage::Format_RGB888).rgbSwapped();
    lable->setPixmap(QPixmap::fromImage(labelimage));
}
//set image show in dialog  AutoRefresh
void Dialog::setLablePicAutoRefresh(QLabel * lable, Mat image){
    QImage labelimage= QImage((uchar*) image.data, image.cols, image.rows, image.step, QImage::Format_RGB888).rgbSwapped();
    lable->setPixmap(QPixmap::fromImage(labelimage));
    lable->repaint();

}
/**
 * @brief Dialog::hogpeople   HOG检测函数，返回含有行人框的图像
 * @param image 对image进行HOG检测
 * @return 返回HOG检测后的图像
 */
Mat Dialog::hogpeople(Mat image)
{
    Mat tempimg;
    image.copyTo(tempimg);
    vector<Rect> found;
    HOGDescriptor defaultHog;
    defaultHog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    defaultHog.detectMultiScale(tempimg, found);
    if(found.size()>0){
        havepeople=true;
    }else{
        havepeople=false;
    }
    for(int i = 0; i < found.size(); i++)
    {
        Rect r = found[i];
        rectangle(tempimg, r.tl(), r.br(), Scalar(0, 255, 0), 2);//top left     bottom right
    }
    return tempimg;
}
/**
 * @brief Dialog::init
 */
void Dialog::init(){
    /*
    //server = new MyTcpServer();
    //初始化 hog 行人检测 init
    //初始化多目标 multi object init
    */
    MHI_DURATION = 1;   //0.5s
    N = 3;
    CONTOUR_MAX_AERA = 1000;
    buf = 0;
    last = 0;
    mhi = 0; // MHI: motion history image
    motion = 0;
    //初始化车辆统计 方式一，图像序列，背景减除
    carnum =0;
    rect = Rect(0,120,320,50);
    bgs = new MixtureOfGaussianV2BGS;
    //mkdir config   QDir::mkdir
    //config 位背景减除的配置文件夹，不存在就new
    QDir dir("config");
    if (!dir.exists()){
        qWarning("Cannot find the config directory");
        dir.mkdir(".");
    }
    //write avi to hdd
    writer.open("result.avi",CV_FOURCC('D','I','V','X'),15,videoSize);
}
/**
 * @brief Dialog::initUi
 */
void Dialog::initUi()
/*
 * 用于初始化UI界面
 */
{
    //set Controls
    /*
    ui->labelrecv->setText("");
    ui->labelmult->setText("");
    ui->labelcar->setText("");
    ui->labelperson->setText("");
    ui->labelcarspeed->setText ("");
    */

    //set icon
    /*
    ui->tabWidget_2->setTabIcon (0,QIcon(":/imgs/rc/images/settings.ico"));
    ui->tabWidget_2->setTabIcon (1,QIcon(":/imgs/rc/images/settings.ico"));
    ui->tabWidget_2->setTabIcon (2,QIcon(":/imgs/rc/images/phone.ico"));
    */
}
/**
 * @brief Dialog::update_mhi
 * @param img
 * @param dst
 */
void Dialog::update_mhi(IplImage *img, IplImage *dst)
/*
 * 用于多目标检测
 */
{
    double timestamp = clock()/100.;
    CvSize size = cvSize(img->width,img->height);
    int i, idx1, idx2;
    IplImage* silh;
    IplImage* pyr = cvCreateImage( cvSize((size.width & -2)/2, (size.height & -2)/2), 8, 1 );
    CvMemStorage *stor;
    CvSeq *cont;
    if( !mhi || mhi->width != size.width || mhi->height != size.height )
    {
        if( buf == 0 )
        {
            buf = (IplImage**)malloc(N*sizeof(buf[0]));
            memset( buf, 0, N*sizeof(buf[0]));
        }
        for( i = 0; i < N; i++ )
        {
            cvReleaseImage( &buf[i] );
            buf[i] = cvCreateImage( size, IPL_DEPTH_8U, 1 );
            cvZero( buf[i] );// clear Buffer Frame at the beginning
        }
        cvReleaseImage( &mhi );
        mhi = cvCreateImage( size, IPL_DEPTH_32F, 1 );
        cvZero( mhi ); // clear MHI at the beginning
    }
    cvCvtColor( img, buf[last], CV_BGR2GRAY ); // convert frame to grayscale
    idx1 = last;
    idx2 = (last + 1) % N; // index of (last - (N-1))th frame
    last = idx2;
    silh = buf[idx2];
    cvAbsDiff( buf[idx1], buf[idx2], silh ); // get difference between frames
    cvThreshold( silh, silh, 50, 255, CV_THRESH_BINARY );
    cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION ); // update MHI
    cvConvert( mhi, dst );
    cvSmooth( dst, dst, CV_MEDIAN, 3, 0, 0, 0 );
    cvPyrDown( dst, pyr, CV_GAUSSIAN_5x5 );
    cvDilate( pyr, pyr, 0, 1 );
    cvPyrUp( pyr, dst, CV_GAUSSIAN_5x5 );
    stor = cvCreateMemStorage(0);
    cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);
    cvFindContours( dst, stor, &cont, sizeof(CvContour),
                    CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    for(;cont;cont = cont->h_next)
    {
        CvRect r = ((CvContour*)cont)->rect;
        if(r.height * r.width > CONTOUR_MAX_AERA)
        {
            cvRectangle( img, cvPoint(r.x,r.y),
                         cvPoint(r.x + r.width, r.y + r.height),
                         CV_RGB(255,0,0), 1, CV_AA,0);
            writetoavi = Mat(img,false);
            writer<<writetoavi;
        }
    }
    cvReleaseMemStorage(&stor);
    cvReleaseImage( &pyr );
}

Mat Dialog::multitrack(Mat image)
{
    IplImage* img = new IplImage(image);
    if( !motion )
    {
        motion = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
        cvZero( motion );
        motion->origin = img->origin;
    }
    update_mhi( img, motion);
    Mat multiimage = Mat(img,false);
    return multiimage;
}
/**
 * @brief Dialog::carSpeedFileNameSetProcess
 */
void Dialog::carSpeedFileNameSetProcess()
/*
 * 车辆设置2 的处理
 */

{
    if(capture == NULL){
        capture = cvCaptureFromAVI(AVIfilename.toStdString().c_str());
    }
    frame_aux = cvQueryFrame(capture);
    if(frame_aux == NULL){
        disconnect(timerforcarspeed, SIGNAL(timeout()), this, SLOT(carSpeedFileNameSetProcess()));
        return;
    }
    frame = cvCreateImage(cvSize((int)((frame_aux->width*resize_factor)/100) , (int)((frame_aux->height*resize_factor)/100)), frame_aux->depth, frame_aux->nChannels);
    if(blobTracking==NULL)
        blobTracking = new BlobTracking;
    /* Vehicle Counting Algorithm */
    //    VehicleCouting* vehicleCouting;
    //    vehicleCouting = new VehicleCouting;
    int key = 0;
    if(key != 'q' )
    {
        frame_aux = cvQueryFrame(capture);
        if(!frame_aux)
        {
            disconnect(timerforcarspeed, SIGNAL(timeout()), this, SLOT(carSpeedFileNameSetProcess()));
            return;
        }
        cvResize(frame_aux, frame);
        cv::Mat img_input(frame);
        cv::Mat img_mask;
        cv::Mat img_background;
        bgscarspeed->process(img_input, img_mask,img_background);
        if(!img_mask.empty())
        {
            //  Perform blob tracking
            blobTracking->process(img_input, img_mask, img_blob);
            // cv:imshow("img_blob",img_blob);
            // Perform vehicle counting
            vehicleCouting->setInput(img_blob);
            vehicleCouting->setTracks(blobTracking->getTracks());
            vehicleCouting->process();

            vector<IdSpeed> idspeeds=vehicleCouting->getIdspeeds();
            //            IdSpeed is;
            if(idspeeds.size()>0){
                QString carspeedlabel;
                for(int i=0;i<idspeeds.size();i++){
                    IdSpeed is1=idspeeds.at(i);
                    std::cout<<"Id: "<<is1.id<<" Speed: "<<is1.speed<<std::endl;
                    std::cout<<"-------------------------------------"<<std::endl;
                    carspeedlabel +="ID:"+QString::number(is1.id)+" SPEED: "+QString::number(is1.speed)+" ; ";
                }
                //显示在message 区域
                ui->textEdit->append (carspeedlabel);
                //                IdSpeed is1=idspeeds.at(0);
                //                std::cout<<"Id: "<<is1.id<<" Speed: "<<is1.speed<<std::endl;
            }
            Mat carspeed =vehicleCouting->getInput();
            setLablePicAutoRefresh(ui->labelcarspeed,carspeed);
            //            imshow("carspeed",carspeed);
        }
        key = cvWaitKey(1);
    }
}
/**
 * @brief Dialog::on_bindIP_clicked
 */
void Dialog::on_bindIP_clicked()
/*
 *手机 panel 设置
 *
 */
{
    firstpanel = new firstPanelDialog();
    firstpanel->exec ();

    this->IP = firstpanel->ipaddr;
    this->PORT =firstpanel->port;

    if(socket == NULL){
        socket = new QTcpSocket(this);
        connect(socket, SIGNAL(connected()),this, SLOT(connected()));
        connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
        connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
        connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
        qRegisterMetaType< cv::Mat >("cv::Mat");
        connect(this, SIGNAL(sendtoandroid(const cv::Mat &)),this, SLOT(sendtoandroidprocess(const cv::Mat &)));
    }
    if((this->IP).replace (" ","").compare ("")==0){
        /*
        ui->bindIP->setText ("手机设置OK");
        ui->bindIP->setStyleSheet("background-color: red");
        */
        QMessageBox::information (this,"your setting"," setting false !");
    }
    QMessageBox::information (this,"your setting"," setting success !");

}
/**
 * @brief Dialog::connected
 */
void Dialog::connected()
/*
 * 用于向android 手机端发送警报图像
 */
{
    //    socket->write("some body come in");
    //    socket->close();
    qDebug()<<"connected....";
}
/**
 * @brief Dialog::disconnected
 */
void Dialog::disconnected()
{
    qDebug() << "disconnected...";
}
/**
 * @brief Dialog::bytesWritten
 * @param bytes
 */
void Dialog::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}
/**
 * @brief Dialog::readyRead
 */
void Dialog::readyRead()
{
    /*
    qDebug() << "reading...";
    qDebug() << socket->readAll();
    */
}
/**
 * @brief Dialog::selfupdateLabel
 * @param img
 */
void Dialog::selfupdateLabel(const Mat &img)
{
    setLablePicAutoRefresh(ui->labelrecv,img);
    setLablePicAutoRefresh (ui->labelperson,img);
}
/**
 * @brief Dialog::setTrackerFirstFrameRect
 * @param rect
 */
void Dialog::setTrackerFirstFrameRect(const Rect &rect)
{
    QString message;
    QTextStream(&message)<<"<font color= red >Init Rect:</font>"<<" x="<<rect.x <<" y="<<rect.y <<
                        " width="<<rect.width <<" height= " <<rect.height;
    ui->textEdit->append (message);
    /*
     * 初始化各个跟踪worker的第一帧Rect
     */
    this->ctworker->initRect =rect;
    this->milworker->initRect =rect;
    this->worker->initRect =rect;
}
/**
 * @brief Dialog::sendtoandroidprocess
 * @param img
 */
void Dialog::sendtoandroidprocess(const Mat &img)
/*
 * 无压缩（jpeg）传输图像到android手机端
 * 尚未考虑压缩传输（6-10倍的传输性能差距），因为手机Java代码部分得修改
 */
{
    qDebug() << "TO android : connecting...";
    if(socket->isOpen()){
        qDebug()<<"already opened ,wait please...";
        return;
    }
    socket->connectToHost(IP.toStdString().c_str(), PORT);
    Mat sentframe = img.reshape(0,1);
    if(sentframe.empty()) return;
    std::string message((char *)sentframe.data,230400);
    socket->write(message.c_str(),230400);
    socket->close();
    qDebug()<<"connected and image send finished...";
}

/**
 * @brief Dialog::updateImage
 * @param img
 */
void Dialog::updateImage(const Mat &img)
/*
 * 用于跟踪算法显示跟踪效果的label
 *
 */
{
    QImage labelimage= QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888).rgbSwapped();
    ui->label_Tracker_Show->setPixmap(QPixmap::fromImage(labelimage));
    ui->label_Tracker_Show->repaint ();
}
/**
 * @brief Dialog::netWorkupdateImage
 * @param img
 */
void Dialog::netWorkupdateImage(const Mat &img)
/*
 * 用于处理网络传输来的图像的slot
 * 实现HOG或多目标检测
 * 只有在处理来至网络的视频时候，检测到行人才有发到手机端
 */
{
    //do the HOG job here
    Mat peopleimage;
    Mat showmulti;
    if(img.empty())
        return;
    if(multiflag){
        showmulti = multitrack(img);
//        setlablepic(ui->labelmult,showmulti);
    }
    if(hogflag){
        peopleimage=hogpeople(img);
        /*
        Mat temp;
        peopleimage.copyTo (temp);
        emit UpdateImageOnLabel(temp);
        */
        setLablePicAutoRefresh(ui->labelrecv,img);
        setLablePicAutoRefresh (ui->labelperson,peopleimage);
        if(socket->isOpen()){
            qDebug()<<"already opened ,wait please...";
            return;
        }
        if(havepeople){
            emit sendtoandroid(peopleimage);
        }
        //                    writetoavi = Mat(peopleimage,false);
        if(havepeople){
            ui->textEdit->setText ("检测结果：有人");

        }else{
            ui->textEdit->setText ("检测结果：无人");
        }
    }
/*
    imshow("HOG result",peopleimage);
    imshow("network worker recieved",img);
    waitKey (10);
*/
}
/**
 * @brief Dialog::on_pushButton_start_tracker_clicked
 */
void Dialog::on_pushButton_start_tracker_clicked()
/*
 * 开始使用跟踪算法：
 * 目前已经实现：
 * 1.粒子滤波
 * 2.CT压缩感知滤波
 * 3.Mil boostring
 *
 */
{
    /*
     * 第一帧Rect初始化
     *
     */
    detectfirstframeobjecttoinitrectworker->abort();
    trackerrectinitthread->wait(); // If the thread is not running, this will immediately return.
    detectfirstframeobjecttoinitrectworker->requestWork();

    if(this->ctFlag){
        ctworker->abort();
        ctthread->wait(); // If the thread is not running, this will immediately return.
        ctworker->requestWork();

    }
    if(this->particleFlag){
        worker->abort();
        thread->wait(); // If the thread is not running, this will immediately return.
        worker->requestWork();
    }
    if(this->milFlag){
        milworker->abort ();
        milthread->wait ();
        milworker->requestWork ();
    }
}
/**
 * @brief Dialog::on_pushButton_set_file2track_clicked
 */
void Dialog::on_pushButton_set_file2track_clicked()
/*
 * 设置用于跟踪的视频文件
 */
{
    //set file2track  var
     QString qfile2track = QFileDialog::getOpenFileName(this,tr("Choose Avi File"), "/home/testtrack.avi", tr("vedio files (*.*)"));
     if (qfile2track == NULL)
     {

         QMessageBox::warning(this, tr("error"),
                                        tr("error choose a avi file"),
                                        QMessageBox::Ok);
         return;
     }
     worker->file2Track = qfile2track.toStdString();
     ctworker->file2Track = qfile2track.toStdString();
     milworker->file2Track = qfile2track.toStdString();
//     qDebug()<<"you select avi file named: "<< qfile2track;

}
//panel 2
// set the image data source panel
/**
 * @brief Dialog::on_pushButton_set_data_source_clicked
 */
void Dialog::on_pushButton_set_data_source_clicked()
/*
 * 设置图像获取源
 * 1，来自网络
 * 2.来自本地视频文件
 * 3.来自本地图像序列
 */
{

    datasourcepanel = new DataSourceDialog(this);
    datasourcepanel->exec ();

    this->uselocal = datasourcepanel->uselocal ;
    this->useseq =datasourcepanel->useseq;

    if(uselocal){
        //use local video file
        this->globalFileName = datasourcepanel->localFileName;
        localhogavi = VideoCapture(globalFileName.toStdString().c_str());

    }

    if(useseq){
        this->globalDirName = datasourcepanel->localSeqDirName;
    }
    //use remote image
    /*
     * 以下为老方法，不使用 networkworker 子线程，而用QT网络的信号机制        connect(server,SIGNAL(buildimage()),this,SLOT(getFrame()));
     */
}

//set the detect kind panel
/**
 * @brief Dialog::on_pushButton_set_detec_method_clicked
 */
void Dialog::on_pushButton_set_detec_method_clicked()
/*
 * 检测方法设置：
 *      1.HOG
 *      2.多目标
 */
{
    detectmethodpanel= new DetectMethodDialog(this);
    detectmethodpanel->exec ();
    multiflag = detectmethodpanel->multidetect;
    hogflag =detectmethodpanel->hogdetect;

}


/**
 * @brief Dialog::on_pushButton_apply_detect_clicked
 */
void Dialog::on_pushButton_apply_detect_clicked()
/*
 * 应用设置 APPLY
 */
{
    if(uselocal){
        localhogtimer = new QTimer(this);
        connect(localhogtimer,SIGNAL(timeout()),this,SLOT(getFrame()));
        localhogtimer->start(50);
    }else{
        /*
         * 获取网络图像：
         * 使用networkworker子线程
         */
        qDebug()<<"networkworker >> get video from network... ... ";
        networkworker->abort();
        networkthread->wait(); // If the thread is not running, this will immediately return.
        networkworker->requestWork();
    }
}
/**
 * @brief Dialog::on_pushButton_car_setting_panel_clicked
 */
void Dialog::on_pushButton_car_setting_panel_clicked()
/*
 * 设置车辆检测面板
 */
{
    carsettingpanel = new CarSettingPanelDialog(this);
    carsettingpanel->exec ();
    dir = carsettingpanel->dirname;
    fileName=carsettingpanel->txtfilename;
    AVIfilename = carsettingpanel->videofilename;

    if(AVIfilename != ""){
        /*
        ui->message->setText(AVIfilename);
        */
        ui->textEdit->append ("the local file you selected:"+AVIfilename);
        timerforcarspeed = new QTimer(this);
        connect(timerforcarspeed, SIGNAL(timeout()), this, SLOT(carSpeedFileNameSetProcess()));
        timerforcarspeed->start(10);
        return;
    }

    if(fileName !=""){
        static QFile datalist(fileName);
        if (datalist.open(QIODevice::ReadOnly))
        {
            QTextStream in(&datalist);
            while ( !in.atEnd() )
            {
                QString line = in.readLine();
                imagelist<<(dir+'/'+line);
            }
            datalist.close();
        }
        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(getlocalFrame()));
        timer->start(10);

    }
}
/**
 * @brief Dialog::on_pushButton_test_function_clicked
 */
void Dialog::on_pushButton_test_function_clicked()
/*
 * 增加新功能时的测试按钮
 */
{
//    ui->textEdit->append ("haha\nlala");
//    scene1 = new FormScene1;
//    scene1->show ();
}
/**
 * @brief Dialog::on_pushButton_tracker_panel_clicked
 */
void Dialog::on_pushButton_tracker_panel_clicked()
/*
 * 跟踪算法设置面板
 * 1.Particle Filter
 * 2.Compress TrackingAlgorithm
 * 3.Mil TrackingAlgorithm
 * 4.
 */
{
    trakerselectpanel = new TrakerSelectDialog(this);
    trakerselectpanel->exec ();
    this->ctFlag = trakerselectpanel->ctFlag;
    this->particleFlag = trakerselectpanel->particleFlag;
    this->milFlag = trakerselectpanel->milFlag;
}
/**
 * @brief Dialog::on_pushButton_selectWhatToTrack_clicked
 */
void Dialog::on_pushButton_selectWhatToTrack_clicked()
/*
 * 选择跟踪的目标
 *  1.行人
 *  2.车辆
 *  3.足球
 *  4.自己鼠标选择
 */
{
    whattotrackselectpanel = new WhatToTrackSelectDialog(this);
    whattotrackselectpanel->exec ();
    currentTrackType = whattotrackselectpanel->currentTrackType;
    if(currentTrackType == SelectMySelf)
    /*
     * 使用鼠标选取跟踪目标
     */
    {
        this->ctworker->initWithMouse = true;
        this->milworker->initWithMouse = true;
        this->worker->initWithMouse = true;

    }else{
        this->ctworker->initWithMouse = false;
        this->milworker->initWithMouse = false;
        this->worker->initWithMouse = false;
    }

    this->detectfirstframeobjecttoinitrectworker->tracktype = currentTrackType;

}
/**
 * @brief Dialog::on_pushButton_firstFrameDetectMethod_clicked
 */
void Dialog::on_pushButton_firstFrameDetectMethod_clicked()
/*
 * 设置跟踪算法中的第一帧图形的初始Rect
 */
{
    firstframedetectmethodssetpanel = new  FirstFrameDetectMethodsSetDialog(this);
    firstframedetectmethodssetpanel->exec ();

    this->trackmethods = firstframedetectmethodssetpanel->trackmethods;

    if(trackmethods == HOG){
        detectfirstframeobjecttoinitrectworker->trackmethods = HOG;
    }
    if(trackmethods == HAAR){
        detectfirstframeobjecttoinitrectworker->trackmethods = HAAR;
    }
    if(trackmethods == LBP){
        detectfirstframeobjecttoinitrectworker->trackmethods = LBP;
    }

}




//++++++++++++++++++++++++ 可拖拽无边框 +++++++++++++++++++++++
/**
 * @brief Dialog::on_pushButton_EXIT_SYSTEM_clicked
 * 退出程序
 */
void Dialog::on_pushButton_EXIT_SYSTEM_clicked()
{
    exit(0);
}
void Dialog::mousePressEvent(QMouseEvent *event)
{
    bool shouldMove=isPointInDragnWidget(getDragnWidget(),event->pos());
       if(shouldMove){
             pressedPoint=event->pos();
             isMove=true;
       }
       event->ignore();

}
void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons()==Qt::LeftButton) && isMove==true){
               QPoint currPoint=this->pos();
               currPoint.setX(currPoint.x()+event->x()-pressedPoint.x());
               currPoint.setY(currPoint.y()+event->y()-pressedPoint.y());
               this->move(currPoint);
        }
}
void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    isMove=false;
}
bool Dialog::isPointInDragnWidget(const QWidget *widget, const QPoint &point)
{
    //判断位置
      QRect rect=widget->rect();
      bool isIn=point.x()>=rect.left()&&point.x()<=rect.right()&&point.y()>=rect.top()&&point.y()<=rect.bottom();
      return isIn;
}
QWidget *Dialog::getDragnWidget()
{
    //当鼠标落在widget_drag中时，可以使用拖拽无边框
    return ui->widget_drag;
}

//----------------------------------------------------------------


//++++++++++++++++++++++++场景设计++++++++++++++++++++++++
void Dialog::on_Scene1_clicked()
{
    scene1 = new FormScene1;
    scene1->show ();
}

void Dialog::on_Scene2_clicked()
{
    scene2 = new FormScene2;
    scene2->show ();
}

void Dialog::on_Scene3_clicked()
{
    scene3 = new FormScene3;
    scene3->show ();
}
//---------------------------------------------------------
