#include "createmultitag_nameconflict.h"
#include "ui_createmultitag_nameconflict.h"

CreateMultitag_NameConflict::CreateMultitag_NameConflict(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMultitag_NameConflict)
{
    ui->setupUi(this);
}

CreateMultitag_NameConflict::~CreateMultitag_NameConflict()
{
    delete ui;
}

void CreateMultitag_NameConflict::on_pushButton_clicked()
{
    close();
}

