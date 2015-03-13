#ifndef FORMSCENE1_H
#define FORMSCENE1_H

#include <QWidget>
#include "moveableframelesswindow/moveableframelesswindow.h"

namespace Ui {
class FormScene1;
}

class FormScene1 : public MoveableFramelessWindow
{
    Q_OBJECT

public:
    explicit FormScene1(QWidget *parent = 0);
    ~FormScene1();

protected:

      QWidget*getDragnWidget();
      void showConfigTest();

private slots:
      void on_pushButton_clicked();

private:
    Ui::FormScene1 *ui;
};

#endif // FORMSCENE1_H
