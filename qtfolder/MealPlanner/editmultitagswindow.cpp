#include "editmultitagswindow.h"
#include "ui_editmultitagswindow.h"
#include "createmultitag_basicparams.h"
#include "editmultitag_basicparams.h"
#include "createmultitag_notagswarning.h"
#include "createmultitag_promptlinktags.h"
#include "deletemultitag_confirmation.h"
#include "editmultitag_assignedtags.h"

EditMultitagsWindow::EditMultitagsWindow(QWidget *parent,
                                         MealManager *mm) :
    QMainWindow(parent),
    ui(new Ui::EditMultitagsWindow)
{
    if (mm == nullptr)
        close();
    else
    {
        this->mm = mm;
    }

    // generate items to use in listWidget, linked in Map to reference later
    RebuildItems();

    ui->setupUi(this);

    // list all Multitags in the listWidget in alphabetical order
    for (auto& item: itemsInOrder)
    {
        ui->listWidget_tags->addItem(item);
    }
}

EditMultitagsWindow::~EditMultitagsWindow()
{
    delete ui;

    //unhide the previous window
    QMainWindow *mw = qobject_cast<QMainWindow*>(parent());
    if (mw != nullptr)
    {
        mw->show();
    }
}

void EditMultitagsWindow::RebuildItems(void)
{
    QString tempStr;
    QVector<MultiTag*> tags = mm->getMultiTags();

    itemToMultitag.clear();
    itemsInOrder.clear();

    for (MultiTag* tagPtr : tags)
    {
        tempStr = "\nName: " + tagPtr->getName()
                  + "\nHas priority over Tags: ";
        tempStr += (tagPtr->getPriority()) ? "Yes" : "No";
        tempStr += "\nRequire all Tags to be fulfilled: ";
        tempStr += (tagPtr->getRequireFulfillment()) ? "Yes" : "No";
        // check if enabled
        if (tagPtr->isDisabled())
        {
            tempStr += "\nTag is DISABLED";
        }
        else
        {
            // get enabled days
            tempStr += "\nEnabled on: " + mm->formatEnabledDays(tagPtr->getEnabledDays());
        }
        tempStr += "\nDescription: " + tagPtr->getDescription() + "\n";

        // create new item
        itemToMultitag[tempStr] = tagPtr;
        itemsInOrder.push_back(tempStr);
    }
}

void EditMultitagsWindow::RefreshTagsList(void)
{
    // make sure items are up to date
    RebuildItems();

    // clear the listWidget
    ui->listWidget_multitags->clear();

    // repopulate the listWidget
    for (auto& item : itemsInOrder)
    {
        ui->listWidget_multitags->addItem(item);
    }
}

// exit
void EditMultitagsWindow::on_pushButton_exit_clicked()
{
    close();
}

// delete multitag
void EditMultitagsWindow::on_pushButton_deleteMultitag_clicked()
{

}

// new multitag
void EditMultitagsWindow::on_pushButton_newMultitag_clicked()
{

}

// edit multitag
void EditMultitagsWindow::on_pushButton_editMultitag_clicked()
{

}

// double clicked multitag
void EditMultitagsWindow::on_listWidget_multitags_itemDoubleClicked(QListWidgetItem *item)
{

}

// edit linked tags
void EditMultitagsWindow::on_pushButton_editLinkedTags_clicked()
{

}

// new item selected
void EditMultitagsWindow::on_listWidget_multitags_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{

}

