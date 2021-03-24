#ifndef __MEAL_HPP__
#define __MEAL_HPP__

#include "Tag.hpp"

class Meal
{
	std::string name;
	std::vector<Tag*> tags; // tags that food is a part of
	double price; // how much it costs to make
	bool isDisabled; // user can disable meals
	bool wasChecked; // used during generation of Meal Plans
	unsigned int mealDuration; // how long this meal lasts in days (useful for large batches)
	unsigned int daysBetweenOccurrences; // how many days before this meal can be rescheduled
	unsigned int dayScheduled; // number of the day this meal was last scheduled

public:
	Meal();
	~Meal();

	// SETTERS
	void setName(const std::string& new_name);
	void setPrice(const double& new_price);
	void setTags(const std::vector<Tag*>& new_tags);
	void addTag(Tag* newTag);
	void setIsDisabled(const bool& isDisabled);
	void setWasChecked(const bool& checked);
	void setMealDuration(const unsigned int& mealDuration);
	void setDaysBetweenOccurrences(const unsigned int& daysBetweenOccurrences);
	void setDayScheduled(const unsigned int& dayScheduled);

	// GETTERS
	std::string getName() const;
	double getPrice() const;
	std::vector<Tag*> getTags() const;
	bool getIsDisabled() const;
	bool getWasChecked() const;
	unsigned int getMealDuration() const;
	unsigned int getDaysBetweenOccurrences() const;
	unsigned int getDayScheduled() const;

	// OTHER FUNCTIONS
	std::map<DaysOfTheWeek, bool> getEnabledDays() const;
};
#endif // !__MEAL_HPP__