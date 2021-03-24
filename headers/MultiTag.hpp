#ifndef __MULTITAG_HPP__
#define __MULTITAG_HPP__

#include "Tag.hpp"
#include <vector>

class MultiTag
{
	bool highestPriority; // default to true, but can be lowered to that of a normal Tag
	std::string name;
	std::string description;
	std::map<DaysOfTheWeek, bool> enabledDays;

	// ints are used to indicate requested amount of Meals of Tag
	// requesting 2 Sandwiches (tags) would mean the Tag called Sandwiches is loaded into linkedTags with an int of 2
	std::map<Tag*, unsigned int> linkedTags;


public:
	MultiTag();
	~MultiTag();

	// SETTERS
	void setName(const std::string& _name) { name = _name; }
	void setDescription(const std::string& _description) { description = _description; }
	void setHighestPriority(const bool& _priority) { highestPriority = _priority; }
	void setEnabledDays(const std::map<DaysOfTheWeek, bool>& _enabledDays) { enabledDays = _enabledDays; }
	void setLinkedTags(const std::map<Tag*, unsigned int>& _linkedTags) { linkedTags = _linkedTags; }

	// GETTERS
	std::string getName() const { return name; }
	std::string getDescription() const { return description; }
	bool isDisabled() const;
	bool hasPriority() const { return highestPriority; }
	std::map<DaysOfTheWeek, bool> getEnabledDays() const { return enabledDays; }
	std::map<Tag*, unsigned int> getLinkedTags() const { return linkedTags; }

	// OTHER
	void addLinkedTag(Tag* new_tag, const unsigned int& amount) { linkedTags.emplace(new_tag, amount); }
};
#endif // !__MULTITAG_HPP__
