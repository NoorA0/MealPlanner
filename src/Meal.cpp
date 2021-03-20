#include "../headers/Meal.hpp"

Meal::Meal()
{
	name = "UNSET";
	price = 0.0;
}

Meal::~Meal()
{
}

void Meal::setName(const std::string& new_name)
{
	name = new_name;
}

void Meal::setPrice(const double& new_price)
{
	price = new_price;
}

void Meal::setTags(const std::vector<Tag*>& new_tags)
{
	tags = new_tags;
}

void Meal::addTag(Tag* newTag)
{
	tags.push_back(newTag);
}

std::string Meal::getName() const
{
	return name;
}

double Meal::getPrice() const
{
	return price;
}

std::vector<Tag*> Meal::getTags() const
{
	return tags;
}
