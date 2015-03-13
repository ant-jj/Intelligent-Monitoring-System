#ifndef FORMSCENE3_H
#define FORMSCENE3_H

#include <QWidget>
#include "moveableframelesswindow/moveableframelesswindow.h"

namespace Ui {
class FormScene3;
}

class FormScene3 : public MoveableFramelessWindow
{
    Q_OBJECT

public:
    explicit FormScene3(QWidget *parent = 0);
    ~FormScene3();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormScene3 *ui;

    // MoveableFramelessWindow interface
protected:
    QWidget *getDragnWidget();
};

#endif // FORMSCENE3_H
