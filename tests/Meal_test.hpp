#ifndef __MEAL_TEST_HPP__
#define __MEAL_TEST_HPP__

#include "../headers/Meal.hpp"

TEST(MealTest, defaultConstructor)
{
	Meal testMeal;

	EXPECT_EQ(testMeal.getName(), "UNSET");
	EXPECT_DOUBLE_EQ(testMeal.getPrice(), 0.0);
	EXPECT_EQ(testMeal.getIsDisabled(), true);
}

TEST(MealTest, setName_getName)
{
	Meal testMeal;
	testMeal.setName("test_name0");
	EXPECT_EQ(testMeal.getName(), "test_name0");
}

TEST(MealTest, setPrice_getPrice)
{
	Meal testMeal;
	testMeal.setPrice(999.21);
	EXPECT_DOUBLE_EQ(testMeal.getPrice(), 999.21);
}

TEST(MealTest, setTags_getTags)
{
	std::map<DaysOfTheWeek, bool> enabledDays = 
	{ {MONDAY, false}, {TUESDAY, true}, {WEDNESDAY, false}, {THURSDAY, true},
	{FRIDAY, false}, {SATURDAY, true}, {SUNDAY, false} };

	Tag newTag("Name", "Desc", enabledDays, true, 3);
	std::vector<Tag*> newTags; newTags.push_back(&newTag);

	Meal testMeal;
	testMeal.setTags(newTags);

	EXPECT_EQ("Name", testMeal.getTags().at(0)->getName());
	EXPECT_EQ("Desc", testMeal.getTags().at(0)->getDescription());
	EXPECT_EQ(true, testMeal.getTags().at(0)->getDependency());
}

TEST(MealTest, addTag)
{
	std::map<DaysOfTheWeek, bool> enabledDays =
	{ {MONDAY, false}, {TUESDAY, true}, {WEDNESDAY, false}, {THURSDAY, true},
	{FRIDAY, false}, {SATURDAY, true}, {SUNDAY, false} };

	Tag* newTag = new Tag("10101", "000111", enabledDays, true, 3);

	Meal testMeal;
	testMeal.addTag(newTag);

	EXPECT_EQ("10101", testMeal.getTags().at(0)->getName());
	EXPECT_EQ("000111", testMeal.getTags().at(0)->getDescription());
	EXPECT_EQ(true, testMeal.getTags().at(0)->getDependency());
	
	delete newTag;
}

TEST(MealTest, isDisabled)
{
	Meal testMeal;
	testMeal.setIsDisabled(false);
	EXPECT_EQ(testMeal.getIsDisabled(), false);
}
#endif // !__MEAL_TEST_HPP__
