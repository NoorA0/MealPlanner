#include "editmultitag_assignedtags.h"
#include "ui_editmultitag_assignedtags.h"

EditMultitag_AssignedTags::EditMultitag_AssignedTags(QWidget *parent,
                                                     MealManager *mm,
                                                     MultiTag *tagPtr) :
    QDialog(parent),
    ui(new Ui::EditMultitag_AssignedTags)
{
    if (mm == nullptr || tagPtr == nullptr)
        close();
    else
    {
        this->mm = mm;
        this->tagPtr = tagPtr;
    }
    ui->setupUi(this);

    // TODO: this
}

void EditMultitag_AssignedTags::RefreshTagsList(void)
{
    // clear the listWidgets
    ui->listWidget_assignedTags->clear();
    ui->listWidget_unassignedTags->clear();

    // repopulate the listWidgets
    for (auto& item : itemsInOrder_Assigned)
    {
        ui->listWidget_assignedTags->addItem(item);
    }
    for (auto& item : itemsInOrder_Unassigned)
    {
        ui->listWidget_assignedTags->addItem(item);
    }
}

EditMultitag_AssignedTags::~EditMultitag_AssignedTags()
{
    delete ui;
}

// done editing
void EditMultitag_AssignedTags::on_pushButton_clicked()
{
    close();
}

// unassign tag
void EditMultitag_AssignedTags::on_pushButton_unassignTag_clicked()
{
    // TODO: this
}

// assign tag
void EditMultitag_AssignedTags::on_pushButton_assignTag_clicked()
{
    // TODO: this
}

