#include "detectmethoddialog.h"
#include "ui_detectmethoddialog.h"

DetectMethodDialog::DetectMethodDialog(QWidget *parent) :hogdetect(false),multidetect(false),
    QDialog(parent),
    ui(new Ui::DetectMethodDialog)
{
    ui->setupUi(this);
}

DetectMethodDialog::~DetectMethodDialog()
{
    delete ui;
}

void DetectMethodDialog::on_pushButton_apply_clicked()
{

    if(ui->radioButton_hog->isChecked ()){
        this->hogdetect=true;
    }


    if(ui->radioButton_multi->isChecked ()){
        this->multidetect=true;
    }

    ui->pushButton_apply->setStyleSheet("background-color: green");
}

void DetectMethodDialog::on_pushButton_exit_clicked()
{
    this->close ();
}
