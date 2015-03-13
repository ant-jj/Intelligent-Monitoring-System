#ifndef CARSETTINGPANELDIALOG_H
#define CARSETTINGPANELDIALOG_H

#include <QDialog>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>


#include <QKeyEvent>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>

namespace Ui {
class CarSettingPanelDialog;
}

class CarSettingPanelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarSettingPanelDialog(QWidget *parent = 0);
    ~CarSettingPanelDialog();
    QString txtfilename;
    QString dirname;
    QString videofilename;

private slots:
    void on_pushButton_select_video_clicked();

    void on_pushButton_set_seq_dir_clicked();

    void on_pushButton_set_seq_list_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_apply_clicked();

private:
    Ui::CarSettingPanelDialog *ui;




};

#endif // CARSETTINGPANELDIALOG_H
