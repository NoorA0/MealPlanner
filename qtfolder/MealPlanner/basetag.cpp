#include "basetag.h"

bool BaseTag::isDisabled() const
{
    bool isDisabled = true;
    bool done = false;
    auto daysIter = enabledDays.begin();

    // check each day
    while (!done && daysIter != enabledDays.end())
    {
        // if day is enabled
        if (daysIter.value())
        {
            isDisabled = false;
            done = true;
        }
        ++daysIter;
    }
    return isDisabled;
}
