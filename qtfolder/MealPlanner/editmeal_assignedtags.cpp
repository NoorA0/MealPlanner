#include "editmeal_assignedtags.h"
#include "ui_editmeal_assignedtags.h"

EditMeal_AssignedTags::EditMeal_AssignedTags(QWidget *parent,
                                             MealManager *mm,
                                             Meal *mealPtr) :
    QDialog(parent),
    ui(new Ui::EditMeal_AssignedTags)
{
    if (mm == nullptr || mealPtr == nullptr)
        close();
    else
    {
        this->mm = mm;
        this->mealPtr = mealPtr;
    }

    itemToTag.clear();
    itemsInOrder_Assigned.clear();
    itemsInOrder_Unassigned.clear();

    ui->setupUi(this);

    // goal: itemize tags already assigned to mealPtr
    QVector<Tag*> assignedTags = mealPtr->getTags();

    for (auto& tag : assignedTags)
    {
        // create string for tag
        QString itemStr = "\nName: " + tag->getName()
                          + "\nDepends on MultiTag? ";
        itemStr += (tag->getDependency()) ? "Yes" : "No";
        itemStr += "\nMaximum consecutive days: " + QString::number(tag->getConsecutiveLimit());

        if (tag->isDisabled())
            itemStr += "\nTag is DISABLED";
        else
        {
            itemStr += "\nEnabled on: " + mm->formatEnabledDays(tag->getEnabledDays());
        }

        itemStr += "\nDescription: ";

        if (tag->getDescription().trimmed() == "")
            itemStr += "[none]\n";
        else
            itemStr += tag->getDescription() + "\n";

        // create entry in map
        itemToTag[itemStr] = tag;

        // add tag in alphabetical order
        if (itemsInOrder_Assigned.empty())
            itemsInOrder_Assigned.push_back(itemStr);
        else
        {
            bool done = false;
            auto itemIter = itemsInOrder_Assigned.begin();
            while (!done && itemIter != itemsInOrder_Assigned.end())
            {
                // get Tag* for item
                Tag* compareTag = itemToTag.value(*itemIter);
                // position found
                if (tag->getName().toUpper() < compareTag->getName().toUpper())
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

    // goal: itemize tags not assigned to mealPtr
    // get all tags
    QVector<Tag*> unassignedTags = mm->getNormalTags();

    // prune tags already allocated to mealPtr
    for (auto assignedTag : assignedTags)
    {
        bool found = false;
        auto unassignedTagIter = unassignedTags.begin();
        while (!found && unassignedTagIter != unassignedTags.end())
        {
            // if names match, then remove from unassignedTags
            if (assignedTag->getName() == (*unassignedTagIter)->getName())
            {
                found = true;
                unassignedTags.erase(unassignedTagIter);
            }
            else
                ++unassignedTagIter;
        }
    }

    // done pruning, now create items for remaining tags
    for (auto& tag : unassignedTags)
    {
        // create string for tag
        QString itemStr = "\nName: " + tag->getName()
                          + "\nDepends on MultiTag? ";
        itemStr += (tag->getDependency()) ? "Yes" : "No";
        itemStr += "\nMaximum consecutive days: " + QString::number(tag->getConsecutiveLimit()) + "\n";

        if (tag->isDisabled())
            itemStr += "Tag is DISABLED";
        else
        {
            itemStr += "\nEnabled on: " + mm->formatEnabledDays(tag->getEnabledDays());
        }
        itemStr += "\nDescription: " + tag->getDescription() + "\n";

        // create entry in map
        itemToTag[itemStr] = tag;

        // add tag in alphabetical order
        if (itemsInOrder_Unassigned.empty())
            itemsInOrder_Unassigned.push_back(itemStr);
        else
        {
            bool done = false;
            auto itemIter = itemsInOrder_Unassigned.begin();
            while (!done && itemIter != itemsInOrder_Unassigned.end())
            {
                // get Tag* for item
                Tag* compareTag = itemToTag.value(*itemIter);
                // position found
                if (tag->getName().toUpper() < compareTag->getName().toUpper())
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

    // display assigned tags
    for (auto& assignedItem : itemsInOrder_Assigned)
    {
        ui->listWidget_assignedTags->addItem(assignedItem);
    }

    // display available tags
    for (auto& unassignedItem : itemsInOrder_Unassigned)
    {
        ui->listWidget_unassignedTags->addItem(unassignedItem);
    }
}

EditMeal_AssignedTags::~EditMeal_AssignedTags()
{
    delete ui;
}

void EditMeal_AssignedTags::RefreshTagsList(void)
{
    // block signals while manipulating listWidgets
    ui->listWidget_assignedTags->blockSignals(true);
    ui->listWidget_unassignedTags->blockSignals(true);

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
        ui->listWidget_unassignedTags->addItem(item);
    }

    // unblock signals
    ui->listWidget_assignedTags->blockSignals(false);
    ui->listWidget_unassignedTags->blockSignals(false);
}

// done editing
void EditMeal_AssignedTags::on_pushButton_clicked()
{
    close();
}

// unassign tag
void EditMeal_AssignedTags::on_pushButton_unassignTag_clicked()
{
    // get selected item
    QListWidgetItem *currentItem = ui->listWidget_assignedTags->currentItem();
    QString currentItemText = "";
    bool positionFound = false;

    // check if item exists
    if (currentItem == nullptr)
        return;

    currentItemText = currentItem->text();

    // remove item from itemsInOrder_Assigned
    auto assignedItemIter = itemsInOrder_Assigned.begin();
    while (!positionFound && assignedItemIter != itemsInOrder_Assigned.end())
    {
        if (*assignedItemIter == currentItemText)
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
            itemsInOrder_Unassigned.push_back(currentItemText);
        else
        {
            auto unassignedItemIter = itemsInOrder_Unassigned.begin();
            positionFound = false;
            while (!positionFound && unassignedItemIter != itemsInOrder_Unassigned.end())
            {
                if (currentItemText < *unassignedItemIter)
                {
                    // position found, insert here
                    positionFound = true;
                    itemsInOrder_Unassigned.emplace(unassignedItemIter, currentItemText);
                }
                else
                    ++unassignedItemIter;
            }

            if (!positionFound)
            {
                itemsInOrder_Unassigned.push_back(currentItemText);
                positionFound = true;
            }
        }
    }
    // if item was removed from assigned list, then unlink in MealManager
    if (positionFound)
    {
        Tag* itemTag = itemToTag.find(currentItemText).value();
        QVector<Tag*> unassignTags;

        unassignTags.push_back(itemTag);
        mm->removeMealTags(mealPtr, unassignTags);
    }
    RefreshTagsList();
}

// assign tag
void EditMeal_AssignedTags::on_pushButton_assignTag_clicked()
{
    // get selected item
    QListWidgetItem *currentItem = ui->listWidget_unassignedTags->currentItem();
    QString currentItemText = "";
    bool positionFound = false;

    // check if item exists
    if (currentItem == nullptr)
        return;

    currentItemText = currentItem->text();

    // remove item from itemsInOrder_Unassigned
    auto unassignedItemIter = itemsInOrder_Unassigned.begin();
    while (!positionFound && unassignedItemIter != itemsInOrder_Unassigned.end())
    {
        if (*unassignedItemIter == currentItemText)
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
        auto assignedItemIter = itemsInOrder_Assigned.begin();
        positionFound = false;
        while (!positionFound && assignedItemIter != itemsInOrder_Assigned.end())
        {
            if (currentItemText < *assignedItemIter)
            {
                // position found, insert here
                positionFound = true;
                itemsInOrder_Assigned.emplace(assignedItemIter, currentItemText);
            }
            else
                ++assignedItemIter;
        }

        if (!positionFound)
        {
            positionFound = true;
            itemsInOrder_Assigned.push_back(currentItemText);
        }
    }

    // if item was found and moved to assigned list, then link the tag
    if (positionFound)
    {
        Tag* itemTag = itemToTag.find(currentItemText).value();
        QVector<Tag*> assignTags;

        assignTags.push_back(itemTag);
        mm->assignMealTags(mealPtr, assignTags);
    }
    RefreshTagsList();
}

