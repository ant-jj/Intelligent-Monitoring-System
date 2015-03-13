#include "trakerselectdialog.h"
#include "ui_trakerselectdialog.h"

TrakerSelectDialog::TrakerSelectDialog(QWidget *parent) :ctFlag(false),particleFlag(false),milFlag(false),
    QDialog(parent),
    ui(new Ui::TrakerSelectDialog)
{
    ui->setupUi(this);
}

TrakerSelectDialog::~TrakerSelectDialog()
{
    delete ui;
}

void TrakerSelectDialog::on_pushButton_apply_clicked()
{
    if(ui->radioButton_ct_tracker->isChecked ()){
        this->ctFlag=true;
        this->particleFlag =false;
        this->milFlag =false;
    }

    if(ui->radioButton_particle->isChecked ()){
        this->ctFlag=false;
        this->particleFlag =true;
        this->milFlag =false;
    }

    if(ui->radioButton_mil_boost->isChecked ()){
        this->ctFlag =false;
        this->particleFlag =false;
        this->milFlag =true;

    }
}

void TrakerSelectDialog::on_pushButton_exit_clicked()
{

    this->close ();
}
