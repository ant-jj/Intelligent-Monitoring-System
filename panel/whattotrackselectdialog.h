#ifndef WHATTOTRACKSELECTDIALOG_H
#define WHATTOTRACKSELECTDIALOG_H

#include <QDialog>
#include "tracktype.h"
//enum TrackType { People,Car,FootBall,SelectMySelf};
//TrackType currentTrackType = People;


namespace Ui {
class WhatToTrackSelectDialog;
}

class WhatToTrackSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WhatToTrackSelectDialog(QWidget *parent = 0);
    ~WhatToTrackSelectDialog();
    TrackType currentTrackType;

private slots:
    void on_pushButton_apply_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::WhatToTrackSelectDialog *ui;
};

#endif // WHATTOTRACKSELECTDIALOG_H
