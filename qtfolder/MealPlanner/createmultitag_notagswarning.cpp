#include "createmultitag_notagswarning.h"
#include "ui_createmultitag_notagswarning.h"

CreateMultitag_NoTagsWarning::CreateMultitag_NoTagsWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMultitag_NoTagsWarning)
{
    ui->setupUi(this);
}

CreateMultitag_NoTagsWarning::~CreateMultitag_NoTagsWarning()
{
    delete ui;
}

void CreateMultitag_NoTagsWarning::on_pushButton_clicked()
{
    close();
}

