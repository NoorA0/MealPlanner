#include "createmultitag_basicparams.h"
#include "ui_createmultitag_basicparams.h"
#include "createmultitag_nameconflict.h"

CreateMultitag_BasicParams::CreateMultitag_BasicParams(QWidget *parent,
                                                       MealManager *mm) :
    QDialog(parent),
    ui(new Ui::CreateMultitag_BasicParams)
{
    if (mm == nullptr)
        close();
    else
        this->mm = mm;
    ui->setupUi(this);

    // TODO: set value limits
}

CreateMultitag_BasicParams::~CreateMultitag_BasicParams()
{
    delete ui;
}

// cancel clicked
void CreateMultitag_BasicParams::on_pushButton_cancel_clicked()
{
    close();
}

// confirm clicked
void CreateMultitag_BasicParams::on_pushButton_confirm_clicked()
{
    // TODO: complete this
}

