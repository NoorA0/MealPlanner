#include "createplan_success.h"
#include "ui_createplan_success.h"

CreatePlan_Success::CreatePlan_Success(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePlan_Success)
{
    ui->setupUi(this);
}

CreatePlan_Success::~CreatePlan_Success()
{
    delete ui;
}

// clicked OK
void CreatePlan_Success::on_pushButton_clicked()
{
    close();
}

