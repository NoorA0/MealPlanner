#include "../headers/Tag.hpp"

Tag::Tag()
{
	this->name = "UNSET";
	this->description = "UNSET";
	this->hasPriority = false;
	this->consecutiveLimit = 0;

	std::map<DaysOfTheWeek, bool> enabledDays =
	{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false}, {FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };

	this->enabledDays = enabledDays;
}

Tag::Tag(const std::string& _name, const std::string& _description, const std::map<DaysOfTheWeek, bool>& _enabledDays, const bool& _hasPriority, const unsigned int& _consecutiveLimit)
{
	name = _name;
	description = _description;
	enabledDays = _enabledDays;
	hasPriority = _hasPriority;
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

void Tag::setPriority(const bool& hasPriority)
{
	this->hasPriority = hasPriority;
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

bool Tag::getPriority() const
{
	return hasPriority;
}

unsigned int Tag::getConsecutiveLimit() const
{
	return consecutiveLimit;
}
