#include "../headers/Tag.hpp"

Tag::Tag(const std::string& name, const std::string& description, const std::map<DaysOfTheWeek, bool>& enabledDays, const bool& hasPriority, const unsigned int& consecutiveLimit)
{
	this->name = name;
	this->description = description;
	this->enabledDays = enabledDays;
	this->hasPriority = hasPriority;
	this->consecutiveLimit = consecutiveLimit;
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
