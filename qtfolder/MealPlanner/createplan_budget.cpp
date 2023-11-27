#include "createplan_budget.h"
#include "ui_createplan_budget.h"

createPlan_Budget::createPlan_Budget(QWidget *parent,
                                     MealManager *mm) :
    QDialog(parent),
    ui(new Ui::createPlan_Budget)
{
    if (mm == nullptr)
        close();
    else
    {
        this->mm = mm;
    }
    ui->setupUi(this);

    // set limits to budget
    ui->doubleSpinBox->setMinimum(mm->getMinimumBudget());
    ui->doubleSpinBox->setMaximum(mm->getMaximumBudget());
}

createPlan_Budget::~createPlan_Budget()
{
    delete ui;
}

// OK button clicked
void createPlan_Budget::on_pushButton_clicked()
{
    // get and send budget
    double budget = ui->doubleSpinBox->value();

    emit sendPlanBudget(true, budget);

    close();
}

// Cancel button clicked
void createPlan_Budget::on_pushButton_2_clicked()
{
    emit sendPlanBudget(false, -1.0);
    close();
}

