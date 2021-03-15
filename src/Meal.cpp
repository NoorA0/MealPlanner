#include "../headers/Meal.hpp"

Meal::Meal()
{
	name = "UNSET";
	price = 0.0;
	tags.push_back(NONE);

	enabledDays.emplace(MONDAY, false);
	enabledDays.emplace(TUESDAY, false);
	enabledDays.emplace(WEDNESDAY, false);
	enabledDays.emplace(THURSDAY, false);
	enabledDays.emplace(FRIDAY, false);
	enabledDays.emplace(SATURDAY, false);
	enabledDays.emplace(SUNDAY, false);
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

void Meal::setTags(const std::vector<Tag>& new_tags)
{
	tags = new_tags;
}

void Meal::setEnabledDays(const std::map<DaysOfTheWeek, bool> new_enabledDays)
{
	enabledDays = new_enabledDays;
}

std::string Meal::getName() const
{
	return name;
}

double Meal::getPrice() const
{
	return price;
}

std::vector<Tag> Meal::getTags() const
{
	return tags;
}

std::map<DaysOfTheWeek, bool> Meal::getEnabledDays() const
{
	return enabledDays;
}
