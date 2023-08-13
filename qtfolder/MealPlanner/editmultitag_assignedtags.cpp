#include "editmultitag_assignedtags.h"
#include "ui_editmultitag_assignedtags.h"
#include "editmultitag_assigntagmeals.h"

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

    // goal: itemize tags already assigned to this multitag
    QVector<QPair<Tag*, unsigned int>> assignedTags = tagPtr->getLinkedTags();

    for (auto& linkedTag : assignedTags)
    {
        // create string for tag
        QString itemStr = BuildItemString(linkedTag.first, linkedTag.second, true);

        // create entry in map
        itemToTag[itemStr] = QPair<Tag*, int>(linkedTag.first, linkedTag.second);

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
                Tag* compareTag = itemToTag.value(*itemIter).first;
                // position found
                if (linkedTag.first->getName().toUpper() < compareTag->getName().toUpper())
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

    // goal: itemize tags not assigned to multitag
    // get all tags
    QVector<Tag*> unassignedTags = mm->getNormalTags();

    // prune tags already allocated to multitag
    auto pruneTagIter = unassignedTags.begin();
    while (pruneTagIter != unassignedTags.end())
    {
        bool found = false;
        auto assignedTagIter = assignedTags.begin();
        // iterate through all tags in assignedTags to find match
        while (!found && assignedTagIter != assignedTags.end())
        {
            if ((*pruneTagIter)->getName() == assignedTagIter->first->getName())
            {
                // tag is allocated, remove from unassignedTags
                found = true;
                unassignedTags.erase(pruneTagIter);
            }
            else
                ++assignedTagIter;
        }
        ++pruneTagIter;
    }

    // done pruning, now create items for remaining unassigned tags
    for (auto& tag : unassignedTags)
    {
        // create string for tag
        QString itemStr = BuildItemString(tag, 0, false);

        // create entry in map
        itemToTag[itemStr] = QPair<Tag*, int>(tag, -1);

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
                Tag* compareTag = itemToTag.value(*itemIter).first;
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

QString EditMultitag_AssignedTags::BuildItemString(const Tag *tagPtr,
                        const unsigned int &requestedMeals,
                        const bool &displayRequestedMeals)
{
    QString itemStr = "\nName: " + tagPtr->getName()
                      + "\nDepends on MultiTag? ";
    itemStr += (tagPtr->getDependency()) ? "Yes" : "No";
    itemStr += "\nMaximum consecutive days: " + QString::number(tagPtr->getConsecutiveLimit()) + "\n";

    if (tagPtr->isDisabled())
        itemStr += "Tag is DISABLED";
    else
    {
        itemStr += "\nEnabled on: " + mm->formatEnabledDays(tagPtr->getEnabledDays());
    }

    if (displayRequestedMeals)
    {
        itemStr += "\nNumber of Meals requested from this Tag: " + QString::number(requestedMeals);
    }

    itemStr += "\nDescription: " + tagPtr->getDescription() + "\n";

    return itemStr;
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
    // get selected item
    QString selectedItem = ui->listWidget_assignedTags->currentItem()->text();
    Tag *targetTagPtr = itemToTag.find(selectedItem).value().first;
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

    // when unassigning, do not display the num. of requested meals
    //  this means the string for the item needs to be rebuilt,
    //  and itemToTag updated for this new string
    itemToTag.remove(selectedItem);
    selectedItem = BuildItemString(targetTagPtr, 0, false);
    itemToTag[selectedItem] = QPair<Tag*, int>(targetTagPtr, -1);

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

        // unassign tag from multitag
        QVector<Tag*> unassignTags;

        unassignTags.push_back(targetTagPtr);
        mm->removeMultiTagTags(tagPtr, unassignTags);
    }
    RefreshTagsList();
}

// assign tag
void EditMultitag_AssignedTags::on_pushButton_assignTag_clicked()
{
    // get selected item
    QString selectedItem = ui->listWidget_unassignedTags->currentItem()->text();
    Tag *targetTagPtr = itemToTag.find(selectedItem).value().first;
    bool positionFound = false;

    // prompt user for number of Meals requested from this Tag
    requestedMeals = -1; // to check for validity

    EditMultitag_AssignTagMeals *window = new EditMultitag_AssignTagMeals(this, mm, requestedMeals);
    window->setAttribute(Qt::WA_DeleteOnClose);

    // connect signal to receiver requestedMeals
    connect(window,
            SIGNAL(sendNumberRequestedMeals(uint)),
            this,
            SLOT(receiveRequestedMeals(uint)),
            Qt::UniqueConnection);

    window->exec();

    // proceed with assigning only if user gave a valid value
    if (requestedMeals > 0)
    {
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

        // when assigning, need to display the num. of requested meals
        //  this means the string for the item needs to be rebult,
        //  and itemToTag updated for this new string
        itemToTag.remove(selectedItem);
        selectedItem = BuildItemString(targetTagPtr, requestedMeals, true);
        itemToTag[selectedItem] = QPair<Tag*, int>(targetTagPtr, requestedMeals);

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

                // assign tag
                QVector<QPair<Tag*, int>> assignTags;

                assignTags.push_back(QPair<Tag*, int>(targetTagPtr, requestedMeals));
                mm->assignMultiTagTags(tagPtr, assignTags);
            }
        }
        RefreshTagsList();
    }
}

// edit the selected meal's requestedMeals
void EditMultitag_AssignedTags::on_pushButton_2_clicked()
{
    // get selected item
    QString selectedItem = ui->listWidget_unassignedTags->currentItem()->text();
    QPair<Tag*, int> targetTag = itemToTag.find(selectedItem).value();
    bool positionFound = false;

    // prompt user for number of Meals requested from this Tag
    // get current number of requestedMeals
    requestedMeals = targetTag.second;

    EditMultitag_AssignTagMeals *window = new EditMultitag_AssignTagMeals(this, mm, requestedMeals);
    window->setAttribute(Qt::WA_DeleteOnClose);

    // connect signal to receiver requestedMeals
    connect(window,
            SIGNAL(sendNumberRequestedMeals(uint)),
            this,
            SLOT(receiveRequestedMeals(uint)),
            Qt::UniqueConnection);

    window->exec();

    // proceed only if user gave a valid value
    if (requestedMeals > 0)
    {
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

        //  rebuild the item string and update itemToTag
        itemToTag.remove(selectedItem);
        selectedItem = BuildItemString(targetTag.first, requestedMeals, true);
        itemToTag[selectedItem] = QPair<Tag*, int>(targetTag.first, requestedMeals);

        // add item back to itemsInOrder_Assigned
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

                // set number of meals requested in multitag
                tagPtr->setLinkedTagRequestedMeals(targetTag.first, requestedMeals);
            }
        }
        RefreshTagsList();
    }
}

