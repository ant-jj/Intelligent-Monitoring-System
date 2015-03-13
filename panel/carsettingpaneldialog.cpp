#include "carsettingpaneldialog.h"
#include "ui_carsettingpaneldialog.h"
#include <QDebug>
CarSettingPanelDialog::CarSettingPanelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CarSettingPanelDialog)
{
    ui->setupUi(this);
//    this->setWindowFlags(Qt::FramelessWindowHint);
}

CarSettingPanelDialog::~CarSettingPanelDialog()
{
    delete ui;
}
/*
 * 方式二
 */
void CarSettingPanelDialog::on_pushButton_select_video_clicked()
{

    QString localFile = QFileDialog::getOpenFileName(this,tr("Choose video File"), "/home/testtrack.avi", tr("vedio files (*.*)"));
    if (localFile == NULL)
    {

        QMessageBox::warning(this, tr("error"),
                                       tr("error choose a video file"),
                                       QMessageBox::Ok);
        return;
    }
    this->videofilename = localFile;
}

/*
 * 方式一
 */
void CarSettingPanelDialog::on_pushButton_set_seq_dir_clicked()
{
    QString localDir =QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    if (localDir == NULL)
    {
        QMessageBox::warning(this, tr("error"),
                                       tr("error choose a dir contain image seq "),
                                       QMessageBox::Ok);
        return;
    }
    this->dirname = localDir;
}

void CarSettingPanelDialog::on_pushButton_set_seq_list_clicked()
{
    QString localFile = QFileDialog::getOpenFileName(this,tr("Choose Txt File"), "/home/testtrack.txt", tr("vedio files (*.*)"));
    if (localFile == NULL)
    {
        QMessageBox::warning(this, tr("error"),
                                       tr("error choose a txt file"),
                                       QMessageBox::Ok);
        return;
    }
    this->txtfilename = localFile;
}

void CarSettingPanelDialog::on_pushButton_exit_clicked()
{
    this->close ();
}

void CarSettingPanelDialog::on_pushButton_apply_clicked()
{
    ui->pushButton_apply->setStyleSheet("background-color: green");
    qDebug()<<"set Ok for car detect ...";
}

