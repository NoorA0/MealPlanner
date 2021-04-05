#ifndef __MEAL_HPP__
#define __MEAL_HPP__

#include "Tag.hpp"

class Meal
{
	std::string name;
	std::vector<Tag*> tags; // tags that food is a part of
	double price; // how much it costs to make
	bool disabled; // user can disable meals
	unsigned int mealDuration; // how long this meal lasts in days (useful for large batches)
	unsigned int daysBetweenOccurrences; // how many days before this meal can be rescheduled
	std::vector<unsigned int> daysScheduled; // logs each number of the day this meal was last scheduled

public:
	Meal();
	~Meal();

	// SETTERS
	void setName(const std::string& new_name);
	void setPrice(const double& new_price);
	void setTags(const std::vector<Tag*>& new_tags);
	void addTag(Tag* newTag);
	bool removeTag(const Tag* tagPtr);
	void clearTags();
	void setDisabled(const bool& disabled);
	void setMealDuration(const unsigned int& mealDuration);
	void setDaysBetweenOccurrences(const unsigned int& daysBetweenOccurrences);
	void addDayScheduled(const unsigned int& scheduledDay) { daysScheduled.push_back(scheduledDay); }
	void setDaysScheduled(const std::vector<unsigned int>& daysScheduled);

	// GETTERS
	std::string getName() const;
	double getPrice() const;
	std::vector<Tag*> getTags() const;
	bool isDisabled() const;
	unsigned int getMealDuration() const;
	unsigned int getDaysBetweenOccurrences() const;
	std::vector<unsigned int> getDaysScheduled() const;

	// OTHER FUNCTIONS
	std::map<DaysOfTheWeek, bool> getEnabledDays() const;
};
#endif // !__MEAL_HPP__