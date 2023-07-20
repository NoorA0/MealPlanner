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

// OK button clicked
void createPlan_Length::on_pushButton_clicked()
{
    // get and send length
    int length = ui->spinBox->value();

    emit sendPlanLength(true, length);

    close();
}

// Cancel button clicked
void createPlan_Length::on_pushButton_2_clicked()
{
    emit sendPlanLength(false, -1.0);
    close();
}

