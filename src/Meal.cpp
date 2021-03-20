#include "../headers/Meal.hpp"

Meal::Meal()
{
	name = "UNSET";
	price = 0.0;
	isDisabled = true;
}

Meal::~Meal()
{
}

void Meal::setName(const std::string& new_name)
{
	name = new_name;
}

void Meal::setPrice(const double& new_price)
{
	price = new_price;
}

void Meal::setTags(const std::vector<Tag*>& new_tags)
{
	tags = new_tags;
}

void Meal::addTag(Tag* newTag)
{
	tags.push_back(newTag);
}

void Meal::setIsDisabled(const bool& isDisabled)
{
	this->isDisabled = isDisabled;
}

std::string Meal::getName() const
{
	return name;
}

double Meal::getPrice() const
{
	return price;
}

std::vector<Tag*> Meal::getTags() const
{
	return tags;
}

bool Meal::getIsDisabled() const
{
	return isDisabled;
}

std::map<DaysOfTheWeek, bool> Meal::getEnabledDays() const
{
	// default to always enabled
	std::map<DaysOfTheWeek, bool> enabledDays =
	{ {MONDAY, true}, {TUESDAY, true}, {WEDNESDAY, true}, {THURSDAY, true},
		{FRIDAY, true}, {SATURDAY, true}, {SUNDAY, true} };

	// check every tag applied
	for (auto tagIter : tags)
	{
		std::map<DaysOfTheWeek, bool> tagDays = tagIter->getEnabledDays();

		// if tag's day is disabled, then disable day
		if (!tagDays.at(MONDAY))
			enabledDays.at(MONDAY) = false;
		if (!tagDays.at(TUESDAY))
			enabledDays.at(TUESDAY) = false;
		if (!tagDays.at(WEDNESDAY))
			enabledDays.at(WEDNESDAY) = false;
		if (!tagDays.at(THURSDAY))
			enabledDays.at(THURSDAY) = false;
		if (!tagDays.at(FRIDAY))
			enabledDays.at(FRIDAY) = false;
		if (!tagDays.at(SATURDAY))
			enabledDays.at(SATURDAY) = false;
		if (!tagDays.at(SUNDAY))
			enabledDays.at(SUNDAY) = false;
	}

	return enabledDays;
}
