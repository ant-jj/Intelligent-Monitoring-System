#include "firstpaneldialog.h"
#include "ui_firstpaneldialog.h"
#include <QDebug>
firstPanelDialog::firstPanelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstPanelDialog)
{
    ui->setupUi(this);
}

firstPanelDialog::~firstPanelDialog()
{
    delete ui;
}


void firstPanelDialog::on_pushButton_apply_clicked()
{
    this->ipaddr=ui->lineEdit_ip->text ();
    this->port =ui->lineEdit_port->text ().toInt ();
    ui->pushButton_apply->setStyleSheet("background-color: green");

}

void firstPanelDialog::on_pushButton_exit_clicked()
{
    this->close ();
}
