#include "editmultitag_basicparams.h"
#include "ui_editmultitag_basicparams.h"
#include "createmultitag_nameconflict.h"

EditMultitag_BasicParams::EditMultitag_BasicParams(QWidget *parent,
                                                   MealManager *mm,
                                                   MultiTag *tag) :
    QDialog(parent),
    ui(new Ui::EditMultitag_BasicParams)
{
    if (mm == nullptr || tag == nullptr)
        close();
    else
    {
        this->mm = mm;
        this-> tag = tag;
    }
    ui->setupUi(this);

    // TODO: set value limits

    // TODO: initialize to MulitTag's values
}

EditMultitag_BasicParams::~EditMultitag_BasicParams()
{
    delete ui;
}

// user said cancel
void EditMultitag_BasicParams::on_pushButton_cancel_clicked()
{
    close();
}

// user wants to apply settings
void EditMultitag_BasicParams::on_pushButton_confirm_clicked()
{
    // TODO: this
}

