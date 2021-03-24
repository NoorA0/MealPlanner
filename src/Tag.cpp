#include "../headers/Tag.hpp"

Tag::Tag()
{
	this->name = "UNSET";
	this->description = "UNSET";
	this->dependsOnMultiTag = false;
	this->consecutiveLimit = 0;

	std::map<DaysOfTheWeek, bool> enabledDays =
	{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false}, {FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };

	this->enabledDays = enabledDays;
}

Tag::Tag(const std::string& _name, const std::string& _description, const std::map<DaysOfTheWeek, bool>& _enabledDays, const bool& _dependsOnMultiTag, const unsigned int& _consecutiveLimit)
{
	name = _name;
	description = _description;
	enabledDays = _enabledDays;
	dependsOnMultiTag = _dependsOnMultiTag;
	consecutiveLimit = _consecutiveLimit;
}

Tag::~Tag()
{
}

void Tag::setName(const std::string& new_name)
{
	name = new_name;
}

void Tag::setDescription(const std::string& new_desc)
{
	description = new_desc;
}

void Tag::setEnabledDays(const std::map<DaysOfTheWeek, bool> new_enabledDays)
{
	enabledDays = new_enabledDays;
}

void Tag::setDependency(const bool& new_depenency)
{
	this->dependsOnMultiTag = new_depenency;
}

void Tag::setConsecutiveLimit(const unsigned int& new_consecutiveLimit)
{
	consecutiveLimit = new_consecutiveLimit;
}

std::string Tag::getName() const
{
	return name;
}

std::string Tag::getDescription() const
{
	return description;
}

std::map<DaysOfTheWeek, bool> Tag::getEnabledDays() const
{
	return enabledDays;
}

bool Tag::getDependency() const
{
	return dependsOnMultiTag;
}

unsigned int Tag::getConsecutiveLimit() const
{
	return consecutiveLimit;
}

DaysOfTheWeek nextDay(const DaysOfTheWeek& day)
{
	switch (day)
	{
	case MONDAY: return TUESDAY;
	case TUESDAY: return WEDNESDAY;
	case WEDNESDAY: return THURSDAY;
	case THURSDAY: return FRIDAY;
	case FRIDAY: return SATURDAY;
	case SATURDAY: return SUNDAY;
	case SUNDAY: return MONDAY;
	}
}

DaysOfTheWeek previousDay(const DaysOfTheWeek& day)
{
	switch (day)
	{
	case MONDAY: return SUNDAY;
	case TUESDAY: return MONDAY;
	case WEDNESDAY: return TUESDAY;
	case THURSDAY: return WEDNESDAY;
	case FRIDAY: return THURSDAY;
	case SATURDAY: return FRIDAY;
	case SUNDAY: return SATURDAY;
	}
}
