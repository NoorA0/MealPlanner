#include "../headers/Multitag.hpp"

Multitag::Multitag()
{
}

Multitag::~Multitag()
{
	for (auto tagIter : linkedTags)
	{
		delete tagIter;
	}
}

void Multitag::setLinkedTags(const std::vector<Tag*>& new_linkedTags)
{
	for (auto tagIter : linkedTags)
	{
		delete tagIter;
	}

	linkedTags = new_linkedTags;
}

void Multitag::addLinkedTag(Tag* new_tag)
{
	linkedTags.push_back(new_tag);
}

std::vector<Tag*> Multitag::getLinkedTags() const
{
	return linkedTags;
}
