#ifndef FIRSTFRAMEDETECTMETHODSSETDIALOG_H
#define FIRSTFRAMEDETECTMETHODSSETDIALOG_H

#include <QDialog>
#include "tracktype.h"

namespace Ui {
class FirstFrameDetectMethodsSetDialog;
}

class FirstFrameDetectMethodsSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FirstFrameDetectMethodsSetDialog(QWidget *parent = 0);
    ~FirstFrameDetectMethodsSetDialog();
    TrackMethods trackmethods;

private slots:
    void on_pushButton_apply_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::FirstFrameDetectMethodsSetDialog *ui;
};

#endif // FIRSTFRAMEDETECTMETHODSSETDIALOG_H
