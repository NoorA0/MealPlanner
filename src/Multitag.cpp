#include "../headers/MultiTag.hpp"
#include "../headers/Tag.hpp"

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

void MultiTag::addLinkedTag(Tag* new_tag, const unsigned int& amount)
{
	linkedTags.emplace(new_tag, amount);
}

bool MultiTag::removeLinkedTag(const Tag* tagPtr)
{
	bool notFound = true;

	// find tagPtr
	auto tagIter = linkedTags.begin();
	while (notFound && tagIter != linkedTags.end())
	{
		// if found, erase from vector
		if (tagPtr == tagIter->first)
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
