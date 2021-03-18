#ifndef __MEAL_HPP__
#define __MEAL_HPP__

#include "Tag.hpp"
#include <vector>



class Meal
{
	std::string name;
	std::map<DaysOfTheWeek, bool> enabledDays; // days of the week that meal is enabled on
	std::vector<Tag*> tags; // tags that food is a part of
	double price; // how much it costs to make

public:
	Meal();
	~Meal();

	// SETTERS
	void setName(const std::string& new_name);
	void setPrice(const double& new_price);
	void setTags(const std::vector<Tag*>& new_tags);
	void addTag(Tag& newTag);
	void setEnabledDays(const std::map<DaysOfTheWeek, bool> new_enabledDays);

	// GETTERS
	std::string getName() const;
	double getPrice() const;
	std::vector<Tag*> getTags() const;
	std::map<DaysOfTheWeek, bool> getEnabledDays() const;
};
#endif // !__MEAL_HPP__
