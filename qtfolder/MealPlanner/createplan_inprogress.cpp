#include "createplan_inprogress.h"
#include "ui_createplan_inprogress.h"
#include <fstream>

CreatePlan_InProgress::CreatePlan_InProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePlan_InProgress)
{
    ui->setupUi(this);

    connect(parent,
            SIGNAL(closeModalWindow()),
            this,
            SLOT(closeWindow()),
            Qt::UniqueConnection);
}

CreatePlan_InProgress::~CreatePlan_InProgress()
{
    delete ui;
}

void CreatePlan_InProgress::closeWindow(void)
{
    close();
}
