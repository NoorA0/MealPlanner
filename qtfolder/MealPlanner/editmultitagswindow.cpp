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
        ui->listWidget_multitags->addItem(item);
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
    ui->listWidget_multitags->blockSignals(true);
    ui->listWidget_multitags->clear();

    // repopulate the listWidget
    for (auto& item : itemsInOrder)
    {
        ui->listWidget_multitags->addItem(item);
    }
    ui->listWidget_multitags->blockSignals(false);
}

// exit
void EditMultitagsWindow::on_pushButton_exit_clicked()
{
    close();
}

// delete multitag
void EditMultitagsWindow::on_pushButton_deleteMultitag_clicked()
{
    MultiTag* tagPtr;
    QListWidgetItem *currentItem = ui->listWidget_multitags->currentItem();

    // check if item exists
    if (currentItem == nullptr)
        return;

    auto iter = itemToMultitag.find(currentItem->text());

    if (iter == itemToMultitag.end())
        return;

    // get the associated multitag
    tagPtr = iter.value();

    // display confirmation window, handles final deletion
    DeleteMultitag_Confirmation *window = new DeleteMultitag_Confirmation(this, mm, tagPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // refresh list of multitags to display
    RefreshTagsList();
}

// new multitag
void EditMultitagsWindow::on_pushButton_newMultitag_clicked()
{
    // create multitag by getting basic info
    CreateMultitag_BasicParams *window = new CreateMultitag_BasicParams(this, mm);
    window->setAttribute(Qt::WA_DeleteOnClose);

    newMT = nullptr;

    // connect to signal to receive new multitag when created
    connect(window,
            SIGNAL(sendNewMultitag(MultiTag*)),
            this,
            SLOT(receiveNewMultiTag(MultiTag*)),
            Qt::UniqueConnection);
    window->exec();

    // if no tags exist, warn user
    if (newMT != nullptr && mm->getNumberOfNormalTags() < 1)
    {
        CreateMultitag_NoTagsWarning *window2 = new CreateMultitag_NoTagsWarning(this);
        window2->setAttribute(Qt::WA_DeleteOnClose);
        window2->exec();
    }
    else if (newMT != nullptr)
    {
        // prompt to link tags with new multitag
        CreateMultitag_PromptLinkTags *window2 = new CreateMultitag_PromptLinkTags(this);
        window2->setAttribute(Qt::WA_DeleteOnClose);

        // connect signal to receive response
        connect(window2,
                SIGNAL(sendUserResponse(bool)),
                SLOT(receiveBoolAssignNewTags(bool)),
                Qt::UniqueConnection);

        window2->exec();

        if (userResponse)
        {
            // create edit tags window
            EditMultitag_AssignedTags *window3 = new EditMultitag_AssignedTags(this, mm, newMT);
            window3->setAttribute(Qt::WA_DeleteOnClose);
            window3->exec();
        }
    }

    // refresh list of multitags
    RefreshTagsList();
}

// edit multitag
void EditMultitagsWindow::on_pushButton_editMultitag_clicked()
{
    // pass selected item to below function
    on_listWidget_multitags_itemDoubleClicked(ui->listWidget_multitags->currentItem());
}

// double clicked multitag
void EditMultitagsWindow::on_listWidget_multitags_itemDoubleClicked(QListWidgetItem *item)
{
    MultiTag* tagPtr;

    // check if item exists
    if (item == nullptr)
        return;

    auto iter = itemToMultitag.find(item->text());

    if (iter == itemToMultitag.end())
        return;

    // get the multitag associated with the item
    tagPtr = iter.value();

    // create edit window
    EditMultitag_BasicParams *window = new EditMultitag_BasicParams(this, mm, tagPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // refresh multitags
    RefreshTagsList();
}

// edit linked tags
void EditMultitagsWindow::on_pushButton_editLinkedTags_clicked()
{
    MultiTag* tagPtr = nullptr;
    QListWidgetItem *currentItem = ui->listWidget_multitags->currentItem();

    // check if item exists
    if (currentItem == nullptr)
        return;

    auto iter = itemToMultitag.find(currentItem->text());

    if (iter == itemToMultitag.end())
        return;

    // get the multitag associated with the item
    tagPtr = iter.value();

    EditMultitag_AssignedTags *window = new EditMultitag_AssignedTags(this, mm, tagPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // refresh display of linked tags
    on_listWidget_multitags_currentItemChanged(ui->listWidget_multitags->currentItem(), nullptr);
}

// item changed, display its assigned tags
void EditMultitagsWindow::on_listWidget_multitags_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    MultiTag* multitagPtr = nullptr;
    QVector<QPair<Tag*, unsigned int>> assignedTags;
    QString tagKey = current->text();

    // clear all displayed tags
    ui->listWidget_assignedTags->blockSignals(true);
    ui->listWidget_assignedTags->clear();

    // get the multitag associated with the item
    multitagPtr = itemToMultitag.find(tagKey).value();
    assignedTags = multitagPtr->getLinkedTags();

    // create listWidget items for every assigned tag
    for (auto& tagPairPtr : assignedTags)
    {
        QString tempStr = "\nName: " + tagPairPtr.first->getName()
                          + "\nDepends on MultiTag: ";
        tempStr += (tagPairPtr.first->getDependency()) ? "Yes" : "No";
        tempStr += "\nMaximum consecutive days: " + QString::number(tagPairPtr.first->getConsecutiveLimit()) + "\n";
        if (tagPairPtr.first->isDisabled())
            tempStr += "Tag is DISABLED";
        else
        {
            tempStr += "\nEnabled on: " + mm->formatEnabledDays(tagPairPtr.first->getEnabledDays());
        }

        tempStr += "\nNumber of Meals requested from this Tag: " + QString::number(tagPairPtr.second);

        tempStr += "\nDescription: " + tagPairPtr.first->getDescription() + "\n";

        // add the item to the listWidget
        ui->listWidget_assignedTags->addItem(tempStr);
        ui->listWidget_assignedTags->blockSignals(false);
    }
}

