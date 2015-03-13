#include "formscene2.h"
#include "ui_formscene2.h"

FormScene2::FormScene2(QWidget *parent) :
    MoveableFramelessWindow(parent),
    ui(new Ui::FormScene2)
{
    ui->setupUi(this);
}

FormScene2::~FormScene2()
{
    delete ui;
}

void FormScene2::on_pushButton_clicked()
{
    this->close ();
}

QWidget *FormScene2::getDragnWidget()
{
    return ui->widget_drag;
}
