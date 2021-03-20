#ifndef __MEALMANAGER_HPP__
#define __MEALMANAGER_HPP__

#include "Meal.hpp"
#include "UIManager.hpp"

class MealManager
{
	std::vector<Tag*> normalTags; // tags assigned to foods
	std::vector<Tag*> globalTags; // tags that override normal tags' settings
	std::vector<Meal*> meals;

	// HELPER FUNCTIONS
	void createTag(Tag* tagPtr, UIManager& uim);
	void editMealTags(Meal* mealPtr, UIManager& uim);
	void editMeal(Meal* mealPtr, UIManager& uim, const double& MINIMUM_PRICE, const double& MAXIMUM_PRICE, const int& NAME_LENGTH);

public:
	MealManager();
	~MealManager();

	void generateSchedule(std::string outputFile, std::ostream& oFile = std::cout);
	void mealEditor(UIManager& uim);
	void tagEditor(UIManager& uim);

	// Save/Load >>>MAKE PRIVATE WHEN NOT TESTING<<< 
	void saveState(std::string fileName, std::ostream& oFile = std::cout);
	void loadState(std::string fileName, std::istream& iFile = std::cin);
};
#endif // !__MEALMANAGER_HPP__
