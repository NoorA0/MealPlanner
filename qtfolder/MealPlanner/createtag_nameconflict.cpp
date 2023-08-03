#include "createtag_nameconflict.h"
#include "ui_createtag_nameconflict.h"

CreateTag_NameConflict::CreateTag_NameConflict(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTag_NameConflict)
{
    ui->setupUi(this);
}

CreateTag_NameConflict::~CreateTag_NameConflict()
{
    delete ui;
}

void CreateTag_NameConflict::on_pushButton_clicked()
{
    close();
}

