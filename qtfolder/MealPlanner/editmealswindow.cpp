#include "editmealswindow.h"
#include "editmeal_basicparams.h"
#include "ui_editmealswindow.h"
#include "createmeal_basicparams.h"
#include "createmeal_promptlinktags.h"
#include "createmeal_notagswarning.h"
#include <QDebug>

EditMealsWindow::EditMealsWindow(QWidget *parent, MealManager *mm) :
    QMainWindow(parent),
    ui(new Ui::EditMealsWindow)
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

EditMealsWindow::~EditMealsWindow()
{
    delete ui;

    // unhide the previous window
    QMainWindow* mw = qobject_cast<QMainWindow*>(parent());
    if (mw != nullptr)
    {
        mw->show();
    }
}

void EditMealsWindow::RebuildItems(void)
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
            tempStr += "\nEnabled on: " + CreateDaysEnabledString(mealEnabledDays) + "\n";
        }

        // create new item
        itemToMeal[tempStr] = mealPtr;
        itemsInOrder.push_back(tempStr);
    }
}

void EditMealsWindow::RefreshMealsList(void)
{
    // make sure items are up to date
    RebuildItems();

    // clear the listWidget
    ui->listWidget_meals->clear();

    // repopulate the listWidget
    for (auto& item : itemsInOrder)
    {
        ui->listWidget_meals->addItem(item);
    }
}

QString EditMealsWindow::CreateDaysEnabledString(const QMap<DaysOfTheWeek, bool> enabledDays)
{
    QString retStr = "[";
    bool needComma = false;

    // check all days of the week
    if (enabledDays.find(MONDAY).value())
    {
        retStr += "Mon";
        needComma = true;
    }

    if (enabledDays.find(TUESDAY).value())
    {
        if (needComma)
            retStr += ", ";
        retStr += "Tue";
    }

    if (enabledDays.find(WEDNESDAY).value())
    {
        if (needComma)
            retStr += ", ";
        retStr += "Wed";
    }

    if (enabledDays.find(THURSDAY).value())
    {
        if (needComma)
            retStr += ", ";
        retStr += "Thu";
    }

    if (enabledDays.find(FRIDAY).value())
    {
        if (needComma)
            retStr += ", ";
        retStr += "Fri";
    }

    if (enabledDays.find(SATURDAY).value())
    {
        if (needComma)
            retStr += ", ";
        retStr += "Sat";
    }

    if (enabledDays.find(SUNDAY).value())
    {
        if (needComma)
            retStr += ", ";
        retStr += "Sun";
    }
    retStr += "]";
    return retStr;
}

// go back to settings menu
void EditMealsWindow::on_pushButton_clicked()
{
    close();
}

// edit selected item, same as double clicking. for convenience.
void EditMealsWindow::on_pushButton_2_clicked()
{
    // pass selected item to on_listWidget_itemDoubleClicked()
    on_listWidget_meals_itemDoubleClicked(ui->listWidget_meals->currentItem());
}

// item double clicked
void EditMealsWindow::on_listWidget_meals_itemDoubleClicked(QListWidgetItem *item)
{
    Meal* mealPtr;
    QString mealKey = item->text();

    // get the meal associated with the item
    mealPtr = itemToMeal.find(mealKey).value();

    EditMeal_BasicParams *window = new EditMeal_BasicParams(this, mm, mealPtr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();
}

// item changed, display it's assigned tags
void EditMealsWindow::on_listWidget_meals_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Meal* mealPtr;
    QVector<Tag*> mealTags;
    QString mealKey = current->text();

    // clear all displayed tags
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
        tempStr += "\nNumber of assigned Meals: " + QString::number(tagPtr->getLinkedMeals().count()) + "\n";

        if (tagPtr->isDisabled())
            tempStr += "Tag is DISABLED";
        else // list the enabled days
        {
            QMap<DaysOfTheWeek, bool> tagEnabledDays = tagPtr->getEnabledDays();

            tempStr += "Tag is ENABLED";
            tempStr + "\nEnabled on: " + CreateDaysEnabledString(tagEnabledDays);
        }
        tempStr += "\nDescription: " + tagPtr->getDescription() + "\n";

        // add the item to the listWidget
        ui->listWidget_tags->addItem(tempStr);
    }
}

// edit tags assigned for selected meal
void EditMealsWindow::on_pushButton_3_clicked()
{
    Meal* mealPtr;
    QString mealKey = ui->listWidget_meals->currentItem()->text();

    // get the meal associated with the item
    mealPtr = itemToMeal.find(mealKey).value();

    // TODO: display tag window to continue

}

// create new meal
void EditMealsWindow::on_pushButton_4_clicked()
{
    // create meal by getting basic info
    createmeal_basicparams *window = new createmeal_basicparams(this, this->mm);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // if no tags exist, warn user
    if (mm->getNumberOfNormalTags() <= 0)
    {
        CreateMeal_NoTagsWarning *window2 = new CreateMeal_NoTagsWarning(this);
        window2->setAttribute(Qt::WA_DeleteOnClose);
        window2->exec();
    }
    else
    {
        CreateMeal_PromptLinkTags *window2 = new CreateMeal_PromptLinkTags(this);
        window2->setAttribute(Qt::WA_DeleteOnClose);
        window2->exec();

        // TODO: if user said yes, go to edit tag menu for the selected meal
    }

    // refresh list of meals
    RefreshMealsList();
}

// delete selected meal
void EditMealsWindow::on_pushButton_5_clicked()
{
    // TODO: create a confirmation dialogue window, move this code to if the user says yes

    Meal* mealPtr;
    QString mealKey = ui->listWidget_meals->currentItem()->text();

    // get the meal associated with the item
    mealPtr = itemToMeal.find(mealKey).value();

    // delete the meal
    mm->deleteMeal(mealPtr);

    // refreshes list of meals to display
    RefreshMealsList();
}

