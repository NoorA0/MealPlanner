#ifndef __MULTITAG_HPP__
#define __MULTITAG_HPP__

#include "BaseTag.hpp"

class Tag;

class MultiTag : public BaseTag
{
private:

	bool highestPriority; // whether MultiTag should be prioritized above Tags or not

	// ints are used to indicate requested amount of Meals of Tag
	// requesting 2 Sandwiches (tags) would mean the Tag called Sandwiches is loaded into linkedTags with an int of 2
	std::map<Tag*, unsigned int> linkedTags;


public:
	MultiTag();
	~MultiTag();

	// SETTERS
	void setName(const std::string& _name) { name = _name; }
	void setHighestPriority(const bool& _priority) { highestPriority = _priority; }
	void setLinkedTags(const std::map<Tag*, unsigned int>& _linkedTags) { linkedTags = _linkedTags; }
	void addLinkedTag(Tag* new_tag, const unsigned int& amount);
	bool removeLinkedTag(const Tag* tagPtr);
	void clearLinkedTags() { linkedTags.clear(); }

	// GETTERS
	bool getPriority() const { return highestPriority; }
	std::map<Tag*, unsigned int> getLinkedTags() const { return linkedTags; }

};
#endif // !__MULTITAG_HPP__
