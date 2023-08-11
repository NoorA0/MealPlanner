#include "deletemultitag_confirmation.h"
#include "ui_deletemultitag_confirmation.h"

const QString NAME_PREAMBLE = "<html><head/><body><p align=\"center\">Name: <span style=\" font-weight:700;\">";
const QString PRIORITY_PREAMBLE = "</span></p><p align=\"center\">Has elevated priority: <span style=\" font-weight:700;\">";
const QString FULFILLMENT_PREAMBLE = "</span></p><p align=\"center\">Allow partial fulfillment: <span style=\" font-weight:700;\">";
const QString ENABLEDDAYS_PREAMBLE = "</span></p><p align=\"center\">Enabled on: <span style=\" font-weight:700;\">";
const QString DESCRIPTION_PREAMBLE = "</span></p><p align=\"center\">Description: <span style=\" font-weight:700;\">";
const QString END_TAGS = "</span></p></body></html>";

DeleteMultitag_Confirmation::DeleteMultitag_Confirmation(QWidget *parent,
                                                         MealManager *mm,
                                                         MulitTag *tagPtr) :
    QDialog(parent),
    ui(new Ui::DeleteMultitag_Confirmation)
{
    if (mm == nullptr || tagPtr == nullptr)
        close();
    else
    {
        this->mm = mm;
        this->tagPtr = tagPtr;
    }
    ui->setupUi(this);

    // TODO: display Multitag's information
}

DeleteMultitag_Confirmation::~DeleteMultitag_Confirmation()
{
    delete ui;
}

// user canceled
void DeleteMultitag_Confirmation::on_pushButton_2_clicked()
{
    //mm->deleteTag(tag);
    //tag = nullptr;
    close();
}

// user confirmed deletion
void DeleteMultitag_Confirmation::on_pushButton_clicked()
{
    close();
}

