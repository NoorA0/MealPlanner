#include "mealmanager.h"
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <sstream>
#include <algorithm>

MealManager::MealManager()
{

}

MealManager::~MealManager()
{
    // delete all tags
    for (auto tagsIter : normalTags)
    {
        // clear tag's linkedMeals
        tagsIter->clearLinkedMeals();
        delete tagsIter;
    }
    normalTags.clear();

    // delete all meals
    for (auto mealsIter : meals)
    {
        // clear vector of tags
        QVector<Tag*> emptyVec;
        mealsIter->setTags(emptyVec);

        delete mealsIter;
    }
    meals.clear();

    // delete all multiTags
    for (auto tagsIter : multiTags)
    {
        // clear map of Tags
        QMap<Tag*, unsigned int> emptyMap;
        tagsIter->setLinkedTags(emptyMap);

        delete tagsIter;
    }
    multiTags.clear();
}

Meal* MealManager::createMeal(const QString &mealName,
                            const double &mealPrice,
                            const int &mealDuration,
                            const int &mealDBO)
{
    bool isInvalid = false;
    Meal* mealptr;
    bool inputValid = false;

    mealptr = new Meal();

    // set initial values
    mealptr->setName(mealName);
    mealptr->setPrice(mealPrice);
    mealptr->setMealDuration(mealDuration);
    mealptr->setDaysBetweenOccurrences(mealDBO);

    // if there are other meals stored, check if names conflict (case insensitive)
    if (meals.size() > 0)
    {
        // get uppercase version of name
        QString upperStr = mealName.toUpper();

        // compare with names of all meals until a match is found
        // also find a place to insert into (alphabetical order)
        bool placeFound = false;
        inputValid = true;
        auto mealsIter = meals.begin();
        while (inputValid && !placeFound && mealsIter != meals.end())
        {
            // get uppercase version of meal's name
            QString compareName = (*mealsIter)->getName().toUpper();

            // check if names match
            if (upperStr == compareName)
                inputValid = false;
            else if (upperStr < compareName) // check if is this a good place to insert
            {
                // insert here
                placeFound = true;
                meals.emplace(mealsIter, mealptr);
            }
            else // keep looking
                ++mealsIter;
        }

        // check if name is valid but place not found
        if (inputValid && !placeFound)
        {
            // place at back of meals
            meals.push_back(mealptr);
        }
        else if (!inputValid) // invalid name
        {
            isInvalid = true;
        }
    }
    else // name is ok, push to back of meals
    {
        meals.push_back(mealptr);
    }

    // enable meal if input was valid, otherwise delete
    if (!isInvalid)
        mealptr->setDisabled(false);
    else
    {
        delete mealptr;
        mealptr = nullptr;
    }

    return mealptr;
}

Meal* MealManager::findMeal(const QString &mealName)
{
    Meal *retMeal = nullptr;
    bool inputValid = false;

    // if there are other meals stored, check if names conflict (case insensitive)
    if (meals.size() > 0)
    {
        // get uppercase version of name
        QString upperStr = mealName.toUpper();

        // compare with names of all meals until a match is found
        inputValid = true;
        auto mealsIter = meals.begin();
        while (inputValid && mealsIter != meals.end())
        {
            // get uppercase version of meal's name
            QString compareName = (*mealsIter)->getName().toUpper();

            // check if names match
            if (upperStr == compareName)
            {
                inputValid = false;
                retMeal = *mealsIter;
            }
            else // keep looking
                ++mealsIter;
        }
    }
    return retMeal;
}

bool MealManager::resortMeal(Meal *mealPtr, const QString &newName)
{
    bool hasFailed = false;

    // make sure meal exists
    if (findMeal(mealPtr->getName()) == nullptr)
    {
        hasFailed = true;
    }
    // if new and old name are functionally the same, rename
    else if (mealPtr->getName().toUpper() == newName.toUpper())
    {
        mealPtr->setName(newName);
    }
    // conflict with new name, abort re-sort
    else if (meals.size() > 0 && findMeal(newName) != nullptr)
    {
        hasFailed = true;
    }
    // perform re-sort if can find old meal
    else if (meals.size() > 0)
    {
        // prepare to remove meal from meals
        auto mealsIter = meals.begin();
        bool mealFound = false;

        // find meal
        while (!mealFound && mealsIter != meals.end())
        {
            if (*mealsIter == mealPtr)
                mealFound = true;
            else
                ++mealsIter;
        }

        // remove meal
        if (mealFound)
        {
            meals.erase(mealsIter);
        }

        // rename meal
        mealPtr->setName(newName);

        // get uppercase version of new name
        QString upperStr = newName.toUpper();

        // find a place to insert into (alphabetical order)
        mealFound = false;
        mealsIter = meals.begin();
        while (!mealFound && mealsIter != meals.end())
        {
            // get uppercase version of meal's name
            QString compareName = (*mealsIter)->getName().toUpper();

            // position found
            if (upperStr < compareName) // check if is this a good place to insert
            {
                // insert here
                mealFound = true;
                meals.emplace(mealsIter, mealPtr);
            }
            else // keep looking
                ++mealsIter;
        }

        // check if place not found
        if (!mealFound)
        {
            // place at back of meals
            meals.push_back(mealPtr);
        }
    }
    else // order doesn't matter
    {
        mealPtr->setName(newName);
    }
    return hasFailed;
}


void MealManager::deleteMeal(Meal* mealPtr)
{
    // get assigned tags
    QVector<Tag*> assignedTags = mealPtr->getTags();

    // if meal has assigned tags, unlink them from meal
    if (assignedTags.size() > 0)
    {
        for (auto tagIter : assignedTags)
        {
            tagIter->removeMeal(mealPtr);
        }

        // clear tags
        mealPtr->clearTags();
    }

    // prepare to remove meal from meals
    bool found = false;
    auto mealIter = meals.begin();

    // find meal
    while (!found && mealIter != meals.end())
    {
        if (*mealIter == mealPtr)
            found = true;
        else
            ++mealIter;
    }

    // delete meal
    if (found)
    {
        meals.erase(mealIter);
        delete mealPtr;
        mealPtr = nullptr;
    }
}

Tag* MealManager::findNormalTag(const QString &tagName)
{
    Tag *retTag = nullptr;
    bool inputValid = false;

    // if there are other tags stored, check if names conflict (case insensitive)
    if (normalTags.size() > 0)
    {
        // get uppercase version of name
        QString upperStr = tagName.toUpper();

        // compare with names of all tags until a match is found
        inputValid = true;
        auto tagsIter = normalTags.begin();
        while (inputValid && tagsIter != normalTags.end())
        {
            // get uppercase version of tag's name
            QString compareName = (*tagsIter)->getName().toUpper();

            // check if names match
            if (upperStr == compareName)
            {
                inputValid = false;
                retTag = *tagsIter;
            }
            else // keep looking
                ++tagsIter;
        }
    }
    return retTag;
}

bool MealManager::resortNormalTag(Tag *tagPtr, const QString &newName)
{
    bool hasFailed = false;

    // make sure tag exists
    if (findNormalTag(tagPtr->getName()) == nullptr)
    {
        hasFailed = true;
    }
    // if new and old name are functionally the same, rename
    else if (tagPtr->getName().toUpper() == newName.toUpper())
    {
        tagPtr->setName(newName);
    }
    // conflict with new name, abort re-sort
    else if (normalTags.size() > 0 && findNormalTag(newName) != nullptr)
    {
        hasFailed = true;
    }
    // perform re-sort if can find old tag
    else if (normalTags.size() > 0)
    {
        // prepare to remove tag from normalTags
        auto tagsIter = normalTags.begin();
        bool tagFound = false;

        // find tag
        while (!tagFound && tagsIter != normalTags.end())
        {
            if (*tagsIter == tagPtr)
                tagFound = true;
            else
                ++tagsIter;
        }

        // remove tag
        if (tagFound)
        {
            normalTags.erase(tagsIter);
        }

        // rename tag
        tagPtr->setName(newName);

        // get uppercase version of new name
        QString upperStr = newName.toUpper();

        // find a place to insert into (alphabetical order)
        tagFound = false;
        tagsIter = normalTags.begin();
        while (!tagFound && tagsIter != normalTags.end())
        {
            // get uppercase version of tag's name
            QString compareName = (*tagsIter)->getName().toUpper();

            // position found
            if (upperStr < compareName) // check if is this a good place to insert
            {
                // insert here
                tagFound = true;
                normalTags.emplace(tagsIter, tagPtr);
            }
            else // keep looking
                ++tagsIter;
        }

        // check if place not found
        if (!tagFound)
        {
            // place at back of normalTags
            normalTags.push_back(tagPtr);
        }
    }
    else // order doesn't matter
    {
        tagPtr->setName(newName);
    }
    return hasFailed;
}

Tag* MealManager::createNormalTag(const QString &name,
               const QString &description,
               const bool &dependsOnMultiTag,
               const int &consecutiveLimit,
               const QMap<DaysOfTheWeek, bool> &enabledDays)
{
    Tag* tagPtr;
    bool isInvalid = false;
    bool inputValid = false;

    // create a new Tag and set values
    tagPtr = new Tag;
    tagPtr->setName(name);
    tagPtr->setDescription(description);
    tagPtr->setDependency(dependsOnMultiTag);
    tagPtr->setConsecutiveLimit(consecutiveLimit);
    tagPtr->setEnabledDays(enabledDays);


    // if there are other tags stored, check if names conflict (case insensitive)
    if (normalTags.size() > 0)
    {
        // get uppercase version of name
        QString upperStr = tagPtr->getName().toUpper();

        // compare with names of all tags until a match is found
        // also find a place to insert into (alphabetical order)
        bool placeFound = false;
        inputValid = true;
        auto tagsIter = normalTags.begin();
        while (inputValid && !placeFound && tagsIter != normalTags.end())
        {
            // get uppercase version of tag's name
            QString compareName = (*tagsIter)->getName().toUpper();

            // check if names match
            if (upperStr == compareName)
                inputValid = false;
            else if (upperStr < compareName) // check if this is a good place to insert
            {
                // insert here
                placeFound = true;
                normalTags.emplace(tagsIter, tagPtr);
            }
            else // keep searching
                ++tagsIter;
        }

        // check if name is valid but place not found
        if (inputValid && !placeFound)
        {
            // push to back
            normalTags.push_back(tagPtr);
        }
        else if (!inputValid) // invalid name, inform user
        {
            isInvalid = true;
        }
    }
    else // no tags to compare so push to back
    {
        // push to back
        normalTags.push_back(tagPtr);
    }

    // delete tag if failed to create
    if (isInvalid)
    {
        delete tagPtr;
        tagPtr = nullptr;
    }
    return tagPtr;
}

void MealManager::deleteNormalTag(Tag* tagPtr)
{
    // get linked meals
    QVector<Meal*> linkedMeals = tagPtr->getLinkedMeals();

    // if tag has linked meals, unlink them
    if (linkedMeals.size() > 0)
    {
        for (auto mealIter : linkedMeals)
        {
            mealIter->removeTag(tagPtr);
        }

        // clear linkedMeals
        tagPtr->clearLinkedMeals();
    }

    // unlink tag from all multiTags
    for (auto mtagIter : multiTags)
    {
        mtagIter->removeLinkedTag(tagPtr);
    }

    // find tag in normalTags
    auto tagIter = normalTags.begin();
    bool found = false;

    while (!found && tagIter != normalTags.end())
    {
        if (*tagIter == tagPtr)
            found = true;
        else
            ++tagIter;
    }

    // remove from normalTags and delete
    if (found)
    {
        normalTags.erase(tagIter);
        delete tagPtr;
        tagPtr = nullptr;
    }
}

void MealManager::createMultiTag(MultiTag* mtagPtr)
{
    QString tempStr = "";
    int tempInt;
    QVector<QString> strVec;
    bool doneCreatingName = false;

    // ensure pointer is instantiated to an object
    if (mtagPtr == nullptr)
        mtagPtr = new MultiTag();

    // multitag's params
    QMap<DaysOfTheWeek, bool> enabledDays =
        { {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false}, {FRIDAY, false},
         {SATURDAY, false}, {SUNDAY, false} };


    // loop while user is not done choosing a name
    while (!doneCreatingName)
    {
        // prompt and get name
        // store in tempStr

        // if there are other tags stored, check if names conflict (case insensitive)
        if (multiTags.size() > 0)
        {
            // get uppercase version of tempStr
            QString upperStr = tempStr.toUpper();

            // compare with names of all tags until a match is found
            bool placeFound = false;
            bool inputValid = true;
            auto tagsIter = multiTags.begin();
            while (inputValid && !placeFound && tagsIter != multiTags.end())
            {
                // get uppercase version of tag's name
                QString compareName = (*tagsIter)->getName().toUpper();

                // check if names match
                if (upperStr == compareName)
                    inputValid = false;
                else if (upperStr < compareName) // check if this is a good place to insert
                {
                    // insert here
                    placeFound = true;
                    mtagPtr->setName(tempStr);
                    multiTags.emplace(tagsIter, mtagPtr);
                    doneCreatingName = true;
                }
                else // keep looking
                    ++tagsIter;
            }

            // check if name is valid but place not found
            if (inputValid && !placeFound)
            {
                // set name and place at back of multitags
                mtagPtr->setName(tempStr);
                multiTags.push_back(mtagPtr);
                doneCreatingName = true;
            }
            else if (!inputValid) // invalid name, inform user
            {
                // display error that name is invalid because it already exists
            }
        }
        else // no tags to compare so push to back of meals
        {
            mtagPtr->setName(tempStr);
            multiTags.push_back(mtagPtr);
            doneCreatingName = true;
        }
    } // while (!doneCreatingName)

    // prompt and get description of tag
    // TODO set to value of mtagPtr->setDescription()

    // prompt and get dependency
    // TODO store in tempStr

    // set priority depending on user choice
    if (tempStr == "1")
        mtagPtr->setHighestPriority(true);
    else
        mtagPtr->setHighestPriority(false);

    tempStr = "";
    // let user set enabled days or quit
    while (tempStr != "Q")
    {
        // TODO prompt list each day of the week to decide if enabled or disabled
        // this code prints the status of each day

        tempStr = "MON: ";
        if (enabledDays.find(MONDAY).value())
            tempStr += "enabled";
        else
            tempStr += "disabled";

        tempStr = "TUE: ";
        if (enabledDays.find(TUESDAY).value())
            tempStr += "enabled";
        else
            tempStr += "disabled";

        tempStr = "WED: ";
        if (enabledDays.find(WEDNESDAY).value())
            tempStr += "enabled";
        else
            tempStr += "disabled";

        tempStr = "THU: ";
        if (enabledDays.find(THURSDAY).value())
            tempStr += "enabled";
        else
            tempStr += "disabled";

        tempStr = "FRI: ";
        if (enabledDays.find(FRIDAY).value())
            tempStr += "enabled";
        else
            tempStr += "disabled";

        tempStr = "SAT: ";
        if (enabledDays.find(SATURDAY).value())
            tempStr += "enabled";
        else
            tempStr += "disabled";

        tempStr = "SUN: ";
        if (enabledDays.find(SUNDAY).value())
            tempStr += "enabled";
        else
            tempStr += "disabled";

        // print options
        strVec.clear();
        strVec = { "1Toggle MONDAY", "2Toggle TUESDAY", "3Toggle WEDNESDAY", "4Toggle THURSDAY",
                  "5Toggle FRIDAY", "6Toggle SATURDAY", "7Toggle SUNDAY", "- ",
                  "EEnable ALL", "DDisable ALL","- ", "QExit & Confirm Selection" };

        // prompt and get input, store in tempStr

        if (tempStr == "E")
        {
            // set all days to TRUE
            enabledDays =
                { {MONDAY, true}, {TUESDAY, true}, {WEDNESDAY, true}, {THURSDAY, true},
                 {FRIDAY, true}, {SATURDAY, true}, {SUNDAY, true} };
        }
        else if (tempStr == "D")
        {
            // set all days to FALSE
            enabledDays =
                { {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
                 {FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };
        }
        else if (tempStr != "Q" && tempStr != "-")
        {
            // get user's choice as integer, store in tempStr
            // TODO: dont need this at all with proper UI
            switch (tempInt)
            {
            case 1: // monday
                if (enabledDays.find(MONDAY).value())
                    enabledDays.find(MONDAY).value() = false;
                else
                    enabledDays.find(MONDAY).value() = true;
                break;
            case 2: // tuesday
                if (enabledDays.find(TUESDAY).value())
                    enabledDays.find(TUESDAY).value() = false;
                else
                    enabledDays.find(TUESDAY).value() = true;
                break;
            case 3: // wednesday
                if (enabledDays.find(WEDNESDAY).value())
                    enabledDays.find(WEDNESDAY).value() = false;
                else
                    enabledDays.find(WEDNESDAY).value() = true;
                break;
            case 4: // thursday
                if (enabledDays.find(THURSDAY).value())
                    enabledDays.find(THURSDAY).value() = false;
                else
                    enabledDays.find(THURSDAY).value() = true;
                break;
            case 5: // friday
                if (enabledDays.find(FRIDAY).value())
                    enabledDays.find(FRIDAY).value() = false;
                else
                    enabledDays.find(FRIDAY).value() = true;
                break;
            case 6: // saturday
                if (enabledDays.find(SATURDAY).value())
                    enabledDays.find(SATURDAY).value() = false;
                else
                    enabledDays.find(SATURDAY).value() = true;
                break;
            case 7: // sunday
                if (enabledDays.find(SUNDAY).value())
                    enabledDays.find(SUNDAY).value() = false;
                else
                    enabledDays.find(SUNDAY).value() = true;
                break;
            default:
                // do nothing
                tempStr = "";
            }
        }
    } // while (tempStr != "Q")

    // set enabledDays
    mtagPtr->setEnabledDays(enabledDays);

    // link to other tags
    editMultiTagTags(mtagPtr);

    // set requireFulfillment
    // TODO

    // set priority depending on user choice
    if (tempStr == "1")
        mtagPtr->setRequireFulfillment(true);
    else
        mtagPtr->setRequireFulfillment(false);
}

void MealManager::deleteMultiTag(MultiTag* mtagPtr)
{
    // clear linkedTags
    mtagPtr->clearLinkedTags();

    // find MultiTag in multiTags
    auto tagIter = multiTags.begin();
    bool found = false;

    while (!found && tagIter != multiTags.end())
    {
        if (*tagIter == mtagPtr)
            found = true;
        else
            ++tagIter;
    }

    // remove from multiTags and delete
    if (found)
    {
        multiTags.erase(tagIter);
        delete mtagPtr;
        mtagPtr = nullptr;
    }
}

void MealManager::assignMealTags(Meal* mealPtr, QVector<Tag*> newTags)
{
    for (auto& newTag : newTags)
    {
        // link Tag to Meal
        mealPtr->addTag(newTag);
        // link Meal to Tag
        newTag->addMeal(mealPtr);
    }
}

void MealManager::removeMealTags(Meal* mealPtr, QVector<Tag*> removeTags)
{
    for (auto& assignedTag : removeTags)
    {
        // remove Tag from Meal
        mealPtr->removeTag(assignedTag);
        // remove Meal from Tag
        assignedTag->removeMeal(mealPtr);
    }
}

void MealManager::assignNormalTagMeals(Tag* tagPtr, QVector<Meal*> newMeals)
{
    for (auto& newMeal : newMeals)
    {
        // link meal to tag
        tagPtr->addMeal(newMeal);
        // link tag to meal
        newMeal->addTag(tagPtr);
    }
}

void MealManager::removeNormalTagMeals(Tag* tagPtr, QVector<Meal*> removeMeals)
{
    for (auto& assignedMeal : removeMeals)
    {
        // remove meal from tag
        tagPtr->removeMeal(assignedMeal);
        // remove tag from meal
        assignedMeal->removeTag(tagPtr);
    }
}

QString MealManager::formatEnabledDays(const QMap<DaysOfTheWeek, bool>& enabledDays)
{
    QString returnStr = "[";
    bool oneDayEnabled = false; // used to determine format between days

    // check days that are disabled by tags
    if (enabledDays.value(MONDAY))
    {
        returnStr += "Mon";
        oneDayEnabled = true;
    }

    if (enabledDays.value(TUESDAY))
    {
        // add spacing
        if (oneDayEnabled)
            returnStr += ", ";
        else
            oneDayEnabled = true;

        returnStr += "Tue";
    }

    if (enabledDays.value(WEDNESDAY))
    {
        // add spacing
        if (oneDayEnabled)
            returnStr += ", ";
        else
            oneDayEnabled = true;

        returnStr += "Wed";
    }

    if (enabledDays.value(THURSDAY))
    {
        // add spacing
        if (oneDayEnabled)
            returnStr += ", ";
        else
            oneDayEnabled = true;

        returnStr += "Thu";
    }

    if (enabledDays.value(FRIDAY))
    {
        // add spacing
        if (oneDayEnabled)
            returnStr += ", ";
        else
            oneDayEnabled = true;

        returnStr += "Fri";
    }

    if (enabledDays.value(SATURDAY))
    {
        // add spacing
        if (oneDayEnabled)
            returnStr += ", ";
        else
            oneDayEnabled = true;

        returnStr += "Sat";
    }

    if (enabledDays.value(SUNDAY))
    {
        // add spacing
        if (oneDayEnabled)
            returnStr += ", ";
        else
            oneDayEnabled = true;

        returnStr += "Sun";
    }

    if (!oneDayEnabled)
        returnStr += "DISABLED";

    // close bracket
    returnStr += "]";

    return returnStr;
}

void MealManager::optimizeData(QMap<DaysOfTheWeek, QVector<MultiTag*>>& highPriorityMultiTags,
                               QMap<DaysOfTheWeek, QVector<MultiTag*>>& normalPriorityMultiTags,
                               QMap<DaysOfTheWeek, QVector<Tag*>>& normalPriorityTags)
{
    DaysOfTheWeek day = MONDAY; // adds elements to function's params for each day of the week

    // ensure params are clear
    highPriorityMultiTags.clear();
    normalPriorityMultiTags.clear();
    normalPriorityTags.clear();

    // create an entry for each day of the week
    do
    {
        QVector<Tag*> tagEntry;
        QVector<MultiTag*> multiTagEntry;

        // create element in params
        highPriorityMultiTags.emplace(day, multiTagEntry);
        normalPriorityMultiTags.emplace(day, multiTagEntry);
        normalPriorityTags.emplace(day, tagEntry);

        day = nextDay(day); // go to next day
    } while (day != MONDAY);

    // populates normalPriorityTags
    do
    {
        // check all tags
        for (auto tagIter : normalTags)
        {
            // check if enabled for current day and does not depend on a multitag
            if (tagIter->getEnabledDays().at(day) && !tagIter->getDependency())
            {
                // copy vector of normalTags in normalPriorityTags
                QVector<Tag*> tagEntry = normalPriorityTags.at(day);

                // add tagIter to tagEntry
                tagEntry.push_back(tagIter);

                // store back into normalPriorityTags
                normalPriorityTags.at(day) = tagEntry;
            }
        }
        day = nextDay(day); // go to next day
    } while (day != MONDAY);

    // populates highPriorityMultiTags and normalPriorityMultiTags
    do
    {
        // check all multiTags
        for (auto multiTagIter : multiTags)
        {
            // check if enabled for current day
            if (multiTagIter->getEnabledDays().at(day))
            {
                // check if multiTagsIter is disabled by its linked tags
                QMap<Tag*, unsigned int> linkedTags = multiTagIter->getLinkedTags();

                bool isDisabled = true;
                auto linkedTagIter = linkedTags.begin();
                while (isDisabled && linkedTagIter != linkedTags.end())
                {
                    // if linked tag is enabled, then quit loop
                    if (linkedTagIter->first->getEnabledDays().at(day))
                        isDisabled = false;
                    else
                        ++linkedTagIter;
                }

                // if multitag is not disabled
                if (!isDisabled)
                {
                    // check if multiTag is high or normal priority
                    if (multiTagIter->getPriority())
                    {
                        // copy vector of high priority multitags
                        QVector<MultiTag*> tagEntry = highPriorityMultiTags.at(day);

                        // add multiTag to vector
                        tagEntry.push_back(multiTagIter);

                        // store back into highPriorityMultiTags
                        highPriorityMultiTags.at(day) = tagEntry;
                    }
                    else
                    {
                        // copy vector of normal priority multitags
                        QVector<MultiTag*> tagEntry = normalPriorityMultiTags.at(day);

                        // add multiTag to vector
                        tagEntry.push_back(multiTagIter);

                        // store back into normalPriorityMultiTags
                        normalPriorityMultiTags.at(day) = tagEntry;
                    }
                }
            }
        }
        day = nextDay(day); // go to next day
    } while (day != MONDAY);
}

void MealManager::writeMeal(const Meal* mealPtr, std::ofstream& oFile)
{
    // name
    oFile << "\t<Name>\n";
    oFile << "\t" << mealPtr->getName() << "\n";
    oFile << "\t</Name>";

    oFile << "\n";

    // price
    oFile << "\t<Price>\n";
    oFile << "\t" << formatPrice(mealPtr->getPrice()) << "\n";
    oFile << "\t</Price>";

    oFile << "\n";

    // isDisabled
    oFile << "\t<IsDisabled>\n";
    oFile << "\t" << mealPtr->isDisabled() << "\n";
    oFile << "\t</IsDisabled>";

    oFile << "\n";

    // mealDuration
    oFile << "\t<MealDuration>\n";
    oFile << "\t" << std::to_string(mealPtr->getMealDuration()) << "\n";
    oFile << "\t</MealDuration>";

    oFile << "\n";

    // daysBetweenOccurrences
    oFile << "\t<DaysBetweenOccurrences>\n";
    oFile << "\t" << std::to_string(mealPtr->getDaysBetweenOccurrences()) << "\n";
    oFile << "\t</DaysBetweenOccurrences>";

    oFile << "\n";

    // name of Tags assigned
    oFile << "\t<MealTags>\n";

    // loop to write all names
    for (auto tagIter : mealPtr->getTags())
    {
        oFile << "\t\t<TagName>\n";
        oFile << "\t\t" << tagIter->getName() << "\n";
        oFile << "\t\t</TagName>\n";
    }

    oFile << "\t</MealTags>";

}

void MealManager::writeTag(const Tag* tagPtr, std::ofstream& oFile)
{
    // name
    oFile << "\t<Name>\n";
    oFile << "\t" << tagPtr->getName() << "\n";
    oFile << "\t</Name>";

    oFile << "\n";

    // description
    oFile << "\t<Description>\n";
    oFile << "\t" << tagPtr->getDescription() << "\n";
    oFile << "\t</Description>";

    oFile << "\n";

    // dependsOnMultitag
    oFile << "\t<DependsOnMultiTag>\n";
    oFile << "\t" << tagPtr->getDependency() << "\n";
    oFile << "\t</DependsOnMultiTag>";

    oFile << "\n";

    // consecutiveLimit
    oFile << "\t<ConsecutiveLimit>\n";
    oFile << "\t" << std::to_string(tagPtr->getConsecutiveLimit()) << "\n";
    oFile << "\t</ConsecutiveLimit>";

    oFile << "\n";

    // enabledDays
    oFile << "\t<EnabledDays>\n";

    // loop to write enabledDays
    for (auto daysIter : tagPtr->getEnabledDays())
    {
        oFile << "\t" << daysIter.second << "\n";
    }
    oFile << "\t</EnabledDays>";
}

void MealManager::writeMultiTag(const MultiTag* mtagPtr, std::ofstream& oFile)
{
    // name
    oFile << "\t<Name>\n";
    oFile << "\t" << mtagPtr->getName() << "\n";
    oFile << "\t</Name>";

    oFile << "\n";

    // description
    oFile << "\t<Description>\n";
    oFile << "\t" << mtagPtr->getDescription() << "\n";
    oFile << "\t</Description>";

    oFile << "\n";

    // highestPriority
    oFile << "\t<HasPriority>\n";
    oFile << "\t" << mtagPtr->getPriority() << "\n";
    oFile << "\t</HasPriority>";

    oFile << "\n";

    // requiresFulfillment
    oFile << "\t<RequiresFulfillment>\n";
    oFile << "\t" << mtagPtr->getRequireFulfillment() << "\n";
    oFile << "\t</RequiresFulfillment>";

    oFile << "\n";

    // enabledDays
    oFile << "\t<EnabledDays>\n";

    // loop to write enabledDays
    for (auto daysIter : mtagPtr->getEnabledDays())
    {
        oFile << "\t" << daysIter.second << "\n";
    }
    oFile << "\t</EnabledDays>";

    oFile << "\n";

    // linked Tags
    oFile << "\t<LinkedTags>\n";

    // loop to write linkedTags
    for (auto linksIter : mtagPtr->getLinkedTags())
    {
        oFile << "\t\t<TagData>\n";
        // name of tag
        oFile << "\t\t" << linksIter.first->getName() << "\n";

        // amount requested
        oFile << "\t\t" << std::to_string(linksIter.second) << "\n";
        oFile << "\t\t</TagData>\n";
    }
    oFile << "\t</LinkedTags>";
}

int MealManager::readMeals(std::ifstream& iFile)
{
    int returnVal = 0; // 0 = good, 1 = error (aborted import), 2 = corrupted (used default values)
    bool error = false; // true when encountering a read error
    bool done = false; // when read all meals
    bool corrupted = false; // true if read values are invalid
    Meal* mealPtr = nullptr; // for creating meals
    QString tempStr; // read lines from file

    // while not done importing
    while (!done)
    {
        // check if name exists
        std::getline(iFile, tempStr);
        if (tempStr == "\t<Name>")
        {
            mealPtr = new Meal;

            // get name
            std::getline(iFile, tempStr);

            // remove tab at beginning
            tempStr.erase(0, 1);

            // check if string is correct length
            if (tempStr.length() > NAME_LENGTH)
            {
                tempStr = tempStr.substr(0, NAME_LENGTH); // insert up to character limit
                corrupted = true;
            }

            // set name
            mealPtr->setName(tempStr);

            // read end header, verify
            std::getline(iFile, tempStr);
            if (tempStr != "\t</Name>")
            {
                done = true;
                error = true;
            }
        }
        else if (tempStr == "</Meals>") // end of meals
            done = true;
        else // unknown position
        {
            done = true;
            error = true;
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if price exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<Price>")
            {
                // get price
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // validate price
                try
                {
                    double price = std::stod(tempStr);

                    if (price <= MAXIMUM_PRICE && price >= MINIMUM_PRICE)
                        mealPtr->setPrice(price);
                    else
                    {
                        mealPtr->setPrice(0); // invalid price, default to 0
                        corrupted = true;
                    }
                }
                catch (...) // value not valid, default to 0
                {
                    mealPtr->setPrice(0);
                    corrupted = true;
                }

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</Price>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if isDisabled exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<IsDisabled>")
            {
                // get value
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // validate before setting value
                if (std::isdigit(tempStr.at(0)))
                    mealPtr->setDisabled(std::stoi(tempStr.substr(0, 1)));
                else
                {
                    mealPtr->setDisabled(true); // default to true
                    corrupted = true;
                }

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</IsDisabled>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if mealDuration exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<MealDuration>")
            {
                // get value
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // validate before setting value
                if (std::isdigit(tempStr.at(0)))
                {
                    int digits = std::to_string(MEAL_DURATION_LIMIT).length(); // maximum number of digits
                    int charLimit = 0;
                    bool isValid = true;

                    if (digits >= tempStr.length()) // string has less than maximum number of digits
                        charLimit = tempStr.length();
                    else // string is too long
                    {
                        charLimit = digits;
                        corrupted = true;

                        tempStr = tempStr.substr(0, digits); // limit to correct number of digits
                    }

                    // validate every character in tempStr
                    int index = 0;
                    while (isValid && index < charLimit)
                    {
                        if (!std::isdigit(tempStr.at(index)))
                            isValid = false;
                        ++index;
                    }

                    if (isValid) // is a number
                    {
                        unsigned int number = std::stoi(tempStr);

                        // check if number is <= limit
                        if (number <= MEAL_DURATION_LIMIT)
                            mealPtr->setMealDuration(number);
                        else // default to 1
                        {
                            mealPtr->setMealDuration(1);
                            corrupted = true;
                        }
                    }
                    else // NaN, default to 1
                    {
                        mealPtr->setMealDuration(1);
                        corrupted = true;
                    }
                }
                else // NaN, default to 1
                {
                    mealPtr->setMealDuration(1);
                    corrupted = true;
                }

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</MealDuration>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if daysBetweenOccurrences exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<DaysBetweenOccurrences>")
            {
                // get value
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // validate before setting value
                if (std::isdigit(tempStr.at(0)))
                {
                    int digits = std::to_string(DAYS_BETWEEN_OCCURRENCES_LIMIT).length(); // maximum number of digits
                    int charLimit = 0;
                    bool isValid = true;

                    if (digits >= tempStr.length()) // string has less than maximum number of digits
                        charLimit = tempStr.length();
                    else // string is too long
                    {
                        charLimit = digits;
                        corrupted = true;

                        tempStr = tempStr.substr(0, digits); // limit to correct number of digits
                    }

                    // validate every character in tempStr
                    int index = 0;
                    while (isValid && index < charLimit)
                    {
                        if (!std::isdigit(tempStr.at(index)))
                            isValid = false;
                        ++index;
                    }

                    if (isValid) // is a number
                    {
                        unsigned int number = std::stoi(tempStr);

                        // check if number is <= limit
                        if (number <= DAYS_BETWEEN_OCCURRENCES_LIMIT)
                            mealPtr->setDaysBetweenOccurrences(number);
                        else // default to 0
                        {
                            mealPtr->setDaysBetweenOccurrences(0);
                            corrupted = true;
                        }
                    }
                    else // NaN, default to 0
                    {
                        mealPtr->setDaysBetweenOccurrences(0);
                        corrupted = true;
                    }
                }
                else // NaN, default to 0
                {
                    mealPtr->setDaysBetweenOccurrences(0);
                    corrupted = true;
                }

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</DaysBetweenOccurrences>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if MealTags exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<MealTags>")
            {
                bool doneGettingLinks = false;

                do
                {
                    // get start of tag data or end of assigned tags
                    std::getline(iFile, tempStr);
                    if (tempStr == "\t\t<TagName>")
                    {
                        bool matchFound = false;

                        // get name of tag
                        std::getline(iFile, tempStr);

                        // remove 2 tabs
                        tempStr.erase(0, 2);

                        // validate length
                        if (tempStr.length() > NAME_LENGTH)
                        {
                            tempStr = tempStr.substr(0, NAME_LENGTH);
                            corrupted = true;
                        }

                        // find a tag with this name
                        auto tagIter = normalTags.begin();
                        while (!matchFound && tagIter != normalTags.end())
                        {
                            // if match found, assign tag to meal, and meal to tag
                            if (tempStr == (*tagIter)->getName())
                            {
                                matchFound = true;
                                mealPtr->addTag(*tagIter);
                                (*tagIter)->addMeal(mealPtr);
                            }
                            else
                                ++tagIter;
                        }

                        // read end header, verify
                        std::getline(iFile, tempStr);
                        if (tempStr != "\t\t</TagName>")
                        {
                            doneGettingLinks = true;
                            done = true;
                            error = true;
                        }
                    }
                    else if (tempStr == "\t</MealTags>")
                        doneGettingLinks = true;
                    else // error
                    {
                        doneGettingLinks = true;
                        error = true;
                    }

                } while (!doneGettingLinks);
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // check if a meal was created
        if (!error && mealPtr != nullptr)
        {
            // got all parameters, load into meals
            meals.push_back(mealPtr);
            mealPtr = nullptr;
        }

        if (!done)
        {
            // check if another meal exists
            std::getline(iFile, tempStr);
            if (tempStr == "</Meals>")
                done = true;
        }

    } // while (!done && !iFile.eof())

    // if error, delete meal
    if (error && mealPtr != nullptr)
        delete mealPtr;

    if (error) // if had to abort, return value
        returnVal = error;
    else if (corrupted) // if corruption detected
        returnVal = 2;

    return returnVal;
}

int MealManager::readTags(std::ifstream& iFile)
{
    int returnVal = 0; // 0 = good, 1 = error (aborted import), 2 = corrupted (used default values)
    bool error = false; // true when encountering a read error
    bool done = false; // when read all tags
    bool corrupted = false;

    Tag* tagPtr = nullptr; // for creating Tags
    QString tempStr; // read lines from file
    QMap<DaysOfTheWeek, bool> enabledDays = // to create EnabledDays in Tags
        { {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
         {FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };

    // while not done importing or ran into end of file
    while (!done && !iFile.eof())
    {
        // check if name exists
        std::getline(iFile, tempStr);
        if (tempStr == "\t<Name>")
        {
            tagPtr = new Tag;

            // get name
            std::getline(iFile, tempStr);

            // remove tab at beginning
            tempStr.erase(0, 1);

            // check if string is correct length
            if (tempStr.length() > NAME_LENGTH)
            {
                tempStr= tempStr.substr(0, NAME_LENGTH); // get up to character limit
                corrupted = true;
            }

            // set value
            tagPtr->setName(tempStr);

            // read end header, verify
            std::getline(iFile, tempStr);
            if (tempStr != "\t</Name>")
            {
                done = true;
                error = true;
            }
        }
        else if (tempStr == "</Tags>") // end of tags
            done = true;
        else // unknown position
        {
            done = true;
            error = true;
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if description exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<Description>")
            {
                // get description
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // check if string is correct length
                if (tempStr.length() > DESC_LENGTH)
                {
                    tempStr = tempStr.substr(0, DESC_LENGTH); // get up to character limit
                    corrupted = true;
                }

                // set value
                tagPtr->setDescription(tempStr);

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</Description>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if dependsOnMultiTag exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<DependsOnMultiTag>")
            {
                // get value
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // validate before setting value
                if (std::isdigit(tempStr.at(0)))
                    tagPtr->setDependency(std::stoi(tempStr.substr(0, 1)));
                else
                {
                    tagPtr->setDependency(false); // default to false
                    corrupted = true;
                }

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</DependsOnMultiTag>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if ConsecutiveLimit exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<ConsecutiveLimit>")
            {
                // get value
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // validate before setting value
                if (std::isdigit(tempStr.at(0)))
                {
                    int digits = std::to_string(CONSECUTIVE_DAYS_LIMIT).length(); // maximum number of digits
                    int charLimit = 0;
                    bool isValid = true;

                    if (digits >= tempStr.length()) // string has less than maximum number of digits
                        charLimit = tempStr.length();
                    else // string is too long
                    {
                        charLimit = digits;
                        corrupted = true;

                        tempStr = tempStr.substr(0, digits); // limit to correct number of digits
                    }

                    // validate every character in tempStr
                    int index = 0;
                    while (isValid && index < charLimit)
                    {
                        if (!std::isdigit(tempStr.at(index)))
                            isValid = false;
                        ++index;
                    }

                    if (isValid) // is a number
                    {
                        unsigned int number = std::stoi(tempStr);

                        // check if number is <= limit
                        if (number <= CONSECUTIVE_DAYS_LIMIT)
                            tagPtr->setConsecutiveLimit(number);
                        else // default to 0
                        {
                            tagPtr->setConsecutiveLimit(0);
                            corrupted = true;
                        }
                    }
                    else // NaN, default to 0
                    {
                        tagPtr->setConsecutiveLimit(0);
                        corrupted = true;
                    }
                }
                else // NaN, default to 0
                {
                    tagPtr->setConsecutiveLimit(0);
                    corrupted = true;
                }

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</ConsecutiveLimit>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if EnabledDays exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<EnabledDays>")
            {
                // loop to get all days
                for (int day = 0; day < 7; ++day)
                {
                    // get value
                    std::getline(iFile, tempStr);

                    // remove tab at beginning
                    tempStr.erase(0, 1);

                    // validate before setting value
                    if (!std::isdigit(tempStr.at(0)) || tempStr.length() > 1)
                    {
                        tempStr = "0"; // default to false
                        corrupted = true;
                    }

                    // set value
                    switch (day)
                    {
                    case 0: // monday
                        enabledDays.at(MONDAY) = std::stoi(tempStr);
                        break;
                    case 1: // tuesday
                        enabledDays.at(TUESDAY) = std::stoi(tempStr);
                        break;
                    case 2: // wednesday
                        enabledDays.at(WEDNESDAY) = std::stoi(tempStr);
                        break;
                    case 3: // thursday
                        enabledDays.at(THURSDAY) = std::stoi(tempStr);
                        break;
                    case 4: // friday
                        enabledDays.at(FRIDAY) = std::stoi(tempStr);
                        break;
                    case 5: // saturday
                        enabledDays.at(SATURDAY) = std::stoi(tempStr);
                        break;
                    case 6: // sunday
                        enabledDays.at(SUNDAY) = std::stoi(tempStr);
                        break;
                    default:
                        error = true;
                        day = 10; // exit loop
                    }
                }
                tagPtr->setEnabledDays(enabledDays);

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</EnabledDays>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // check if a tag was created
        if (!error && tagPtr != nullptr)
        {
            // got all parameters, load into normalTags
            normalTags.push_back(tagPtr);
            tagPtr = nullptr;
        }

        if (!done)
        {
            // check if another tag exists
            std::getline(iFile, tempStr);
            if (tempStr == "</Tags>")
                done = true;
        }

    } // while (!done && !iFile.eof())

    // if error, delete tagPtr
    if (error && tagPtr != nullptr)
        delete tagPtr;

    // if met end of file, error
    if (iFile.eof())
        error = true;

    if (error)
        returnVal = 1;
    else if (corrupted)
        returnVal = 2;

    return returnVal;
}

int MealManager::readMultiTags(std::ifstream& iFile)
{
    int returnVal = 0; // 0 = good, 1 = error (aborted import), 2 = corrupted (used default values)
    bool error = false; // true when encountering a read error
    bool done = false; // when read all tags
    bool corrupted = false;

    MultiTag* mtagPtr = nullptr; // for creating MultiTags
    QString tempStr; // read lines from file

    QMap<DaysOfTheWeek, bool> enabledDays = // to create EnabledDays in Tags
        { {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
         {FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };

    // while not done importing or ran into end of file
    while (!done && !iFile.eof())
    {
        // check if name exists
        std::getline(iFile, tempStr);
        if (tempStr == "\t<Name>")
        {
            mtagPtr = new MultiTag;

            // get name
            std::getline(iFile, tempStr);

            // remove tab at beginning
            tempStr.erase(0, 1);

            // check if string is correct length
            if (tempStr.length() > NAME_LENGTH)
            {
                tempStr = tempStr.substr(0, NAME_LENGTH); // get up to character limit
                corrupted = true;
            }

            // set value
            mtagPtr->setName(tempStr);

            // read end header, verify
            std::getline(iFile, tempStr);
            if (tempStr != "\t</Name>")
            {
                done = true;
                error = true;
            }
        }
        else if (tempStr == "</MultiTags>") // end of multitags
            done = true;
        else // unknown position
        {
            done = true;
            error = true;
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if description exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<Description>")
            {
                // get description
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // check if string is correct length
                if (tempStr.length() > DESC_LENGTH)
                {
                    tempStr = tempStr.substr(0, DESC_LENGTH); // get up to character limit
                    corrupted = true;
                }

                // set value
                mtagPtr->setDescription(tempStr);

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</Description>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if hasPriority exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<HasPriority>")
            {
                // get value
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // validate before setting value
                if (std::isdigit(tempStr.at(0)))
                    mtagPtr->setHighestPriority(std::stoi(tempStr.substr(0, 1)));
                else
                {
                    mtagPtr->setHighestPriority(false); // default to false
                    corrupted = true;
                }

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</HasPriority>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if requiresFulfillment exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<RequiresFulfillment>")
            {
                // get value
                std::getline(iFile, tempStr);

                // remove tab at beginning
                tempStr.erase(0, 1);

                // validate before setting value
                if (std::isdigit(tempStr.at(0)))
                    mtagPtr->setRequireFulfillment(std::stoi(tempStr.substr(0, 1)));
                else
                {
                    mtagPtr->setRequireFulfillment(false); // default to false
                    corrupted = true;
                }

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</RequiresFulfillment>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if EnabledDays exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<EnabledDays>")
            {
                // loop to get all days
                for (int day = 0; day < 7; ++day)
                {
                    // get value
                    std::getline(iFile, tempStr);

                    // remove tab at beginning
                    tempStr.erase(0, 1);

                    // validate before setting value
                    if (!std::isdigit(tempStr.at(0)) || tempStr.length() > 1)
                    {
                        tempStr = "0"; // default to false
                        corrupted = true;
                    }

                    // set value
                    switch (day)
                    {
                    case 0: // monday
                        enabledDays.at(MONDAY) = std::stoi(tempStr);
                        break;
                    case 1: // tuesday
                        enabledDays.at(TUESDAY) = std::stoi(tempStr);
                        break;
                    case 2: // wednesday
                        enabledDays.at(WEDNESDAY) = std::stoi(tempStr);
                        break;
                    case 3: // thursday
                        enabledDays.at(THURSDAY) = std::stoi(tempStr);
                        break;
                    case 4: // friday
                        enabledDays.at(FRIDAY) = std::stoi(tempStr);
                        break;
                    case 5: // saturday
                        enabledDays.at(SATURDAY) = std::stoi(tempStr);
                        break;
                    case 6: // sunday
                        enabledDays.at(SUNDAY) = std::stoi(tempStr);
                        break;
                    default:
                        error = true;
                        day = 10; // exit loop
                    }
                }
                mtagPtr->setEnabledDays(enabledDays);

                // read end header, verify
                std::getline(iFile, tempStr);
                if (tempStr != "\t</EnabledDays>")
                {
                    done = true;
                    error = true;
                }
            }
            else // error
            {
                done = true;
                error = true;
            }
        }

        // continue if no error and not done
        if (!error && !done)
        {
            // check if LinkedTags exists
            std::getline(iFile, tempStr);
            if (tempStr == "\t<LinkedTags>")
            {
                bool doneGettingLinks = false;

                do
                {
                    // get start of tag data or end of LinkedTags
                    std::getline(iFile, tempStr);
                    if (tempStr == "\t\t<TagData>")
                    {
                        Tag* tagPtr = nullptr;
                        bool matchFound = false;

                        // get name of tag
                        std::getline(iFile, tempStr);

                        // remove 2 tabs
                        tempStr.erase(0, 2);

                        // check if string is correct length
                        if (tempStr.length() > NAME_LENGTH)
                        {
                            tempStr = tempStr.substr(0, NAME_LENGTH); // get up to character limit
                            corrupted = true;
                        }

                        // find a tag with this name
                        auto tagIter = normalTags.begin();
                        while (!matchFound && tagIter != normalTags.end())
                        {
                            // if match found, set tagPtr to this tag
                            if (tempStr == (*tagIter)->getName())
                            {
                                tagPtr = *tagIter;
                                matchFound = true;
                            }
                            else
                                ++tagIter;
                        }

                        // get requested amount of meals for this tag
                        std::getline(iFile, tempStr);

                        // remove 2 tabs
                        tempStr.erase(0, 2);

                        // validate before setting value
                        if (std::isdigit(tempStr.at(0)))
                        {
                            int digits = std::to_string(REQUESTED_MEALS_LIMIT).length(); // maximum number of digits
                            int charLimit = 0;
                            bool isValid = true;

                            if (digits >= tempStr.length()) // string has less than maximum number of digits
                                charLimit = tempStr.length();
                            else // string is too long
                            {
                                charLimit = digits;
                                corrupted = true;

                                tempStr = tempStr.substr(0, digits); // limit to correct number of digits
                            }

                            // validate every character in tempStr
                            int index = 0;
                            while (isValid && index < charLimit)
                            {
                                if (!std::isdigit(tempStr.at(index)))
                                    isValid = false;
                                ++index;
                            }

                            if (isValid) // is a number
                            {
                                unsigned int number = std::stoi(tempStr);

                                // check if number is > limit
                                if (number > REQUESTED_MEALS_LIMIT)
                                {
                                    tempStr = "1"; // defaults to 1
                                    corrupted = true;
                                }
                            }
                            else // NaN, default to 1
                            {
                                tempStr = "1";
                                corrupted = true;
                            }
                        }
                        else // NaN, default to 1
                        {
                            tempStr = "1";
                            corrupted = true;
                        }

                        // set values
                        mtagPtr->addLinkedTag(tagPtr, std::stoi(tempStr));

                        // read end header, verify
                        std::getline(iFile, tempStr);
                        if (tempStr != "\t\t</TagData>")
                        {
                            doneGettingLinks = true;
                            done = true;
                            error = true;
                        }
                    }
                    else if (tempStr == "\t</LinkedTags>")
                        doneGettingLinks = true;
                    else // error
                    {
                        doneGettingLinks = true;
                        error = true;
                    }

                } while (!doneGettingLinks);
            }
            else // error
            {
                done = true;
                error = true;
            }
        }


        // check if a multiTag was created
        if (!error && mtagPtr != nullptr)
        {
            // got all parameters, load into MultiTags
            multiTags.push_back(mtagPtr);
            mtagPtr = nullptr;
        }

        if (!done)
        {
            // check if another tag exists
            std::getline(iFile, tempStr);
            if (tempStr == "</MultiTags>")
                done = true;
        }
    } // while (!done && !iFile.eof())

    // if error, delete tagPtr
    if (error && mtagPtr != nullptr)
        delete mtagPtr;

    // if met end of file, error
    if (iFile.eof())
        error = true;

    if (error)
        returnVal = error;
    else if (corrupted)
        returnVal = 2;

    return returnVal;
}

bool MealManager::validateTag(const Tag* tagPtr, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod, const QVector<Meal*>& assignedMeals, const QVector<QVector<Meal*>>& scheduledMeals)
{
    bool validTag = false;
    int lookBack; // checks previous days
    int lookBackLimit; // the furthest day in the past necessary to check

    // set to previous day if one exists
    if (currentDayNumber > 0)
        lookBack = currentDayNumber - 1;
    else
        lookBack = 0;

    // find earliest day to check if safe
    if (currentDayNumber >= tagPtr->getConsecutiveLimit())
        lookBackLimit = currentDayNumber - tagPtr->getConsecutiveLimit();
    else
        lookBackLimit = 0;

    auto assignedMealsIter = assignedMeals.begin(); // iter to meals assigned to the tag

    // if consecutiveLimit is greater than calculationPeriod, or not enough days has passed, then skip detailed check
    if (tagPtr->getConsecutiveLimit() < calculationPeriod && currentDayNumber >= tagPtr->getConsecutiveLimit() && currentDayNumber > 0)
    {
        // loop until all days checked or a gap in scheduled days is found
        while (!validTag && lookBack >= lookBackLimit)
        {
            // get meals of day lookBack
            QVector<Meal*> checkMeals = scheduledMeals.at(lookBack);

            auto scheduledMealsIter = checkMeals.begin();
            bool matchFound = false; // true when any of the previous day's meals was assigned to the tag being checked

            // check all meals of the day
            while (!validTag && scheduledMealsIter != checkMeals.end())
            {
                // loop to compare scheduled meals with assigned meals
                while (!matchFound && assignedMealsIter != assignedMeals.end())
                {
                    // if a meal matches
                    if (*assignedMealsIter == *scheduledMealsIter)
                        matchFound = true;

                    ++assignedMealsIter;
                }

                ++scheduledMealsIter;
            }

            // if no match was found, then the tag is valid
            if (!matchFound)
                validTag = true;

            --lookBack;
        }
    }
    else
        validTag = true;

    return validTag;
}

bool MealManager::validateMeal(const Meal* mealPtr, const unsigned int& currentDayNumber)
{
    bool validMeal = false;

    // if meal is enabled, check further
    if (!mealPtr->isDisabled())
    {
        // if meal was never scheduled, then daysBetweenOccurrences will not prevent scheduling
        if (mealPtr->getDaysScheduled().size() > 0)
        {
            // get most recent day scheduled
            unsigned int lastDayScheduled = mealPtr->getDaysScheduled().back();

            // calculate how long to wait until selectedMeal can be scheduled again
            unsigned int waitUntil = lastDayScheduled + mealPtr->getDaysBetweenOccurrences() + 1;

            // if enough days has passed, then meal is valid
            if (currentDayNumber >= waitUntil)
                validMeal = true;
        }
        else
            validMeal = true;
    }

    return validMeal;
}

void MealManager::addFutureMeals(QVector<Meal*>& futureMeals, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod, QVector<QVector<Meal*>>& scheduledMeals)
{
    // schedule all meals in futureMeals
    for (auto mealIter : futureMeals)
    {
        // add each future day meal is scheduled into Meal::daysScheduled
        for (unsigned int daysInAdvance = 1; daysInAdvance < mealIter->getMealDuration(); ++daysInAdvance)
        {
            // if the future day is not beyond the calculationPeriod
            if (currentDayNumber + daysInAdvance < calculationPeriod)
            {
                // update Meal::daysScheduled
                mealIter->addDayScheduled(currentDayNumber + daysInAdvance);

                // if a vector of meals has already been created in the future day, then add this meal to it
                if (scheduledMeals.size() - 1 >= currentDayNumber + daysInAdvance)
                {
                    // retreive existing future day's meals
                    QVector<Meal*> existingFutureDay = scheduledMeals.at(currentDayNumber + daysInAdvance);

                    // add this meal to the scheduled meals in a future day
                    existingFutureDay.push_back(mealIter);

                    // update the meals in the future day
                    scheduledMeals.at(currentDayNumber + daysInAdvance) = existingFutureDay;
                }
                else // create meals on future day in scheduledMeals
                {
                    QVector<Meal*> futureDay; // meals scheduled for a future day
                    futureDay.push_back(mealIter); // add meal to futureDay

                    // create an entry for meals in a future day
                    scheduledMeals.push_back(futureDay);
                }
            }

        }
    }
}

bool MealManager::scheduleMultiTags(const QVector<MultiTag*>& availableMultiTags, const unsigned int& currentDayNumber,
                                    const unsigned int& calculationPeriod, const DaysOfTheWeek& currentDay, QVector<QVector<Meal*>>& scheduledMeals)
{
    QVector<int> searchedMultiTags; // stores indices of multitags that were accessed
    bool mealFound = false;

    // while there are multitags to choose from
    while (!mealFound && searchedMultiTags.size() < availableMultiTags.size())
    {
        bool validMultiTag = false;

        // randomly choose index
        int multitagIndex = rand() % availableMultiTags.size();

        // retrieve multiTag
        MultiTag* chosenMultiTag = availableMultiTags.at(multitagIndex);

        // if no indices previously searched, then ok to proceed
        if (searchedMultiTags.size() == 0)
        {
            searchedMultiTags.push_back(multitagIndex);
            validMultiTag = true;
        }
        else // need to check if index was already searched
        {
            bool indexMatch = false;
            auto searchedMultiTagsIter = searchedMultiTags.begin();

            while (!indexMatch && searchedMultiTagsIter != searchedMultiTags.end())
            {
                if (multitagIndex == *searchedMultiTagsIter)
                    indexMatch = true;
                else
                    ++searchedMultiTagsIter;
            }

            // if no match, then ok to proceed
            if (!indexMatch)
            {
                searchedMultiTags.push_back(multitagIndex);
                validMultiTag = true;
            }
        }

        // if a valid index was found, search tags for a meal
        if (validMultiTag)
        {
            QMap<Tag*, unsigned int> availableTags = chosenMultiTag->getLinkedTags(); // get linked tags of multitag
            QVector<Meal*> mealsOfDay; // meals scheduled for the current day
            QVector<Meal*> futureMeals; // meals that last more than one day are grouped together
            unsigned int requiredMeals = 0; // meals needed to consider MultiTag valid
            unsigned int totalMealsScheduled = 0; // meals scheduled for MultiTag

            // check if MultiTag requires all Tags to be fulfilled
            if (chosenMultiTag->getRequireFulfillment())
            {
                // sum all of the required meals
                for (auto linkedTags : availableTags)
                {
                    requiredMeals += linkedTags.second;
                }
            }
            else requiredMeals = 1; // only require 1 meal

            // update mealsOfDay if meals were already scheduled (multi-day meals)
            if (scheduledMeals.size() >= currentDayNumber + 1)
                mealsOfDay = scheduledMeals.at(currentDayNumber);

            // all linked tags' requested Meals must be fulfilled
            for (auto tagIter : availableTags)
            {
                unsigned int mealsScheduled = 0; // meals scheduled for Tag
                QVector<Meal*> assignedMeals = tagIter.first->getLinkedMeals(); // meals that are assigned to the current Tag
                QVector<int> searchedMeals; // tracks which meals were already searched

                // check if tag is enabled and has meals
                if (tagIter.first->getEnabledDays().at(currentDay) && assignedMeals.size() > 0)
                {
                    bool validTag = false;

                    // checks tag's params and returns T if valid, F if not
                    validTag = validateTag(tagIter.first, currentDayNumber, calculationPeriod, assignedMeals, scheduledMeals);

                    // begin to schedule meals if tag is allowed to schedule meals
                    if (validTag)
                    {
                        // ends loop when requested number of meals are met or all meals were searched
                        while (mealsScheduled < tagIter.second && searchedMeals.size() < assignedMeals.size())
                        {
                            bool validMeal = false;

                            // randomly choose index of a meal
                            int mealIndex = rand() % assignedMeals.size();

                            // get meal
                            Meal* selectedMeal = assignedMeals.at(mealIndex);

                            // if no meals were searched, then add this meal to searchedMeals
                            if (searchedMeals.size() == 0)
                            {
                                searchedMeals.push_back(mealIndex);
                                validMeal = validateMeal(selectedMeal, currentDayNumber);
                            }
                            else // check if meal has been used before
                            {
                                bool matchFound = false;
                                auto searchedMealsIter = searchedMeals.begin();

                                // compare mealIndex with those that have been previously used
                                while (!matchFound && searchedMealsIter != searchedMeals.end())
                                {
                                    if (mealIndex == *searchedMealsIter)
                                        matchFound = true;
                                    else
                                        ++searchedMealsIter;
                                }

                                // if no match, then meal is a candidate to be scheduled
                                if (!matchFound)
                                {
                                    searchedMeals.push_back(mealIndex);
                                    validMeal = validateMeal(selectedMeal, currentDayNumber);
                                }
                            }

                            // if meal is valid so far, then check if it has already been scheduled (a multi-day meal)
                            if (validMeal)
                            {
                                // if there are meals to check
                                if (mealsOfDay.size() > 0)
                                {
                                    auto mealIter = mealsOfDay.begin();
                                    bool found = false;

                                    while (!found && mealIter != mealsOfDay.end())
                                    {
                                        if (*mealIter == selectedMeal)
                                        {
                                            found = true;
                                            validMeal = false;
                                        }
                                        else
                                            ++mealIter;
                                    }
                                }
                            }

                            // try to schedule the selected meal if it is valid
                            if (validMeal)
                            {
                                // add meal to mealsOfDay
                                mealsOfDay.push_back(selectedMeal);
                                ++mealsScheduled;
                                ++totalMealsScheduled;

                                // update daysScheduled
                                selectedMeal->addDayScheduled(currentDayNumber);

                                // if selectedMeal lasts for more than 1 day, then add to futureMeals
                                if (selectedMeal->getMealDuration() > 1)
                                    futureMeals.push_back(selectedMeal);
                            }

                        } // while: ends loop when requested number of meals are met or all meals were searched

                        // DEBUG INFO
                        // if not enough meals were found to fulfil tag's requirement
                        /*if (mealsScheduled < tagIter.second)
                        {
                            // display error to user
                            std::cout << "\nWARNING: Tag \"" << tagIter.first->getName() << "\" requests " << tagIter.second << " meals, but only " << std::to_string(mealsScheduled)
                                << " meals were scheduled.\nPossibly caused by insufficient Meals that are enabled and linked to Tag, and/or Tag settings are too strict."
                                << "\nThis occurred on: " << dayToString(currentDay) << ", day #" << std::to_string(currentDayNumber) << std::endl;

                        }*/
                    } // if (validTag)
                    } // if tag is enabled and has meals
            } // for (auto tagIter : availableTags)

            // if there are enough meals to schedule
            if (totalMealsScheduled >= requiredMeals)
            {
                mealFound = true;
                // add mealsOfDay to scheduledMeals
                if (scheduledMeals.size() >= currentDayNumber + 1)
                    scheduledMeals.at(currentDayNumber) = mealsOfDay;
                else
                    scheduledMeals.push_back(mealsOfDay);
            }

            // if there are meals that last longer than 1 day, add them to future days in the schedule
            if (futureMeals.size() > 0)
            {
                addFutureMeals(futureMeals, currentDayNumber, calculationPeriod, scheduledMeals);
            }
        } // if (validMultiTag)
        } // while there are multitags to choose from

    return mealFound;
}

bool MealManager::scheduleNormalTags(const QVector<Tag*>& availableTags, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod, const DaysOfTheWeek& currentDay, QVector<QVector<Meal*>>& scheduledMeals)
{
    bool mealFound = false;

    QVector<int> searchedTags; // stores indices of tags that were accessed

    // no meal found and there are tags to choose from
    while (!mealFound && searchedTags.size() < availableTags.size())
    {
        bool validIndex = false;

        // randomly choose index
        int tagIndex = rand() % availableTags.size();

        // retreive Tag
        Tag* chosenTag = availableTags.at(tagIndex);

        // get meals assigned chosenTag
        QVector<Meal*> assignedMeals = chosenTag->getLinkedMeals();

        // if no indices searched, then ok to proceed
        if (searchedTags.size() == 0)
        {
            searchedTags.push_back(tagIndex);
            validIndex = true;
        }
        else // need to check if tag already searched
        {
            bool indexMatch = false;
            auto searchedTagsIter = searchedTags.begin();

            while (!indexMatch && searchedTagsIter != searchedTags.end())
            {
                if (tagIndex == *searchedTagsIter)
                    indexMatch = true;
                else
                    ++searchedTagsIter;
            }

            // if no match, then ok to proceed
            if (!indexMatch)
            {
                searchedTags.push_back(tagIndex);
                validIndex = true;
            }
        }

        // begin checking tag if index is valid
        if (validIndex)
        {
            // check if tag is enabled and has meals
            if (chosenTag->getEnabledDays().at(currentDay) && assignedMeals.size() > 0)
            {
                bool validTag = false;

                // checks tag's params and returns T if tag is valid, F if not
                validTag = validateTag(chosenTag, currentDayNumber, calculationPeriod, assignedMeals, scheduledMeals);

                // begin to schedule meals if tag is valid
                if (validTag)
                {
                    QVector<int> searchedMeals; // stores index of all meals searched within the chosenTag
                    QVector<Meal*> mealsOfDay; // meals scheduled/to be scheduled on the current day

                    // update mealsOfDay if meals were already scheduled (multi-day meals)
                    if (scheduledMeals.size() >= currentDayNumber + 1)
                        mealsOfDay = scheduledMeals.at(currentDayNumber);

                    // ends loop when a meal was found or all meals were searched
                    while (!mealFound && searchedMeals.size() < assignedMeals.size())
                    {
                        bool validMeal = false;

                        // randomly choose index of a meal
                        int mealIndex = rand() % assignedMeals.size();

                        // get meal
                        Meal* selectedMeal = assignedMeals.at(mealIndex);

                        // if no meals were searched, then ok to proceed
                        if (searchedMeals.size() == 0)
                        {
                            searchedMeals.push_back(mealIndex);
                            validMeal = validateMeal(selectedMeal, currentDayNumber);
                        }
                        else // check if meal has been used before
                        {
                            bool matchFound = false;
                            auto searchedMealsIter = searchedMeals.begin();

                            // compare mealIndex with those that have been previously used
                            while (!matchFound && searchedMealsIter != searchedMeals.end())
                            {
                                if (mealIndex == *searchedMealsIter)
                                    matchFound = true;
                                else
                                    ++searchedMealsIter;
                            }

                            // if no match, then check if meal's daysBetweenOccurrences allows it to be scheduled
                            if (!matchFound)
                            {
                                searchedMeals.push_back(mealIndex);
                                validMeal = validateMeal(selectedMeal, currentDayNumber);
                            }
                        }

                        // if meal is valid so far, then check if it has already been scheduled (a multi-day meal)
                        if (validMeal)
                        {
                            // if there are meals to check
                            if (mealsOfDay.size() > 0)
                            {
                                auto mealIter = mealsOfDay.begin();
                                bool found = false;

                                while (!found && mealIter != mealsOfDay.end())
                                {
                                    if (*mealIter == selectedMeal)
                                    {
                                        found = true;
                                        validMeal = false;
                                    }
                                    else
                                        ++mealIter;
                                }
                            }
                        }

                        // try to schedule the selected meal if it is valid
                        if (validMeal)
                        {
                            mealFound = true;

                            // add meal to mealsOfDay
                            mealsOfDay.push_back(selectedMeal);

                            // update daysScheduled
                            selectedMeal->addDayScheduled(currentDayNumber);

                            // add mealsOfDay to scheduledMeals
                            if (scheduledMeals.size() >= currentDayNumber + 1) // an entry for the day already exists
                                scheduledMeals.at(currentDayNumber) = mealsOfDay;
                            else
                                scheduledMeals.push_back(mealsOfDay);

                            // if selectedMeal lasts for more than 1 day, then add to futureMeals
                            if (selectedMeal->getMealDuration() > 1)
                            {
                                addFutureMeals(mealsOfDay, currentDayNumber, calculationPeriod, scheduledMeals);
                            }
                        }

                    } // while: ends loop when a meal was found or all meals were searched
                } // if (validTag)
                } // if tag is enabled and has meals
        } // if (validIndex)
        } // while no meal found and there are tags to choose from

    return mealFound;
}

void MealManager::printSchedule(const QVector<QVector<Meal*>>& mealPlan, const unsigned int& erroredDays, const double& totalCost, const double& budgetLimit, std::ofstream& oFile)
{
    const unsigned int LINE_WIDTH = 80; // output line width in characters

    QString lineDivider = ""; // divides sections of the meal plan
    DaysOfTheWeek currentDay = MONDAY; // used to display meals of each day
    unsigned int weekCount = 0; // tracks each week

    // create divider of length LINE_WIDTH
    for (int index = 0; index < LINE_WIDTH; ++index)
        lineDivider += "-";


    // header
    oFile << centeredText("Meal Plan", LINE_WIDTH) << "\n";

    oFile << lineDivider << "\n";

    // creation time
    // get system time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);;

    oFile << "Created on: " << ctime(&currentTime);

    // plan duration
    oFile << "Duration: " << std::to_string(mealPlan.size() / 7) << " weeks (" << std::to_string(mealPlan.size()) << " days)\n";

    // budget limit
    oFile << "Budget limit: " << formatPrice(budgetLimit) << "\n";

    // calculated cost
    oFile << "Calculated cost: " << formatPrice(totalCost) << "\n";

    // number of errors
    oFile << "Days with errors: " << std::to_string(erroredDays) << "\n";

    // iterates through each day of mealPlan
    auto mealIter = mealPlan.begin();
    while (mealIter != mealPlan.end())
    {
        // if today is the start of a new week, print week header
        if (currentDay == MONDAY)
        {
            QString tempStr = "WEEK "; // outputs "WEEK XX : cost yyyy"
            double costOfWeek = 0; // total of all meals in the upcoming week
            DaysOfTheWeek dayInWeek = MONDAY; // start on monday of current week
            auto weekIter = mealIter; // copy iterator

            ++weekCount;

            // if weekCount is only 1 digit
            if (weekCount < 10)
                tempStr += "0";

            tempStr += std::to_string(weekCount);

            tempStr += " : cost ";

            // sum the price of all meals in the current week
            do
            {
                QVector<Meal*> mealsInDay = *weekIter;

                for (auto meal : mealsInDay)
                {
                    costOfWeek += meal->getPrice();
                }

                ++weekIter;
                dayInWeek = nextDay(dayInWeek);
            } while (dayInWeek != MONDAY && weekIter != mealPlan.end());
            tempStr += formatPrice(costOfWeek);

            // begin printing week header
            oFile << lineDivider << "\n"; // divides previous week
            oFile << lineDivider << "\n";

            oFile << centeredText(tempStr, LINE_WIDTH) << "\n\n";
        }

        // day name
        QString tempStr = ">> " + dayToString(currentDay) + " <<";
        oFile << centeredText(tempStr, LINE_WIDTH) << "\n";
        tempStr = "";

        // meals in the current day
        QVector<Meal*> daysMeals = *mealIter;

        for (auto meal : daysMeals)
        {
            // meal name
            oFile << centeredText(meal->getName(), LINE_WIDTH) << "\n";

            // check if this is a multi-day meal
            if (meal->getMealDuration() > 1)
            {
                int occurranceNumber = 0; // number of times the meal has occurred over its duration
                auto lookBack = mealIter; // copy iter at current day
                bool doneChecking = false; // true when a gap of scheduled days is found or checked to the first day

                // check previous days to count occurranceNumber
                while (!doneChecking)
                {
                    // lookBack is on the first day, stop checking further back
                    if (lookBack == mealPlan.begin())
                    {
                        doneChecking = true;
                    }

                    QVector<Meal*> mealsOfDay = *lookBack; // get meals of the previous day
                    auto mealsOfDayIter = mealsOfDay.begin();

                    bool matchFound = false; // true when the current meal has found to have occurred before
                    while (!matchFound && mealsOfDayIter != mealsOfDay.end())
                    {
                        // if meal occurred in the past
                        if (*mealsOfDayIter == meal)
                        {
                            ++occurranceNumber;
                            matchFound = true;
                        }
                        else
                            ++mealsOfDayIter;
                    }

                    // if no match found in the previous day, then a gap was found
                    if (!matchFound)
                        doneChecking = true;

                    // if done checking, dont go further back
                    if (!doneChecking)
                        --lookBack;
                }

                // since all occurrances are counted, need to reset occurranceNumber every time it counts to duration
                if (occurranceNumber > meal->getMealDuration())
                {
                    occurranceNumber = occurranceNumber % meal->getMealDuration();

                    // set to 3 instead of 0
                    if (occurranceNumber == 0)
                        occurranceNumber = 3;
                }

                // if this is the first occurrance, then display price with occcurrance number
                if (occurranceNumber == 1)
                {
                    // meal price
                    tempStr = "price: " + formatPrice(meal->getPrice());
                    oFile << centeredText(tempStr, LINE_WIDTH) << "\n";

                    // occurrance number
                    tempStr = "[Day " + std::to_string(occurranceNumber) + " of " + std::to_string(meal->getMealDuration()) + "]";
                }
                else
                {
                    // occurrance number
                    tempStr = "[Day " + std::to_string(occurranceNumber) + " of " + std::to_string(meal->getMealDuration()) + "]";
                }

            }
            else // display price normally
            {
                // meal price
                tempStr = "price: " + formatPrice(meal->getPrice());
            }
            oFile << centeredText(tempStr, LINE_WIDTH) << "\n\n";
            tempStr = "";
        }

        currentDay = nextDay(currentDay);
        ++mealIter;
    }

    oFile << lineDivider << "\n";
}

QString MealManager::centeredText(const QString& str, const unsigned int& lineWidth)
{
    QString returnStr = "";

    int emptySpaces; // number of empty spaces needed to center text

    // calculate starting point
    emptySpaces = (lineWidth / 2) - (str.length() / 2);

    // place empty spaces before text
    for (int index = 0; index < emptySpaces; index++)
        returnStr += " ";

    // print input string
    returnStr += str;

    emptySpaces += str.length(); // use emptySpaces as an index

    // place empty spaces after text
    for (emptySpaces; emptySpaces < lineWidth; emptySpaces++)
        returnStr += " ";

    return returnStr;
}

bool MealManager::generateSchedule(const QString& fileName, std::ofstream& oFile, unsigned int& failedPlanErrors, double& failedPlanCost)
{
    const unsigned int MIN_CALCULATION_LENGTH = 1; // 1 week
    const unsigned int MAX_CALCULATION_LENGTH = 52; // just under a year (in weeks)
    const unsigned int MIN_BUDGET = 0;
    const unsigned int MAX_BUDGET = 100000;
    const unsigned int GENERATED_PLANS = 500; // number of meal plans to make
    const double ERROR_THRESHOLD_PER_WEEK = 0.5; // amount of errored days allowed per week

    bool errorPresent = false; // return value
    bool insufficientData = true; // stops generation if no meals or tags exist
    double budget = 0; // budget for the calculated period
    unsigned int calculationPeriod = 0; // period of calculation
    unsigned int failedDays[GENERATED_PLANS] = { 0 }; // number of days that calculation was unable to find a meal

    failedPlanErrors = 0;
    failedPlanCost = 0;

    DaysOfTheWeek currentDay = MONDAY; // start of the week
    QVector<QVector<Meal*>> scheduledMeals[GENERATED_PLANS]; // meals scheduled for each attempt

    QString tempStr = "";
    QVector<QString> strVec;

    // optmized data
    QMap<DaysOfTheWeek, QVector<MultiTag*>> highPriorityMultiTags; // multiTags with elevated priority, sorted by available day
    QMap<DaysOfTheWeek, QVector<MultiTag*>> normalPriorityMultiTags; // multiTags with priority of Tag, sorted by available day
    QMap<DaysOfTheWeek, QVector<Tag*>> normalPriorityTags; // Tags, sorted by available day

    // check if meals and tags exist, otherwise cannot make a plan
    if (meals.size() > 0 && normalTags.size() > 0)
    {
        insufficientData = false;
    }
    else // tell uesr
    {
        errorPresent = true;

        uim->centeredText("Error");
        uim->skipLines(2);
        uim->centeredText("You have not created any Meals or Tags.");
        uim->centeredText("Please create some and try again.");
        uim->display();
    }

    // loop until user is ok with settings
    while (!insufficientData && tempStr != "Q")
    {
        // prompt for calculationPeriod
        uim->centeredText("Schedule Length");
        uim->skipLines(2);
        uim->centeredText("How many weeks do you want to schedule for?");
        uim->prompt_FreeInt(MIN_CALCULATION_LENGTH, MAX_CALCULATION_LENGTH);
        calculationPeriod = std::stoi(uim->display());

        // prompt for budget
        uim->centeredText("Budget Limit");
        uim->skipLines(2);
        tempStr = "What is your budget over a " + std::to_string(calculationPeriod) + " week period?";
        uim->centeredText(tempStr);
        uim->prompt_FreeDouble(MIN_BUDGET, MAX_BUDGET);
        budget = std::stod(uim->display());

        // summary of users choices
        uim->centeredText("Confirm Settings");
        uim->skipLines(2);
        uim->centeredText("Is this OK?");
        uim->skipLines(1);

        tempStr = "BUDGET LIMIT: " + formatPrice(budget);
        uim->centeredText(tempStr);

        tempStr = "SCHEDULE LENGTH: " + std::to_string(calculationPeriod) + " weeks";
        uim->centeredText(tempStr);

        strVec = { "YYes", "NNo" };
        uim->prompt_List_Case_Insensitive(strVec);

        tempStr = uim->display();
        tempStr = std::toupper(tempStr.at(0));

        // exit loop if user accepts settings
        if (tempStr == "Y")
            tempStr = "Q";
    }

    if (!insufficientData) // tell user to be patient
    {
        QStringstream skipPrompt = QStringstream("\n"); // skips prompt in the following ui window

        uim->centeredText("Please wait");
        uim->skipLines(3);
        uim->centeredText("Your Meal Plan is being created.");
        uim->skipLines(1);
        uim->centeredText("Wait time may vary between a few seconds and a few minutes, depending on the speed of your computer.");
        uim->display(std::cout, skipPrompt);
        std::cout << "\n\n";
    }

    // convert weeks into days
    calculationPeriod *= 7;

    // OPTIMIZATION
    if (!insufficientData)
        optimizeData(highPriorityMultiTags, normalPriorityMultiTags, normalPriorityTags);

    // CALCULATION
    if (!insufficientData)
    {
        // calculates GENERATED_PLANS number of complete meal plans
        for (unsigned int attemptNum = 0; attemptNum < GENERATED_PLANS; ++attemptNum)
        {
            currentDay = MONDAY;

            // calculate one complete meal plan
            for (unsigned int dayNumber = 0; dayNumber < calculationPeriod; ++dayNumber)
            {

                bool createdMeals = false; // if meal creation failed on a certain priority, attempts to use next lower priority

                // check if a high priority MT is available
                if (highPriorityMultiTags.find(currentDay)->second.size() > 0)
                {
                    createdMeals = scheduleMultiTags(highPriorityMultiTags.find(currentDay)->second, dayNumber,
                                                     calculationPeriod, currentDay, scheduledMeals[attemptNum]);

                }

                // if above priority failed, check next lower priority
                if (!createdMeals &&
                    normalPriorityMultiTags.find(currentDay)->second.size() > 0 &&
                    normalPriorityTags.find(currentDay)->second.size()) // if both MTs and tags are available
                {
                    // flip coin to decide which to choose from
                    int coinFlip = rand() % 1000 + 1; // 1-1000

                    if (coinFlip > 500) // choose MTs
                    {
                        createdMeals = scheduleMultiTags(normalPriorityMultiTags.find(currentDay)->second, dayNumber,
                                                         calculationPeriod, currentDay, scheduledMeals[attemptNum]);
                    }
                    else // choose tags
                    {
                        createdMeals = scheduleNormalTags(normalPriorityTags.find(currentDay)->second, dayNumber,
                                                          calculationPeriod, currentDay, scheduledMeals[attemptNum]);
                    }
                }

                // if above priority failed, check next lower priority
                if (!createdMeals && normalPriorityMultiTags.find(currentDay)->second.size() > 0) // if only MTs are available
                {
                    createdMeals = scheduleMultiTags(normalPriorityMultiTags.find(currentDay)->second, dayNumber,
                                                     calculationPeriod, currentDay, scheduledMeals[attemptNum]);
                }

                // if above priority failed, check next lower priority
                if (!createdMeals && normalPriorityTags.find(currentDay)->second.size() > 0) // if only tags are available
                {
                    createdMeals = scheduleNormalTags(normalPriorityTags.find(currentDay)->second, dayNumber,
                                                      calculationPeriod, currentDay, scheduledMeals[attemptNum]);
                }

                if (!createdMeals) // no tags available
                {
                    QVector<Meal*> daysMeals; // the meals for one day
                    // insert dummy meal
                    Meal* mealPtr = new Meal;
                    mealPtr->setName("NO AVAILABLE MEALS");
                    mealPtr->setPrice(0);
                    mealPtr->setDisabled(true);

                    daysMeals.push_back(mealPtr);
                    scheduledMeals[attemptNum].push_back(daysMeals);
                    ++failedDays[attemptNum];

                    mealPtr = nullptr;

                    // warn user
                    //std::cout << "\nWARNING: No available meals on: " << dayToString(currentDay) << ", day #" << std::to_string(dayNumber) << std::endl;
                }
                currentDay = nextDay(currentDay); // go to next day
            } // for: calculates a meal for every day

            // reset meals' scheduledDays
            for (auto mealIter : meals)
            {
                QVector<unsigned int> resetDays;
                mealIter->setDaysScheduled(resetDays);
            }
        } // for: calculates an entire plan
    }

    // select the plan that had the least number of errors and met budget
    QVector<QVector<Meal*>> finalPlan; // plan with least errors
    double finalCost = 0; // cost of all the meals in the plan
    unsigned int finalErrors = 0;

    if (!insufficientData)
    {
        bool planFound = false;
        QMap<unsigned int, std::pair<QVector<QVector<Meal*>>, double>> errorsToPlans; // maps errors to scheduled plans with total cost for plan
        unsigned int lowestErrors = 0;

        int planIndex = 0;
        // while a perfect plan was not found, and can check for more plans
        do
        {
            // if plan has no errors
            if (failedDays[planIndex] == 0)
            {
                // check budget
                double totalCost = 0;

                // sum the cost of all meals
                for (int index = 0; index < scheduledMeals[planIndex].size(); ++index)
                {
                    // get day of meals
                    QVector<Meal*> currentDay = scheduledMeals[planIndex].at(index);

                    for (auto mealIter : currentDay)
                    {
                        // if meal lasts multiple days, divide the cost over its duration
                        if (mealIter->getMealDuration() > 1)
                        {
                            totalCost += (mealIter->getPrice() / mealIter->getMealDuration());
                        }
                        else
                            totalCost += mealIter->getPrice();
                    }
                }

                // if totalCost is within budget, then plan is ok to use
                if (totalCost <= budget)
                {
                    planFound = true;
                    lowestErrors = 0;
                    errorsToPlans.emplace(lowestErrors, std::pair<QVector<QVector<Meal*>>, double>(scheduledMeals[planIndex], totalCost));
                }
            }
            else // set lowestErrors
            {
                // if lowestErrors was never set, then prepare to set it to this plan
                if (errorsToPlans.size() == 0)
                {
                    // check if more errors than error threshold
                    unsigned int calculatedWeeks = calculationPeriod / 7;
                    double allowedErrors = calculatedWeeks * ERROR_THRESHOLD_PER_WEEK;

                    // only considers plans that do not exceed allowedErrors
                    if (failedDays[planIndex] <= allowedErrors)
                    {
                        // check budget
                        double totalCost = 0;

                        // sum the cost of all meals
                        for (int index = 0; index < scheduledMeals[planIndex].size(); ++index)
                        {
                            // get day of meals
                            QVector<Meal*> currentDay = scheduledMeals[planIndex].at(index);

                            for (auto mealIter : currentDay)
                            {
                                // if meal lasts multiple days, divide the cost over its duration
                                if (mealIter->getMealDuration() > 1)
                                {
                                    totalCost += (mealIter->getPrice() / mealIter->getMealDuration());
                                }
                                else
                                    totalCost += mealIter->getPrice();
                            }
                        }

                        // if totalCost is within budget, then plan is ok to use
                        if (totalCost <= budget)
                        {
                            lowestErrors = failedDays[planIndex];
                            errorsToPlans.emplace(lowestErrors, std::pair<QVector<QVector<Meal*>>, double>(scheduledMeals[planIndex], totalCost));
                        }
                    }
                }
                else if (failedDays[planIndex] < lowestErrors) // if a better plan was found
                {
                    // check budget
                    double totalCost = 0;

                    // sum the cost of all meals
                    for (int index = 0; index < scheduledMeals[planIndex].size(); ++index)
                    {
                        // get day of meals
                        QVector<Meal*> currentDay = scheduledMeals[planIndex].at(index);

                        for (auto mealIter : currentDay)
                        {
                            // if meal lasts multiple days, divide the cost over its duration
                            if (mealIter->getMealDuration() > 1)
                            {
                                totalCost += (mealIter->getPrice() / mealIter->getMealDuration());
                            }
                            else
                                totalCost += mealIter->getPrice();
                        }
                    }

                    // if totalCost is within budget, then plan is ok to use
                    if (totalCost <= budget)
                    {
                        // attempt to insert new element
                        auto iter = errorsToPlans.insert(std::pair<unsigned int, std::pair<QVector<QVector<Meal*>>, double>>(lowestErrors, { scheduledMeals[planIndex], totalCost }));

                        // check if key already exists
                        if (iter.second == false)
                        {
                            // replace value at key
                            errorsToPlans[lowestErrors] = std::pair<QVector<QVector<Meal*>>, double>(scheduledMeals[planIndex], totalCost);
                        }
                    }
                }
            }
            ++planIndex;
        } while (!planFound && planIndex < GENERATED_PLANS);

        // set finalPlan to plan with lowestErrors that met budget
        if (errorsToPlans.size() > 0)
        {
            finalPlan = errorsToPlans.at(lowestErrors).first;
            finalCost = errorsToPlans.at(lowestErrors).second;
            finalErrors = lowestErrors;
        }
        else // no suitable plans found
        {
            errorPresent = true;

            // retreive total budget and errors from the first plan generated
            failedPlanErrors = failedDays[0];

            // check budget
            failedPlanCost = 0;

            // sum the cost of all meals
            for (int index = 0; index < scheduledMeals[0].size(); ++index)
            {
                // get day of meals
                QVector<Meal*> currentDay = scheduledMeals[0].at(index);

                for (auto mealIter : currentDay)
                {
                    // if meal lasts multiple days, divide the cost over its duration
                    if (mealIter->getMealDuration() > 1)
                    {
                        failedPlanCost += (mealIter->getPrice() / mealIter->getMealDuration());
                    }
                    else
                        failedPlanCost += mealIter->getPrice();
                }
            }
        }
    }// if (!insufficientData)


    // if finalPlan is not empty
    if (finalPlan.size() > 0)
    {
        errorPresent = false;

        oFile.open(fileName);
        printSchedule(finalPlan, finalErrors, finalCost, budget, oFile);
        oFile.close();
    }

    // CLEANUP
    // checks every plan generated
    for (int attemptNumber = 0; attemptNumber < GENERATED_PLANS; ++attemptNumber)
    {
        // checks every day
        for (unsigned int dayNumber = 0; dayNumber < calculationPeriod; ++dayNumber)
        {
            // checks every meal
            for (unsigned int mealNumber = 0; mealNumber < scheduledMeals[attemptNumber].at(dayNumber).size(); ++mealNumber)
            {
                // if meal is an error meal (i.e. not a real meal), then delete it
                Meal* mealPtr = scheduledMeals[attemptNumber].at(dayNumber).at(mealNumber);

                if (mealPtr->getName() == "NO AVAILABLE MEALS" && mealPtr->getPrice() == 0.0 && mealPtr->isDisabled() == true)
                {
                    delete mealPtr;
                    mealPtr = nullptr;
                }
            }
        }
    } // for: checks every plan generated

    return errorPresent;
}

void MealManager::saveState(std::ofstream& oFile)
{
    // open file for writing
    oFile.open(DATA_NAME.toStdString());

    // check if open
    if (oFile.is_open())
    {
        // write tags
        oFile << "<Tags>\n";

        // check if tags exist to save
        if (normalTags.size() > 0)
        {
            auto tagIter = normalTags.begin();

            // write first tag
            writeTag(*tagIter, oFile);
            ++tagIter;

            // loop to write rest of tags
            while (tagIter != normalTags.end())
            {
                oFile << "\n\n";
                writeTag(*tagIter, oFile);
                ++tagIter;
            }
            oFile << "\n";
        }
        oFile << "</Tags>\n\n";


        // write muliTags
        oFile << "<MultiTags>\n";

        // check if multiTags exist to save
        if (multiTags.size() > 0)
        {
            auto tagIter = multiTags.begin();

            // write first tag
            writeMultiTag(*tagIter, oFile);
            ++tagIter;

            // loop to write rest of tags
            while (tagIter != multiTags.end())
            {
                oFile << "\n\n";
                writeMultiTag(*tagIter, oFile);
                ++tagIter;
            }
            oFile << "\n";
        }
        oFile << "</MultiTags>\n\n";

        // write meals
        oFile << "<Meals>\n";

        // check if meals exist to save
        if (meals.size() > 0)
        {
            auto mealIter = meals.begin();

            // write first tag
            writeMeal(*mealIter, oFile);
            ++mealIter;

            // loop to write rest of tags
            while (mealIter != meals.end())
            {
                oFile << "\n\n";
                writeMeal(*mealIter, oFile);
                ++mealIter;
            }
            oFile << "\n";
        }
        oFile << "</Meals>";

        // close file
        oFile.close();
    }
    else
        throw QString("Cannot access data file");
}

int MealManager::loadState(std::ifstream& iFile)
{
    QString tagError = "Error reading Tags";
    QString multiTagError = "Error reading MultiTags";
    QString mealError = "Error reading Meals";

    int returnVal = 0; // 0 = good, 1 = corrupted

    bool corruptionDetected = false;
    std::string line; // lines read from file

    // open file
    iFile.open(DATA_NAME.toStdString());

    // check if failed
    if (iFile.is_open())
    {
        // get Tags header
        std::getline(iFile, line);

        // check if header is correct
        if (line == "<Tags>")
        {
            int returnVal = readTags(iFile);

            // if tags imported with error
            if (returnVal == 1)
                throw tagError;
            else if (returnVal == 2)
                corruptionDetected = true;
        }
        else throw tagError;

        // get blank space
        std::getline(iFile, line);
        // get multiTags header
        std::getline(iFile, line);

        // check if header is correct
        if (line == "<MultiTags>")
        {
            int returnVal = readMultiTags(iFile);

            // if tags imported with error
            if (returnVal == 1)
                throw multiTagError;
            else if (returnVal == 2)
                corruptionDetected = true;
        }
        else throw multiTagError;

        // get blank space
        std::getline(iFile, line);
        // get Meals header
        std::getline(iFile, line);

        // check if header is correct
        if (line == "<Meals>")
        {
            int returnVal = readMeals(iFile);

            // if tags imported with error
            if (returnVal == 1)
                throw mealError;
            else if (returnVal == 2)
                corruptionDetected = true;
        }
        else throw mealError;

        iFile.close();
    }

    if (corruptionDetected)
        returnVal = 1;

    // organize by alphabetic order
    {
        std::sort(normalTags.begin(), normalTags.end(),
                  [](Tag* lhs, Tag* rhs) { return lhs->getName() < rhs->getName(); });

        std::sort(meals.begin(), meals.end(),
                  [](Meal* lhs, Meal* rhs) { return lhs->getName() < rhs->getName(); });

        std::sort(multiTags.begin(), multiTags.end(),
                  [](MultiTag* lhs, MultiTag* rhs) { return lhs->getName() < rhs->getName(); });
    }

    return returnVal;
}

DaysOfTheWeek nextDay(const DaysOfTheWeek& day)
{
    DaysOfTheWeek returnDay = MONDAY;

    switch (day)
    {
    case MONDAY:  returnDay = TUESDAY;
        break;
    case TUESDAY:  returnDay = WEDNESDAY;
        break;
    case WEDNESDAY:  returnDay = THURSDAY;
        break;
    case THURSDAY:  returnDay = FRIDAY;
        break;
    case FRIDAY:  returnDay = SATURDAY;
        break;
    case SATURDAY:  returnDay = SUNDAY;
        break;
    case SUNDAY:  returnDay = MONDAY;
        break;
    }

    return  returnDay;
}

DaysOfTheWeek previousDay(const DaysOfTheWeek& day)
{
    DaysOfTheWeek returnDay = MONDAY;

    switch (day)
    {
    case MONDAY:  returnDay = SUNDAY;
        break;
    case TUESDAY:  returnDay = MONDAY;
        break;
    case WEDNESDAY:  returnDay = TUESDAY;
        break;
    case THURSDAY:  returnDay = WEDNESDAY;
        break;
    case FRIDAY:  returnDay = THURSDAY;
        break;
    case SATURDAY:  returnDay = FRIDAY;
        break;
    case SUNDAY:  returnDay = SATURDAY;
        break;
    }

    return returnDay;
}

QString dayToString(const DaysOfTheWeek& day)
{
    QString returnStr = "";

    switch (day)
    {
    case MONDAY: returnStr = "MONDAY";
        break;
    case TUESDAY: returnStr = "TUESDAY";
        break;
    case WEDNESDAY: returnStr = "WEDNESDAY";
        break;
    case THURSDAY: returnStr = "THURSDAY";
        break;
    case FRIDAY: returnStr = "FRIDAY";
        break;
    case SATURDAY: returnStr = "SATURDAY";
        break;
    case SUNDAY: returnStr = "SUNDAY";
        break;
    default: returnStr = "UNKNOWN";
    }

    return returnStr;
}
