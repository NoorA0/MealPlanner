#include "createplan_budget.h"
#include "ui_createplan_budget.h"

createPlan_Budget::createPlan_Budget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createPlan_Budget)
{
    ui->setupUi(this);
}

createPlan_Budget::~createPlan_Budget()
{
    delete ui;
}
