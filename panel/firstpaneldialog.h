#ifndef FIRSTPANELDIALOG_H
#define FIRSTPANELDIALOG_H

#include <QDialog>
#include <QString>


namespace Ui {
class firstPanelDialog;
}

class firstPanelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit firstPanelDialog(QWidget *parent = 0);
    ~firstPanelDialog();

    QString ipaddr;
    int port;


private slots:


    void on_pushButton_apply_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::firstPanelDialog *ui;
};

#endif // FIRSTPANELDIALOG_H
