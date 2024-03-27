#include "editmeals.h"
#include "ui_editmeals.h"
#include "editmeal_basicparams.h"
#include "createmeal_basicparams.h"
#include "createmeal_promptlinktags.h"
#include "createmeal_notagswarning.h"
#include "deletemeal_confirmation.h"
#include "editmeal_assignedtags.h"
#include <QDebug>


EditMeals::EditMeals(QWidget *parent,
                     MealManager *mm)
    : QDialog(parent)
    , ui(new Ui::EditMeals)
{
    if (mm == nullptr)
        close();
    else
        this->mm = mm;

    // generate items to use in listWidget, linked in Map to reference later
    RebuildItems();

    ui->setupUi(this);

    // lists all Meals in the listWidget in alphabetical order
    for (auto& item : itemsInOrder)
    {
        ui->listWidget_meals->addItem(item);
    }
}

EditMeals::~EditMeals()
{
    delete ui;
}

void EditMeals::RebuildItems(void)
{
    QString tempStr;
    QVector<Meal*> meals = mm->getMeals();

    itemToMeal.clear();
    itemsInOrder.clear();

    for (Meal* mealPtr : meals)
    {
        tempStr = "\nName: " + mealPtr->getName()
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
            QMap<DaysOfTheWeek, bool> mealEnabledDays = mealPtr->getEnabledDays();
            tempStr += "\nEnabled (by tags) on: " + mm->formatEnabledDays(mealEnabledDays) + "\n";
        }

        // create new item
        itemToMeal[tempStr] = mealPtr;
        itemsInOrder.push_back(tempStr);
    }
}

void EditMeals::RefreshMealsList(void)
{
    // make sure items are up to date
    RebuildItems();

    // clear the listWidget
    ui->listWidget_meals->blockSignals(true);
    ui->listWidget_meals->clear();

    // repopulate the listWidget
    for (auto& item : itemsInOrder)
    {
        ui->listWidget_meals->addItem(item);
    }
    ui->listWidget_meals->blockSignals(false);
}

// create a new meal
void EditMeals::on_newMealButton_clicked()
{
    // create meal by getting basic info
    createmeal_basicparams *window = new createmeal_basicparams(this, this->mm);
    window->setAttribute(Qt::WA_DeleteOnClose);

    newMeal = nullptr;

    // connect to signal to receive new meals when created
    connect(window,
            SIGNAL(sendNewMeal(Meal*)),
            this,
            SLOT(receiveNewMeal(Meal*)),
            Qt::UniqueConnection);

    window->exec();

    // if no tags exist, warn user
    if (newMeal != nullptr && mm->getNumberOfNormalTags() < 1)
    {
        CreateMeal_NoTagsWarning *window2 = new CreateMeal_NoTagsWarning(this);
        window2->setAttribute(Qt::WA_DeleteOnClose);
        window2->exec();
    }
    else if (newMeal != nullptr)
    {
        // prompt to link tags with new meal
        CreateMeal_PromptLinkTags *window2 = new CreateMeal_PromptLinkTags(this);
        window2->setAttribute(Qt::WA_DeleteOnClose);

        // connect signal to receive user response
        connect(window2,
                SIGNAL(sendUserResponse(bool)),
                this,
                SLOT(receiveBoolAssignNewTags(bool)),
                Qt::UniqueConnection);

        window2->exec();

        if (userResponse)
        {
            // create edit tag window
            EditMeal_AssignedTags *window3 = new EditMeal_AssignedTags(this, mm, newMeal);
            window3->setAttribute(Qt::WA_DeleteOnClose);
            window3->exec();
        }
    }

    // refresh list of meals
    RefreshMealsList();
}

// edit the selected meal
// same functionality as double clicking
void EditMeals::on_editMealButton_clicked()
{
    // pass the selected item to edit
    on_listWidget_meals_itemDoubleClicked(ui->listWidget_meals->currentItem());
}

// edit the selected meal
void EditMeals::on_listWidget_meals_itemDoubleClicked(QListWidgetItem *item)
{
    Meal* mealPtr;

    // check if item exists
    if (item == nullptr)
        return;

    auto iter = itemToMeal.find(item->text());

    if (iter == itemToMeal.end())
        return;

    // get the meal associated with the item
    mealPtr = iter.value();

    // create edit window
    EditMeal_BasicParams *window = new EditMeal_BasicParams(this, mm, mealPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // refresh list of meals
    RefreshMealsList();
}

// item changed, display its assigned tags
void EditMeals::on_listWidget_meals_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Meal* mealPtr;
    QVector<Tag*> mealTags;
    QString mealKey = current->text();

    // clear all displayed tags
    ui->listWidget_tags->blockSignals(true);
    ui->listWidget_tags->clear();

    // get the meal associated with the item
    mealPtr = itemToMeal.find(mealKey).value();
    mealTags = mealPtr->getTags();

    // create listWidget items for every assigned tag
    for (auto& tagPtr : mealTags)
    {
        QString tempStr = "\nName: " + tagPtr->getName() + "\nDepends on MultiTag? ";
        tempStr += (tagPtr->getDependency()) ? "Yes" : "No";
        tempStr += "\nMaximum consecutive days: " + QString::number(tagPtr->getConsecutiveLimit());
        tempStr += "\nNumber of assigned Meals: " + QString::number(tagPtr->getLinkedMeals().count());

        if (tagPtr->isDisabled())
            tempStr += "\nTag is DISABLED";
        else // list the enabled days
        {
            tempStr + "\nEnabled on: " + mm->formatEnabledDays(tagPtr->getEnabledDays());
        }

        tempStr += "\nDesc.: ";

        if (tagPtr->getDescription().trimmed() == "")
            tempStr += "[none]\n";
        else
            tempStr += tagPtr->getDescription() + "\n";

        // add the item to the listWidget
        ui->listWidget_tags->addItem(tempStr);
        ui->listWidget_tags->blockSignals(false);
    }
}

// edit the tags assigned to the selected meal
void EditMeals::on_editTagsButton_clicked()
{
    Meal* mealPtr = nullptr;
    QListWidgetItem *currentItem = ui->listWidget_meals->currentItem();

    // check if item exists
    if (currentItem == nullptr)
    {
        return;
    }

    auto iter = itemToMeal.find(currentItem->text());

    if (iter == itemToMeal.end())
        return;

    // get the meal associated with the item
    mealPtr = iter.value();

    EditMeal_AssignedTags *window = new EditMeal_AssignedTags(this, mm, mealPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // refresh display of linked tags
    on_listWidget_meals_currentItemChanged(ui->listWidget_meals->currentItem(), nullptr);
}

// delete selected meal
void EditMeals::on_deleteMealButton_clicked()
{
    Meal* mealPtr;
    QListWidgetItem *currentItem = ui->listWidget_meals->currentItem();

    // check if item exists
    if (currentItem == nullptr)
        return;

    auto iter = itemToMeal.find(currentItem->text());

    if (iter == itemToMeal.end())
        return;

    // get the meal associated with the item
    mealPtr = iter.value();

    // display confirmation window, handles final deletion
    DeleteMeal_Confirmation *window = new DeleteMeal_Confirmation(this, mm, mealPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // refreshes list of meals to display
    RefreshMealsList();
}

// close this window
void EditMeals::on_exitButton_clicked()
{
    close();
}







