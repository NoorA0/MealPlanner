#ifndef __MEAL_TEST_HPP__
#define __MEAL_TEST_HPP__

#include "../headers/Meal.hpp"

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
	Meal testMeal;
	std::vector<Tag> testTags = { NONE, TAKESLONG };
	testMeal.setTags(testTags);

	EXPECT_EQ(NONE, testMeal.getTags().at(0));
	EXPECT_EQ(TAKESLONG, testMeal.getTags().at(1));
}

TEST(MealTest, setEnabledDays_getEnabledDays)
{
	Meal testMeal;
	std::map<DaysOfTheWeek, bool> testDOTW =
	{ {MONDAY, false}, {TUESDAY, true}, {WEDNESDAY, false}, {THURSDAY, true},
	{FRIDAY, false}, {SATURDAY, true}, {SUNDAY, false} };

	testMeal.setEnabledDays(testDOTW);
	std::map<DaysOfTheWeek, bool> returnDOTW = testMeal.getEnabledDays();

	EXPECT_EQ(returnDOTW.find(MONDAY)->first, MONDAY);
	EXPECT_EQ(returnDOTW.find(MONDAY)->second, false);
	EXPECT_EQ(returnDOTW.find(TUESDAY)->second, true);
	EXPECT_EQ(returnDOTW.find(WEDNESDAY)->second, false);
	EXPECT_EQ(returnDOTW.find(THURSDAY)->second, true);
	EXPECT_EQ(returnDOTW.find(FRIDAY)->second, false);
	EXPECT_EQ(returnDOTW.find(SATURDAY)->second, true);
	EXPECT_EQ(returnDOTW.find(SUNDAY)->second, false);
}


#endif // !__MEAL_TEST_HPP__
