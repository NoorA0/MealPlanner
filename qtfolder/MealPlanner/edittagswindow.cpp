#include "edittagswindow.h"
#include "ui_edittagswindow.h"
#include "edittag_basicparams.h"
#include "createtag_basicparams.h"
#include "createtag_nomealswarning.h"
#include "createtag_promptlinkmeals.h"
#include "edittag_assignedmeals.h"
#include "deletetag_confirmation.h"

EditTagsWindow::EditTagsWindow(QWidget *parent,
                               MealManager *mm) :
    QMainWindow(parent),
    ui(new Ui::EditTagsWindow)
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

    // list all Tags in the listWidget in alphabetical order
    for (auto& item : itemsInOrder)
    {
        ui->listWidget_tags->addItem(item);
    }
}

EditTagsWindow::~EditTagsWindow()
{
    delete ui;

    // unhide the previous window
    QMainWindow *mw = qobject_cast<QMainWindow*>(parent());
    if (mw != nullptr)
    {
        mw->show();
    }
}

void EditTagsWindow::RebuildItems(void)
{
    QString tempStr;
    QVector<Tag*> tags = mm->getNormalTags();

    itemToTag.clear();
    itemsInOrder.clear();

    for (Tag* tagPtr : tags)
    {
        tempStr = "\nName: " + tagPtr->getName()
                  + "\nDepends on MultiTag? ";
        tempStr += (tagPtr->getDependency()) ? "Yes" : "No";
        tempStr += "\nMaximum consecutive days: " + QString::number(tagPtr->getConsecutiveLimit()) + "\n";

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
        itemToTag[tempStr] = tagPtr;
        itemsInOrder.push_back(tempStr);
    }
}

void EditTagsWindow::RefreshTagsList(void)
{
    // make sure items are up to date
    RebuildItems();

    // clear the listWidget
    ui->listWidget_tags->clear();

    // repopulate the listWidget
    for (auto& item : itemsInOrder)
    {
        ui->listWidget_tags->addItem(item);
    }
}

void EditTagsWindow::on_pushButton_exit_clicked()
{
    close();
}

// edit tag properties
void EditTagsWindow::on_pushButton_editTag_clicked()
{
    // pass selected item to below function
    on_listWidget_tags_itemDoubleClicked(ui->listWidget_tags->currentItem());
}

// double clicked tag
void EditTagsWindow::on_listWidget_tags_itemDoubleClicked(QListWidgetItem *item)
{
    Tag* tagPtr;
    QString tagKey = item->text();

    // get the tag associated with the item
    tagPtr = itemToTag.find(tagKey).value();

    // create edit window
    EditTag_BasicParams *window = new EditTag_BasicParams(this, mm, tagPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();
}

// create new tag
void EditTagsWindow::on_pushButton_newTag_clicked()
{
    // create tag by getting basic info
    CreateTag_BasicParams *window = new CreateTag_BasicParams(this, mm);
    window->setAttribute(Qt::WA_DeleteOnClose);

    // connect to signal to receive new tags when created
    connect(window,
            SIGNAL(sendNewTag(Tag*)),
            this,
            SLOT(receiveNewTag(Tag*)),
            Qt::UniqueConnection);
    window->exec();

    // if no meals exist, warn user
    if (mm->getNumberOfMeals() <= 0)
    {
        CreateTag_NoMealsWarning *window2 = new CreateTag_NoMealsWarning(this);
        window2->setAttribute(Qt::WA_DeleteOnClose);
        window2->exec();
    }
    else if (newTag != nullptr)
    {
        CreateTag_PromptLinkMeals *window2 = new CreateTag_PromptLinkMeals(this);
        window2->setAttribute(Qt::WA_DeleteOnClose);

        // connect signal to receive response
        connect(window2,
                SIGNAL(sendUserResponse(bool)),
                SLOT(receiveBoolAssignNewMeals(bool)),
                Qt::UniqueConnection);

        window2->exec();

        if (userResponse)
        {
            // create edit meals window
            EditTag_AssignedMeals *window3 = new EditTag_AssignedMeals(this, mm, newTag);
            window3->setAttribute(Qt::WA_DeleteOnClose);
            window3->exec();
        }
    }

    // refresh list of tags
    RefreshTagsList();
}

// delete tag
void EditTagsWindow::on_pushButton_deleteTag_clicked()
{
    Tag* tagPtr;
    QString tagKey = ui->listWidget_tags->currentItem()->text();

    // get the tag associated with the item
    tagPtr = itemToTag.find(tagKey).value();

    // display confirmation window, handles final deletion
    DeleteTag_Confirmation *window = new DeleteTag_Confirmation(this, mm, tagPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // refreshes list of tags to display
    RefreshTagsList();
}

// edit assigned meals
void EditTagsWindow::on_pushButton_editAssignedMeals_clicked()
{
    Tag* tagPtr;
    QString tagKey = ui->listWidget_tags->currentItem()->text();

    // get the tag associated with the item
    tagPtr = itemToTag.find(tagKey).value();

    EditTag_AssignedMeals *window = new EditTag_AssignedMeals(this, mm, tagPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();
}

// item changed, display its assigned meals
void EditTagsWindow::on_listWidget_tags_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Tag* tagPtr;
    QVector<Meal*> assignedMeals;
    QString tagKey = current->text();

    // clear all displayed meals
    ui->listWidget_assignedMeals->clear();

    // get the tag associated with the item
    tagPtr = itemToTag.find(tagKey).value();
    assignedMeals = tagPtr->getLinkedMeals();

    // create listWidget items for every assigned meal
    for (auto& mealPtr : assignedMeals)
    {
        QString tempStr = "\nName: " + mealPtr->getName()
                          + "\nPrice: " + QString::number(mealPtr->getPrice())
                          + "\nDuration: " + QString::number(mealPtr->getMealDuration()) + " day(s)"
                          + "\nNum. Tags Assigned: " + QString::number(mealPtr->getTags().size());

        // check if enabled
        if (mealPtr->isDisabled())
        {
            tempStr += "\nMeal is DISABLED";
        }
        else
        {
            // get enabled days
            tempStr += "\nEnabled on: " + mm->formatEnabledDays(mealPtr->getEnabledDays()) + "\n";
        }

        // add the item to the listWidget
        ui->listWidget_tags->addItem(tempStr);
    }
}


