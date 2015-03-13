#include "formscene1.h"
#include "ui_formscene1.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "lib/parameters.h"


using namespace cv;
using namespace std;


FormScene1::FormScene1(QWidget *parent) :
    MoveableFramelessWindow(parent),
    ui(new Ui::FormScene1)
{
    ui->setupUi(this);
}

FormScene1::~FormScene1()
{
    delete ui;
}

QWidget *FormScene1::getDragnWidget()
{
    return ui->widget_drag;
}

void FormScene1::showConfigTest()
{
    std::string configfilename = std::string ("config/sample.conf" );
    Parameters hp ( configfilename );
    std::string seq_name = hp.readStringParameter("Tracker.Name");
    float maxScale = static_cast<float>(hp.readDoubleParameter("Tracker.maxScale"));
    Rect object;
    if(hp.settingExists("Tracker.startRegion"))
        object = hp.readRectParameter("Tracker.startRegion");
    else
        object = cv::Rect(0,0,0,0);
    int increment =static_cast<int>(hp.readIntParameter ("Input.increment"));
    cout<<"seq_name: "<<seq_name<<endl;
    cout<<"maxScale: "<<maxScale<<endl;
    cout<<"Rect: "<<object.x<<","<<object.y<<","<<object.height<<","<<object.width<<endl;
    cout<<"Input Parameter**************************"<<endl;
    cout<<"increment: "<<increment<<endl;

}

void FormScene1::on_pushButton_clicked()
{
    this->showConfigTest ();
    this->close ();
}
