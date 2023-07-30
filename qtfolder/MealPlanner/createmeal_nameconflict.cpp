#include "createmeal_nameconflict.h"
#include "ui_createmeal_nameconflict.h"

CreateMeal_NameConflict::CreateMeal_NameConflict(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMeal_NameConflict)
{
    ui->setupUi(this);
}

CreateMeal_NameConflict::~CreateMeal_NameConflict()
{
    delete ui;
}

// clicked OK
void CreateMeal_NameConflict::on_pushButton_clicked()
{
    close();
}

