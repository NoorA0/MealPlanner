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
    // budget between MIN_BUDGET and MAX_BUDGET (inclusive)
    // planWeeks, in integer number of weeks, between MIN/MAX_CALCULATION_LENGTH (inclusive)
    bool generateSchedule(const QString& fileName,
                          const double &budget,
                          const unsigned int &planWeeks,
                          std::ofstream& oFile,
                          unsigned int& failedPlanErrors,
                          double& failedPlanCost);

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
    *   tagPtr  : meals added to tag.
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

    /* createMultiTag
    * INPUTS:
    *	QString name           : name of MultiTag
    *   QString description    : describes the MultiTag's purpose
    *   bool elevatedPriority  : set whether or not the MulitTag takes priority before Tags or with them
    *   bool totalFulfillment  : set whether or not all of this MT's assigned Tags must be fuilfilled to plan Meals
    *   QMap<DaysOfTheWeek, bool> enabledDays : the days the MultiTag is enabled on
    *
    * RETURN: a valid pointer to the new MultiTag if succeeded, nullptr if name conflict
    *
    * creates a MultiTag using inputs and adds the MultiTag to multiTags in alphabetical order of its name
    */
    MultiTag* createMultiTag(const QString &name,
                             const QString &description,
                             const bool &elevatedPriority,
                             const bool &totalFulfillment,
                             const QMap<DaysOfTheWeek, bool> &enabledDays);

    /* deleteMultiTag
    * INPUTS:
    *	MultiTag* tagPtr: pointer to instantiated MultiTag object
    *
    * RETURN: none
    *
    * CHANGES: tagPtr: MultiTag is removed from any linked Tags, then deleted and set to nullptr
    */
    void deleteMultiTag(MultiTag* tagPtr);

    /* findMultiTag
    * INPUTS:
    *	QString tagName  : name of MultiTag
    *
    * RETURN: pointer to MultiTag if exists, otherwise nullptr
    *
    * searches for a MultiTag with the given name
    */
    MultiTag* findMultiTag(const QString &tagName);

    /* resortMultiTag
    * INPUTS:
    *	MultiTag* tagPtr: pointer to existing MultiTag, with old name
    *   QString newName : the desired new name for the MultiTag
    *
    * RETURN: 0 if succeeded, 1 if MultiTag's old name not found, or name conflict
    *
    * changes a MultiTag's name and sorts by alphabetical order in mulitTags
    */
    bool resortMultiTag(MultiTag *tagPtr, const QString &newName);

    /* assignMultiTagTags
    * INPUTS:
    *	MultiTag* tagPtr: pointer to existing MultiTag
    *   QVector<QPair<Tag*, int>> assignTags: Tags to assign, with their number of requested Meals
    *
    * RETURN: none
    *
    * CHANGES:
    *   tagPtr  : Tags assigned to this MultiTag
    */
    void assignMultiTagTags(MultiTag* tagPtr, QVector<QPair<Tag*, int>> assignTags);

    /* removeMultiTagTags
    * INPUTS:
    *	MultiTag* tagPtr: pointer to existing MultiTag
    *   QVector<Tag*> removeTags: all Tags to remove from the MultiTag
    *
    * RETURN: none
    *
    * CHANGES:
    *   tagPtr  : Tags removed from this MultiTag.
    */
    void removeMultiTagTags(MultiTag* tagPtr, QVector<Tag*> removeMeals);

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
    unsigned int getRequestedMealsLimit(void) {return MAX_REQUESTEDMEALS;}
    QString getDataFileName(void) {return DATA_NAME;}
    QString getLogFileName(void) {return LOGFILE;}
    double getMinimumPrice(void) {return MINIMUM_PRICE;}
    double getMaximumPrice(void) {return MAXIMUM_PRICE;}
    unsigned int getMaximumNameLength(void) {return NAME_LENGTH;}
    unsigned int getMaximumFileNameLength(void) {return MAX_FILENAME_LENGTH;}
    unsigned int getMaximumDescriptionLength(void) {return DESC_LENGTH;}
    unsigned int getMinRequestedMeals(void) {return MIN_REQUESTEDMEALS;}
    unsigned int getMaxRequestedMeals(void) {return MAX_REQUESTEDMEALS;}
    unsigned int getMinimumPlanPeriodWeeks(void) {return MIN_CALCULATION_LENGTH;}
    unsigned int getMaximumPlanPeriodWeeks(void) {return MAX_CALCULATION_LENGTH;}

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
    const QString DATA_NAME = "MealPlanner_Data.txt";
    const QString LOGFILE = "MealPlanner_Logs.txt";
    const double MINIMUM_PRICE = 0.0;    // lowest price for a single meal
    const double MAXIMUM_PRICE = 999.99; // highest price for a single meal
    const unsigned int NAME_LENGTH = 40; // limit for tag and meal names
    const unsigned int MAX_FILENAME_LENGTH = 50; // char limit for filenames
    const unsigned int DESC_LENGTH = 80; // limit for tag descriptions
    const unsigned int MIN_REQUESTEDMEALS = 1;   // min number of requested meals from a Tag
    const unsigned int MAX_REQUESTEDMEALS = 100; // max number of requested meals from a Tag
    const unsigned int MIN_CALCULATION_LENGTH = 1; // 1 week
    const unsigned int MAX_CALCULATION_LENGTH = 52; // just under a year (in weeks)
    const unsigned int MIN_BUDGET = 0;
    const unsigned int MAX_BUDGET = 100000;

    // used in generateSchedule()
    const unsigned int GENERATED_PLANS = 500; // number of meal plans to make
    const double ERROR_THRESHOLD_PER_WEEK = 0.5; // amount of errored days allowed per week

    QVector<Tag*> normalTags; // tags assigned to foods
    QVector<MultiTag*> multiTags; // special tags that are linked to normalTags
    QVector<Meal*> meals; // stores all meals


    /* formatPrice
    * INPUTS:
    *	double price: stores a double value
    *
    * OUTPUTS: string: output-friendly form of price, with extra trailing zeros and decimal point removed
    */
    std::string formatPrice(const double& price);

    // return string of enabledDays, if no days enabled, then will return "[]"

    /* optimizeData
    * Populates passed params with only enabled Tags/MultiTags/Meals to speed calculations
    */
    void optimizeData(QMap<DaysOfTheWeek, QVector<MultiTag*>>& highPriorityMultiTags,
                      QMap<DaysOfTheWeek, QVector<MultiTag*>>& normalPriorityMultiTags,
                      QMap<DaysOfTheWeek, QVector<Tag*>>& normalPriorityTags);

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
