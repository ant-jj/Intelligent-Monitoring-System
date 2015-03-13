#include "whattotrackselectdialog.h"
#include "ui_whattotrackselectdialog.h"


WhatToTrackSelectDialog::WhatToTrackSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WhatToTrackSelectDialog)
{
    ui->setupUi(this);
}

WhatToTrackSelectDialog::~WhatToTrackSelectDialog()
{
    delete ui;
}

void WhatToTrackSelectDialog::on_pushButton_apply_clicked()
{

    //People,Car,FootBall
    if(ui->radioButton_car->isChecked ()){
        currentTrackType = Car;
    }
    if(ui->radioButton_football->isChecked ()){
        currentTrackType = FootBall;
    }
    if(ui->radioButton_Pedestrian->isChecked ()){
        currentTrackType = People;
    }
    if(ui->radioButton_selectByMouse->isChecked ()){
        currentTrackType = SelectMySelf;
    }


}

void WhatToTrackSelectDialog::on_pushButton_exit_clicked()
{
    this->close ();
}
