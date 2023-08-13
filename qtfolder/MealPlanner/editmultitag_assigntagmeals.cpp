#include "editmultitag_assigntagmeals.h"
#include "ui_editmultitag_assigntagmeals.h"

EditMultitag_AssignTagMeals::EditMultitag_AssignTagMeals(QWidget *parent,
                                                         MealManager *mm,
                                                         unsigned int requestedMeals) :
    QDialog(parent),
    ui(new Ui::EditMultitag_AssignTagMeals)
{
    if (mm == nullptr)
        close();
    else
    {
        this->mm = mm;
    }

    ui->setupUi(this);

    // set min and max values for spinbox
    ui->spinBox_requestedMeals->setMinimum(mm->getMinRequestedMeals());
    ui->spinBox_requestedMeals->setMaximum(mm->getMaxRequestedMeals());

    // if getting an existing Tag's value, display it
    //  otherwise default to minimum value
    if (requestedMeals >= mm->getMinRequestedMeals())
        ui->spinBox_requestedMeals->setValue(requestedMeals);
}

EditMultitag_AssignTagMeals::~EditMultitag_AssignTagMeals()
{
    delete ui;
}

// send value
void EditMultitag_AssignTagMeals::on_pushButton_done_clicked()
{
    emit sendNumberRequestedMeals(ui->spinBox_requestedMeals->value());
    close();
}

