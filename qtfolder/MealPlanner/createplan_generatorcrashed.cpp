#include "createplan_generatorcrashed.h"
#include "ui_createplan_generatorcrashed.h"

CreatePlan_GeneratorCrashed::CreatePlan_GeneratorCrashed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePlan_GeneratorCrashed)
{
    ui->setupUi(this);
}

CreatePlan_GeneratorCrashed::~CreatePlan_GeneratorCrashed()
{
    delete ui;
}

void CreatePlan_GeneratorCrashed::on_pushButton_OK_clicked()
{
    close();
}

