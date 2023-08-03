#ifndef MEALMANAGER_H
#define MEALMANAGER_H

#include "meal.h"
#include "tag.h"
#include "multitag.h"

DaysOfTheWeek nextDay(const DaysOfTheWeek& day);
DaysOfTheWeek previousDay(const DaysOfTheWeek& day);
QString dayToString(const DaysOfTheWeek& day);

class MealManager
{
public:
    MealManager();
    ~MealManager();

    // failedPlanErrors and failedPlanCost are 0 unless generation fails,
    //  then they are the number of errored days and total cost of the first plan generated
    bool generateSchedule(const QString& fileName, std::ofstream& oFile, unsigned int& failedPlanErrors, double& failedPlanCost);
    void mealEditor();
    void tagEditor();
    void multitagEditor();

    /* createMeal
    * INPUTS:
    *	QString mealName  : name of meal
    *   double mealPrice : price of meal
    *   int mealDuration : how long the meal lasts
    *   int mealDBO      : days between meal occurrences
    *
    * RETURN: a valid pointer to the new Meal if succeeded, nullptr if name conflict
    *
    * creates a meal using inputs and adds meal to meals in alphabetical order
    */
    Meal* createMeal(const QString &mealName,
                   const double &mealPrice,
                   const int &mealDuration,
                   const int &mealDBO);

    /* findMeal
    * INPUTS:
    *	QString mealName  : name of meal
    *
    * RETURN: pointer to meal if exists, otherwise nullptr
    *
    * searches for a meal with the given name
    */
    Meal* findMeal(const QString &mealName);

    /* resortMeal
    * INPUTS:
    *	Meal* mealPtr   : pointer to existing meal, with old name
    *   QString newName : the desired new name for the meal
    *
    * RETURN: 0 if succeeded, 1 if meal's old name not found, or name conflict
    *
    * changes a meal's name and sorts by alphabetical order in meals
    */
    bool resortMeal(Meal *mealPtr, const QString &newName);

    /* deleteMeal
    * INPUTS:
    *	Meal* mealptr: pointer to an existing Meal object
    *
    * RETURN: none
    *
    * CHANGES: mealPtr: Meal is removed from meals, and unlinked from Tags, then deleted and set to nullptr
    */
    void deleteMeal(Meal* mealPtr);

    /* assignMealTags
    * INPUTS:
    *	Meal* mealPtr: pointer to existing meal
    *   QVector<Tag*> newTags: all tags to assign to the meal
    *
    * RETURN: none
    *
    * CHANGES:
    *   mealPtr: tags added to Meal.
    *   tags   : meal added to tags.
    */
    void assignMealTags(Meal* mealPtr, QVector<Tag*> newTags);

    /* removeMealTags
    * INPUTS:
    *	Meal* mealPtr: pointer to existing meal
    *   QVector<Tag*> removeTags: all tags to remove from the meal
    *
    * RETURN: none
    *
    * CHANGES:
    *   mealPtr: tags removed from Meal.
    *   tags   : meal removed from tags.
    */
    void removeMealTags(Meal* mealPtr, QVector<Tag*> removeTags);

    /* createTag
    * INPUTS:
    *	QString name           : name of Tag
    *   QString description    : describes the tag's purpose
    *   bool dependsOnMultiTag : set whether or not the tag requires a MultiTag
    *   int consecutiveLimit   : number of days the tag can occur for consecutively
    *   QMap<DaysOfTheWeek, bool> enabledDays : the days the Tag is enabled on
    *
    * RETURN: a valid pointer to the new Tag if succeeded, nullptr if name conflict
    *
    * creates a Tag using inputs and adds Tag to normalTags in alphabetical order of its name
    */
    Tag* createNormalTag(const QString &name,
                   const QString &description,
                   const bool &dependsOnMultiTag,
                   const int &consecutiveLimit,
                   const QMap<DaysOfTheWeek, bool> &enabledDays);

    /* findNormalTag
    * INPUTS:
    *	QString tagName  : name of normal Tag
    *
    * RETURN: pointer to normal Tag if exists, otherwise nullptr
    *
    * searches for a normal Tag with the given name
    */
    Tag* findNormalTag(const QString &tagName);

    /* resortNormalTag
    * INPUTS:
    *	Tag* tagPtr   : pointer to existing normal tag, with old name
    *   QString newName : the desired new name for the Tag
    *
    * RETURN: 0 if succeeded, 1 if tag's old name not found, or name conflict
    *
    * changes a Tag's name and sorts by alphabetical order in normalTags
    */
    bool resortNormalTag(Tag *tagPtr, const QString &newName);

    /* deleteTag
    * INPUTS:
    *	Tag* tagPtr: pointer to instantiated Tag object
    *
    * RETURN: none
    *
    * CHANGES: tagPtr: Tag is removed any linked MultiTags, Meals, and normalTags, then deleted and set to nullptr
    */
    void deleteNormalTag(Tag* tagPtr);

    /* assignNormalTagMeals
    * INPUTS:
    *	Tag* tagPtr: pointer to existing tag
    *   QVector<Meal*> newMeals: all meals to assign to the tag
    *
    * RETURN: none
    *
    * CHANGES:
    *   tagPtr: meals added to tag.
    *   meals   : tag added to meals.
    */
    void assignNormalTagMeals(Tag* tagPtr, QVector<Meal*> newMeals);

    /* removeNormalTagMeals
    * INPUTS:
    *	Tag* TagPtr: pointer to existing tag
    *   QVector<Meal*> removeMeals: all meals to remove from the tag
    *
    * RETURN: none
    *
    * CHANGES:
    *   tagPtr: meals removed from tag.
    *   meals   : tag removed from meals.
    */
    void removeNormalTagMeals(Tag* tagPtr, QVector<Meal*> removeMeals);

    /* formatEnabledDays
    * INPUTS:
    *	map<DaysOfTheWeek, bool> enabledDays: the enabled days of a Tag or MultiTag
    *
    * OUTPUTS: QString: output-friendly version of enabledDays
    *	example return values:
    *	[DISABLED] - if all days are FALSE
    *	[Mon, Tue, Wed, Thu, Fri, Sat, Sun] - if all days are TRUE
    */
    QString formatEnabledDays(const QMap<DaysOfTheWeek, bool>& enabledDays);

    // Save/Load
    void saveState(std::ofstream& oFile);

    // returns 0 if good, 1 if data corrupted
    int loadState(std::ifstream& iFile);

    // getters
    // constants
    unsigned int getConsecutiveDaysLimit(void) {return CONSECUTIVE_DAYS_LIMIT;}
    unsigned int getMealDurationLimit(void) {return MEAL_DURATION_LIMIT;}
    unsigned int getDaysBetweenOccurrencesLimit(void) {return DAYS_BETWEEN_OCCURRENCES_LIMIT;}
    unsigned int getRequestedMealsLimit(void) {return REQUESTED_MEALS_LIMIT;}
    QString getDataFileName(void) {return DATA_NAME;}
    QString getLogFileName(void) {return LOGFILE;}
    double getMinimumPrice(void) {return MINIMUM_PRICE;}
    double getMaximumPrice(void) {return MAXIMUM_PRICE;}
    unsigned int getMaximumNameLength(void) {return NAME_LENGTH;}
    unsigned int getMaximumDescriptionLength(void) {return DESC_LENGTH;}

    // variables
    int getNumberOfNormalTags(void) { return normalTags.size(); }
    int getNumberOfMultiTags(void) { return multiTags.size(); }
    int getNumberOfMeals(void) { return meals.size(); }
    const QVector<Meal*> getMeals(void) { return meals; }
    const QVector<Tag*> getNormalTags(void) { return normalTags; }
    const QVector<MultiTag*> getMultiTags(void) { return multiTags; }



private:
    // constant limits/parameters
    const unsigned int CONSECUTIVE_DAYS_LIMIT = 1000;
    const unsigned int MEAL_DURATION_LIMIT = 30;
    const unsigned int DAYS_BETWEEN_OCCURRENCES_LIMIT = 30;
    const unsigned int REQUESTED_MEALS_LIMIT = 100;
    const QString DATA_NAME = "MealPlanner_Data.txt";
    const QString LOGFILE = "MealPlanner_Logs.txt";
    const double MINIMUM_PRICE = 0.0;    // lowest price for a single meal
    const double MAXIMUM_PRICE = 999.99; // highest price for a single meal
    const unsigned int NAME_LENGTH = 40; // limit for tag and meal names
    const unsigned int DESC_LENGTH = 80; // limit for tag descriptions

    QVector<Tag*> normalTags; // tags assigned to foods
    QVector<MultiTag*> multiTags; // special tags that are linked to normalTags
    QVector<Meal*> meals; // stores all meals






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
    QString formatPrice(const double& price);

    // return string of enabledDays, if no days enabled, then will return "[]"

    /* optimizeData
    * Populates passed params with only enabled Tags/MultiTags/Meals to speed calculations
    */
    void optimizeData(QMap<DaysOfTheWeek, QVector<MultiTag*>>& highPriorityMultiTags,
                      QMap<DaysOfTheWeek, QVector<MultiTag*>>& normalPriorityMultiTags,
                      QMap<DaysOfTheWeek, QVector<Tag*>>& normalPriorityTags);

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
    int displayTags(int& lastPageVisited, Tag*& tagPtr, const QVector<Tag*>& excludeTags = QVector<Tag*>());
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
    bool validateTag(const Tag* tagPtr, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod,
                     const QVector<Meal*>& assignedMeals, const QVector<QVector<Meal*>>& scheduledMeals);
    bool validateMeal(const Meal* mealPtr, const unsigned int& currentDayNumber);
    void addFutureMeals(QVector<Meal*>& futureMeals, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod,
                        QVector<QVector<Meal*>>& scheduledMeals);

    bool scheduleMultiTags(const QVector<MultiTag*>& availableMultiTags, const unsigned int& currentDayNumber,
                           const unsigned int& calculationPeriod, const DaysOfTheWeek& currentDay, QVector<QVector<Meal*>>& scheduledMeals);

    bool scheduleNormalTags(const QVector<Tag*>& availableTags, const unsigned int& currentDayNumber,
                            const unsigned int& calculationPeriod, const DaysOfTheWeek& currentDay, QVector<QVector<Meal*>>& scheduledMeals);

    // write plan to file
    void printSchedule(const QVector<QVector<Meal*>>& mealPlan, const unsigned int& erroredDays , const double& totalCost, const double& budgetLimit, std::ofstream& oFile);

    // format strings
    QString centeredText(const QString& str, const unsigned int& lineWidth);
};

#endif // MEALMANAGER_H
