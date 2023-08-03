#include "deletetag_confirmation.h"
#include "ui_deletetag_confirmation.h"

const QString NAME_PREAMBLE = "<html><head/><body><p align=\"center\">Name: <span style=\" font-weight:700;\">";
const QString DEPENDS_PREAMBLE = "</span></p><p align=\"center\">Depends on MultiTag: <span style=\" font-weight:700;\">";
const QString CONSECUTIVE_PREAMBLE = "</span></p><p align=\"center\">Consecutive occurrences: <span style=\" font-weight:700;\">";
const QString ENABLEDDAYS_PREAMBLE = "</span></p><p align=\"center\">Enabled on: <span style=\" font-weight:700;\">";
const QString DESCRIPTION_PREAMBLE = "</span></p><p align=\"center\">Description: <span style=\" font-weight:700;\">";
const QString END_TAGS = "</span></p></body></html>";

DeleteTag_Confirmation::DeleteTag_Confirmation(QWidget *parent,
                                               MealManager *mm,
                                               Tag *tagPtr) :
    QDialog(parent),
    ui(new Ui::DeleteTag_Confirmation)
{
    if (mm == nullptr || tagPtr == nullptr)
        close();
    else
    {
        this->mm = mm;
        this->tagPtr = tagPtr;
    }
    ui->setupUi(this);

    // display tag's information
    QString labelText = NAME_PREAMBLE + tagPtr->getName()
                        + DEPENDS_PREAMBLE;
    labelText += (tagPtr->getDependency() == true) ? "Yes" : "No";
    labelText += CONSECUTIVE_PREAMBLE + QString::number(tagPtr->getConsecutiveLimit());

    labelText += ENABLEDDAYS_PREAMBLE;
    if (tagPtr->isDisabled())
        labelText += "Tag is DISABLED";
    else
        labelText += mm->formatEnabledDays(tagPtr->getEnabledDays());

    labelText += DESCRIPTION_PREAMBLE + tagPtr->getDescription()
                 + END_TAGS;

    ui->label_tagInfo->setText(labelText);
}

DeleteTag_Confirmation::~DeleteTag_Confirmation()
{
    delete ui;
}

// said yes
void DeleteTag_Confirmation::on_pushButton_clicked()
{
    mm->deleteNormalTag(tagPtr);
    tagPtr = nullptr;
    close();
}

// said cancel
void DeleteTag_Confirmation::on_pushButton_2_clicked()
{
    close();
}

