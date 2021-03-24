#include "../headers/MultiTag.hpp"

MultiTag::MultiTag()
{
	highestPriority = false;

	name = "UNSET";
	description = "UNSET";

	// disable on all days
	enabledDays = { {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
		{FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };
}

MultiTag::~MultiTag()
{
	for (std::map<Tag*, unsigned int>::iterator tagIter = linkedTags.begin(); tagIter != linkedTags.end(); ++tagIter)
	{
		delete tagIter->first;
	}
}

bool MultiTag::isDisabled() const
{
	bool isDisabled = true;
	bool done = false;
	auto daysIter = enabledDays.begin();

	// check each day
	while (!done && daysIter != enabledDays.end())
	{
		// if day is enabled
		if (daysIter->second)
		{
			isDisabled = false;
			done = true;
		}
		++daysIter;
	}
	return isDisabled;
}
