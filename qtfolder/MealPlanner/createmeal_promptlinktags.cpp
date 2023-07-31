#include "createmeal_promptlinktags.h"
#include "ui_createmeal_promptlinktags.h"

CreateMeal_PromptLinkTags::CreateMeal_PromptLinkTags(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMeal_PromptLinkTags)
{
    ui->setupUi(this);
}

CreateMeal_PromptLinkTags::~CreateMeal_PromptLinkTags()
{
    delete ui;
}

// assign tags to the new meal
void CreateMeal_PromptLinkTags::on_pushButton_clicked()
{

}

// chose to assign tags later
void CreateMeal_PromptLinkTags::on_pushButton_2_clicked()
{
    close();
}

