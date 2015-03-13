#include "datasourcedialog.h"
#include "ui_datasourcedialog.h"

DataSourceDialog::DataSourceDialog(QWidget *parent) :uselocal(false),useremote(false),useseq(false),
    QDialog(parent),
    ui(new Ui::DataSourceDialog)
{
    ui->setupUi(this);
}

DataSourceDialog::~DataSourceDialog()
{
    delete ui;
}

//panel :set local  avi file
void DataSourceDialog::on_pushButton_set_local_file_clicked()
{

    QString localFile = QFileDialog::getOpenFileName(this,tr("Choose Avi File"), "/home/testtrack.avi", tr("vedio files (*.*)"));
    if (localFile == NULL)
    {

        QMessageBox::warning(this, tr("error"),
                                       tr("error choose a avi file"),
                                       QMessageBox::Ok);
        return;

    }
    this->localFileName=localFile;

}

//panel : set local seq dir
void DataSourceDialog::on_pushButton_set_local_seq_clicked()
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


    this->localSeqDirName =localDir;
}

void DataSourceDialog::on_pushButton_apply_clicked()
{
    if(ui->radioButton_remoteSource->isChecked ()){
        this->useremote =true;
    }
    if(ui->radioButton_local_file_source->isChecked ()){
        this->uselocal =true;
    }
    if(ui->radioButton_local_source_seq->isChecked ()){
        this->useseq =true;
    }

    ui->pushButton_apply->setStyleSheet("background-color: green");


}

void DataSourceDialog::on_pushButton_exit_clicked()
{
    this->close ();
}
