CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CXX_STANDARD 11)

ADD_EXECUTABLE(MealPlanner
src/main.cpp src/UIManager.cpp src/Meal.cpp src/Tag.cpp src/MealManager.cpp)

ADD_EXECUTABLE(test 
tests/test.cpp src/UIManager.cpp src/Meal.cpp src/Tag.cpp src/MealManager.cpp)

ADD_SUBDIRECTORY(googletest)

TARGET_LINK_LIBRARIES(test gtest)

TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
