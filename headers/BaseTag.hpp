#ifndef __BASETAG_HPP__
#define __BASETAG_HPP__

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

class BaseTag
{
protected:
	std::string name;
	std::string description;
	std::map<DaysOfTheWeek, bool> enabledDays;

public:
	virtual ~BaseTag() {};

	// SETTERS
	virtual void setName(const std::string& _name) = 0;
	void setDescription(const std::string& _description) { description = _description; }
	void setEnabledDays(const std::map<DaysOfTheWeek, bool>& _enabledDays) { enabledDays = _enabledDays; }

	// GETTERS
	std::string getName() const { return name; }
	std::string getDescription() const { return description; }
	std::map<DaysOfTheWeek, bool> getEnabledDays() const { return enabledDays; }
	bool isDisabled() const;
};
#endif // !__BASETAG_HPP__
