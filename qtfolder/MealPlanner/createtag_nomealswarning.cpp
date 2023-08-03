#include "createtag_nomealswarning.h"
#include "ui_createtag_nomealswarning.h"

CreateTag_NoMealsWarning::CreateTag_NoMealsWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTag_NoMealsWarning)
{
    ui->setupUi(this);
}

CreateTag_NoMealsWarning::~CreateTag_NoMealsWarning()
{
    delete ui;
}

void CreateTag_NoMealsWarning::on_pushButton_clicked()
{
    close();
}

