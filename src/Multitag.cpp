#include "../headers/MultiTag.hpp"

MultiTag::MultiTag()
{
	enabled = false;
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
