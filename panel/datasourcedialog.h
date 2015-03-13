#ifndef DATASOURCEDIALOG_H
#define DATASOURCEDIALOG_H

#include <QDialog>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class DataSourceDialog;
}

class DataSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSourceDialog(QWidget *parent = 0);
    ~DataSourceDialog();
    bool uselocal;
    bool useremote;
    bool useseq;
    QString localFileName;
    QString localSeqDirName;


private slots:
    void on_pushButton_set_local_file_clicked();

    void on_pushButton_set_local_seq_clicked();

    void on_pushButton_apply_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::DataSourceDialog *ui;
};

#endif // DATASOURCEDIALOG_H
