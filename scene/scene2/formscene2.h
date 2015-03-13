#ifndef FORMSCENE2_H
#define FORMSCENE2_H

#include <QWidget>
#include "moveableframelesswindow/moveableframelesswindow.h"

namespace Ui {
class FormScene2;
}

class FormScene2 : public MoveableFramelessWindow
{
    Q_OBJECT

public:
    explicit FormScene2(QWidget *parent = 0);
    ~FormScene2();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormScene2 *ui;

    // MoveableFramelessWindow interface
protected:
    QWidget *getDragnWidget();
};

#endif // FORMSCENE2_H
