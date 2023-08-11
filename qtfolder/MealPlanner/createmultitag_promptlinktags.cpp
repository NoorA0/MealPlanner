#include "createmultitag_promptlinktags.h"
#include "ui_createmultitag_promptlinktags.h"

CreateMultitag_PromptLinkTags::CreateMultitag_PromptLinkTags(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMultitag_PromptLinkTags)
{
    ui->setupUi(this);
}

CreateMultitag_PromptLinkTags::~CreateMultitag_PromptLinkTags()
{
    delete ui;
}

// user wants to link tags
void CreateMultitag_PromptLinkTags::on_pushButton_clicked()
{
    // send user response
    emit sendUserResponse(true);
    close();
}

// said later
void CreateMultitag_PromptLinkTags::on_pushButton_2_clicked()
{
    // send user response
    emit sendUserResponse(false);
    close();
}

