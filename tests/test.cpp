#include "gtest/gtest.h"
#include "UIManager_test.hpp"
#include "Meal_test.hpp"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
