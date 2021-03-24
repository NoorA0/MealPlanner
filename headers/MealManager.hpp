#ifndef __MEALMANAGER_HPP__
#define __MEALMANAGER_HPP__

#include "Meal.hpp"
#include "MultiTag.hpp"
#include "UIManager.hpp"

class MealManager
{
	std::vector<Tag*> normalTags; // tags assigned to foods
	std::vector<MultiTag*> multiTags; // special tags that are linked to normalTags
	std::vector<Meal*> meals; // stores all meals
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
	*
	* RETURN: none
	* 
	* CHANGES: mealPtr: creates a new Meal and sets parameters according to user input
	*/
	void createMeal(Meal* mealptr);

	/* createTag
	* INPUTS:
	*	Tag* TagPtr: should be instantiated to nullptr
	*
	* RETURN: none
	*
	* CHANGES: tagPtr: creates a new Tag and sets parameters according to user input
	*/
	void createTag(Tag* tagPtr);

	/* createMultiTag
	* INPUTS:
	*	MultiTag* mTagPtr: should be instantiated to nullptr
	*
	* RETURN: none
	*
	* CHANGES: mTagPtr: creates a new MultiTag and sets parameters according to user input
	*/
	void createMultiTag(MultiTag* mtagPtr);

	/* editMealTags
	* INPUTS:
	*	Meal* mealPtr: point to existing meal
	*
	* RETURN: none
	*
	* CHANGES: mealPtr: existing tags added to passed in Meal.
	*	if no Tags exist, then a new one is created and added to Meal and normalTags
	*/
	void editMealTags(Meal* mealPtr);

	/* editMultiTag
	* INPUTS:
	*	MultiTag* mtagPtr: point to existing MultiTag
	*
	* OUTPUTS: none
	*
	* CHANGES: mtagPtr: params edited according to user choice
	*/
	void editMultiTag(MultiTag* mtagPtr);

	/* editMultiTagTags
	* INPUTS:
	*	MultiTag* mtagPtr: point to existing MultiTag
	*
	* RETURN: none
	*
	* CHANGES: mtagPtr: existing tags added to passed in MultiTag.
	*	if no Tags exist, then a new one is created and added to MulitTag and normalTags
	*/
	void editMultiTagTags(MultiTag* mtagPtr);

	/* editMeal
	* INPUTS:
	*	Meal* mealPtr: point to existing Meal
	*
	* OUTPUTS: none
	*
	* CHANGES: mealPtr: params edited according to user choice
	*/
	void editMeal(Meal* mealPtr);

	/* editTag
	* INPUTS:
	*	Tag* tagPtr: point to existing Tag
	*
	* OUTPUTS: none
	*
	* CHANGES: tagPtr: params edited according to user choice
	*/
	void editTag(Tag* tagPtr);

	/* formatPrice
	* INPUTS:
	*	double price: stores a double value
	*
	* OUTPUTS: string: output-friendly form of price, with extra trailing zeros and decimal point removed
	*/
	std::string formatPrice(const double& price);

	// return string of enabledDays, if no days enabled, then will return "[]"

	/* formatEnabledDays
	* INPUTS:
	*	map<DaysOfTheWeek, bool> enabledDays: the enabled days of a Tag or MultiTag
	*
	* OUTPUTS: string: output-friendly version of enabledDays
	*	example return values: 
	*	[] - if all days are FALSE
	*	[M, T, W, Th, F, Sa, Su] - if all days are TRUE
	*/
	std::string formatEnabledDays(const std::map<DaysOfTheWeek, bool>& enabledDays);

	/* optimizeData
	* Populates passed params with only enabled Tags/MultiTags/Meals to speed calculations
	*/
	void optimizeData(std::map<DaysOfTheWeek, std::vector<MultiTag*>>& highPriorityMultiTags,
		std::map<DaysOfTheWeek, std::vector<MultiTag*>>& normalPriorityMultiTags,
		std::map<DaysOfTheWeek, std::vector<Tag*>> normalPriorityTags,
		std::map<Tag*, std::vector<Meal*>> availableMeals);

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

	void generateSchedule(const std::string& fileName, std::ofstream& oFile);
	void mealEditor();
	void tagEditor();

	// Save/Load >>>MAKE PRIVATE WHEN NOT TESTING<<< 
	void saveState(const std::string& dataFile, std::ofstream& oFile);
	void loadState(const std::string& outputFile, std::ifstream& iFile);
};
#endif // !__MEALMANAGER_HPP__
