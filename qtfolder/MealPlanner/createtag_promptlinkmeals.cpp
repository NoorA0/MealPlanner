#include "createtag_promptlinkmeals.h"
#include "ui_createtag_promptlinkmeals.h"

CreateTag_PromptLinkMeals::CreateTag_PromptLinkMeals(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTag_PromptLinkMeals)
{
    ui->setupUi(this);
}

CreateTag_PromptLinkMeals::~CreateTag_PromptLinkMeals()
{
    delete ui;
}

// chose to assign meals later
void CreateTag_PromptLinkMeals::on_pushButton_2_clicked()
{
    emit sendUserResponse(false);
    close();
}

// assign meals to the new tag
void CreateTag_PromptLinkMeals::on_pushButton_clicked()
{
    emit sendUserResponse(true);
    close();
}

