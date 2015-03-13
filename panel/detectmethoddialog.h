#ifndef DETECTMETHODDIALOG_H
#define DETECTMETHODDIALOG_H

#include <QDialog>

namespace Ui {
class DetectMethodDialog;
}

class DetectMethodDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetectMethodDialog(QWidget *parent = 0);
    ~DetectMethodDialog();
    bool hogdetect;
    bool multidetect;

private slots:
    void on_pushButton_apply_clicked();
    void on_pushButton_exit_clicked();
private:
    Ui::DetectMethodDialog *ui;
};

#endif // DETECTMETHODDIALOG_H
