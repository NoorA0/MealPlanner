#ifndef __TAG_HPP__
#define __TAG_HPP__

#include <map>
#include <string>

enum DaysOfTheWeek
{
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
};

class Tag
{
	std::string name;
	std::string description;
	std::map<DaysOfTheWeek, bool> enabledDays; // days of the week that meal is enabled on
	bool hasPriority; // if tag has priority over any conflicting rules
	unsigned int consecutiveLimit; // times a meal with a tag can occurr consecutively

public:
	Tag();
	Tag(const std::string& _name, const std::string& _description, const std::map<DaysOfTheWeek, bool>& _enabledDays,
		const bool& _hasPriority, const unsigned int& _consecutiveLimit);
	~Tag();

	// SETTERS
	void setName(const std::string& new_name);
	void setDescription(const std::string& new_desc);
	void setEnabledDays(const std::map<DaysOfTheWeek, bool> new_enabledDays);
	void setPriority(const bool& hasPriority);
	void setConsecutiveLimit(const unsigned int& new_consecutiveLimit);

	// GETTERS
	std::string getName() const;
	std::string getDescription() const;
	std::map<DaysOfTheWeek, bool> getEnabledDays() const;
	bool getPriority() const;
	unsigned int getConsecutiveLimit() const;
};
#endif // !__TAG_HPP__
