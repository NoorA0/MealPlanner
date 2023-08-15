#include "multitag.h"
#include "tag.h"

MultiTag::MultiTag()
{
    highestPriority = false;
    requireFulfillment = false;

    name = "UNSET";
    description = "UNSET";

    // disable on all days
    enabledDays = { {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
                   {FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };
}

MultiTag::~MultiTag()
{
    for (auto tagIter = linkedTags.begin(); tagIter != linkedTags.end(); ++tagIter)
    {
        delete tagIter->first;
    }
}

void MultiTag::addLinkedTag(Tag* new_tag, const unsigned int& amount)
{
    linkedTags.push_back(QPair<Tag*, unsigned int>(new_tag, amount));
}

void MultiTag::setLinkedTagRequestedMeals(Tag* assignedTag, const unsigned int& amount)
{
    // find assignedTag
    auto linkedTagIter = linkedTags.begin();
    bool found = false;

    while (!found && linkedTagIter != linkedTags.end())
    {
        if (assignedTag->getName().toUpper() == linkedTagIter->first->getName().toUpper())
        {
            found = true;

            // set new value
            linkedTagIter->second = amount;
        }
        else
            ++linkedTagIter;
    }
}


bool MultiTag::removeLinkedTag(const Tag* tagPtr)
{
    bool notFound = true;

    // find the Tag in linkedTags
    auto tagIter = linkedTags.begin();
    while (notFound && tagIter != linkedTags.end())
    {
        // if found, erase from vector
        if (tagPtr->getName() == tagIter->first->getName())
        {
            notFound = false;
            linkedTags.erase(tagIter);
        }
        else
            ++tagIter;
    }

    // if nothing found, return error value
    return notFound;
}
