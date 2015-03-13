#include "formscene3.h"
#include "ui_formscene3.h"

FormScene3::FormScene3(QWidget *parent) :
    MoveableFramelessWindow(parent),
    ui(new Ui::FormScene3)
{
    ui->setupUi(this);
}

FormScene3::~FormScene3()
{
    delete ui;
}

void FormScene3::on_pushButton_clicked()
{
    this->close ();
}

QWidget *FormScene3::getDragnWidget()
{

    return ui->widget_drag;
}
