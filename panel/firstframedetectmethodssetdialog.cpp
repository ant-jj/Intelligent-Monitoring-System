#include "firstframedetectmethodssetdialog.h"
#include "ui_firstframedetectmethodssetdialog.h"

FirstFrameDetectMethodsSetDialog::FirstFrameDetectMethodsSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirstFrameDetectMethodsSetDialog)
{
    ui->setupUi(this);
}

FirstFrameDetectMethodsSetDialog::~FirstFrameDetectMethodsSetDialog()
{
    delete ui;
}

void FirstFrameDetectMethodsSetDialog::on_pushButton_apply_clicked()
{
    if(ui->radioButton_hog->isChecked ()){

        trackmethods = HOG;
    }
    if(ui->radioButton_lbp->isChecked ()){

        trackmethods = LBP;

    }
    if(ui->radioButton_haar->isChecked ()){

        trackmethods = HAAR;
    }

}

void FirstFrameDetectMethodsSetDialog::on_pushButton_exit_clicked()
{

    this->close ();
}
