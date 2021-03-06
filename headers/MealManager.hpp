#ifndef __MEALMANAGER_HPP__
#define __MEALMANAGER_HPP__

#include "Meal.hpp"
#include "Tag.hpp"
#include "MultiTag.hpp"
#include "UIManager.hpp"


DaysOfTheWeek nextDay(const DaysOfTheWeek& day);
DaysOfTheWeek previousDay(const DaysOfTheWeek& day);
std::string dayToString(const DaysOfTheWeek& day);

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
	const unsigned int CONSECUTIVE_DAYS_LIMIT = 1000;
	const unsigned int MEAL_DURATION_LIMIT = 30;
	const unsigned int DAYS_BETWEEN_OCCURRENCES_LIMIT = 30;
	const unsigned int REQUESTED_MEALS_LIMIT = 100;

	// HELPER FUNCTIONS
	/* createMeal
	* INPUTS: 
	*	Meal* mealptr: pointer to instantiated Meal object
	*
	* RETURN: none
	* 
	* CHANGES: mealPtr: sets parameters according to user input, adds meal to meals in alphabetical order
	*/
	void createMeal(Meal* mealptr);

	/* deleteMeal
	* INPUTS:
	*	Meal* mealptr: pointer to instantiated Meal object
	*
	* RETURN: none
	*
	* CHANGES: mealPtr: Meal is removed from meals, and unlinked from Tags, then deleted and set to nullptr
	*/
	void deleteMeal(Meal* mealPtr);

	/* createTag
	* INPUTS:
	*	Tag* TagPtr: pointer to instantiated Tag object
	*
	* RETURN: none
	*
	* CHANGES: tagPtr: sets parameters according to user input, adds Tag to normalTags in alphabetical order
	*/
	void createTag(Tag* tagPtr);

	/* deleteTag
	* INPUTS:
	*	Tag* tagPtr: pointer to instantiated Tag object
	*
	* RETURN: none
	*
	* CHANGES: tagPtr: Tag is removed any linked MultiTags, Meals, and normalTags, then deleted and set to nullptr
	*/
	void deleteTag(Tag* tagPtr);

	/* createMultiTag
	* INPUTS:
	*	MultiTag* mTagPtr: pointer to instantiated MultiTag object
	*
	* RETURN: none
	*
	* CHANGES: mTagPtr: sets parameters according to user input, adds MultiTag to MultiTags in alphabetical order
	*/
	void createMultiTag(MultiTag* mtagPtr);

	/* deleteMultiTag
	* INPUTS:
	*	Tag* mtagPtr: pointer to instantiated MultiTag object
	*
	* RETURN: none
	*
	* CHANGES: mtagPtr: MultiTag is removed from any linked Tags and multiTags, then deleted and set to nullptr
	*/
	void deleteMultiTag(MultiTag* mtagPtr);

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
		std::map<DaysOfTheWeek, std::vector<Tag*>>& normalPriorityTags);

	// display all Meals/Tags, tagPtr points to Meal/Tag/MultiTag selected/created, or nullptr if user chose to quit
	// lastPageVistied is set to the last page (if pagee exist) the user was viewing
	// passing in a valid page in lastPageVisited will start the menu on that page
	// return values:
	//	if user quit menu: -1, if created a new tag: -2, if chose a tag: 0
	Meal* displayMeals(int& lastPagevisited);

	/* displayTags
	* INPUTS:
	*	int lastPage Visted: if pages need to be displayed, this will start at any valid page (instead of at the first page)
	*					 if value is incorrect (less than 0 or greater than last page), then menu starts at page 1
	* 
	*	Tag*& tagPtr: if user chooses or creates a tag, tagPtr will contain it's pointer, otherwise is set to nullptr
	* 
	*	const vector<Tag*>& excludeTags: optional param, if passed with tags, they will not be displayed in the menu
	*
	* OUTPUTS: int - menu status: returns 0 if user chose a tag, -1 if user quit, and -2 if user created a tag
	*/
	int displayTags(int& lastPageVisited, Tag*& tagPtr, const std::vector<Tag*>& excludeTags = std::vector<Tag*>()); 
	MultiTag* displayMultiTags(int& lastPageVisited);

	// queues information in UIM for one Meal/Tag
	void displayMealInfo(const Meal* mealPtr);
	void displayTagInfo(const Tag* tagPtr);
	void displayMultiTagInfo(const MultiTag* mtagPtr);

	// SAVING TO FILE
	void writeMeal(const Meal* mealPtr, std::ofstream& oFile);
	void writeTag(const Tag* tagPtr, std::ofstream& oFile);
	void writeMultiTag(const MultiTag* mtagPtr, std::ofstream& oFile);

	// READING FROM FILE
	// RETURN VALUES: 0 = good, 1 = error (aborted import), 2 = corrupted (used default values)
	int readMeals(std::ifstream& iFile);
	int readTags(std::ifstream& iFile);
	int readMultiTags(std::ifstream& iFile);

	// SCHEDULE CALCULATION
	// these functions return true if its object is valid (able to be used in calculations)
	// or false if their params forbid use on the current day
	bool validateTag(const Tag* tagPtr, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod, const std::vector<Meal*>& assignedMeals, const std::vector<std::vector<Meal*>>& scheduledMeals);
	bool validateMeal(const Meal* mealPtr, const unsigned int& currentDayNumber);
	void addFutureMeals(std::vector<Meal*>& futureMeals, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod, std::vector<std::vector<Meal*>>& scheduledMeals);

	bool scheduleMultiTags(const std::vector<MultiTag*>& availableMultiTags, const unsigned int& currentDayNumber,
		const unsigned int& calculationPeriod, const DaysOfTheWeek& currentDay, std::vector<std::vector<Meal*>>& scheduledMeals);

	bool scheduleNormalTags(const std::vector<Tag*>& availableTags, const unsigned int& currentDayNumber,
		const unsigned int& calculationPeriod, const DaysOfTheWeek& currentDay, std::vector<std::vector<Meal*>>& scheduledMeals);

	// write plan to file
	void printSchedule(const std::vector<std::vector<Meal*>>& mealPlan, const unsigned int& erroredDays , const double& totalCost, const double& budgetLimit, std::ofstream& oFile);

	// format strings
	std::string centeredText(const std::string& str, const unsigned int& lineWidth);

public:
	MealManager(const double& MINIMUM_PRICE, const double& MAXIMUM_PRICE, 
				const unsigned int& NAME_LENGTH, const unsigned int& DESC_LENGTH, UIManager& uim);
	~MealManager();

	// last 2 params are 0 unless generation fails, then they are the number of errored days and total cost of the first plan generated
	bool generateSchedule(const std::string& fileName, std::ofstream& oFile, unsigned int& failedPlanErrors, double& failedPlanCost);
	void mealEditor();
	void tagEditor();
	void multitagEditor();

	// Save/Load
	void saveState(const std::string& dataFile, std::ofstream& oFile);

	// returns 0 if good, 1 if data corrupted
	int loadState(const std::string& outputFile, std::ifstream& iFile);
};
#endif // !__MEALMANAGER_HPP__
