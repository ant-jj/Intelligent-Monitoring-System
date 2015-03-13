#ifndef TRAKERSELECTDIALOG_H
#define TRAKERSELECTDIALOG_H

#include <QDialog>

namespace Ui {
class TrakerSelectDialog;
}

class TrakerSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrakerSelectDialog(QWidget *parent = 0);
    ~TrakerSelectDialog();
    bool ctFlag;
    bool particleFlag;
    bool milFlag;



private slots:
    void on_pushButton_apply_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::TrakerSelectDialog *ui;
};

#endif // TRAKERSELECTDIALOG_H
