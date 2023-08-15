#include "createplan_nomealsortagswarning.h"
#include "ui_createplan_nomealsortagswarning.h"

CreatePlan_NoMealsOrTagsWarning::CreatePlan_NoMealsOrTagsWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePlan_NoMealsOrTagsWarning)
{
    ui->setupUi(this);
}

CreatePlan_NoMealsOrTagsWarning::~CreatePlan_NoMealsOrTagsWarning()
{
    delete ui;
}

void CreatePlan_NoMealsOrTagsWarning::on_pushButton_OK_clicked()
{
    close();
}

