#ifndef __MULTITAG_HPP__
#define __MULTITAG_HPP__

#include "Tag.hpp"
#include <vector>

class Multitag : public Tag
{
	std::vector<Tag*> linkedTags;

public:
	Multitag();
	~Multitag();

	void setLinkedTags(const std::vector<Tag*>& new_linkedTags);
	void addLinkedTag(Tag* new_tag);

	std::vector<Tag*> getLinkedTags() const;
};
#endif // !__MULTITAG_HPP__
