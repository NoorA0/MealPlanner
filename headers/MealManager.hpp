#ifndef __MEALMANAGER_HPP__
#define __MEALMANAGER_HPP__

#include "Meal.hpp"
#include "MultiTag.hpp"
#include "UIManager.hpp"

class MealManager
{
	std::vector<Tag*> normalTags; // tags assigned to foods
	std::vector<MultiTag*> multiTags; // special tags that are linked to normalTags
	std::vector<Meal*> meals;

	std::string DATAFILE; // name of file that stores program data
	std::string OUTPUTFILE; // name of file that stores generated meal list

	// initialized on object creation
	double MINIMUM_PRICE;
	double MAXIMUM_PRICE;
	unsigned int NAME_LENGTH;
	unsigned int DESC_LENGTH;

	// HELPER FUNCTIONS
	void createMeal(Meal* mealptr, UIManager& uim);
	void createTag(Tag* tagPtr, UIManager& uim);
	void createMultiTag(MultiTag* mtagPtr, UIManager& uim);
	void editMealTags(Meal* mealPtr, UIManager& uim);
	void editMultiTag(MultiTag* mtagPtr, UIManager& uim);
	void editMultiTagTags(MultiTag* mtagPtr, UIManager& uim);
	void editMeal(Meal* mealPtr, UIManager& uim);
	void editTag(Tag* tagPtr, UIManager& uim);
	std::string formatPrice(const double& price); // return string of price without trailing zeros

	// return string of enabledDays, if no days enabled, then will return "[]"
	std::string formatEnabledDays(const Meal* mealPtr);

	// display all Meals/Tags, returns index for meals or normalTags(), return -1 if user chose to quit
	int displayMeals(UIManager& uim);
	int displayTags(UIManager& uim);
	int displayMultiTags(UIManager& uim);

	// queues information in UIM for one Meal/Tag
	void displayMealInfo(const Meal* mealPtr, UIManager& uim);
	void displayTagInfo(const Tag* tagPtr, UIManager& uim);
	void displayMultiTagInfo(const MultiTag* mtagPtr, UIManager& uim);

public:
	MealManager(const double& MINIMUM_PRICE, const double& MAXIMUM_PRICE, 
				const unsigned int& NAME_LENGTH, const unsigned int& DESC_LENGTH);
	~MealManager();

	void generateSchedule(std::ostream& oFile = std::cout);
	void mealEditor(UIManager& uim);
	void tagEditor(UIManager& uim);

	// Save/Load >>>MAKE PRIVATE WHEN NOT TESTING<<< 
	void saveState(std::ostream& oFile = std::cout);
	void loadState(std::istream& iFile = std::cin);
};
#endif // !__MEALMANAGER_HPP__
