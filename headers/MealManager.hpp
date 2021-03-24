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
	UIManager* uim; // used to create and display UI to user

	// initialized on object creation
	double MINIMUM_PRICE;
	double MAXIMUM_PRICE;
	unsigned int NAME_LENGTH;
	unsigned int DESC_LENGTH;

	// HELPER FUNCTIONS
	/* createMeal
	* INPUTS: 
	*	Meal* mealptr: pointer to Meal object, should be instantiated to nullptr
	*	UIManager&
	* 
	*/
	void createMeal(Meal* mealptr);
	void createTag(Tag* tagPtr);
	void createMultiTag(MultiTag* mtagPtr);
	void editMealTags(Meal* mealPtr);
	void editMultiTag(MultiTag* mtagPtr);
	void editMultiTagTags(MultiTag* mtagPtr);
	void editMeal(Meal* mealPtr);
	void editTag(Tag* tagPtr);
	std::string formatPrice(const double& price); // return string of price without trailing zeros

	// return string of enabledDays, if no days enabled, then will return "[]"
	std::string formatEnabledDays(const Meal* mealPtr);

	// display all Meals/Tags, returns index for meals or normalTags(), return -1 if user chose to quit
	int displayMeals();
	int displayTags();
	int displayMultiTags();

	// queues information in UIM for one Meal/Tag
	void displayMealInfo(const Meal* mealPtr);
	void displayTagInfo(const Tag* tagPtr);
	void displayMultiTagInfo(const MultiTag* mtagPtr);

	// SAVING TO FILE
	void writeMeal(const Meal* mealPtr, std::ofstream& oFile);
	void writeTag(const Tag* tagPtr, std::ofstream& oFile);
	void writeMultiTag(const MultiTag* mtagPtr, std::ofstream& oFile);

	// READING FROM FILE
	bool readMeals(std::ifstream& iFile);
	bool readTags(std::ifstream& iFile);
	bool readMultiTags(std::ifstream& iFile);

public:
	MealManager(const double& MINIMUM_PRICE, const double& MAXIMUM_PRICE, 
				const unsigned int& NAME_LENGTH, const unsigned int& DESC_LENGTH, UIManager& uim);
	~MealManager();

	void generateSchedule(std::ostream& oFile = std::cout);
	void mealEditor();
	void tagEditor();

	// Save/Load >>>MAKE PRIVATE WHEN NOT TESTING<<< 
	void saveState(const std::string& dataFile, std::ofstream& oFile);
	void loadState(const std::string& outputFile, std::ifstream& iFile);
};
#endif // !__MEALMANAGER_HPP__
