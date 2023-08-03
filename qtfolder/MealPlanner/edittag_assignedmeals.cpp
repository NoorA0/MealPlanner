#include "edittag_assignedmeals.h"
#include "ui_edittag_assignedmeals.h"

EditTag_AssignedMeals::EditTag_AssignedMeals(QWidget *parent,
                                             MealManager *mm,
                                             Tag *tagPtr) :
    QDialog(parent),
    ui(new Ui::EditTag_AssignedMeals)
{
    if (mm == nullptr || tagPtr == nullptr)
        close();
    else
    {
        this->mm = mm;
        this->tagPtr = tagPtr;
    }
    ui->setupUi(this);

    // goal: itemize Meals already assigned to tagPtr
    QVector<Meal*> assignedMeals = tagPtr->getLinkedMeals();

    for (auto& meal : assignedMeals)
    {
        // create string for Meal
        QString itemStr = "\nName: " + meal->getName()
                          + "\nPrice: " + QString::number(meal->getPrice())
                          + "\nDuration: " + QString::number(meal->getMealDuration()) + " day(s)";

        // check if enabled
        if (meal->isDisabled())
            itemStr += "\nMeal is DISABLED";
        else
        {
            // get enabled days
            itemStr += "\nEnabled on: " + mm->formatEnabledDays(meal->getEnabledDays());
        }

        // create entry in map
        itemToMeal[itemStr] = meal;

        // add meal in alphabetical order
        if (itemsInOrder_Assigned.empty())
            itemsInOrder_Assigned.push_back(itemStr);
        else
        {
            bool done = false;
            auto itemIter = itemsInOrder_Assigned.begin();
            while (!done && itemIter != itemsInOrder_Assigned.end())
            {
                // get Meal* for item
                Meal* compareMeal = itemToMeal.value(*itemIter);
                // position found
                if (meal->getName().toUpper() < compareMeal->getName().toUpper())
                {
                    done = true;
                    itemsInOrder_Assigned.emplace(itemIter, itemStr);
                }
                else
                    ++itemIter;
            }

            if (!done)
                itemsInOrder_Assigned.push_back(itemStr);
        }
    }

    // goal: itemize Meals not assigned to tagPtr
    // get all Meals
    QVector<Meal*> unassignedMeals = mm->getMeals();

    // prune meals already allocated to tagPtr
    auto pruneMealIter = unassignedMeals.begin();
    while (pruneMealIter != unassignedMeals.end())
    {
        bool found = false;
        auto assignedMealIter = assignedMeals.begin();
        // iterate through all meals in assignedMeals to find match
        while (!found && assignedMealIter != assignedMeals.end())
        {
            if ((*pruneMealIter)->getName() == (*assignedMealIter)->getName())
            {
                // meal is allocated, remove from unassignedMeals
                found = true;
                unassignedMeals.erase(pruneMealIter);
            }
            else
                ++assignedMealIter;
        }
        ++pruneMealIter;
    }

    // done pruning, now create items for remaining meals
    for (auto& meal : unassignedMeals)
    {
        // create string for Meal
        QString itemStr = "\nName: " + meal->getName()
                          + "\nPrice: " + QString::number(meal->getPrice())
                          + "\nDuration: " + QString::number(meal->getMealDuration()) + " day(s)";

        // check if disabled
        if (meal->isDisabled())
            itemStr += "\nMeal is DISABLED";
        else
        {
            itemStr += "\nEnabled on: " + mm->formatEnabledDays(meal->getEnabledDays());
        }

        // create entry in map
        itemToMeal[itemStr] = meal;

        // add meal in alphabetical order
        if (itemsInOrder_Unassigned.empty())
            itemsInOrder_Unassigned.push_back(itemStr);
        else
        {
            bool done = false;
            auto itemIter = itemsInOrder_Unassigned.begin();
            while (!done && itemIter != itemsInOrder_Unassigned.end())
            {
                // get Meal* for item
                Meal* compareMeal = itemToMeal.value(*itemIter);
                // position found
                if (meal->getName().toUpper() < compareMeal->getName().toUpper())
                {
                    done = true;
                    itemsInOrder_Unassigned.emplace(itemIter, itemStr);
                }
                else
                    ++itemIter;
            }

            if (!done)
                itemsInOrder_Unassigned.push_back(itemStr);
        }
    }

    // display assigned meals
    for (auto& assignedItem : itemsInOrder_Assigned)
    {
        ui->listWidget_assignedMeals->addItem(assignedItem);
    }

    // display available meals
    for (auto& unassignedItem : itemsInOrder_Unassigned)
    {
        ui->listWidget_unassignedMeals->addItem(unassignedItem);
    }
}

EditTag_AssignedMeals::~EditTag_AssignedMeals()
{
    delete ui;
}

void EditTag_AssignedMeals::RefreshMealsList(void)
{
    // clear the listWidgets
    ui->listWidget_assignedMeals->clear();
    ui->listWidget_unassignedMeals->clear();

    // repopulate the listWidgets
    for (auto& item : itemsInOrder_Assigned)
    {
        ui->listWidget_assignedMeals->addItem(item);
    }
    for (auto& item : itemsInOrder_Unassigned)
    {
        ui->listWidget_assignedMeals->addItem(item);
    }
}

// unassign selected meal
void EditTag_AssignedMeals::on_pushButton_unassignMeal_clicked()
{
    // get selected item
    QString selectedItem = ui->listWidget_assignedMeals->currentItem()->text();
    bool positionFound = false;

    // remove item from itemsInOrder_Assigned
    auto assignedItemIter = itemsInOrder_Assigned.begin();
    while (!positionFound && assignedItemIter != itemsInOrder_Assigned.end())
    {
        if (*assignedItemIter == selectedItem)
        {
            // remove item
            positionFound = true;
            itemsInOrder_Assigned.erase(assignedItemIter);
        }
        else
            ++assignedItemIter;
    }

    // add item to itemsInOrder_Unassigned
    if (positionFound)
    {
        if (itemsInOrder_Unassigned.empty())
            itemsInOrder_Unassigned.push_back(selectedItem);
        else
        {
            auto unassignedItemIter = itemsInOrder_Unassigned.begin();
            positionFound = false;
            while (!positionFound && unassignedItemIter != itemsInOrder_Unassigned.end())
            {
                if (selectedItem < *unassignedItemIter)
                {
                    // position found, insert here
                    positionFound = true;
                    itemsInOrder_Unassigned.emplace(unassignedItemIter, selectedItem);
                }
                else
                    ++unassignedItemIter;
            }

            if (!positionFound)
            {
                itemsInOrder_Unassigned.push_back(selectedItem);
                positionFound = true;
            }
        }

        // unassign meal
        Meal* itemMeal = itemToMeal.find(selectedItem).value();
        QVector<Meal*> unassignMeals;

        unassignMeals.push_back(itemMeal);
        mm->removeNormalTagMeals(tagPtr, unassignMeals);
    }
    RefreshMealsList();
}

// assign selected meal
void EditTag_AssignedMeals::on_pushButton_assignMeal_clicked()
{
    // get selected item
    QString selectedItem = ui->listWidget_unassignedMeals->currentItem()->text();
    bool positionFound = false;

    // remove item from itemsInOrder_Unassigned
    auto unassignedItemIter = itemsInOrder_Unassigned.begin();
    while (!positionFound && unassignedItemIter != itemsInOrder_Unassigned.end())
    {
        if (*unassignedItemIter == selectedItem)
        {
            // remove item
            positionFound = true;
            itemsInOrder_Unassigned.erase(unassignedItemIter);
        }
        else
            ++unassignedItemIter;
    }

    // add item to itemsInOrder_Assigned
    if (positionFound)
    {
        if (itemsInOrder_Assigned.empty())
            itemsInOrder_Assigned.push_back(selectedItem);
        else
        {
            auto assignedItemIter = itemsInOrder_Assigned.begin();
            positionFound = false;
            while (!positionFound && assignedItemIter != itemsInOrder_Assigned.end())
            {
                if (selectedItem < *assignedItemIter)
                {
                    // position found, insert here
                    positionFound = true;
                    itemsInOrder_Assigned.emplace(assignedItemIter, selectedItem);
                }
                else
                    ++assignedItemIter;
            }

            if (!positionFound)
            {
                itemsInOrder_Assigned.push_back(selectedItem);
                positionFound = true;
            }

            // assign meal
            Meal* itemMeal = itemToMeal.find(selectedItem).value();
            QVector<Meal*> assignMeals;

            assignMeals.push_back(itemMeal);
            mm->assignNormalTagMeals(tagPtr, assignMeals);
        }
    }
    RefreshMealsList();
}

// clicked done
void EditTag_AssignedMeals::on_pushButton_clicked()
{
    close();
}

