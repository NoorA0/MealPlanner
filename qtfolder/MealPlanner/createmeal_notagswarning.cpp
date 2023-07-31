#include "createmeal_notagswarning.h"
#include "ui_createmeal_notagswarning.h"

CreateMeal_NoTagsWarning::CreateMeal_NoTagsWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMeal_NoTagsWarning)
{
    ui->setupUi(this);
}

CreateMeal_NoTagsWarning::~CreateMeal_NoTagsWarning()
{
    delete ui;
}

// OK clicked
void CreateMeal_NoTagsWarning::on_pushButton_clicked()
{
    close();
}

