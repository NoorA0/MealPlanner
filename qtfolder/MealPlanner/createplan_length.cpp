#include "createplan_length.h"
#include "ui_createplan_length.h"

createPlan_Length::createPlan_Length(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createPlan_Length)
{
    ui->setupUi(this);
}

createPlan_Length::~createPlan_Length()
{
    delete ui;
}
