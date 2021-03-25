#include "../headers/Tag.hpp"
#include "../headers/Meal.hpp"

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

Tag::~Tag()
{
	for (auto mealIter : linkedMeals)
	{
		delete mealIter;
	}
}

bool Tag::removeMeal(const Meal* mealPtr)
{
	bool notFound = true;
	
	// find mealPtr
	auto mealIter = linkedMeals.begin();
	while (notFound && mealIter != linkedMeals.end())
	{
		// if found, erase from vector
		if (*mealIter == mealPtr)
		{
			notFound = false;
			linkedMeals.erase(mealIter);
		}
		++mealIter;
	}

	// if nothing found, return error value
	return notFound;
}