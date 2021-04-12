#include "../headers/MealManager.hpp"
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <sstream>
#include <algorithm>

MealManager::MealManager(const double& MINIMUM_PRICE, const double& MAXIMUM_PRICE,
	const unsigned int& NAME_LENGTH, const unsigned int& DESC_LENGTH, UIManager& uim)
{
	// verify values
	if (MINIMUM_PRICE >= 0)
		this->MINIMUM_PRICE = MINIMUM_PRICE;
	else
		this->MINIMUM_PRICE = 0;

	if (MAXIMUM_PRICE < this->MINIMUM_PRICE)
		this->MAXIMUM_PRICE = MINIMUM_PRICE;
	else
		this->MAXIMUM_PRICE = MAXIMUM_PRICE;

	this->NAME_LENGTH = NAME_LENGTH;
	this->DESC_LENGTH = DESC_LENGTH;

	this->uim = &uim;
}

MealManager::~MealManager()
{
	// delete all tags
	for (auto tagsIter : normalTags)
	{
		// clear tag's linkedMeals
		tagsIter->clearLinkedMeals();
		delete tagsIter;
	}
	normalTags.clear();

	// delete all meals
	for (auto mealsIter : meals)
	{
		// clear vector of tags
		std::vector<Tag*> emptyVec;
		mealsIter->setTags(emptyVec);

		delete mealsIter;
	}
	meals.clear();

	// delete all multiTags
	for (auto tagsIter : multiTags)
	{
		// clear map of Tags
		std::map<Tag*, unsigned int> emptyMap;
		tagsIter->setLinkedTags(emptyMap);

		delete tagsIter;
	}
	multiTags.clear();
	uim = nullptr;
}

void MealManager::createMeal(Meal* mealptr)
{
	bool doneCreating = false;
	bool inputValid = false;
	std::string tempStr = "";
	int tempInt;
	double tempDouble;
	std::vector<std::string> strVec;

	// ensure mealPtr is instantiated
	if (mealptr == nullptr)
		mealptr = new Meal();

	// loop while user is not done choosing a name
	while (!doneCreating)
	{
		uim->centeredText("New Meal");
		uim->skipLines(2);

		// prompt and get name
		uim->centeredText("Enter a name:");
		uim->skipLines(2);
		uim->leftAllignedText("Examples:");
		uim->skipLines(1);
		uim->leftAllignedText("Hamburgers");
		uim->leftAllignedText("Tacos");
		uim->leftAllignedText("Ceasar Salad");
		uim->leftAllignedText("The Dijkstra Special");
		uim->prompt_FreeString(1, NAME_LENGTH);
		tempStr = uim->display();

		// if there are other meals stored, check if names conflict (case insensitive)
		// also try to find place to insert into (alphabetical order)
		if (meals.size() > 0)
		{
			// get uppercase version of tempStr
			std::string upperStr = tempStr;
			for (int i = 0; i < upperStr.size(); ++i)
			{
				upperStr.at(i) = std::toupper(upperStr.at(i));
			}

			// compare with names of all meals until a match is found
			// also find a place to insert into (alphabetical order)
			bool placeFound = false;
			inputValid = true;
			auto mealsIter = meals.begin();
			while (inputValid && !placeFound && mealsIter != meals.end())
			{
				// get uppercase version of meal's name
				std::string compareName = (*mealsIter)->getName();
				for (int i = 0; i < compareName.size(); ++i)
				{
					compareName.at(i) = std::toupper(compareName.at(i));
				}

				// check if names match
				if (upperStr == compareName)
					inputValid = false;
				else if (upperStr < compareName) // check if is this a good place to insert
				{
					// insert here
					placeFound = true;
					mealptr->setName(tempStr);
					meals.emplace(mealsIter, mealptr);
					doneCreating = true;
				}
				else // keep looking
					++mealsIter;
			}

			// check if name is valid but place not found
			if (inputValid && !placeFound)
			{
				// set name and place at back of meals
				mealptr->setName(tempStr);
				meals.push_back(mealptr);
				doneCreating = true; // exits section of creation
			}
			else if (!inputValid) // invalid name, inform user
			{
				uim->centeredText("Error: Invalid Name");
				uim->skipLines(2);
				uim->centeredText("The name you entered already exists. (case insensitive)");
				uim->display();
			}
		}
		else // name is ok, push to back of meals
		{
			mealptr->setName(tempStr);
			meals.push_back(mealptr);
			doneCreating = true;
		}
	}// while (!doneCreating)

	// price
	tempStr = "New Meal: \"" + mealptr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->skipLines(2);

	// prompt and get price
	uim->centeredText("Enter a price:");
	uim->prompt_FreeDouble(MINIMUM_PRICE, MAXIMUM_PRICE);
	tempDouble = std::stod(uim->display());

	mealptr->setPrice(tempDouble);

	// mealDuration
	tempStr = "New Meal: \"" + mealptr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->centeredText("Duration");
	uim->skipLines(2);
	uim->centeredText("How many days does this meal last?");
	uim->centeredText("(1 day is typical, Meals prepared in large batches may last longer.)");
	uim->prompt_FreeInt(1, MEAL_DURATION_LIMIT);
	tempInt = std::stoi(uim->display());

	mealptr->setMealDuration(tempInt);

	// daysBetweenOccurrences
	tempStr = "New Meal: \"" + mealptr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->centeredText("Days Between Occurrences");
	uim->skipLines(2);
	uim->centeredText("When this Meal is scheduled, how many days should pass before it can be scheduled again?");
	uim->centeredText("(A value of 0 will not delay scheduling, while a value of 1 will make it occur every other day at most.)");
	uim->prompt_FreeInt(0, DAYS_BETWEEN_OCCURRENCES_LIMIT);
	tempInt = std::stoi(uim->display());

	mealptr->setDaysBetweenOccurrences(tempInt);

	// enable meal
	mealptr->setDisabled(false);

	// assign tags to meal
	editMealTags(mealptr);
}

void MealManager::deleteMeal(Meal* mealPtr)
{
	// get assigned tags
	std::vector<Tag*> assignedTags = mealPtr->getTags();

	// if meal has assigned tags, unlink them from meal
	if (assignedTags.size() > 0)
	{
		for (auto tagIter : assignedTags)
		{
			tagIter->removeMeal(mealPtr);
		}
		
		// clear tags
		mealPtr->clearTags();
	}

	// remove meal from meals
	bool found = false;
	auto mealIter = meals.begin();

	// find meal
	while (!found && mealIter != meals.end())
	{
		if (*mealIter == mealPtr)
			found = true;
		else
			++mealIter;
	}

	if (found)
	{
		meals.erase(mealIter);
		delete mealPtr;
		mealPtr = nullptr;
	}
}

void MealManager::createTag(Tag* tagPtr)
{
	std::string tempStr = "";
	int tempInt = 0;
	std::vector<std::string> strVec;
	bool doneCreatingName = false;

	// tag's params
	std::map<DaysOfTheWeek, bool> enabledDays =
	{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false}, {FRIDAY, false},
		{SATURDAY, false}, {SUNDAY, false} };


	// ensure pointer is instantiated
	if (tagPtr == nullptr)
	{
		tagPtr = new Tag();
	}

	// loop while user is not done choosing a name
	while (!doneCreatingName)
	{
		// prompt and get name
		uim->centeredText("New Tag");
		uim->skipLines(2);

		uim->centeredText("Enter a name:");
		uim->centeredText("(recommended 2 - 4 words)");
		uim->skipLines(2);
		uim->centeredText("Examples:"); 
		uim->skipLines(1);
		uim->centeredText("Chicken");
		uim->centeredText("Vegetarian");
		uim->centeredText("Takes long to cook");
		uim->centeredText("Dessert");
		uim->centeredText("Weekends Only");
		uim->prompt_FreeString(1, NAME_LENGTH);

		tempStr = uim->display();

		// if there are other tags stored, check if names conflict (case insensitive)
		if (normalTags.size() > 0)
		{
			// get uppercase version of tempStr
			std::string upperStr = tempStr;
			for (int i = 0; i < upperStr.size(); ++i)
			{
				upperStr.at(i) = std::toupper(upperStr.at(i));
			}

			// compare with names of all tags until a match is found
			// also find a place to insert into (alphabetical order)
			bool placeFound = false;
			bool inputValid = true;
			auto tagsIter = normalTags.begin();
			while (inputValid && !placeFound && tagsIter != normalTags.end())
			{
				// get uppercase version of tag's name
				std::string compareName = (*tagsIter)->getName();
				for (int i = 0; i < compareName.size(); ++i)
				{
					compareName.at(i) = std::toupper(compareName.at(i));
				}

				// check if names match
				if (upperStr == compareName)
					inputValid = false;
				else if (upperStr < compareName) // check if this is a good place to insert
				{
					// insert here
					placeFound = true;
					tagPtr->setName(tempStr);
					normalTags.emplace(tagsIter, tagPtr);
					doneCreatingName = true;
				}
				else // keep searching
					++tagsIter;
			}

			// check if name is valid but place not found
			if (inputValid && !placeFound)
			{
				// set name
				tagPtr->setName(tempStr);

				// push to back
				normalTags.push_back(tagPtr);
				doneCreatingName = true;
			}
			else if (!inputValid) // invalid name, inform user
			{
				uim->centeredText("Error: Invalid Name");
				uim->skipLines(2);
				uim->centeredText("The name you entered already exists. (case insensitive)");
				uim->display();
			}
		}
		else // no tags to compare so set name
		{
			tagPtr->setName(tempStr);

			// push to back
			normalTags.push_back(tagPtr);
			doneCreatingName = true;
		}
	} // while (!doneCreatingName)

	// prompt and get description
	tempStr = "New Tag: \"" + tagPtr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->centeredText("Description");
	uim->skipLines(2);

	uim->centeredText("Enter a description:");
	uim->centeredText("(Press <enter> to skip)");
	uim->prompt_FreeString(0, DESC_LENGTH);
	tagPtr->setDescription(uim->display());

	// prompt and get dependency
	tempStr = "New Tag: \"" + tagPtr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->centeredText("MultiTag Dependency");
	uim->skipLines(2);

	uim->centeredText("Disable this Tag unless linked with a MultiTag?");
	uim->centeredText("(Recommended: No)");
	strVec = { "YYes", "NNo" };
	uim->prompt_List_Case_Insensitive(strVec);

	tempStr = uim->display();
	tempStr = std::toupper(tempStr.at(0));

	// set hasPriority depending on user choice
	if (tempStr == "Y")
		tagPtr->setDependency(true);
	else
		tagPtr->setDependency(false);

	// prompt and get consecutiveLimit
	tempStr = "New Tag: \"" + tagPtr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->centeredText("Consecutive Days Limit");
	uim->skipLines(2);

	uim->centeredText("How many consecutive days can a Meal with this Tag occur for?");
	uim->skipLines(1);
	uim->centeredText("This setting is Tag-specific, not Meal-specific. It applies to all Meals assigned to this Tag.");
	uim->centeredText("(A value of 1 means that Meals will occur every other day at most, a value of 3 allows 3 days of Meals from this Tag to occur in a row.)");
	uim->prompt_FreeInt(1, CONSECUTIVE_DAYS_LIMIT);
	tagPtr->setConsecutiveLimit(std::stoi(uim->display()));

	tempStr = "";
	// let user set enabled days or quit
	while (tempStr != "Q")
	{
		tempStr = "New Tag: \"" + tagPtr->getName() + "\"";
		uim->centeredText(tempStr);
		uim->centeredText("Enabled Days");
		uim->skipLines(2);

		uim->centeredText("Choose days that Meals with this Tag may occur on:");
		uim->skipLines(1);

		tempStr = "MON: ";
		if (enabledDays.find(MONDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "TUE: ";
		if (enabledDays.find(TUESDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "WED: ";
		if (enabledDays.find(WEDNESDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "THU: ";
		if (enabledDays.find(THURSDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "FRI: ";
		if (enabledDays.find(FRIDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "SAT: ";
		if (enabledDays.find(SATURDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "SUN: ";
		if (enabledDays.find(SUNDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		// print options
		strVec.clear();
		strVec = { "1Toggle MONDAY", "2Toggle TUESDAY", "3Toggle WEDNESDAY", "4Toggle THURSDAY",
				  "5Toggle FRIDAY", "6Toggle SATURDAY", "7Toggle SUNDAY", "",
				  "EEnable ALL", "DDisable ALL","", "QExit & Confirm Selection" };

		// prompt and get input
		uim->prompt_List_Case_Insensitive(strVec);
		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr == "E")
		{
			// set all days to TRUE
			enabledDays =
			{ {MONDAY, true}, {TUESDAY, true}, {WEDNESDAY, true}, {THURSDAY, true},
			{FRIDAY, true}, {SATURDAY, true}, {SUNDAY, true} };
		}
		else if (tempStr == "D")
		{
			// set all days to FALSE
			enabledDays =
			{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
			{FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };
		}
		else if (tempStr != "Q" && tempStr != "-")
		{
			// get user's choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1: // monday
				if (enabledDays.find(MONDAY)->second)
					enabledDays.find(MONDAY)->second = false;
				else
					enabledDays.find(MONDAY)->second = true;
				break;
			case 2: // tuesday
				if (enabledDays.find(TUESDAY)->second)
					enabledDays.find(TUESDAY)->second = false;
				else
					enabledDays.find(TUESDAY)->second = true;
				break;
			case 3: // wednesday
				if (enabledDays.find(WEDNESDAY)->second)
					enabledDays.find(WEDNESDAY)->second = false;
				else
					enabledDays.find(WEDNESDAY)->second = true;
				break;
			case 4: // thursday
				if (enabledDays.find(THURSDAY)->second)
					enabledDays.find(THURSDAY)->second = false;
				else
					enabledDays.find(THURSDAY)->second = true;
				break;
			case 5: // friday
				if (enabledDays.find(FRIDAY)->second)
					enabledDays.find(FRIDAY)->second = false;
				else
					enabledDays.find(FRIDAY)->second = true;
				break;
			case 6: // saturday
				if (enabledDays.find(SATURDAY)->second)
					enabledDays.find(SATURDAY)->second = false;
				else
					enabledDays.find(SATURDAY)->second = true;
				break;
			case 7: // sunday
				if (enabledDays.find(SUNDAY)->second)
					enabledDays.find(SUNDAY)->second = false;
				else
					enabledDays.find(SUNDAY)->second = true;
				break;
			default:
				// do nothing 
				tempStr = "";
			}
		}
	} // while (tempStr != "Q")
	tagPtr->setEnabledDays(enabledDays);
}

void MealManager::deleteTag(Tag* tagPtr)
{
	// get linked meals
	std::vector<Meal*> linkedMeals = tagPtr->getLinkedMeals();

	// if tag has linked meals, unlink them
	if (linkedMeals.size() > 0)
	{
		for (auto mealIter : linkedMeals)
		{
			mealIter->removeTag(tagPtr);
		}

		// clear linkedMeals
		tagPtr->clearLinkedMeals();
	}

	// unlink tag from all multiTags
	for (auto mtagIter : multiTags)
	{
		mtagIter->removeLinkedTag(tagPtr);
	}

	// find tag in normalTags
	auto tagIter = normalTags.begin();
	bool found = false;

	while (!found && tagIter != normalTags.end())
	{
		if (*tagIter == tagPtr)
			found = true;
		else
			++tagIter;
	}

	// remove from normalTags and delete
	if (found)
	{
		normalTags.erase(tagIter);
		delete tagPtr;
		tagPtr = nullptr;
	}
}

void MealManager::createMultiTag(MultiTag* mtagPtr)
{
	std::string tempStr = "";
	int tempInt;
	std::vector<std::string> strVec;
	bool doneCreatingName = false;

	// ensure pointer is instantiated to an object
	if (mtagPtr == nullptr)
		mtagPtr = new MultiTag();

	// multitag's params
	std::map<DaysOfTheWeek, bool> enabledDays =
	{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false}, {FRIDAY, false},
		{SATURDAY, false}, {SUNDAY, false} };


	// loop while user is not done choosing a name
	while (!doneCreatingName)
	{
		// prompt and get name
		uim->centeredText("New MultiTag");
		uim->skipLines(2);

		uim->centeredText("Enter a name:");
		uim->centeredText("(recommended 2 - 4 words)");
		uim->skipLines(2);
		uim->leftAllignedText("Examples:");
		uim->skipLines(1);
		uim->leftAllignedText("Guest Meals");
		uim->leftAllignedText("No Time");
		uim->leftAllignedText("Birthday Party");
		uim->leftAllignedText("Many Meals");
		uim->prompt_FreeString(1, NAME_LENGTH);

		tempStr = uim->display();

		// if there are other tags stored, check if names conflict (case insensitive)
		if (multiTags.size() > 0)
		{
			// get uppercase version of tempStr
			std::string upperStr = tempStr;
			for (int i = 0; i < upperStr.size(); ++i)
			{
				upperStr.at(i) = std::toupper(upperStr.at(i));
			}

			// compare with names of all tags until a match is found
			bool placeFound = false;
			bool inputValid = true;
			auto tagsIter = multiTags.begin();
			while (inputValid && !placeFound && tagsIter != multiTags.end())
			{
				// get uppercase version of tag's name
				std::string compareName = (*tagsIter)->getName();
				for (int i = 0; i < compareName.size(); ++i)
				{
					compareName.at(i) = std::toupper(compareName.at(i));
				}

				// check if names match
				if (upperStr == compareName)
					inputValid = false;
				else if (upperStr < compareName) // check if this is a good place to insert
				{
					// insert here
					placeFound = true;
					mtagPtr->setName(tempStr);
					multiTags.emplace(tagsIter, mtagPtr);
					doneCreatingName = true;
				}
				else // keep looking
					++tagsIter;
			}

			// check if name is valid but place not found
			if (inputValid && !placeFound)
			{
				// set name and place at back of multitags
				mtagPtr->setName(tempStr);
				multiTags.push_back(mtagPtr);
				doneCreatingName = true;
			}
			else if (!inputValid) // invalid name, inform user
			{
				uim->centeredText("Error: Invalid Name");
				uim->skipLines(2);
				uim->centeredText("The name you entered already exists. (case insensitive)");
				uim->display();
			}
		}
		else // no tags to compare so push to back of meals
		{
			mtagPtr->setName(tempStr);
			multiTags.push_back(mtagPtr);
			doneCreatingName = true;
		}
	} // while (!doneCreatingName)

	// prompt and get description
	tempStr = "New MultiTag: \"" + mtagPtr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->skipLines(2);

	uim->centeredText("Enter a description:");
	uim->centeredText("(Press <enter> to skip)");
	uim->prompt_FreeString(0, DESC_LENGTH);
	mtagPtr->setDescription(uim->display());

	// prompt and get dependency
	tempStr = "New MultiTag: \"" + mtagPtr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->centeredText("Priority Level");
	uim->skipLines(2);

	uim->centeredText("MultiTags can take priority over normal Tags when creating a Meal Plan.");
	uim->centeredText("Do you want to give this MultiTag elevated priority, or make it behave like normal Tag?");

	strVec = { "1Elevated Priority", "2Behave like a normal Tag" };
	uim->prompt_List_Case_Insensitive(strVec);

	tempStr = uim->display();

	// set priority depending on user choice
	if (tempStr == "1")
		mtagPtr->setHighestPriority(true);
	else
		mtagPtr->setHighestPriority(false);

	tempStr = "";
	// let user set enabled days or quit
	while (tempStr != "Q")
	{
		tempStr = "New MultiTag: \"" + mtagPtr->getName() + "\"";
		uim->centeredText(tempStr);
		uim->centeredText("Enabled Days");
		uim->skipLines(2);

		tempStr = "Choose the days that you want \"" + mtagPtr->getName() + "\" to be active on:";
		uim->centeredText(tempStr);
		uim->skipLines(1);

		tempStr = "MON: ";
		if (enabledDays.find(MONDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "TUE: ";
		if (enabledDays.find(TUESDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "WED: ";
		if (enabledDays.find(WEDNESDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "THU: ";
		if (enabledDays.find(THURSDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "FRI: ";
		if (enabledDays.find(FRIDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "SAT: ";
		if (enabledDays.find(SATURDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		tempStr = "SUN: ";
		if (enabledDays.find(SUNDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim->centeredText(tempStr);

		// print options
		strVec.clear();
		strVec = { "1Toggle MONDAY", "2Toggle TUESDAY", "3Toggle WEDNESDAY", "4Toggle THURSDAY",
				  "5Toggle FRIDAY", "6Toggle SATURDAY", "7Toggle SUNDAY", "- ",
				  "EEnable ALL", "DDisable ALL","- ", "QExit & Confirm Selection" };

		// prompt and get input
		uim->prompt_List_Case_Insensitive(strVec);
		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr == "E")
		{
			// set all days to TRUE
			enabledDays =
			{ {MONDAY, true}, {TUESDAY, true}, {WEDNESDAY, true}, {THURSDAY, true},
			{FRIDAY, true}, {SATURDAY, true}, {SUNDAY, true} };
		}
		else if (tempStr == "D")
		{
			// set all days to FALSE
			enabledDays =
			{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
			{FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };
		}
		else if (tempStr != "Q" && tempStr != "-")
		{
			// get user's choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1: // monday
				if (enabledDays.find(MONDAY)->second)
					enabledDays.find(MONDAY)->second = false;
				else
					enabledDays.find(MONDAY)->second = true;
				break;
			case 2: // tuesday
				if (enabledDays.find(TUESDAY)->second)
					enabledDays.find(TUESDAY)->second = false;
				else
					enabledDays.find(TUESDAY)->second = true;
				break;
			case 3: // wednesday
				if (enabledDays.find(WEDNESDAY)->second)
					enabledDays.find(WEDNESDAY)->second = false;
				else
					enabledDays.find(WEDNESDAY)->second = true;
				break;
			case 4: // thursday
				if (enabledDays.find(THURSDAY)->second)
					enabledDays.find(THURSDAY)->second = false;
				else
					enabledDays.find(THURSDAY)->second = true;
				break;
			case 5: // friday
				if (enabledDays.find(FRIDAY)->second)
					enabledDays.find(FRIDAY)->second = false;
				else
					enabledDays.find(FRIDAY)->second = true;
				break;
			case 6: // saturday
				if (enabledDays.find(SATURDAY)->second)
					enabledDays.find(SATURDAY)->second = false;
				else
					enabledDays.find(SATURDAY)->second = true;
				break;
			case 7: // sunday
				if (enabledDays.find(SUNDAY)->second)
					enabledDays.find(SUNDAY)->second = false;
				else
					enabledDays.find(SUNDAY)->second = true;
				break;
			default:
				// do nothing 
				tempStr = "";
			}
		}
	} // while (tempStr != "Q")

	// set enabledDays
	mtagPtr->setEnabledDays(enabledDays);

	// link to other tags
	editMultiTagTags(mtagPtr);

	// set requireFulfillment
	tempStr = "New MultiTag: \"" + mtagPtr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->centeredText("Require Linked Tag Fulfillment");
	uim->skipLines(2);

	uim->centeredText("Do you require all linked Tags to meet their requested number of Meals? (recommended YES)");
	uim->skipLines(1);
	uim->centeredText("If YES, then if a Tag's assigned Meals are unavailable, or not enough can be scheduled, then the program will skip this MultiTag.");
	uim->centeredText("If NO, then no matter how many Tags are linked, if at least ONE meal is scheduled, the program will commit it to the schedule.");
	uim->skipLines(3);
	uim->centeredText("Examples if set to REQUIRE ALL TAGS TO BE FULFILLED:");
	uim->skipLines(1);
	uim->centeredText("If this MultiTag has 1 linked Tag which requests 3 Meals but only 2 are available, then a different Tag/MultiTag will be found.");
	uim->centeredText("If this MultiTag has 2 linked Tags and both request 1 Meal but only 1 Meal is found, then a different Tag/MultiTag will be found.");
	uim->skipLines(2);
	uim->centeredText("Examples if set to ALLOW PARTIAL FULFILLMENT:");
	uim->skipLines(1);
	uim->centeredText("If this MultiTag has 5 linked Tags, all request 1 Meal but only 1 Meal in total is found, then the day will have only 1 Meal scheduled.");

	strVec = { "1Require all Tags to be fulfilled", "2Allow partial fulfillment" };
	uim->prompt_List_Case_Insensitive(strVec);

	tempStr = uim->display();

	// set priority depending on user choice
	if (tempStr == "1")
		mtagPtr->setRequireFulfillment(true);
	else
		mtagPtr->setRequireFulfillment(false);
}

void MealManager::deleteMultiTag(MultiTag* mtagPtr)
{
	// clear linkedTags
	mtagPtr->clearLinkedTags();

	// find MultiTag in multiTags
	auto tagIter = multiTags.begin();
	bool found = false;

	while (!found && tagIter != multiTags.end())
	{
		if (*tagIter == mtagPtr)
			found = true;
		else
			++tagIter;
	}

	// remove from multiTags and delete
	if (found)
	{
		multiTags.erase(tagIter);
		delete mtagPtr;
		mtagPtr = nullptr;
	}
}

void MealManager::editMealTags(Meal* mealPtr)
{
	const int TAGS_PER_PAGE = 5;
	std::string tempStr = "";
	int tempInt = -1;
	std::vector<std::string> strVec;
	int currentPage = 1; // page to display when displaying linked tags

	// while user has not quit menu
	while (tempStr != "Q")
	{
		// if meal has no tags, ask if user wants to assign some
		if (mealPtr->getTags().size() == 0)
		{
			// if tags exist to add
			if (normalTags.size() > 0)
			{
				// ui title
				uim->centeredText("Assign Tags");
				uim->skipLines(2);

				tempStr = "\"" + mealPtr->getName() + "\" has no assigned Tags, do you want to assign some?";
				uim->centeredText(tempStr);

				strVec = { "YYes", "NNo" };
				uim->prompt_List_Case_Insensitive(strVec);

				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				if (tempStr == "Y")
					tempStr = "A"; // add tags
				else
					tempStr = "Q"; // quit menu
			}
			else // no tags to display
			{
				tempStr = "\"" + mealPtr->getName() + "\" has no assigned Tags, and there are no new Tags to assign.";
				uim->centeredText(tempStr);
				uim->centeredText("Please create more Tags in the Tag Menu.");
				uim->display();

				tempStr = "Q";
			}

		}
		else // display assigned tags
		{
			// calculate total pages
			int totalPages = mealPtr->getTags().size() / TAGS_PER_PAGE;

			// check for remainder
			if (mealPtr->getTags().size() % TAGS_PER_PAGE > 0)
				++totalPages;

			// if currentPage is out of bounds, set to 1
			if (currentPage <= 0 || currentPage > totalPages)
				currentPage = 1;

			// create ui header
			uim->centeredText("Viewing Assigned Tags");
			// display page x/y
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim->centeredText(tempStr);

			// start index for current page
			int startIndex = (TAGS_PER_PAGE * (currentPage - 1));

			// create iterator to linkedTags
			std::vector<Tag*> assignedTagsCopy = mealPtr->getTags();
			auto tagIter = assignedTagsCopy.begin();

			// increment iter to startIndex
			for (int count = 0; count < startIndex; ++count)
				++tagIter;

			// if on last page
			if (currentPage == totalPages)
			{
				// display until end of tags
				int choiceNum = 1;
				while (tagIter != assignedTagsCopy.end())
				{
					// display tag info
					displayTagInfo(*tagIter);
					uim->skipLines(1);

					// create user choice
					tempStr = std::to_string(choiceNum) + (*tagIter)->getName();
					strVec.push_back(tempStr);

					++choiceNum;
					++tagIter;
				}
			}
			else // display TAGS_PER_PAGE of tags
			{
				int choiceNum = 1;
				while (choiceNum <= TAGS_PER_PAGE)
				{
					// display tag info
					displayTagInfo(*tagIter);
					uim->skipLines(1);

					// create user choice
					tempStr = std::to_string(choiceNum) + (*tagIter)->getName();
					strVec.push_back(tempStr);

					++choiceNum;
					++tagIter;
				}
			}

			// create rest of user choices
			tempStr = "";
			strVec.push_back(tempStr);

			tempStr = "AAdd a Tag";
			strVec.push_back(tempStr);

			tempStr = "";
			strVec.push_back(tempStr);

			tempStr = "NNext Page";
			strVec.push_back(tempStr);

			tempStr = "PPrevious Page";
			strVec.push_back(tempStr);

			tempStr = "QQuit Selection";
			strVec.push_back(tempStr);

			uim->prompt_List_Case_Insensitive(strVec);

			// display and get input
			tempStr = uim->display();
			tempStr = std::toupper(tempStr.at(0));

			// if user chose to go to next page
			if (tempStr == "N")
			{
				// increment page number (loops around if at end)
				if (currentPage == totalPages)
					currentPage = 1;
				else
					++currentPage;
			}
			else if (tempStr == "P") // previous page
			{
				// decrement page number (loops around if at beginning)
				if (currentPage == 1)
					currentPage = totalPages;
				else
					--currentPage;
			}
			else if (tempStr != "Q" && tempStr != "A") // chose a tag to edit
			{
				uim->centeredText("Tag Details");
				uim->skipLines(2);

				// get index of tag 
				tempInt = std::stoi(tempStr) - 1;

				// get tag and display info
				auto assignedTagsIter = assignedTagsCopy.begin();
				for (int count = 0; count < tempInt; ++count)
					++assignedTagsIter;

				displayTagInfo(*assignedTagsIter);

				// create choices
				strVec = {"RRemove Tag", "", "QQuit Selection" };
				uim->prompt_List_Case_Insensitive(strVec);

				// get choice
				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// remove tag
				if (tempStr == "R")
				{
					Tag* tagPtr = *assignedTagsIter;

					uim->centeredText("Confirm Removal");
					uim->skipLines(2);
					tempStr = "Are you sure you want to remove \"" + tagPtr->getName() + "\"?";
					uim->centeredText(tempStr);

					strVec = { "YYes", "NNo" };
					uim->prompt_List_Case_Insensitive(strVec);

					tempStr = std::toupper(uim->display().at(0));

					// if user chose to remove
					if (tempStr == "Y")
					{
						// remove tag from meal
						mealPtr->removeTag(tagPtr);
						// remove meal from tag
						tagPtr->removeMeal(mealPtr);

						// confirm to user
						uim->centeredText("Success!");
						uim->skipLines(2);

						tempStr = "\"" + tagPtr->getName() + "\" was removed from \"" + mealPtr->getName() + "\"";
						uim->centeredText(tempStr);
						uim->display();

						tagPtr = nullptr;
					}
				}
			}
		}

		// assign tags
		if (tempStr == "A")
		{
			// prompt for tag, or quit
			do
			{
				int lastPageVisited = -1;
				int returnStatus = -2;
				Tag* compare = nullptr;

				// while user is still choosing tags to link
				do
				{
					// exclude Tags already linked
					std::vector<Tag*> assignedTags = mealPtr->getTags();;

					// check if no tags to add
					if (assignedTags.size() == normalTags.size())
					{
						uim->centeredText("There are no Tags to assign, please create more in the Tag Menu.");
						uim->display();

						returnStatus = -1;
					}
					else
						returnStatus = displayTags(lastPageVisited, compare, assignedTags);

				} while (returnStatus == -2);

				// if user chose a tag
				if (returnStatus == 0)
				{
					// check if tag already added
					std::vector<Tag*> assignedTags = mealPtr->getTags();;

					bool isValid = true;
					auto tagsIter = assignedTags.begin();

					while (isValid && tagsIter != assignedTags.end())
					{
						// if match then tag already exists in meal
						if (compare == *tagsIter)
							isValid = false;
						++tagsIter;
					}

					if (!isValid)
					{
						// tell user the tag exists
						uim->centeredText("Error");
						uim->skipLines(2);
						tempStr = "\"" + compare->getName() + "\" is already assigned to \"" + mealPtr->getName() + "\".";
						uim->centeredText(tempStr);
						uim->display();
					}
					else
					{
						// link Tag to meal
						mealPtr->addTag(compare);
						// link meal to tag
						compare->addMeal(mealPtr);

						// display confirmation
						uim->centeredText("Success!");
						uim->skipLines(2);

						tempStr = "\"" + compare->getName() + "\" was assigned to \"" + mealPtr->getName() + "\".";
						uim->centeredText(tempStr);
						uim->display();

						compare = nullptr;
					}
				}
				else // user chose to quit
				{
					tempStr = "Q";
				}
			} while (tempStr != "Q");
		}
	}
}

void MealManager::editMultiTagTags(MultiTag* mtagPtr)
{
	const int TAGS_PER_PAGE = 5;

	std::string tempStr = "";
	int tempInt = 0;
	std::vector<std::string> strVec;
	int currentPage = 1; // page to display when displaying linked tags

	// while user has not quit menu
	while (tempStr != "Q")
	{
		// if multitag has no tags, ask if user wants to assign some
		if (mtagPtr->getLinkedTags().size() == 0)
		{
			// if tags exist to add
			if (normalTags.size() > 0)
			{
				// ui title
				uim->centeredText("Link Tags");
				uim->skipLines(2);

				tempStr = "\"" + mtagPtr->getName() + "\" has no linked Tags, do you want to link some?";
				uim->centeredText(tempStr);

				strVec = { "YYes", "NNo" };
				uim->prompt_List_Case_Insensitive(strVec);

				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				if (tempStr == "Y")
					tempStr = "A"; // add tags
				else
					tempStr = "Q"; // quit menu
			}
			else // no tags to display
			{
				tempStr = "\"" + mtagPtr->getName() + "\" has no linked Tags, and there are no Tags to link to.";
				uim->centeredText(tempStr);
				uim->centeredText("Please create more Tags in the Tag Menu.");
				uim->display();

				tempStr = "Q";
			}

		}
		else // display linked tags
		{
			// calculate total pages
			int totalPages = mtagPtr->getLinkedTags().size() / TAGS_PER_PAGE;

			// check for remainder
			if (mtagPtr->getLinkedTags().size() % TAGS_PER_PAGE > 0)
				++totalPages;

			// if currentPage is out of bounds, set to 1
			if (currentPage <= 0 || currentPage > totalPages)
				currentPage = 1;

			// create ui header
			uim->centeredText("Viewing Linked Tags");
			// display page x/y
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim->centeredText(tempStr);

			// start index for current page
			int startIndex = (TAGS_PER_PAGE * (currentPage - 1));

			// create iterator to linkedTags
			std::map<Tag*, unsigned int> linkedTagsCopy = mtagPtr->getLinkedTags();
			auto tagIter = linkedTagsCopy.begin();

			// increment iter to startIndex
			for (int count = 0; count < startIndex; ++count)
				++tagIter;

			// if on last page
			if (currentPage == totalPages)
			{
				// display until end of tags
				int choiceNum = 1;
				while (tagIter != linkedTagsCopy.end())
				{
					// display tag info
					displayTagInfo(tagIter->first);
					tempStr = "Meals Requested: " + std::to_string(tagIter->second);
					uim->leftAllignedText(tempStr);
					uim->skipLines(1);

					// create user choice
					tempStr = std::to_string(choiceNum) + tagIter->first->getName();
					strVec.push_back(tempStr);

					++choiceNum;
					++tagIter;
				}
			}
			else // display TAGS_PER_PAGE of tags
			{
				int choiceNum = 1;
				while (choiceNum <= TAGS_PER_PAGE)
				{
					// display tag info
					displayTagInfo(tagIter->first);
					tempStr = "Meals Requested: " + std::to_string(tagIter->second);
					uim->leftAllignedText(tempStr);
					uim->skipLines(1);

					// create user choice
					tempStr = std::to_string(choiceNum) + tagIter->first->getName();
					strVec.push_back(tempStr);

					++choiceNum;
					++tagIter;
				}
			}

			// create rest of user choices
			tempStr = "";
			strVec.push_back(tempStr);

			tempStr = "AAdd a Tag";
			strVec.push_back(tempStr);

			tempStr = "";
			strVec.push_back(tempStr);

			tempStr = "NNext Page";
			strVec.push_back(tempStr);

			tempStr = "PPrevious Page";
			strVec.push_back(tempStr);

			tempStr = "QQuit Selection";
			strVec.push_back(tempStr);

			uim->prompt_List_Case_Insensitive(strVec);

			// display and get input
			tempStr = uim->display();
			tempStr = std::toupper(tempStr.at(0));

			// if user chose to go to next page
			if (tempStr == "N")
			{
				// increment page number (loops around if at end)
				if (currentPage == totalPages)
					currentPage = 1;
				else
					++currentPage;
			}
			else if (tempStr == "P") // previous page
			{
				// decrement page number (loops around if at beginning)
				if (currentPage == 1)
					currentPage = totalPages;
				else
					--currentPage;
			}
			else if (tempStr != "Q" && tempStr != "A") // chose a tag to edit
			{
				uim->centeredText("Tag Details");
				uim->skipLines(2);

				// get index of tag 
				tempInt = std::stoi(tempStr) - 1;

				// get tag and display info
				auto linkedTagsIter = linkedTagsCopy.begin();
				for (int count = 0; count < tempInt; ++count)
					++linkedTagsIter;

				displayTagInfo(linkedTagsIter->first);
				tempStr = "Meals Requested: " + std::to_string(linkedTagsIter->second);
				uim->leftAllignedText(tempStr);

				// create choices
				strVec = { "EEdit Requested Meals", "", "RRemove Tag", "", "QQuit Selection" };
				uim->prompt_List_Case_Insensitive(strVec);

				// get choice
				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// edit requested meals
				if (tempStr == "E")
				{
					unsigned int newVal = 1;

					uim->centeredText("Editing Requested Meals");
					uim->skipLines(2);

					tempStr = "How many meals from \"" + linkedTagsIter->first->getName() + "\" do you want to eat in one day?";
					uim->centeredText(tempStr);

					tempStr = "(original value: " + std::to_string(linkedTagsIter->second) + ")";
					uim->centeredText(tempStr);

					uim->prompt_FreeInt(1, REQUESTED_MEALS_LIMIT);
					newVal = std::stoi(uim->display());

					// set new value
					linkedTagsIter->second = newVal;

					// confirm to user
					uim->centeredText("Success!");
					uim->skipLines(2);

					tempStr = "Meals Requested set to: " + std::to_string(linkedTagsIter->second);
					uim->centeredText(tempStr);
					uim->display();
				}
				else if (tempStr == "R") // remove tag
				{
					Tag* tagPtr = linkedTagsIter->first;

					uim->centeredText("Confirm Removal");
					uim->skipLines(2);
					tempStr = "Are you sure you want to remove \"" + tagPtr->getName() + "\"?";
					uim->centeredText(tempStr);

					strVec = { "YYes", "NNo" };
					uim->prompt_List_Case_Insensitive(strVec);

					tempStr = std::toupper(uim->display().at(0));

					// if user chose to remove
					if (tempStr == "Y")
					{
						// remove tag from multiTag
						mtagPtr->removeLinkedTag(tagPtr);

						// confirm to user
						uim->centeredText("Success!");
						uim->skipLines(2);

						tempStr = "\"" + tagPtr->getName() + "\" was removed from \"" + mtagPtr->getName() + "\"";
						uim->centeredText(tempStr);
						uim->display();

						tagPtr = nullptr;
					}
				}
			}
		}

		// assign tags
		if (tempStr == "A") 
		{
			// prompt for tag, or quit
			do
			{
				int lastPageVisited = -1;
				int returnStatus = -2;
				Tag* compare = nullptr;

				// while user is still choosing tags to link
				do
				{
					// exclude Tags already linked
					std::vector<Tag*> linkedTags;
					if (mtagPtr->getLinkedTags().size() > 0)
					{
						std::map<Tag*, unsigned int> linkedTagsMap = mtagPtr->getLinkedTags();

						// get tags into vector
						for (auto tagIter : linkedTagsMap)
						{
							linkedTags.push_back(tagIter.first);
						}
					}

					// check if no tags to add
					if (linkedTags.size() == normalTags.size())
					{
						uim->centeredText("There are no Tags to assign, please create more in the Tag Menu.");
						uim->display();

						returnStatus = -1;
					}
					else
						returnStatus = displayTags(lastPageVisited, compare, linkedTags);

				} while (returnStatus == -2);

				// if user chose a tag
				if (returnStatus == 0)
				{
					// check if tag already added
					std::map<Tag*, unsigned int> linkedTags = mtagPtr->getLinkedTags();

					bool isValid = true;
					auto tagsIter = linkedTags.begin();

					while (isValid && tagsIter != linkedTags.end())
					{
						// if match then tag already exists in meal
						if (compare == tagsIter->first)
							isValid = false;
						++tagsIter;
					}

					if (!isValid)
					{
						// tell user the tag exists
						uim->centeredText("Error");
						uim->skipLines(2);
						tempStr = "\"" + compare->getName() + "\" is already linked to \"" + mtagPtr->getName() + "\".";
						uim->centeredText(tempStr);
						uim->display();
					}
					else
					{
						// ask for amount
						uim->centeredText("Link Tags");
						uim->skipLines(2);
						tempStr = "How many meals from \"" + compare->getName() + "\" do you want to eat in one day?";
						uim->centeredText(tempStr);
						uim->prompt_FreeInt(1, REQUESTED_MEALS_LIMIT);
						tempInt = std::stoi(uim->display());

						// link Tag to multitag
						mtagPtr->addLinkedTag(compare, tempInt);

						// display confirmation
						uim->centeredText("Success!");
						uim->skipLines(2);

						tempStr = "\"" + compare->getName() + "\" was linked to \"" + mtagPtr->getName() + "\".";
						uim->centeredText(tempStr);
						uim->display();

						compare = nullptr;
					}
				}
				else // user chose to quit
				{
					tempStr = "Q";
				}
			} while (tempStr != "Q");
		} 
	}
}

void MealManager::editMultiTag(MultiTag* mtagPtr)
{
	std::string tempStr = "";
	int tempInt;
	std::vector<std::string> strVec;

	while (tempStr != "Q")
	{
		// header
		tempStr = "Viewing \"" + mtagPtr->getName() + "\"";
		uim->centeredText(tempStr);
		uim->skipLines(2);

		// display tag info
		displayMultiTagInfo(mtagPtr);

		// prompt user for attribute to edit or quit
		strVec = { "1Edit Name", "2Edit Description", "3Edit Enabled Days",
					"4Edit priority level", "5View/Edit Linked Tags", "6Edit Tag Fulfillment Requirement", "", "DDelete MultiTag", "", "QQuit Selection" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// user chose to delete tag
		if (tempStr == "D")
		{
			// confirm deletion
			uim->centeredText("Confirm Deletion");
			uim->skipLines(2);
			uim->centeredText("Are you sure you want to delete this MultiTag?");
			uim->skipLines(1);

			// display tag info
			displayMultiTagInfo(mtagPtr);

			strVec = { "YYes", "NNo" };

			uim->prompt_List_Case_Insensitive(strVec);
			tempStr = uim->display();
			tempStr = std::toupper(tempStr.at(0));

			// check user's choice
			if (tempStr == "Y")
			{
				deleteMultiTag(mtagPtr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);
				uim->centeredText("MultiTag deleted!");
				uim->display();
			}

			// quit menu 
			tempStr = "Q";
		}
		else if (tempStr != "Q")
		{
			// get choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1: // edit name
				tempStr = "Editing \"" + mtagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// prompt and get input
				uim->centeredText("Enter a new name: ");
				uim->prompt_FreeString(1, NAME_LENGTH);

				tempStr = uim->display();

				//set new name
				mtagPtr->setName(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Name changed to \"" + mtagPtr->getName() + "\".";
				uim->centeredText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			case 2: // edit description
				tempStr = "Editing \"" + mtagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// display original description
				tempStr = "Original description:";
				uim->centeredText(tempStr);

				if (mtagPtr->getDescription().length() == 0)
					tempStr = "(no description)";
				else
					tempStr = "\"" + mtagPtr->getDescription() + "\".";

				uim->centeredText(tempStr);
				uim->skipLines(1);

				// prompt and get input
				uim->centeredText("Enter a new description: ");
				uim->centeredText("(Press <enter> to skip)");
				uim->prompt_FreeString(0, DESC_LENGTH);

				tempStr = uim->display();

				//set new description
				mtagPtr->setDescription(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Description changed to:";
				uim->centeredText(tempStr);

				if (mtagPtr->getDescription().length() == 0)
					tempStr = "(no description)";
				else
					tempStr = "\"" + mtagPtr->getDescription() + "\".";

				uim->centeredText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			case 3: // set enabled days
			{
				tempStr = "";
				std::map<DaysOfTheWeek, bool> enabledDays = mtagPtr->getEnabledDays();

				// let user set enabled days or quit
				while (tempStr != "Q")
				{
					tempStr = "Editing \"" + mtagPtr->getName() + "\"";
					uim->centeredText(tempStr);
					uim->skipLines(2);

					uim->centeredText("Choose the days that this MultiTag will be active on:");
					uim->skipLines(1);

					tempStr = "MON: ";
					if (enabledDays.find(MONDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "TUE: ";
					if (enabledDays.find(TUESDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "WED: ";
					if (enabledDays.find(WEDNESDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "THU: ";
					if (enabledDays.find(THURSDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "FRI: ";
					if (enabledDays.find(FRIDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "SAT: ";
					if (enabledDays.find(SATURDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "SUN: ";
					if (enabledDays.find(SUNDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					// print options
					strVec.clear();
					strVec = { "1Toggle MONDAY", "2Toggle TUESDAY", "3Toggle WEDNESDAY", "4Toggle THURSDAY",
							  "5Toggle FRIDAY", "6Toggle SATURDAY", "7Toggle SUNDAY", "",
							  "EEnable ALL", "DDisable ALL","", "QExit & Confirm Selection" };

					// prompt and get input
					uim->prompt_List_Case_Insensitive(strVec);
					tempStr = uim->display();
					tempStr = std::toupper(tempStr.at(0));

					if (tempStr == "E")
					{
						// set all days to TRUE
						enabledDays =
						{ {MONDAY, true}, {TUESDAY, true}, {WEDNESDAY, true}, {THURSDAY, true},
						{FRIDAY, true}, {SATURDAY, true}, {SUNDAY, true} };
					}
					else if (tempStr == "D")
					{
						// set all days to FALSE
						enabledDays =
						{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
						{FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };
					}
					else if (tempStr != "Q" && tempStr != "-")
					{
						// get user's choice
						tempInt = std::stoi(tempStr);

						switch (tempInt)
						{
						case 1: // monday
							if (enabledDays.find(MONDAY)->second)
								enabledDays.find(MONDAY)->second = false;
							else
								enabledDays.find(MONDAY)->second = true;
							break;
						case 2: // tuesday
							if (enabledDays.find(TUESDAY)->second)
								enabledDays.find(TUESDAY)->second = false;
							else
								enabledDays.find(TUESDAY)->second = true;
							break;
						case 3: // wednesday
							if (enabledDays.find(WEDNESDAY)->second)
								enabledDays.find(WEDNESDAY)->second = false;
							else
								enabledDays.find(WEDNESDAY)->second = true;
							break;
						case 4: // thursday
							if (enabledDays.find(THURSDAY)->second)
								enabledDays.find(THURSDAY)->second = false;
							else
								enabledDays.find(THURSDAY)->second = true;
							break;
						case 5: // friday
							if (enabledDays.find(FRIDAY)->second)
								enabledDays.find(FRIDAY)->second = false;
							else
								enabledDays.find(FRIDAY)->second = true;
							break;
						case 6: // saturday
							if (enabledDays.find(SATURDAY)->second)
								enabledDays.find(SATURDAY)->second = false;
							else
								enabledDays.find(SATURDAY)->second = true;
							break;
						case 7: // sunday
							if (enabledDays.find(SUNDAY)->second)
								enabledDays.find(SUNDAY)->second = false;
							else
								enabledDays.find(SUNDAY)->second = true;
							break;
						default:
							// do nothing 
							tempStr = "";
						}
					}
				} // while (tempStr != "Q")

				// set enabledDays
				mtagPtr->setEnabledDays(enabledDays);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);
				uim->centeredText("Enabled Days set!");
				uim->display();
			}
			break;
			case 4: // set priority
			{
				tempStr = "Editing \"" + mtagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// report status
				if (mtagPtr->getPriority())
					tempStr = "This MultiTag has priority over Tags.";
				else
					tempStr = "This MultiTag has the same priority as Tags.";

				uim->centeredText(tempStr);

				// prompt user
				tempStr = "Do you want to ";

				if (mtagPtr->getPriority())
					tempStr += "lower priority to be the same as Tags? (Normal Priority)";
				else
					tempStr += "raise priority over Tags? (High Priority)";


				uim->centeredText(tempStr);

				strVec = { "YYes", "NNo" };
				uim->prompt_List_Case_Insensitive(strVec);

				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// process user's input
				if (tempStr == "Y")
				{
					// toggle state
					if (mtagPtr->getPriority())
						mtagPtr->setHighestPriority(false);
					else
						mtagPtr->setHighestPriority(true);
				}

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Priority changed to: ";

				if (mtagPtr->getPriority())
					tempStr += "HIGH PRIORITY.";
				else
					tempStr += "NORMAL PRIORITY.";

				uim->centeredText(tempStr);
				uim->prompt_None();
				uim->display();
			}
			break;
			case 5: // view/edit linked tags
				editMultiTagTags(mtagPtr);
				tempStr = ""; // make sure menu will loop
				break;
			case 6:
				// set requireFulfillment
				tempStr = "Editing \"" + mtagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				tempStr = "Current status: ";

				if (mtagPtr->getRequireFulfillment())
					tempStr += "REQUIRE ALL TAGS TO BE FULFILLED";
				else
					tempStr += "ALLOW PARTIAL FULFILLMENT";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				uim->centeredText("Do you require all linked Tags to meet their requested number of Meals? (recommended YES)");
				uim->skipLines(1);
				uim->centeredText("If YES, then if a Tag's assigned Meals are unavailable, or not enough can be scheduled, then the program will skip this MultiTag.");
				uim->centeredText("If NO, then no matter how many Tags are linked, if at least ONE meal is scheduled, the program will commit it to the schedule.");
				uim->skipLines(3);
				uim->centeredText("Examples if set to REQUIRE ALL TAGS TO BE FULFILLED:");
				uim->skipLines(1);
				uim->centeredText("If this MultiTag has 1 linked Tag which requests 3 Meals but only 2 are available, then a different Tag/MultiTag will be found.");
				uim->centeredText("If this MultiTag has 2 linked Tags and both request 1 Meal but only 1 Meal is found, then a different Tag/MultiTag will be found.");
				uim->skipLines(2);
				uim->centeredText("Examples if set to ALLOW PARTIAL FULFILLMENT:");
				uim->skipLines(1);
				uim->centeredText("If this MultiTag has 5 linked Tags, all request 1 Meal but only 1 Meal in total is found, then the day will have only 1 Meal scheduled.");

				strVec = { "1Require all Tags to be fulfilled", "2Allow partial fulfillment" };
				uim->prompt_List_Case_Insensitive(strVec);

				tempStr = uim->display();

				// set priority depending on user choice
				if (tempStr == "1")
					mtagPtr->setRequireFulfillment(true);
				else
					mtagPtr->setRequireFulfillment(false);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Requirement changed to: ";

				if (mtagPtr->getRequireFulfillment())
					tempStr += "REQUIRE ALL TAGS TO BE FULFILLED";
				else
					tempStr += "ALLOW PARTIAL FULFILLMENT";

				uim->centeredText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			default:
				// do nothing
				tempInt = 0;
			}// switch (tempInt)
		} // if (tempStr != "Q")
	}// while (tempStr != "Q")
}

void MealManager::editMeal(Meal* mealPtr)
{
	std::string tempStr = "";
	int tempInt;
	double tempDouble;
	std::vector<std::string> strVec;

	while (tempStr != "Q")
	{
		// header
		tempStr = "Viewing \"" + mealPtr->getName() + "\".";
		uim->centeredText(tempStr);
		uim->skipLines(2);

		// display meal info
		displayMealInfo(mealPtr);

		// prompt user for attribute to edit or quit
		strVec = { "1Edit Name", "2Edit Price", "3Edit Duration", "4Edit Days Between Occurrences","5View/Edit Tags", "6Enable/Disable Meal", "", "DDelete Meal", "", "QQuit Selection" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr == "D") // user chose to delete
		{
			// confirm deletion
			uim->centeredText("Confirm Deletion");
			uim->skipLines(2);
			uim->centeredText("Are you sure you want to delete this Meal?");
			uim->skipLines(1);

			// display meal info
			displayMealInfo(mealPtr);

			strVec = { "YYes", "NNo" };

			uim->prompt_List_Case_Insensitive(strVec);
			tempStr = uim->display();
			tempStr = std::toupper(tempStr.at(0));

			// check user's choice
			if (tempStr == "Y")
			{
				deleteMeal(mealPtr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);
				uim->centeredText("Meal deleted!");
				uim->display();
			}
			
			// quit menu 
			tempStr = "Q";
		}
		else if (tempStr != "Q")
		{
			// get choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1: // name
				tempStr = "Editing \"" + mealPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// prompt and get input
				uim->centeredText("Enter a new name:");
				uim->prompt_FreeString(1, NAME_LENGTH);

				tempStr = uim->display();

				//set new name
				mealPtr->setName(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Name changed to \"" + mealPtr->getName() + "\".";
				uim->centeredText(tempStr);
				uim->display();
				break;
			case 2: // price
				tempStr = "Editing " + mealPtr->getName();
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// display original price
				tempStr = "Original price: " + formatPrice(mealPtr->getPrice());
				uim->centeredText(tempStr);
				uim->skipLines(1);

				// prompt and get input
				uim->centeredText("Enter a new price:");
				uim->prompt_FreeDouble(MINIMUM_PRICE, MAXIMUM_PRICE);

				tempDouble = std::stod(uim->display());

				// set new price
				mealPtr->setPrice(tempDouble);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Price changed to: " + formatPrice(mealPtr->getPrice());
				uim->centeredText(tempStr);
				uim->display();
				break;
			case 3: // duration
			{
				unsigned int mealDuration = mealPtr->getMealDuration();

				tempStr = "Editing \"" + mealPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);
				
				tempStr = "\"" + mealPtr->getName() + "\" has a duration of " + std::to_string(mealDuration) + " days.";
				uim->centeredText(tempStr);
				uim->skipLines(1);

				uim->centeredText("Enter a new Duration:");
				uim->prompt_FreeInt(1, MEAL_DURATION_LIMIT);
				tempInt = std::stoi(uim->display());

				mealPtr->setMealDuration(tempInt);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Duration changed to: " + std::to_string(mealPtr->getMealDuration()) + " days.";
				uim->centeredText(tempStr);
				uim->display();
			}
				break;
			case 4: // daysBetweenOccurrences
			{
				unsigned int occurrences = mealPtr->getDaysBetweenOccurrences();

				tempStr = "Editing \"" + mealPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				tempStr = "When this Meal is scheduled, it must wait " + std::to_string(occurrences) + " days before it can occur again.";
				uim->centeredText(tempStr);
				uim->skipLines(1);

				uim->centeredText("Enter a new delay time:");
				tempStr = "(Original value: " + std::to_string(mealPtr->getDaysBetweenOccurrences()) + ")";
				uim->centeredText(tempStr);
				uim->prompt_FreeInt(0, DAYS_BETWEEN_OCCURRENCES_LIMIT);
				tempInt = std::stoi(uim->display());

				mealPtr->setDaysBetweenOccurrences(tempInt);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Delay time changed to: " + std::to_string(mealPtr->getDaysBetweenOccurrences()) + " days.";

				uim->centeredText(tempStr);
				uim->display();
			}
				break;
			case 5: // edit tags

				editMealTags(mealPtr);
				tempStr = ""; // make sure menu will loop
				break;
			case 6: // enable/disable
			{
				// toggle state
				if (mealPtr->isDisabled())
					mealPtr->setDisabled(false);
				else
					mealPtr->setDisabled(true);
			}
				break;
			default:
				// do nothing
				tempInt = 0;
			}// switch (tempInt)
		} // if (tempStr != "Q")
	}// while (tempStr != "Q")
}

void MealManager::editTag(Tag* tagPtr)
{
	std::string tempStr = "";
	int tempInt;
	std::vector<std::string> strVec;

	while (tempStr != "Q")
	{
		// header
		tempStr = "Viewing \"" + tagPtr->getName() + "\"";
		uim->centeredText(tempStr);
		uim->skipLines(2);

		// display tag info
		displayTagInfo(tagPtr);

		// prompt user for attribute to edit or quit
		strVec = { "1Edit Name", "2Edit Description", "3Edit Enabled Days", 
					"4Edit MultiTag Dependency", "5Edit Consecutive Occurrences", "", "DDelete Tag", "", "QQuit Selection" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// user chose to delete tag
		if (tempStr == "D")
		{
			// confirm deletion
			uim->centeredText("Confirm Deletion");
			uim->skipLines(2);
			uim->centeredText("Are you sure you want to delete this Tag?");
			uim->skipLines(1);

			// display tag info
			displayTagInfo(tagPtr);

			strVec = { "YYes", "NNo" };

			uim->prompt_List_Case_Insensitive(strVec);
			tempStr = uim->display();
			tempStr = std::toupper(tempStr.at(0));

			// check user's choice
			if (tempStr == "Y")
			{
				deleteTag(tagPtr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);
				uim->centeredText("Tag deleted!");
				uim->display();
			}

			// quit menu 
			tempStr = "Q";
		}
		else if (tempStr != "Q")
		{
			// get choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1: // edit name
				tempStr = "Editing \"" + tagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// prompt and get input
				uim->centeredText("Enter a new name:");
				uim->prompt_FreeString(1, NAME_LENGTH);

				tempStr = uim->display();

				//set new name
				tagPtr->setName(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Name changed to \"" + tagPtr->getName() + "\".";
				uim->centeredText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			case 2: // edit description
				tempStr = "Editing \"" + tagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// display original description
				tempStr = "Original description:";
				uim->centeredText(tempStr);

				if (tagPtr->getDescription().length() == 0)
					tempStr = "(no description)";
				else
					tempStr = "\"" + tagPtr->getDescription() + "\".";

				uim->centeredText(tempStr);
				uim->skipLines(1);

				// prompt and get input
				uim->centeredText("Enter a new description:");
				uim->centeredText("(Press <enter> to skip)");
				uim->prompt_FreeString(0, DESC_LENGTH);

				tempStr = uim->display();

				//set new description
				tagPtr->setDescription(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Description changed to:";
				uim->centeredText(tempStr);

				if (tagPtr->getDescription().length() == 0)
					tempStr = "(no description)";
				else
					tempStr = "\"" + tagPtr->getDescription() + "\".";

				uim->centeredText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			case 3: // set enabled days
			{
				tempStr = "";
				std::map<DaysOfTheWeek, bool> enabledDays = tagPtr->getEnabledDays();

				// let user set enabled days or quit
				while (tempStr != "Q")
				{
					tempStr = "Editing \"" + tagPtr->getName() + "\"";
					uim->centeredText(tempStr);
					uim->skipLines(2);

					uim->centeredText("Choose the days that this Tag will be active on:");
					uim->skipLines(1);

					tempStr = "MON: ";
					if (enabledDays.find(MONDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "TUE: ";
					if (enabledDays.find(TUESDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "WED: ";
					if (enabledDays.find(WEDNESDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "THU: ";
					if (enabledDays.find(THURSDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "FRI: ";
					if (enabledDays.find(FRIDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "SAT: ";
					if (enabledDays.find(SATURDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					tempStr = "SUN: ";
					if (enabledDays.find(SUNDAY)->second)
						tempStr += "enabled";
					else
						tempStr += "disabled";
					uim->centeredText(tempStr);

					// print options
					strVec.clear();
					strVec = { "1Toggle MONDAY", "2Toggle TUESDAY", "3Toggle WEDNESDAY", "4Toggle THURSDAY",
							  "5Toggle FRIDAY", "6Toggle SATURDAY", "7Toggle SUNDAY", "",
							  "EEnable ALL", "DDisable ALL","", "QExit & Confirm Selection" };

					// prompt and get input
					uim->prompt_List_Case_Insensitive(strVec);
					tempStr = uim->display();
					tempStr = std::toupper(tempStr.at(0));

					if (tempStr == "E")
					{
						// set all days to TRUE
						enabledDays =
						{ {MONDAY, true}, {TUESDAY, true}, {WEDNESDAY, true}, {THURSDAY, true},
						{FRIDAY, true}, {SATURDAY, true}, {SUNDAY, true} };
					}
					else if (tempStr == "D")
					{
						// set all days to FALSE
						enabledDays =
						{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
						{FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };
					}
					else if (tempStr != "Q" && tempStr != "-")
					{
						// get user's choice
						tempInt = std::stoi(tempStr);

						switch (tempInt)
						{
						case 1: // monday
							if (enabledDays.find(MONDAY)->second)
								enabledDays.find(MONDAY)->second = false;
							else
								enabledDays.find(MONDAY)->second = true;
							break;
						case 2: // tuesday
							if (enabledDays.find(TUESDAY)->second)
								enabledDays.find(TUESDAY)->second = false;
							else
								enabledDays.find(TUESDAY)->second = true;
							break;
						case 3: // wednesday
							if (enabledDays.find(WEDNESDAY)->second)
								enabledDays.find(WEDNESDAY)->second = false;
							else
								enabledDays.find(WEDNESDAY)->second = true;
							break;
						case 4: // thursday
							if (enabledDays.find(THURSDAY)->second)
								enabledDays.find(THURSDAY)->second = false;
							else
								enabledDays.find(THURSDAY)->second = true;
							break;
						case 5: // friday
							if (enabledDays.find(FRIDAY)->second)
								enabledDays.find(FRIDAY)->second = false;
							else
								enabledDays.find(FRIDAY)->second = true;
							break;
						case 6: // saturday
							if (enabledDays.find(SATURDAY)->second)
								enabledDays.find(SATURDAY)->second = false;
							else
								enabledDays.find(SATURDAY)->second = true;
							break;
						case 7: // sunday
							if (enabledDays.find(SUNDAY)->second)
								enabledDays.find(SUNDAY)->second = false;
							else
								enabledDays.find(SUNDAY)->second = true;
							break;
						default:
							// do nothing 
							tempStr = "";
						}
					}
				} // while (tempStr != "Q")

				// set enabledDays
				tagPtr->setEnabledDays(enabledDays);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);
				uim->centeredText("Enabled Days set!");
				uim->display();
			}
			break;
			case 4: // set dependency
			{
				bool dependsOnMultitag = tagPtr->getDependency();

				tempStr = "Editing \"" + tagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// report status
				if (dependsOnMultitag)
					tempStr = "This Tag is only active when linked with a MuliTag. (advanced behavior)";
				else
					tempStr = "This Tag does not rely on a MultiTag. (default behavior)";

				uim->centeredText(tempStr);

				// prompt user
				tempStr = "Do you want to ";

				if (dependsOnMultitag)
					tempStr += "enable this Tag regardless of being linked to a MultiTag? (default behavior)";
				else
					tempStr += "disable this Tag unless linked to a MultiTag? (advanced behavior)";

				uim->centeredText(tempStr);

				strVec = { "YYes", "NNo" };
				uim->prompt_List_Case_Insensitive(strVec);

				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// process user's input
				if (tempStr == "Y")
				{
					// toggle state
					if (dependsOnMultitag)
						tagPtr->setDependency(false);
					else
						tagPtr->setDependency(true);
				}

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				if (tempStr == "Y")
					tempStr = "Dependency changed to: ";
				else
					tempStr = "Dependency kept to: ";

				if (tagPtr->getDependency())
					tempStr += "DEPENDS ON MULTITAG.";
				else
					tempStr += "DOES NOT REQUIRE MULTITAG.";

				uim->centeredText(tempStr);
				uim->prompt_None();
				uim->display();
			}
				break;
			case 5: // set consecutive occurrences
				tempStr = "Editing \"" + tagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->skipLines(2);

				// display original value
				tempStr = "Original value: " + std::to_string(tagPtr->getConsecutiveLimit())
					 + " consecutive occurrences allowed.";
				uim->centeredText(tempStr);
				uim->skipLines(1);

				// prompt and get input
				uim->centeredText("How many consecutive days can a Meal with this Tag occur for?");
				uim->skipLines(1);
				uim->centeredText("This setting is Tag-specific, not Meal-specific. It applies to all Meals assigned to this Tag.");
				uim->centeredText("(A value of 1 means that Meals will occur every other day at most, a value of 3 allows 3 days of Meals from this Tag to occur in a row.)");
				uim->prompt_FreeInt(1, CONSECUTIVE_DAYS_LIMIT);

				// set new value
				tagPtr->setConsecutiveLimit(std::stoi(uim->display()));

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Consecutive occurrences set to " + std::to_string(tagPtr->getConsecutiveLimit());
				uim->centeredText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			default:
				// do nothing
				tempInt = 0;
			}// switch (tempInt)
		} // if (tempStr != "Q")
	}// while (tempStr != "Q")
}

std::string MealManager::formatPrice(const double& price)
{
	std::string formatStr = std::to_string(price);

	// erase trailing zeros, leaves '.' at end of string
	formatStr.erase(formatStr.find_last_not_of("0") + 1, std::string::npos);

	// erase '.' if exists
	if (formatStr[formatStr.length() - 1] == '.')
		formatStr.pop_back();

	return formatStr;
}

std::string MealManager::formatEnabledDays(const std::map<DaysOfTheWeek, bool>& enabledDays)
{
	std::string returnStr = "[";
	bool oneDayEnabled = false; // used to determine format between days

	// check days that are disabled by tags
	if (enabledDays.at(MONDAY))
	{
		returnStr += "M";
		oneDayEnabled = true;
	}

	if (enabledDays.at(TUESDAY))
	{
		// add spacing
		if (oneDayEnabled)
			returnStr += ", ";
		else
			oneDayEnabled = true;

		returnStr += "T";
	}

	if (enabledDays.at(WEDNESDAY))
	{
		// add spacing
		if (oneDayEnabled)
			returnStr += ", ";
		else
			oneDayEnabled = true;

		returnStr += "W";
	}

	if (enabledDays.at(THURSDAY))
	{
		// add spacing
		if (oneDayEnabled)
			returnStr += ", ";
		else
			oneDayEnabled = true;

		returnStr += "Th";
	}

	if (enabledDays.at(FRIDAY))
	{
		// add spacing
		if (oneDayEnabled)
			returnStr += ", ";
		else
			oneDayEnabled = true;

		returnStr += "F";
	}

	if (enabledDays.at(SATURDAY))
	{
		// add spacing
		if (oneDayEnabled)
			returnStr += ", ";
		else
			oneDayEnabled = true;

		returnStr += "Sa";
	}

	if (enabledDays.at(SUNDAY))
	{
		// add spacing
		if (oneDayEnabled)
			returnStr += ", ";
		else
			oneDayEnabled = true;

		returnStr += "Su";
	}
	 // close bracket
	returnStr += "]";

	return returnStr;
}

void MealManager::optimizeData(std::map<DaysOfTheWeek, std::vector<MultiTag*>>& highPriorityMultiTags, 
	std::map<DaysOfTheWeek, std::vector<MultiTag*>>& normalPriorityMultiTags, 
	std::map<DaysOfTheWeek, std::vector<Tag*>>& normalPriorityTags)
{
	DaysOfTheWeek day = MONDAY; // adds elements to function's params for each day of the week

	// ensure params are clear
	highPriorityMultiTags.clear();
	normalPriorityMultiTags.clear();
	normalPriorityTags.clear();

	// create an entry for each day of the week
	do
	{
		std::vector<Tag*> tagEntry;
		std::vector<MultiTag*> multiTagEntry;

		// create element in params
		highPriorityMultiTags.emplace(day, multiTagEntry);
		normalPriorityMultiTags.emplace(day, multiTagEntry);
		normalPriorityTags.emplace(day, tagEntry);

		day = nextDay(day); // go to next day
	} while (day != MONDAY);

	// populates normalPriorityTags
	do
	{
		// check all tags
		for (auto tagIter : normalTags)
		{
			// check if enabled for current day and does not depend on a multitag
			if (tagIter->getEnabledDays().at(day) && !tagIter->getDependency())
			{
				// copy vector of normalTags in normalPriorityTags
				std::vector<Tag*> tagEntry = normalPriorityTags.at(day);

				// add tagIter to tagEntry
				tagEntry.push_back(tagIter);

				// store back into normalPriorityTags
				normalPriorityTags.at(day) = tagEntry;
			}
		}
		day = nextDay(day); // go to next day
	} while (day != MONDAY);

	// populates highPriorityMultiTags and normalPriorityMultiTags
	do
	{
		// check all multiTags
		for (auto multiTagIter : multiTags)
		{
			// check if enabled for current day
			if (multiTagIter->getEnabledDays().at(day))
			{
				// check if multiTagsIter is disabled by its linked tags
				std::map<Tag*, unsigned int> linkedTags = multiTagIter->getLinkedTags();

				bool isDisabled = true;
				auto linkedTagIter = linkedTags.begin();
				while (isDisabled && linkedTagIter != linkedTags.end())
				{
					// if linked tag is enabled, then quit loop
					if (linkedTagIter->first->getEnabledDays().at(day))
						isDisabled = false;
					else
						++linkedTagIter;
				}

				// if multitag is not disabled
				if (!isDisabled)
				{
					// check if multiTag is high or normal priority
					if (multiTagIter->getPriority())
					{
						// copy vector of high priority multitags
						std::vector<MultiTag*> tagEntry = highPriorityMultiTags.at(day);

						// add multiTag to vector
						tagEntry.push_back(multiTagIter);

						// store back into highPriorityMultiTags
						highPriorityMultiTags.at(day) = tagEntry;
					}
					else
					{
						// copy vector of normal priority multitags
						std::vector<MultiTag*> tagEntry = normalPriorityMultiTags.at(day);

						// add multiTag to vector
						tagEntry.push_back(multiTagIter);

						// store back into normalPriorityMultiTags
						normalPriorityMultiTags.at(day) = tagEntry;
					}
				}
			}
		}
		day = nextDay(day); // go to next day
	} while (day != MONDAY);
}

Meal* MealManager::displayMeals(int& lastPageVisited)
{
	const int MEALS_PER_PAGE = 5; 

	std::string tempStr = "";
	std::vector<std::string> strVec;
	Meal* returnPtr = nullptr;
	int tempInt = -1;

	// if there are no meals to list, let user create one 
	if (meals.size() == 0)
	{
		uim->centeredText("There are no Meals to display, do you want to create one?");

		strVec = { "YYes", "NNo" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// if user chose yes
		if (tempStr == "Y")
		{
			// create a meal
			Meal* mealPtr = new Meal;
			createMeal(mealPtr);
			mealPtr = nullptr;
		}
	}

	// if there are meals to list
	if (meals.size() > 0)
	{
		// if meals > 10, then display in pages
		if (meals.size() > MEALS_PER_PAGE)
		{
			tempStr = "";

			int currentPage = 1;
			int totalPages = meals.size() / MEALS_PER_PAGE; // remainder may be present

			// if remainder, then add an extra page
			if (meals.size() % MEALS_PER_PAGE > 0)
				++totalPages;

			// if lastPageVisited is valid, then replace currentPage with lastPageVisited
			if (lastPageVisited >= 1 && lastPageVisited <= totalPages)
				currentPage = lastPageVisited;

			// loop while user has not quit menu
			while (tempStr != "Q")
			{
				uim->centeredText("Viewing Meals");
				// display "Page x/y"
				tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
				uim->centeredText(tempStr);

				// start index for the current page
				int startIndex = (MEALS_PER_PAGE * (currentPage - 1));

				//if displaying a page that is NOT last page, then display MEALS_PER_PAGE items
				if (currentPage < totalPages)
				{
					// create iterator at index's position
					auto mealIter = meals.begin() + startIndex;

					// create prompts for meals and display meal
					for (int count = 1; count <= MEALS_PER_PAGE; ++count)
					{
						displayMealInfo(*mealIter);
						uim->skipLines(1);
						tempStr = std::to_string(count) + (*mealIter)->getName();
						strVec.push_back(tempStr);

						++mealIter;
					}
				}
				else // the current page is the last page, so there may be less than MEALS_PER_PAGE elements to display
				{
					// create iterator at index's position
					auto mealIter = meals.begin() + startIndex;
					int count = 1; // used to print prompt number

					// create prompts for the rest of the elements
					do
					{
						displayMealInfo(*mealIter);
						uim->skipLines(1);
						tempStr = std::to_string(count) + (*mealIter)->getName();
						strVec.push_back(tempStr);

						++count;
						++mealIter;
					} while (mealIter != meals.end());
				}

				// add prompts to go to next page, previous, and quit
				tempStr = "";
				strVec.push_back(tempStr);

				tempStr = "CCreate a Meal";
				strVec.push_back(tempStr);

				tempStr = "";
				strVec.push_back(tempStr);

				tempStr = "NNext Page";
				strVec.push_back(tempStr);

				tempStr = "PPrevious Page";
				strVec.push_back(tempStr);

				tempStr = "QQuit Selection";
				strVec.push_back(tempStr);

				// display UI and get input
				uim->prompt_List_Case_Insensitive(strVec);
				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// check choices
				if (tempStr == "C")
				{
					// create a new meal
					Meal* mealPtr = new Meal;
					createMeal(mealPtr);
					mealPtr = nullptr;

					// confirm to user
					uim->centeredText("Success!");
					uim->skipLines(2);
					uim->centeredText("Meal creation successful!");
					uim->display();
				}
				else if (tempStr == "N")
				{
					// increment page number (loops around if at end)
					if (currentPage == totalPages)
						currentPage = 1;
					else
						++currentPage;
				}
				else if (tempStr == "P")
				{
					// decrement page number (loops around if at beginning)
					if (currentPage == 1)
						currentPage = totalPages;
					else
						--currentPage;
				}
				else if (tempStr != "Q")
				{
					// get the user's choice
					tempInt = std::stoi(tempStr);

					// convert into meal's index
					--tempInt; // user's choices started at 1
					tempInt += startIndex;

					// get pointer
					returnPtr = meals.at(tempInt);

					// exit loop and return lastPageVisited
					lastPageVisited = currentPage;
					tempStr = "Q";
				}
			} // while (tempStr != "Q")
		} // if (meals.size() > 10)
		else // display all in one page
		{
			tempStr = "";
			// while user has not chosen to quit 
			while (tempStr != "Q")
			{
				uim->centeredText("Viewing Meals");
				uim->skipLines(1);

				// create prompts
				int choice = 1;
				for (auto mealIter : meals)
				{
					displayMealInfo(mealIter);
					uim->skipLines(1);
					tempStr = std::to_string(choice) + mealIter->getName();
					strVec.push_back(tempStr);
					++choice;
				}
				// add prompts to quit
				tempStr = "";
				strVec.push_back(tempStr);

				tempStr = "CCreate a Meal";
				strVec.push_back(tempStr);

				tempStr = "";
				strVec.push_back(tempStr);

				tempStr = "QQuit Selection";
				strVec.push_back(tempStr);

				uim->prompt_List_Case_Insensitive(strVec);
				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// process choices
				if (tempStr == "C")
				{
					// create a new meal
					Meal* mealPtr = new Meal;
					createMeal(mealPtr);
					mealPtr = nullptr;

					// confirm to user
					uim->centeredText("Success!");
					uim->skipLines(2);
					uim->centeredText("Meal creation successful!");
					uim->display();
				}
				else if (tempStr != "Q")
				{
					// get index of meal
					tempInt = std::stoi(tempStr);
					--tempInt; // user's choices started at 1

					// get pointer
					returnPtr = meals.at(tempInt);

					// exit loop
					tempStr = "Q";
				}
			}// while (tempStr != "Q")
		} // else display all meals
	}
	
	return returnPtr;
}

int MealManager::displayTags(int& lastPageVisited, Tag*& tagPtr, const std::vector<Tag*>& excludeTags)
{
	const int TAGS_PER_PAGE = 5;

	std::string tempStr = "";
	std::vector<std::string> strVec;
	std::vector<Tag*> availableTags = normalTags; // make all tags available to display
	int tempInt = -1;

	// ensure tagPtr is set to nullptr
	tagPtr = nullptr;

	// check if tags need to be excluded
	if (availableTags.size() > 0 && excludeTags.size() > 0)
	{
		// loop while tags need to be excluded
		for (auto excludeTagsIter : excludeTags)
		{
			bool found = false;
			auto availableTagsIter = availableTags.begin();

			while (!found && availableTagsIter != availableTags.end())
			{
				// check if match
				if (*availableTagsIter == excludeTagsIter)
				{
					found = true;

					// remove this tag from availableTags
					availableTags.erase(availableTagsIter);
				}
				else
					++availableTagsIter;
			}
		}
	}


	// if there are no tags to list, let user create one 
	if (availableTags.size() == 0)
	{
		uim->centeredText("There are no Tags to display, do you want to create one?");

		strVec = { "YYes", "NNo" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// if user chose yes
		if (tempStr == "Y")
		{
			// create a tag
			tagPtr = new Tag;
			createTag(tagPtr);

			// return TagCreated status and quit
			tempInt = -2;
		}
	}
	else // display tags
	{
		int currentPage = 1; // default to first page

		// calculate total pages
		int totalPages = availableTags.size() / TAGS_PER_PAGE; // remainder may be present

		// if remainder, then add an extra page
		if (availableTags.size() % TAGS_PER_PAGE > 0)
			++totalPages;

		// if lastPageVisited is valid, then replace currentPage with lastPageVisited
		if (lastPageVisited >= 1 && lastPageVisited <= totalPages)
			currentPage = lastPageVisited;

		// while user did not quit
		while (tempStr != "Q")
		{
			// ui header
			uim->centeredText("Viewing Tags");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim->centeredText(tempStr);

			// start index for the current page
			int startIndex = (TAGS_PER_PAGE * (currentPage - 1));

			// create iterator at index's position
			auto tagIter = availableTags.begin() + startIndex;

			//if displaying a page that is NOT last page, then display TAGS_PER_PAGE items
			if (currentPage < totalPages)
			{

				// display tags
				for (int count = 1; count <= TAGS_PER_PAGE; ++count)
				{
					displayTagInfo(*tagIter);
					uim->skipLines(1);

					tempStr = std::to_string(count) + (*tagIter)->getName();
					strVec.push_back(tempStr);

					++tagIter;
				}
			}
			else // the current page is the last page, so display until end of availableTAgs
			{
				int count = 1; // used to print prompt number

				// create prompts for the rest of the elements
				while (tagIter != availableTags.end())
				{
					displayTagInfo(*tagIter);
					uim->skipLines(1);

					tempStr = std::to_string(count) + (*tagIter)->getName();
					strVec.push_back(tempStr);

					++tagIter;
					++count;
				}
			}

			// add prompts to go to next page, previous, and quit
			tempStr = "";
			strVec.push_back(tempStr);

			tempStr = "CCreate a Tag";
			strVec.push_back(tempStr);

			tempStr = "";
			strVec.push_back(tempStr);

			tempStr = "NNext Page";
			strVec.push_back(tempStr);

			tempStr = "PPrevious Page";
			strVec.push_back(tempStr);

			tempStr = "QQuit Selection";
			strVec.push_back(tempStr);

			// display UI and get input
			uim->prompt_List_Case_Insensitive(strVec);
			tempStr = uim->display();
			tempStr = std::toupper(tempStr.at(0));

			// process inputs
			if (tempStr == "C")
			{
				// create a new tag
				tagPtr = new Tag;
				createTag(tagPtr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);
				uim->centeredText("Tag creation successful!");
				uim->display();

				// return TagCreated status and quit
				tempInt = -2;
				tempStr = "Q";
			}
			else if (tempStr == "N")
			{
				// increment page number (loops around if at end)
				if (currentPage == totalPages)
					currentPage = 1;
				else
					++currentPage;
			}
			else if (tempStr == "P")
			{
				// decrement page number (loops around if at beginning)
				if (currentPage == 1)
					currentPage = totalPages;
				else
					--currentPage;
			}
			else if (tempStr != "Q")
			{
				// get choice, set to correct index
				tempInt = std::stoi(tempStr) - 1;
				tempInt += startIndex; // add offset

				// get Tag
				tagPtr = availableTags.at(tempInt);

				// return tagSelected status
				tempInt = 0;

				// end loop
				tempStr = "Q";
				lastPageVisited = currentPage;
			}
			else // user chose to quit
				tempInt = -1; // return exited status
		}

			
	} // else display tags

	return tempInt;
}

MultiTag* MealManager::displayMultiTags(int& lastPageVisited)
{
	const int MULTITAGS_PER_PAGE = 5;

	std::string tempStr = "";
	std::vector<std::string> strVec;
	MultiTag* returnPtr = nullptr;
	int tempInt = -1;

	// if there are no multitags to list, let user create one 
	if (multiTags.size() == 0)
	{
		uim->centeredText("There are no MultiTags to display, do you want to create one?");

		strVec = { "YYes", "NNo" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// if user chose yes
		if (tempStr == "Y")
		{
			// create a tag
			MultiTag* mtagPtr = new MultiTag;
			createMultiTag(mtagPtr);
			mtagPtr = nullptr;
		}
	}

	// check if multitags exist
	if (multiTags.size() > 0)
	{
		// can print all multitags in 1 page
		if (multiTags.size() <= MULTITAGS_PER_PAGE)
		{
			// while user has not quit
			while (tempStr != "Q")
			{
				uim->centeredText("Viewing MultiTags");
				uim->skipLines(1);

				// display multitags with description and create prompt
				int count = 1;
				for (auto tagIter : multiTags)
				{
					displayMultiTagInfo(tagIter);
					uim->skipLines(1);

					tempStr = std::to_string(count) + tagIter->getName();
					strVec.push_back(tempStr);
					++count;
				}

				// add create and quit option
				tempStr = "";
				strVec.push_back(tempStr);

				tempStr = "CCreate a MultiTag";
				strVec.push_back(tempStr);

				tempStr = "";
				strVec.push_back(tempStr);

				tempStr = "QQuit Selection";
				strVec.push_back(tempStr);

				// prompt and get input
				uim->prompt_List_Case_Insensitive(strVec);
				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// process inputs
				if (tempStr == "C")
				{
					// create a new multitag
					MultiTag* mtagPtr = new MultiTag;
					createMultiTag(mtagPtr);
					mtagPtr = nullptr;

					// confirm to user
					uim->centeredText("Success!");
					uim->skipLines(2);
					uim->centeredText("MultiTag creation successful!");
					uim->display();
				}
				else if (tempStr != "Q")
				{
					// get choice
					tempInt = std::stoi(tempStr);
					--tempInt; // choices start at 1

					// get pointer
					returnPtr = multiTags.at(tempInt);

					// exit menu
					tempStr = "Q";
				}
			}
		}
		else // print tags in pages
		{
			tempStr = "";

			int currentPage = 1;
			int totalPages = multiTags.size() / MULTITAGS_PER_PAGE; // remainder may be present

			// if remainder, then add an extra page
			if (multiTags.size() % MULTITAGS_PER_PAGE > 0)
				++totalPages;

			// if lastPageVisited is valid, then replace currentPage with lastPageVisited
			if (lastPageVisited >= 1 && lastPageVisited <= totalPages)
				currentPage = lastPageVisited;

			// loop while user has not quit menu
			while (tempStr != "Q")
			{
				uim->centeredText("Viewing MultiTags");
				// display "Page x/y"
				tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
				uim->centeredText(tempStr);

				// start index for the current page
				int startIndex = (MULTITAGS_PER_PAGE * (currentPage - 1));

				//if displaying a page that is NOT last page, then display MULTITAGS_PER_PAGE items
				if (currentPage < totalPages)
				{
					// create iterator at index's position
					auto tagIter = multiTags.begin() + startIndex;

					// display tag with description and create prompt
					for (int count = 1; count <= MULTITAGS_PER_PAGE; ++count)
					{
						displayMultiTagInfo(*tagIter);
						uim->skipLines(1);

						tempStr = std::to_string(count) + (*tagIter)->getName();
						strVec.push_back(tempStr);

						++tagIter;
					}
				}
				else // the current page is the last page, so there may be less than 10 elements to display
				{
					// create iterator at index's position
					auto tagIter = multiTags.begin() + startIndex;
					int count = 1; // used to print prompt number

					// create prompts for the rest of the elements
					while (tagIter != multiTags.end())
					{
						displayMultiTagInfo(*tagIter);
						uim->skipLines(1);

						tempStr = std::to_string(count) + (*tagIter)->getName();
						strVec.push_back(tempStr);

						++tagIter;
						++count;
					}
				}

				// add prompts to go to next page, previous, and quit
				tempStr = "";
				strVec.push_back(tempStr);

				tempStr = "CCreate a MultiTag";
				strVec.push_back(tempStr);

				tempStr = "";
				strVec.push_back(tempStr);

				tempStr = "NNext Page";
				strVec.push_back(tempStr);

				tempStr = "PPrevious Page";
				strVec.push_back(tempStr);

				tempStr = "QQuit Selection";
				strVec.push_back(tempStr);

				// display UI and get input
				uim->prompt_List_Case_Insensitive(strVec);
				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// process inputs
				if (tempStr == "C")
				{
					// create a new multitag
					MultiTag* mtagPtr = new MultiTag;
					createMultiTag(mtagPtr);
					mtagPtr = nullptr;

					// confirm to user
					uim->centeredText("Success!");
					uim->skipLines(2);
					uim->centeredText("MultiTag creation successful!");
					uim->display();
				}
				else if (tempStr == "N")
				{
					// increment page number (loops around if at end)
					if (currentPage == totalPages)
						currentPage = 1;
					else
						++currentPage;
				}
				else if (tempStr == "P")
				{
					// decrement page number (loops around if at beginning)
					if (currentPage == 1)
						currentPage = totalPages;
					else
						--currentPage;
				}
				else if (tempStr != "Q")
				{
					// get choice, set to correct index
					tempInt = std::stoi(tempStr);
					--tempInt; // user's prompts began at 1
					tempInt += startIndex;

					// get pointer
					returnPtr = multiTags.at(tempInt);

					// end loop
					tempStr = "Q";
					lastPageVisited = currentPage;
				}
			} // while (tempStr != "Q")
		} // else print tag in pages
	} // if more than 0 tags

	return returnPtr;
}

void MealManager::displayMealInfo(const Meal* mealPtr)
{
	std::string tempStr = "";
	
	// name
	tempStr = "Name: " + mealPtr->getName();
	uim->leftAllignedText(tempStr);

	// price
	tempStr = "Price: " + formatPrice(mealPtr->getPrice());
	uim->leftAllignedText(tempStr);

	// duration 
	tempStr = "Duration: " + std::to_string(mealPtr->getMealDuration()) + " days.";
	uim->leftAllignedText(tempStr);

	// tags
	tempStr = std::to_string(mealPtr->getTags().size());
	
	if (mealPtr->getTags().size() == 1)
		tempStr += " Tag assigned.";
	else tempStr += " Tags assigned.";

	uim->leftAllignedText(tempStr);

	// enabledDays
	if (mealPtr->isDisabled())
		uim->leftAllignedText("Meal is disabled.");
	else
	{
		tempStr = "Enabled on: " + formatEnabledDays(mealPtr->getEnabledDays());

		// check if disabled by tags
		if (tempStr == "Enabled on: []")
			uim->leftAllignedText("Meal is disabled by assigned Tags.");
		else
			uim->leftAllignedText(tempStr);
	}
}

void MealManager::displayTagInfo(const Tag* tagPtr)
{
	std::string tempStr = "";

	// name
	tempStr = "Name: " + tagPtr->getName();
	uim->leftAllignedText(tempStr);

	// description
	tempStr = "Description: ";
	if (tagPtr->getDescription().size() == 0)
		tempStr += "(no description)";
	else 
		tempStr += tagPtr->getDescription();

	uim->leftAllignedText(tempStr);

	// hasPriority
	tempStr = "Depends on MultiTag: ";

	if (tagPtr->getDependency())
		tempStr += "YES     ";
	else
		tempStr += "NO      ";

	// consecutiveLimit
	tempStr += "Consecutive occurrences allowed: " + std::to_string(tagPtr->getConsecutiveLimit());
	uim->leftAllignedText(tempStr);

	// enabledDays
	tempStr = "Enabled on: " + formatEnabledDays(tagPtr->getEnabledDays());

	// check if disabled by tags
	if (tempStr == "Enabled on: []")
		uim->leftAllignedText("Tag is disabled on all days.");
	else
		uim->leftAllignedText(tempStr);

	// number of assigned meals
	tempStr = "Assigned Meals: " + std::to_string(tagPtr->getLinkedMeals().size());
	uim->leftAllignedText(tempStr);
}

void MealManager::displayMultiTagInfo(const MultiTag* mtagPtr)
{
	std::string tempStr = "";

	// name
	tempStr = "Name: " + mtagPtr->getName();
	uim->leftAllignedText(tempStr);

	// description
	tempStr = "Description: ";
	if (mtagPtr->getDescription().size() == 0)
		tempStr += "(no description)";
	else 
		tempStr += mtagPtr->getDescription();

	uim->leftAllignedText(tempStr);

	// hasPriority
	tempStr = "Has priority over Tags: ";

	if (mtagPtr->getPriority())
		tempStr += "YES";
	else
		tempStr += "NO";
	uim->leftAllignedText(tempStr);

	// enabledDays
	tempStr = "Enabled on: " + formatEnabledDays(mtagPtr->getEnabledDays());

	// check if MultiTag has all days disabled
	if (tempStr == "Enabled on: []")
		uim->leftAllignedText("All days are disabled.");
	else
		uim->leftAllignedText(tempStr);

	// requires fulfillment
	tempStr = "Require all Tags to be fulfilled: ";

	if (mtagPtr->getRequireFulfillment())
		tempStr += "YES";
	else
		tempStr += "NO";

	// number of linked tags
	tempStr += "     Linked Tags: " + std::to_string(mtagPtr->getLinkedTags().size());
	uim->leftAllignedText(tempStr);
}

void MealManager::writeMeal(const Meal* mealPtr, std::ofstream& oFile)
{
	// name
	oFile << "\t<Name>\n";
	oFile << "\t" << mealPtr->getName() << "\n";
	oFile << "\t</Name>";

	oFile << "\n";

	// price
	oFile << "\t<Price>\n";
	oFile << "\t" << formatPrice(mealPtr->getPrice()) << "\n";
	oFile << "\t</Price>";

	oFile << "\n";

	// isDisabled
	oFile << "\t<IsDisabled>\n";
	oFile << "\t" << mealPtr->isDisabled() << "\n";
	oFile << "\t</IsDisabled>";

	oFile << "\n";

	// mealDuration
	oFile << "\t<MealDuration>\n";
	oFile << "\t" << std::to_string(mealPtr->getMealDuration()) << "\n";
	oFile << "\t</MealDuration>";

	oFile << "\n";

	// daysBetweenOccurrences
	oFile << "\t<DaysBetweenOccurrences>\n";
	oFile << "\t" << std::to_string(mealPtr->getDaysBetweenOccurrences()) << "\n";
	oFile << "\t</DaysBetweenOccurrences>";

	oFile << "\n";

	// name of Tags assigned
	oFile << "\t<MealTags>\n";

	// loop to write all names
	for (auto tagIter : mealPtr->getTags())
	{
		oFile << "\t\t<TagName>\n";
		oFile << "\t\t" << tagIter->getName() << "\n";
		oFile << "\t\t</TagName>\n";
	}

	oFile << "\t</MealTags>";

}

void MealManager::writeTag(const Tag* tagPtr, std::ofstream& oFile)
{
	// name
	oFile << "\t<Name>\n";
	oFile << "\t" << tagPtr->getName() << "\n";
	oFile << "\t</Name>";

	oFile << "\n";

	// description
	oFile << "\t<Description>\n";
	oFile << "\t" << tagPtr->getDescription() << "\n";
	oFile << "\t</Description>";

	oFile << "\n";

	// dependsOnMultitag
	oFile << "\t<DependsOnMultiTag>\n";
	oFile << "\t" << tagPtr->getDependency() << "\n";
	oFile << "\t</DependsOnMultiTag>";

	oFile << "\n";

	// consecutiveLimit
	oFile << "\t<ConsecutiveLimit>\n";
	oFile << "\t" << std::to_string(tagPtr->getConsecutiveLimit()) << "\n";
	oFile << "\t</ConsecutiveLimit>";

	oFile << "\n";

	// enabledDays
	oFile << "\t<EnabledDays>\n";

	// loop to write enabledDays
	for (auto daysIter : tagPtr->getEnabledDays())
	{
		oFile << "\t" << daysIter.second << "\n";
	}
	oFile << "\t</EnabledDays>";
}

void MealManager::writeMultiTag(const MultiTag* mtagPtr, std::ofstream& oFile)
{
	// name
	oFile << "\t<Name>\n";
	oFile << "\t" << mtagPtr->getName() << "\n";
	oFile << "\t</Name>";

	oFile << "\n";

	// description
	oFile << "\t<Description>\n";
	oFile << "\t" << mtagPtr->getDescription() << "\n";
	oFile << "\t</Description>";

	oFile << "\n";

	// highestPriority
	oFile << "\t<HasPriority>\n";
	oFile << "\t" << mtagPtr->getPriority() << "\n";
	oFile << "\t</HasPriority>";

	oFile << "\n";

	// requiresFulfillment
	oFile << "\t<RequiresFulfillment>\n";
	oFile << "\t" << mtagPtr->getRequireFulfillment() << "\n";
	oFile << "\t</RequiresFulfillment>";

	oFile << "\n";

	// enabledDays
	oFile << "\t<EnabledDays>\n";

	// loop to write enabledDays
	for (auto daysIter : mtagPtr->getEnabledDays())
	{
		oFile << "\t" << daysIter.second << "\n";
	}
	oFile << "\t</EnabledDays>";

	oFile << "\n";

	// linked Tags
	oFile << "\t<LinkedTags>\n";

	// loop to write linkedTags
	for (auto linksIter : mtagPtr->getLinkedTags())
	{
		oFile << "\t\t<TagData>\n";
		// name of tag
		oFile << "\t\t" << linksIter.first->getName() << "\n";

		// amount requested
		oFile << "\t\t" << std::to_string(linksIter.second) << "\n";
		oFile << "\t\t</TagData>\n";
	}
	oFile << "\t</LinkedTags>";
}

int MealManager::readMeals(std::ifstream& iFile)
{
	int returnVal = 0; // 0 = good, 1 = error (aborted import), 2 = corrupted (used default values)
	bool error = false; // true when encountering a read error
	bool done = false; // when read all meals
	bool corrupted = false; // true if read values are invalid
	Meal* mealPtr = nullptr; // for creating meals
	std::string tempStr; // read lines from file

	// while not done importing
	while (!done)
	{
		// check if name exists
		std::getline(iFile, tempStr);
		if (tempStr == "\t<Name>")
		{
			mealPtr = new Meal;

			// get name
			std::getline(iFile, tempStr);

			// remove tab at beginning
			tempStr.erase(0, 1);

			// check if string is correct length
			if (tempStr.length() > NAME_LENGTH)
			{
				tempStr = tempStr.substr(0, NAME_LENGTH); // insert up to character limit
				corrupted = true;
			}

			// set name
			mealPtr->setName(tempStr);

			// read end header, verify
			std::getline(iFile, tempStr);
			if (tempStr != "\t</Name>")
			{
				done = true;
				error = true;
			}
		}
		else if (tempStr == "</Meals>") // end of meals
			done = true;
		else // unknown position
		{
			done = true;
			error = true;
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if price exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<Price>")
			{
				// get price
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// validate price
				try
				{
					double price = std::stod(tempStr);

					if (price <= MAXIMUM_PRICE && price >= MINIMUM_PRICE)
						mealPtr->setPrice(price);
					else
					{
						mealPtr->setPrice(0); // invalid price, default to 0
						corrupted = true;
					}
				}
				catch (...) // value not valid, default to 0
				{
					mealPtr->setPrice(0);
					corrupted = true;
				}

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</Price>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if isDisabled exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<IsDisabled>")
			{
				// get value
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// validate before setting value
				if (std::isdigit(tempStr.at(0)))
					mealPtr->setDisabled(std::stoi(tempStr.substr(0, 1)));
				else
				{
					mealPtr->setDisabled(true); // default to true 
					corrupted = true;
				}

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</IsDisabled>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if mealDuration exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<MealDuration>")
			{
				// get value
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// validate before setting value
				if (std::isdigit(tempStr.at(0)))
				{
					int digits = std::to_string(MEAL_DURATION_LIMIT).length(); // maximum number of digits
					int charLimit = 0;
					bool isValid = true;

					if (digits >= tempStr.length()) // string has less than maximum number of digits
						charLimit = tempStr.length();
					else // string is too long
					{
						charLimit = digits;
						corrupted = true;

						tempStr = tempStr.substr(0, digits); // limit to correct number of digits
					}

					// validate every character in tempStr
					int index = 0;
					while (isValid && index < charLimit)
					{
						if (!std::isdigit(tempStr.at(index)))
							isValid = false;
						++index;
					}

					if (isValid) // is a number 
					{
						unsigned int number = std::stoi(tempStr);

						// check if number is <= limit
						if (number <= MEAL_DURATION_LIMIT)
							mealPtr->setMealDuration(number);
						else // default to 1
						{
							mealPtr->setMealDuration(1);
							corrupted = true;
						}
					}
					else // NaN, default to 1
					{
						mealPtr->setMealDuration(1);
						corrupted = true;
					}
				}
				else // NaN, default to 1
				{
					mealPtr->setMealDuration(1);
					corrupted = true;
				}

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</MealDuration>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if daysBetweenOccurrences exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<DaysBetweenOccurrences>")
			{
				// get value
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// validate before setting value
				if (std::isdigit(tempStr.at(0)))
				{
					int digits = std::to_string(DAYS_BETWEEN_OCCURRENCES_LIMIT).length(); // maximum number of digits
					int charLimit = 0;
					bool isValid = true;

					if (digits >= tempStr.length()) // string has less than maximum number of digits
						charLimit = tempStr.length();
					else // string is too long
					{
						charLimit = digits;
						corrupted = true;

						tempStr = tempStr.substr(0, digits); // limit to correct number of digits
					}

					// validate every character in tempStr
					int index = 0;
					while (isValid && index < charLimit)
					{
						if (!std::isdigit(tempStr.at(index)))
							isValid = false;
						++index;
					}

					if (isValid) // is a number 
					{
						unsigned int number = std::stoi(tempStr);

						// check if number is <= limit
						if (number <= DAYS_BETWEEN_OCCURRENCES_LIMIT)
							mealPtr->setDaysBetweenOccurrences(number);
						else // default to 0
						{
							mealPtr->setDaysBetweenOccurrences(0);
							corrupted = true;
						}
					}
					else // NaN, default to 0
					{
						mealPtr->setDaysBetweenOccurrences(0);
						corrupted = true;
					}
				}
				else // NaN, default to 0
				{
					mealPtr->setDaysBetweenOccurrences(0);
					corrupted = true;
				}

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</DaysBetweenOccurrences>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if MealTags exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<MealTags>")
			{
				bool doneGettingLinks = false;

				do
				{
					// get start of tag data or end of assigned tags
					std::getline(iFile, tempStr);
					if (tempStr == "\t\t<TagName>")
					{
						bool matchFound = false;

						// get name of tag
						std::getline(iFile, tempStr);

						// remove 2 tabs
						tempStr.erase(0, 2);

						// validate length
						if (tempStr.length() > NAME_LENGTH)
						{
							tempStr = tempStr.substr(0, NAME_LENGTH);
							corrupted = true;
						}

						// find a tag with this name
						auto tagIter = normalTags.begin();
						while (!matchFound && tagIter != normalTags.end())
						{
							// if match found, assign tag to meal, and meal to tag
							if (tempStr == (*tagIter)->getName())
							{
								matchFound = true;
								mealPtr->addTag(*tagIter);
								(*tagIter)->addMeal(mealPtr);
							}
							else
								++tagIter;
						}

						// read end header, verify
						std::getline(iFile, tempStr);
						if (tempStr != "\t\t</TagName>")
						{
							doneGettingLinks = true;
							done = true;
							error = true;
						}
					}
					else if (tempStr == "\t</MealTags>")
						doneGettingLinks = true;
					else // error
					{
						doneGettingLinks = true;
						error = true;
					}

				} while (!doneGettingLinks);
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// check if a meal was created
		if (!error && mealPtr != nullptr)
		{
			// got all parameters, load into meals
			meals.push_back(mealPtr);
			mealPtr = nullptr;
		}

		if (!done)
		{
			// check if another meal exists
			std::getline(iFile, tempStr);
			if (tempStr == "</Meals>")
				done = true;
		}

	} // while (!done && !iFile.eof())

	// if error, delete meal
	if (error && mealPtr != nullptr)
		delete mealPtr;

	if (error) // if had to abort, return value
		returnVal = error;
	else if (corrupted) // if corruption detected
		returnVal = 2;

	return returnVal;
}

int MealManager::readTags(std::ifstream& iFile)
{
	int returnVal = 0; // 0 = good, 1 = error (aborted import), 2 = corrupted (used default values)
	bool error = false; // true when encountering a read error
	bool done = false; // when read all tags
	bool corrupted = false;

	Tag* tagPtr = nullptr; // for creating Tags
	std::string tempStr; // read lines from file
	std::map<DaysOfTheWeek, bool> enabledDays = // to create EnabledDays in Tags
	{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
		{FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} }; 

	// while not done importing or ran into end of file
	while (!done && !iFile.eof())
	{
		// check if name exists
		std::getline(iFile, tempStr);
		if (tempStr == "\t<Name>")
		{
			tagPtr = new Tag;

			// get name
			std::getline(iFile, tempStr);

			// remove tab at beginning
			tempStr.erase(0, 1);

			// check if string is correct length
			if (tempStr.length() > NAME_LENGTH)
			{
				tempStr= tempStr.substr(0, NAME_LENGTH); // get up to character limit
				corrupted = true;
			}

			// set value
			tagPtr->setName(tempStr);

			// read end header, verify
			std::getline(iFile, tempStr);
			if (tempStr != "\t</Name>")
			{
				done = true;
				error = true;
			}
		}
		else if (tempStr == "</Tags>") // end of tags 
			done = true;
		else // unknown position
		{
			done = true;
			error = true;
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if description exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<Description>")
			{
				// get description
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// check if string is correct length
				if (tempStr.length() > DESC_LENGTH)
				{
					tempStr = tempStr.substr(0, DESC_LENGTH); // get up to character limit
					corrupted = true;
				}

				// set value
				tagPtr->setDescription(tempStr);

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</Description>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if dependsOnMultiTag exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<DependsOnMultiTag>")
			{
				// get value
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// validate before setting value
				if (std::isdigit(tempStr.at(0)))
					tagPtr->setDependency(std::stoi(tempStr.substr(0, 1)));
				else
				{
					tagPtr->setDependency(false); // default to false 
					corrupted = true;
				}

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</DependsOnMultiTag>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if ConsecutiveLimit exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<ConsecutiveLimit>")
			{
				// get value
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// validate before setting value
				if (std::isdigit(tempStr.at(0)))
				{
					int digits = std::to_string(CONSECUTIVE_DAYS_LIMIT).length(); // maximum number of digits
					int charLimit = 0;
					bool isValid = true;

					if (digits >= tempStr.length()) // string has less than maximum number of digits
						charLimit = tempStr.length();
					else // string is too long
					{
						charLimit = digits;
						corrupted = true;

						tempStr = tempStr.substr(0, digits); // limit to correct number of digits
					}

					// validate every character in tempStr
					int index = 0;
					while (isValid && index < charLimit)
					{
						if (!std::isdigit(tempStr.at(index)))
							isValid = false;
						++index;
					}

					if (isValid) // is a number 
					{
						unsigned int number = std::stoi(tempStr);

						// check if number is <= limit
						if (number <= CONSECUTIVE_DAYS_LIMIT)
							tagPtr->setConsecutiveLimit(number);
						else // default to 0
						{
							tagPtr->setConsecutiveLimit(0);
							corrupted = true;
						}
					}
					else // NaN, default to 0
					{
						tagPtr->setConsecutiveLimit(0);
						corrupted = true;
					}
				}
				else // NaN, default to 0
				{
					tagPtr->setConsecutiveLimit(0);
					corrupted = true;
				}

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</ConsecutiveLimit>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if EnabledDays exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<EnabledDays>")
			{
				// loop to get all days
				for (int day = 0; day < 7; ++day)
				{
					// get value
					std::getline(iFile, tempStr);

					// remove tab at beginning
					tempStr.erase(0, 1);

					// validate before setting value
					if (!std::isdigit(tempStr.at(0)) || tempStr.length() > 1)
					{
						tempStr = "0"; // default to false 
						corrupted = true;
					}

					// set value
					switch (day)
					{
					case 0: // monday
						enabledDays.at(MONDAY) = std::stoi(tempStr);
						break;
					case 1: // tuesday
						enabledDays.at(TUESDAY) = std::stoi(tempStr);
						break;
					case 2: // wednesday
						enabledDays.at(WEDNESDAY) = std::stoi(tempStr);
						break;
					case 3: // thursday
						enabledDays.at(THURSDAY) = std::stoi(tempStr);
						break;
					case 4: // friday
						enabledDays.at(FRIDAY) = std::stoi(tempStr);
						break;
					case 5: // saturday
						enabledDays.at(SATURDAY) = std::stoi(tempStr);
						break;
					case 6: // sunday
						enabledDays.at(SUNDAY) = std::stoi(tempStr);
						break;
					default:
						error = true;
						day = 10; // exit loop
					}
				}
				tagPtr->setEnabledDays(enabledDays);

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</EnabledDays>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// check if a tag was created
		if (!error && tagPtr != nullptr)
		{
			// got all parameters, load into normalTags
			normalTags.push_back(tagPtr);
			tagPtr = nullptr;
		}

		if (!done)
		{
			// check if another tag exists
			std::getline(iFile, tempStr);
			if (tempStr == "</Tags>")
				done = true;
		}

	} // while (!done && !iFile.eof())

	// if error, delete tagPtr
	if (error && tagPtr != nullptr)
		delete tagPtr;

	// if met end of file, error
	if (iFile.eof())
		error = true;

	if (error)
		returnVal = 1;
	else if (corrupted)
		returnVal = 2;

	return returnVal;
}

int MealManager::readMultiTags(std::ifstream& iFile)
{
	int returnVal = 0; // 0 = good, 1 = error (aborted import), 2 = corrupted (used default values)
	bool error = false; // true when encountering a read error
	bool done = false; // when read all tags
	bool corrupted = false;

	MultiTag* mtagPtr = nullptr; // for creating MultiTags
	std::string tempStr; // read lines from file

	std::map<DaysOfTheWeek, bool> enabledDays = // to create EnabledDays in Tags
	{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
		{FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };

	// while not done importing or ran into end of file
	while (!done && !iFile.eof())
	{
		// check if name exists
		std::getline(iFile, tempStr);
		if (tempStr == "\t<Name>")
		{
			mtagPtr = new MultiTag;

			// get name
			std::getline(iFile, tempStr);

			// remove tab at beginning
			tempStr.erase(0, 1);

			// check if string is correct length
			if (tempStr.length() > NAME_LENGTH)
			{
				tempStr = tempStr.substr(0, NAME_LENGTH); // get up to character limit
				corrupted = true;
			}

			// set value
			mtagPtr->setName(tempStr);

			// read end header, verify
			std::getline(iFile, tempStr);
			if (tempStr != "\t</Name>")
			{
				done = true;
				error = true;
			}
		}
		else if (tempStr == "</MultiTags>") // end of multitags 
			done = true;
		else // unknown position
		{
			done = true;
			error = true;
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if description exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<Description>")
			{
				// get description
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// check if string is correct length
				if (tempStr.length() > DESC_LENGTH)
				{
					tempStr = tempStr.substr(0, DESC_LENGTH); // get up to character limit
					corrupted = true;
				}

				// set value
				mtagPtr->setDescription(tempStr);

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</Description>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if hasPriority exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<HasPriority>")
			{
				// get value
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// validate before setting value
				if (std::isdigit(tempStr.at(0)))
					mtagPtr->setHighestPriority(std::stoi(tempStr.substr(0, 1)));
				else
				{
					mtagPtr->setHighestPriority(false); // default to false 
					corrupted = true;
				}

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</HasPriority>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if requiresFulfillment exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<RequiresFulfillment>")
			{
				// get value
				std::getline(iFile, tempStr);

				// remove tab at beginning
				tempStr.erase(0, 1);

				// validate before setting value
				if (std::isdigit(tempStr.at(0)))
					mtagPtr->setRequireFulfillment(std::stoi(tempStr.substr(0, 1)));
				else
				{
					mtagPtr->setRequireFulfillment(false); // default to false 
					corrupted = true;
				}

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</RequiresFulfillment>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if EnabledDays exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<EnabledDays>")
			{
				// loop to get all days
				for (int day = 0; day < 7; ++day)
				{
					// get value
					std::getline(iFile, tempStr);

					// remove tab at beginning
					tempStr.erase(0, 1);

					// validate before setting value
					if (!std::isdigit(tempStr.at(0)) || tempStr.length() > 1)
					{
						tempStr = "0"; // default to false 
						corrupted = true;
					}

					// set value
					switch (day)
					{
					case 0: // monday
						enabledDays.at(MONDAY) = std::stoi(tempStr);
						break;
					case 1: // tuesday
						enabledDays.at(TUESDAY) = std::stoi(tempStr);
						break;
					case 2: // wednesday
						enabledDays.at(WEDNESDAY) = std::stoi(tempStr);
						break;
					case 3: // thursday
						enabledDays.at(THURSDAY) = std::stoi(tempStr);
						break;
					case 4: // friday
						enabledDays.at(FRIDAY) = std::stoi(tempStr);
						break;
					case 5: // saturday
						enabledDays.at(SATURDAY) = std::stoi(tempStr);
						break;
					case 6: // sunday
						enabledDays.at(SUNDAY) = std::stoi(tempStr);
						break;
					default:
						error = true;
						day = 10; // exit loop
					}
				}
				mtagPtr->setEnabledDays(enabledDays);

				// read end header, verify
				std::getline(iFile, tempStr);
				if (tempStr != "\t</EnabledDays>")
				{
					done = true;
					error = true;
				}
			}
			else // error
			{
				done = true;
				error = true;
			}
		}

		// continue if no error and not done
		if (!error && !done)
		{
			// check if LinkedTags exists
			std::getline(iFile, tempStr);
			if (tempStr == "\t<LinkedTags>")
			{
				bool doneGettingLinks = false; 

				do
				{
					// get start of tag data or end of LinkedTags
					std::getline(iFile, tempStr);
					if (tempStr == "\t\t<TagData>")
					{
						Tag* tagPtr = nullptr;
						bool matchFound = false;

						// get name of tag
						std::getline(iFile, tempStr);

						// remove 2 tabs
						tempStr.erase(0, 2);

						// check if string is correct length
						if (tempStr.length() > NAME_LENGTH)
						{
							tempStr = tempStr.substr(0, NAME_LENGTH); // get up to character limit
							corrupted = true;
						}

						// find a tag with this name
						auto tagIter = normalTags.begin();
						while (!matchFound && tagIter != normalTags.end())
						{
							// if match found, set tagPtr to this tag
							if (tempStr == (*tagIter)->getName())
							{
								tagPtr = *tagIter;
								matchFound = true;
							}
							else
								++tagIter;
						}

						// get requested amount of meals for this tag
						std::getline(iFile, tempStr);

						// remove 2 tabs
						tempStr.erase(0, 2);

						// validate before setting value
						if (std::isdigit(tempStr.at(0)))
						{
							int digits = std::to_string(REQUESTED_MEALS_LIMIT).length(); // maximum number of digits
							int charLimit = 0;
							bool isValid = true;

							if (digits >= tempStr.length()) // string has less than maximum number of digits
								charLimit = tempStr.length();
							else // string is too long
							{
								charLimit = digits;
								corrupted = true;

								tempStr = tempStr.substr(0, digits); // limit to correct number of digits
							}

							// validate every character in tempStr
							int index = 0;
							while (isValid && index < charLimit)
							{
								if (!std::isdigit(tempStr.at(index)))
									isValid = false;
								++index;
							}

							if (isValid) // is a number 
							{
								unsigned int number = std::stoi(tempStr);

								// check if number is > limit
								if (number > REQUESTED_MEALS_LIMIT)
								{
									tempStr = "1"; // defaults to 1
									corrupted = true;
								}
							}
							else // NaN, default to 1
							{
								tempStr = "1";
								corrupted = true;
							}
						}
						else // NaN, default to 1
						{
							tempStr = "1";
							corrupted = true;
						}

						// set values
						mtagPtr->addLinkedTag(tagPtr, std::stoi(tempStr));

						// read end header, verify
						std::getline(iFile, tempStr);
						if (tempStr != "\t\t</TagData>")
						{
							doneGettingLinks = true;
							done = true;
							error = true;
						}
					}
					else if (tempStr == "\t</LinkedTags>")
						doneGettingLinks = true;
					else // error
					{
						doneGettingLinks = true;
						error = true;
					}

				} while (!doneGettingLinks);
			}
			else // error
			{
				done = true;
				error = true;
			}
		}


		// check if a multiTag was created
		if (!error && mtagPtr != nullptr)
		{
			// got all parameters, load into MultiTags
			multiTags.push_back(mtagPtr);
			mtagPtr = nullptr;
		}

		if (!done)
		{
			// check if another tag exists
			std::getline(iFile, tempStr);
			if (tempStr == "</MultiTags>")
				done = true;
		}
	} // while (!done && !iFile.eof())

	// if error, delete tagPtr
	if (error && mtagPtr != nullptr)
		delete mtagPtr;

	// if met end of file, error
	if (iFile.eof())
		error = true;

	if (error)
		returnVal = error;
	else if (corrupted)
		returnVal = 2;

	return returnVal;
}

bool MealManager::validateTag(const Tag* tagPtr, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod, const std::vector<Meal*>& assignedMeals, const std::vector<std::vector<Meal*>>& scheduledMeals)
{
	bool validTag = false;
	int lookBack; // checks previous days
	int lookBackLimit; // the furthest day in the past necessary to check

	// set to previous day if one exists
	if (currentDayNumber > 0)
		lookBack = currentDayNumber - 1;
	else
		lookBack = 0;

	// find earliest day to check if safe
	if (currentDayNumber >= tagPtr->getConsecutiveLimit())
		lookBackLimit = currentDayNumber - tagPtr->getConsecutiveLimit();
	else
		lookBackLimit = 0;

	auto assignedMealsIter = assignedMeals.begin(); // iter to meals assigned to the tag

	// if consecutiveLimit is greater than calculationPeriod, or not enough days has passed, then skip detailed check
	if (tagPtr->getConsecutiveLimit() < calculationPeriod && currentDayNumber >= tagPtr->getConsecutiveLimit() && currentDayNumber > 0)
	{
		// loop until all days checked or a gap in scheduled days is found
		while (!validTag && lookBack >= lookBackLimit)
		{
			// get meals of day lookBack
			std::vector<Meal*> checkMeals = scheduledMeals.at(lookBack);

			auto scheduledMealsIter = checkMeals.begin();
			bool matchFound = false; // true when any of the previous day's meals was assigned to the tag being checked

			// check all meals of the day
			while (!validTag && scheduledMealsIter != checkMeals.end())
			{
				// loop to compare scheduled meals with assigned meals
				while (!matchFound && assignedMealsIter != assignedMeals.end())
				{
					// if a meal matches
					if (*assignedMealsIter == *scheduledMealsIter)
						matchFound = true;

					++assignedMealsIter;
				}

				++scheduledMealsIter;
			}

			// if no match was found, then the tag is valid
			if (!matchFound)
				validTag = true;

			--lookBack;
		}
	}
	else
		validTag = true;

	return validTag;
}

bool MealManager::validateMeal(const Meal* mealPtr, const unsigned int& currentDayNumber)
{
	bool validMeal = false;

	// if meal is enabled, check further
	if (!mealPtr->isDisabled())
	{
		// if meal was never scheduled, then daysBetweenOccurrences will not prevent scheduling
		if (mealPtr->getDaysScheduled().size() > 0)
		{
			// get most recent day scheduled
			unsigned int lastDayScheduled = mealPtr->getDaysScheduled().back();

			// calculate how long to wait until selectedMeal can be scheduled again
			unsigned int waitUntil = lastDayScheduled + mealPtr->getDaysBetweenOccurrences() + 1;

			// if enough days has passed, then meal is valid
			if (currentDayNumber >= waitUntil)
				validMeal = true;
		}
		else
			validMeal = true;
	}

	return validMeal;
}

void MealManager::addFutureMeals(std::vector<Meal*>& futureMeals, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod, std::vector<std::vector<Meal*>>& scheduledMeals)
{
	// schedule all meals in futureMeals
	for (auto mealIter : futureMeals)
	{
		// add each future day meal is scheduled into Meal::daysScheduled
		for (unsigned int daysInAdvance = 1; daysInAdvance < mealIter->getMealDuration(); ++daysInAdvance)
		{
			// if the future day is not beyond the calculationPeriod
			if (currentDayNumber + daysInAdvance < calculationPeriod)
			{
				// update Meal::daysScheduled
				mealIter->addDayScheduled(currentDayNumber + daysInAdvance);

				// if a vector of meals has already been created in the future day, then add this meal to it
				if (scheduledMeals.size() - 1 >= currentDayNumber + daysInAdvance)
				{
					// retreive existing future day's meals
					std::vector<Meal*> existingFutureDay = scheduledMeals.at(currentDayNumber + daysInAdvance);

					// add this meal to the scheduled meals in a future day
					existingFutureDay.push_back(mealIter);

					// update the meals in the future day
					scheduledMeals.at(currentDayNumber + daysInAdvance) = existingFutureDay;
				}
				else // create meals on future day in scheduledMeals
				{
					std::vector<Meal*> futureDay; // meals scheduled for a future day
					futureDay.push_back(mealIter); // add meal to futureDay

					// create an entry for meals in a future day
					scheduledMeals.push_back(futureDay);
				}
			}

		}
	}
}

bool MealManager::scheduleMultiTags(const std::vector<MultiTag*>& availableMultiTags, const unsigned int& currentDayNumber,
	const unsigned int& calculationPeriod, const DaysOfTheWeek& currentDay, std::vector<std::vector<Meal*>>& scheduledMeals)
{
	std::vector<int> searchedMultiTags; // stores indices of multitags that were accessed
	bool mealFound = false;

	// while there are multitags to choose from
	while (!mealFound && searchedMultiTags.size() < availableMultiTags.size())
	{
		bool validMultiTag = false;

		// randomly choose index
		int multitagIndex = rand() % availableMultiTags.size();

		// retrieve multiTag
		MultiTag* chosenMultiTag = availableMultiTags.at(multitagIndex);

		// if no indices previously searched, then ok to proceed
		if (searchedMultiTags.size() == 0)
		{
			searchedMultiTags.push_back(multitagIndex);
			validMultiTag = true;
		}
		else // need to check if index was already searched
		{
			bool indexMatch = false;
			auto searchedMultiTagsIter = searchedMultiTags.begin();

			while (!indexMatch && searchedMultiTagsIter != searchedMultiTags.end())
			{
				if (multitagIndex == *searchedMultiTagsIter)
					indexMatch = true;
				else
					++searchedMultiTagsIter;
			}

			// if no match, then ok to proceed
			if (!indexMatch)
			{
				searchedMultiTags.push_back(multitagIndex);
				validMultiTag = true;
			}
		}

		// if a valid index was found, search tags for a meal
		if (validMultiTag)
		{
			std::map<Tag*, unsigned int> availableTags = chosenMultiTag->getLinkedTags(); // get linked tags of multitag
			std::vector<Meal*> mealsOfDay; // meals scheduled for the current day
			std::vector<Meal*> futureMeals; // meals that last more than one day are grouped together
			unsigned int requiredMeals = 0; // meals needed to consider MultiTag valid
			unsigned int totalMealsScheduled = 0; // meals scheduled for MultiTag

			// check if MultiTag requires all Tags to be fulfilled
			if (chosenMultiTag->getRequireFulfillment())
			{
				// sum all of the required meals
				for (auto linkedTags : availableTags)
				{
					requiredMeals += linkedTags.second;
				}
			}
			else requiredMeals = 1; // only require 1 meal

			// update mealsOfDay if meals were already scheduled (multi-day meals)
			if (scheduledMeals.size() >= currentDayNumber + 1)
				mealsOfDay = scheduledMeals.at(currentDayNumber);

			// all linked tags' requested Meals must be fulfilled
			for (auto tagIter : availableTags)
			{
				unsigned int mealsScheduled = 0; // meals scheduled for Tag
				std::vector<Meal*> assignedMeals = tagIter.first->getLinkedMeals(); // meals that are assigned to the current Tag
				std::vector<int> searchedMeals; // tracks which meals were already searched

				// check if tag is enabled and has meals
				if (tagIter.first->getEnabledDays().at(currentDay) && assignedMeals.size() > 0)
				{
					bool validTag = false;

					// checks tag's params and returns T if valid, F if not
					validTag = validateTag(tagIter.first, currentDayNumber, calculationPeriod, assignedMeals, scheduledMeals);

					// begin to schedule meals if tag is allowed to schedule meals
					if (validTag)
					{
						// ends loop when requested number of meals are met or all meals were searched
						while (mealsScheduled < tagIter.second && searchedMeals.size() < assignedMeals.size())
						{
							bool validMeal = false;

							// randomly choose index of a meal
							int mealIndex = rand() % assignedMeals.size();

							// get meal
							Meal* selectedMeal = assignedMeals.at(mealIndex);

							// if no meals were searched, then add this meal to searchedMeals
							if (searchedMeals.size() == 0)
							{
								searchedMeals.push_back(mealIndex);
								validMeal = validateMeal(selectedMeal, currentDayNumber);
							}
							else // check if meal has been used before
							{
								bool matchFound = false;
								auto searchedMealsIter = searchedMeals.begin();

								// compare mealIndex with those that have been previously used
								while (!matchFound && searchedMealsIter != searchedMeals.end())
								{
									if (mealIndex == *searchedMealsIter)
										matchFound = true;
									else
										++searchedMealsIter;
								}

								// if no match, then meal is a candidate to be scheduled
								if (!matchFound)
								{
									searchedMeals.push_back(mealIndex);
									validMeal = validateMeal(selectedMeal, currentDayNumber);
								}
							}

							// if meal is valid so far, then check if it has already been scheduled (a multi-day meal)
							if (validMeal)
							{
								// if there are meals to check
								if (mealsOfDay.size() > 0)
								{
									auto mealIter = mealsOfDay.begin();
									bool found = false;

									while (!found && mealIter != mealsOfDay.end())
									{
										if (*mealIter == selectedMeal)
										{
											found = true;
											validMeal = false;
										}
										else
											++mealIter;
									}
								}
							}

							// try to schedule the selected meal if it is valid
							if (validMeal)
							{
								// add meal to mealsOfDay
								mealsOfDay.push_back(selectedMeal);
								++mealsScheduled;
								++totalMealsScheduled;

								// update daysScheduled
								selectedMeal->addDayScheduled(currentDayNumber);

								// if selectedMeal lasts for more than 1 day, then add to futureMeals
								if (selectedMeal->getMealDuration() > 1)
									futureMeals.push_back(selectedMeal);
							}

						} // while: ends loop when requested number of meals are met or all meals were searched

						// DEBUG INFO
						// if not enough meals were found to fulfil tag's requirement
						/*if (mealsScheduled < tagIter.second)
						{
							// display error to user
							std::cout << "\nWARNING: Tag \"" << tagIter.first->getName() << "\" requests " << tagIter.second << " meals, but only " << std::to_string(mealsScheduled)
								<< " meals were scheduled.\nPossibly caused by insufficient Meals that are enabled and linked to Tag, and/or Tag settings are too strict."
								<< "\nThis occurred on: " << dayToString(currentDay) << ", day #" << std::to_string(currentDayNumber) << std::endl;

						}*/
					} // if (validTag)
				} // if tag is enabled and has meals
			} // for (auto tagIter : availableTags)

			// if there are enough meals to schedule
			if (totalMealsScheduled >= requiredMeals)
			{
				mealFound = true;
				// add mealsOfDay to scheduledMeals
				if (scheduledMeals.size() >= currentDayNumber + 1)
					scheduledMeals.at(currentDayNumber) = mealsOfDay;
				else
					scheduledMeals.push_back(mealsOfDay);
			}

			// if there are meals that last longer than 1 day, add them to future days in the schedule
			if (futureMeals.size() > 0)
			{
				addFutureMeals(futureMeals, currentDayNumber, calculationPeriod, scheduledMeals);
			}
		} // if (validMultiTag)
	} // while there are multitags to choose from

	return mealFound;
}

bool MealManager::scheduleNormalTags(const std::vector<Tag*>& availableTags, const unsigned int& currentDayNumber, const unsigned int& calculationPeriod, const DaysOfTheWeek& currentDay, std::vector<std::vector<Meal*>>& scheduledMeals)
{
	bool mealFound = false;

	std::vector<int> searchedTags; // stores indices of tags that were accessed

	// no meal found and there are tags to choose from
	while (!mealFound && searchedTags.size() < availableTags.size())
	{
		bool validIndex = false;

		// randomly choose index
		int tagIndex = rand() % availableTags.size();

		// retreive Tag
		Tag* chosenTag = availableTags.at(tagIndex);
		
		// get meals assigned chosenTag
		std::vector<Meal*> assignedMeals = chosenTag->getLinkedMeals();

		// if no indices searched, then ok to proceed
		if (searchedTags.size() == 0)
		{
			searchedTags.push_back(tagIndex);
			validIndex = true;
		}
		else // need to check if tag already searched
		{
			bool indexMatch = false;
			auto searchedTagsIter = searchedTags.begin();

			while (!indexMatch && searchedTagsIter != searchedTags.end())
			{
				if (tagIndex == *searchedTagsIter)
					indexMatch = true;
				else
					++searchedTagsIter;
			}

			// if no match, then ok to proceed
			if (!indexMatch)
			{
				searchedTags.push_back(tagIndex);
				validIndex = true;
			}
		}

		// begin checking tag if index is valid
		if (validIndex)
		{
			// check if tag is enabled and has meals
			if (chosenTag->getEnabledDays().at(currentDay) && assignedMeals.size() > 0)
			{
				bool validTag = false;

				// checks tag's params and returns T if tag is valid, F if not
				validTag = validateTag(chosenTag, currentDayNumber, calculationPeriod, assignedMeals, scheduledMeals);

				// begin to schedule meals if tag is valid
				if (validTag)
				{
					std::vector<int> searchedMeals; // stores index of all meals searched within the chosenTag
					std::vector<Meal*> mealsOfDay; // meals scheduled/to be scheduled on the current day

					// update mealsOfDay if meals were already scheduled (multi-day meals)
					if (scheduledMeals.size() >= currentDayNumber + 1)
						mealsOfDay = scheduledMeals.at(currentDayNumber);

					// ends loop when a meal was found or all meals were searched
					while (!mealFound && searchedMeals.size() < assignedMeals.size())
					{
						bool validMeal = false;

						// randomly choose index of a meal
						int mealIndex = rand() % assignedMeals.size();

						// get meal
						Meal* selectedMeal = assignedMeals.at(mealIndex);

						// if no meals were searched, then ok to proceed
						if (searchedMeals.size() == 0)
						{
							searchedMeals.push_back(mealIndex);
							validMeal = validateMeal(selectedMeal, currentDayNumber);
						}
						else // check if meal has been used before
						{
							bool matchFound = false;
							auto searchedMealsIter = searchedMeals.begin();

							// compare mealIndex with those that have been previously used
							while (!matchFound && searchedMealsIter != searchedMeals.end())
							{
								if (mealIndex == *searchedMealsIter)
									matchFound = true;
								else
									++searchedMealsIter;
							}

							// if no match, then check if meal's daysBetweenOccurrences allows it to be scheduled
							if (!matchFound)
							{
								searchedMeals.push_back(mealIndex);
								validMeal = validateMeal(selectedMeal, currentDayNumber);
							}
						}

						// if meal is valid so far, then check if it has already been scheduled (a multi-day meal)
						if (validMeal)
						{
							// if there are meals to check
							if (mealsOfDay.size() > 0)
							{
								auto mealIter = mealsOfDay.begin();
								bool found = false;

								while (!found && mealIter != mealsOfDay.end())
								{
									if (*mealIter == selectedMeal)
									{
										found = true;
										validMeal = false;
									}
									else
										++mealIter;
								}
							}
						}

						// try to schedule the selected meal if it is valid
						if (validMeal)
						{
							mealFound = true;

							// add meal to mealsOfDay
							mealsOfDay.push_back(selectedMeal);

							// update daysScheduled
							selectedMeal->addDayScheduled(currentDayNumber);

							// add mealsOfDay to scheduledMeals
							if (scheduledMeals.size() >= currentDayNumber + 1) // an entry for the day already exists
								scheduledMeals.at(currentDayNumber) = mealsOfDay;
							else
								scheduledMeals.push_back(mealsOfDay);

							// if selectedMeal lasts for more than 1 day, then add to futureMeals
							if (selectedMeal->getMealDuration() > 1)
							{
								addFutureMeals(mealsOfDay, currentDayNumber, calculationPeriod, scheduledMeals);
							}
						}

					} // while: ends loop when a meal was found or all meals were searched
				} // if (validTag)
			} // if tag is enabled and has meals
		} // if (validIndex)
	} // while no meal found and there are tags to choose from

	return mealFound;
}

void MealManager::printSchedule(const std::vector<std::vector<Meal*>>& mealPlan, const unsigned int& erroredDays, const double& totalCost, const double& budgetLimit, std::ofstream& oFile)
{
	const unsigned int LINE_WIDTH = 80; // output line width in characters

	std::string lineDivider = ""; // divides sections of the meal plan
	DaysOfTheWeek currentDay = MONDAY; // used to display meals of each day
	unsigned int weekCount = 0; // tracks each week

	// create divider of length LINE_WIDTH
	for (int index = 0; index < LINE_WIDTH; ++index)
		lineDivider += "-";


	// header
	oFile << centeredText("Meal Plan", LINE_WIDTH) << "\n";

	oFile << lineDivider << "\n";

	// creation time 
	// get system time 
	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);;

	oFile << "Created on: " << ctime(&currentTime);

	// plan duration
	oFile << "Duration: " << std::to_string(mealPlan.size() / 7) << " weeks (" << std::to_string(mealPlan.size()) << " days)\n";

	// budget limit 
	oFile << "Budget limit: " << formatPrice(budgetLimit) << "\n";

	// calculated cost
	oFile << "Calculated cost: " << formatPrice(totalCost) << "\n";

	// number of errors
	oFile << "Days with errors: " << std::to_string(erroredDays) << "\n";

	// iterates through each day of mealPlan
	auto mealIter = mealPlan.begin();
	while (mealIter != mealPlan.end())
	{
		// if today is the start of a new week, print week header
		if (currentDay == MONDAY)
		{
			std::string tempStr = "WEEK "; // outputs "WEEK XX : cost yyyy"
			double costOfWeek = 0; // total of all meals in the upcoming week
			DaysOfTheWeek dayInWeek = MONDAY; // start on monday of current week
			auto weekIter = mealIter; // copy iterator

			++weekCount;

			// if weekCount is only 1 digit
			if (weekCount < 10)
				tempStr += "0";

			tempStr += std::to_string(weekCount);

			tempStr += " : cost ";

			// sum the price of all meals in the current week
			do
			{
				std::vector<Meal*> mealsInDay = *weekIter;

				for (auto meal : mealsInDay)
				{
					costOfWeek += meal->getPrice();
				}

				++weekIter;
				dayInWeek = nextDay(dayInWeek);
			} while (dayInWeek != MONDAY && weekIter != mealPlan.end());
			tempStr += formatPrice(costOfWeek);

			// begin printing week header
			oFile << lineDivider << "\n"; // divides previous week
			oFile << lineDivider << "\n";
			
			oFile << centeredText(tempStr, LINE_WIDTH) << "\n\n";
		}

		// day name
		std::string tempStr = ">> " + dayToString(currentDay) + " <<";
		oFile << centeredText(tempStr, LINE_WIDTH) << "\n";
		tempStr = "";

		// meals in the current day
		std::vector<Meal*> daysMeals = *mealIter;

		for (auto meal : daysMeals)
		{
			// meal name
			oFile << centeredText(meal->getName(), LINE_WIDTH) << "\n";

			// check if this is a multi-day meal
			if (meal->getMealDuration() > 1)
			{
				int occurranceNumber = 0; // number of times the meal has occurred over its duration
				auto lookBack = mealIter; // copy iter at current day
				bool doneChecking = false; // true when a gap of scheduled days is found or checked to the first day

				// check previous days to count occurranceNumber
				while (!doneChecking)
				{
					// lookBack is on the first day, stop checking further back
					if (lookBack == mealPlan.begin())
					{
						doneChecking = true;
					}

					std::vector<Meal*> mealsOfDay = *lookBack; // get meals of the previous day
					auto mealsOfDayIter = mealsOfDay.begin();

					bool matchFound = false; // true when the current meal has found to have occurred before
					while (!matchFound && mealsOfDayIter != mealsOfDay.end())
					{
						// if meal occurred in the past
						if (*mealsOfDayIter == meal)
						{
							++occurranceNumber;
							matchFound = true;
						}
						else
							++mealsOfDayIter;
					}

					// if no match found in the previous day, then a gap was found
					if (!matchFound)
						doneChecking = true;

					// if done checking, dont go further back
					if (!doneChecking)
						--lookBack;
				}

				// since all occurrances are counted, need to reset occurranceNumber every time it counts to duration
				if (occurranceNumber > meal->getMealDuration())
				{
					occurranceNumber = occurranceNumber % meal->getMealDuration();
					
					// set to 3 instead of 0
					if (occurranceNumber == 0)
						occurranceNumber = 3;
				}

				// if this is the first occurrance, then display price with occcurrance number
				if (occurranceNumber == 1)
				{
					// meal price
					tempStr = "price: " + formatPrice(meal->getPrice());
					oFile << centeredText(tempStr, LINE_WIDTH) << "\n";

					// occurrance number
					tempStr = "[Day " + std::to_string(occurranceNumber) + " of " + std::to_string(meal->getMealDuration()) + "]";
				}
				else
				{
					// occurrance number
					tempStr = "[Day " + std::to_string(occurranceNumber) + " of " + std::to_string(meal->getMealDuration()) + "]";
				}

			}
			else // display price normally
			{
				// meal price
				tempStr = "price: " + formatPrice(meal->getPrice());
			}
			oFile << centeredText(tempStr, LINE_WIDTH) << "\n\n";
			tempStr = "";
		}

		currentDay = nextDay(currentDay);
		++mealIter;
	}

	oFile << lineDivider << "\n";
}

std::string MealManager::centeredText(const std::string& str, const unsigned int& lineWidth)
{
	std::string returnStr = "";

	int emptySpaces; // number of empty spaces needed to center text

	// calculate starting point
	emptySpaces = (lineWidth / 2) - (str.length() / 2);

	// place empty spaces before text
	for (int index = 0; index < emptySpaces; index++)
		returnStr += " ";

	// print input string
	returnStr += str;

	emptySpaces += str.length(); // use emptySpaces as an index

	// place empty spaces after text
	for (emptySpaces; emptySpaces < lineWidth; emptySpaces++)
		returnStr += " ";

	return returnStr;
}

bool MealManager::generateSchedule(const std::string& fileName, std::ofstream& oFile, unsigned int& failedPlanErrors, double& failedPlanCost)
{
	const unsigned int MIN_CALCULATION_LENGTH = 1; // 1 week
	const unsigned int MAX_CALCULATION_LENGTH = 52; // just under a year (in weeks)
	const unsigned int MIN_BUDGET = 0;
	const unsigned int MAX_BUDGET = 100000;
	const unsigned int GENERATED_PLANS = 500; // number of meal plans to make
	const double ERROR_THRESHOLD_PER_WEEK = 0.5; // amount of errored days allowed per week

	bool errorPresent = false; // return value
	bool insufficientData = true; // stops generation if no meals or tags exist
	double budget = 0; // budget for the calculated period
	unsigned int calculationPeriod = 0; // period of calculation
	unsigned int failedDays[GENERATED_PLANS] = { 0 }; // number of days that calculation was unable to find a meal
	
	failedPlanErrors = 0;
	failedPlanCost = 0;

	DaysOfTheWeek currentDay = MONDAY; // start of the week
	std::vector<std::vector<Meal*>> scheduledMeals[GENERATED_PLANS]; // meals scheduled for each attempt 

	std::string tempStr = "";
	std::vector<std::string> strVec;

	// optmized data
	std::map<DaysOfTheWeek, std::vector<MultiTag*>> highPriorityMultiTags; // multiTags with elevated priority, sorted by available day
	std::map<DaysOfTheWeek, std::vector<MultiTag*>> normalPriorityMultiTags; // multiTags with priority of Tag, sorted by available day
	std::map<DaysOfTheWeek, std::vector<Tag*>> normalPriorityTags; // Tags, sorted by available day

	// check if meals and tags exist, otherwise cannot make a plan
	if (meals.size() > 0 && normalTags.size() > 0)
	{
		insufficientData = false;
	}
	else // tell uesr
	{
		errorPresent = true;

		uim->centeredText("Error");
		uim->skipLines(2);
		uim->centeredText("You have not created any Meals or Tags.");
		uim->centeredText("Please create some and try again.");
		uim->display();
	}

	// loop until user is ok with settings
	while (!insufficientData && tempStr != "Q")
	{
		// prompt for calculationPeriod
		uim->centeredText("Schedule Length");
		uim->skipLines(2);
		uim->centeredText("How many weeks do you want to schedule for?");
		uim->prompt_FreeInt(MIN_CALCULATION_LENGTH, MAX_CALCULATION_LENGTH);
		calculationPeriod = std::stoi(uim->display());

		// prompt for budget
		uim->centeredText("Budget Limit");
		uim->skipLines(2);
		tempStr = "What is your budget over a " + std::to_string(calculationPeriod) + " week period?";
		uim->centeredText(tempStr);
		uim->prompt_FreeDouble(MIN_BUDGET, MAX_BUDGET);
		budget = std::stod(uim->display());

		// summary of users choices
		uim->centeredText("Confirm Settings");
		uim->skipLines(2);
		uim->centeredText("Is this OK?");
		uim->skipLines(1);
		
		tempStr = "BUDGET LIMIT: " + formatPrice(budget);
		uim->centeredText(tempStr);
		
		tempStr = "SCHEDULE LENGTH: " + std::to_string(calculationPeriod) + " weeks";
		uim->centeredText(tempStr);

		strVec = { "YYes", "NNo" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// exit loop if user accepts settings
		if (tempStr == "Y")
			tempStr = "Q";
	}

	if (!insufficientData) // tell user to be patient
	{
		std::stringstream skipPrompt = std::stringstream("\n"); // skips prompt in the following ui window

		uim->centeredText("Please wait");
		uim->skipLines(3);
		uim->centeredText("Your Meal Plan is being created.");
		uim->skipLines(1);
		uim->centeredText("Wait time may vary between a few seconds and a few minutes, depending on the speed of your computer.");
		uim->display(std::cout, skipPrompt);
		std::cout << "\n\n";
	}

	// convert weeks into days
	calculationPeriod *= 7;

	// OPTIMIZATION
	if (!insufficientData)
		optimizeData(highPriorityMultiTags, normalPriorityMultiTags, normalPriorityTags);

	// CALCULATION
	if (!insufficientData)
	{	
		// calculates GENERATED_PLANS number of complete meal plans
		for (unsigned int attemptNum = 0; attemptNum < GENERATED_PLANS; ++attemptNum)
		{
			currentDay = MONDAY;

			// calculate one complete meal plan
			for (unsigned int dayNumber = 0; dayNumber < calculationPeriod; ++dayNumber)
			{

				bool createdMeals = false; // if meal creation failed on a certain priority, attempts to use next lower priority

				// check if a high priority MT is available
				if (highPriorityMultiTags.find(currentDay)->second.size() > 0)
				{
					createdMeals = scheduleMultiTags(highPriorityMultiTags.find(currentDay)->second, dayNumber,
						calculationPeriod, currentDay, scheduledMeals[attemptNum]);

				}

				// if above priority failed, check next lower priority
				if (!createdMeals &&
					normalPriorityMultiTags.find(currentDay)->second.size() > 0 &&
					normalPriorityTags.find(currentDay)->second.size()) // if both MTs and tags are available
				{
					// flip coin to decide which to choose from
					int coinFlip = rand() % 1000 + 1; // 1-1000

					if (coinFlip > 500) // choose MTs
					{
						createdMeals = scheduleMultiTags(normalPriorityMultiTags.find(currentDay)->second, dayNumber,
							calculationPeriod, currentDay, scheduledMeals[attemptNum]);
					}
					else // choose tags
					{
						createdMeals = scheduleNormalTags(normalPriorityTags.find(currentDay)->second, dayNumber,
							calculationPeriod, currentDay, scheduledMeals[attemptNum]);
					}
				}

				// if above priority failed, check next lower priority
				if (!createdMeals && normalPriorityMultiTags.find(currentDay)->second.size() > 0) // if only MTs are available
				{
					createdMeals = scheduleMultiTags(normalPriorityMultiTags.find(currentDay)->second, dayNumber,
						calculationPeriod, currentDay, scheduledMeals[attemptNum]);
				}

				// if above priority failed, check next lower priority
				if (!createdMeals && normalPriorityTags.find(currentDay)->second.size() > 0) // if only tags are available
				{
					createdMeals = scheduleNormalTags(normalPriorityTags.find(currentDay)->second, dayNumber,
						calculationPeriod, currentDay, scheduledMeals[attemptNum]);
				}

				if (!createdMeals) // no tags available
				{
					std::vector<Meal*> daysMeals; // the meals for one day
					// insert dummy meal
					Meal* mealPtr = new Meal;
					mealPtr->setName("NO AVAILABLE MEALS");
					mealPtr->setPrice(0);
					mealPtr->setDisabled(true);

					daysMeals.push_back(mealPtr);
					scheduledMeals[attemptNum].push_back(daysMeals);
					++failedDays[attemptNum];

					mealPtr = nullptr;

					// warn user
					//std::cout << "\nWARNING: No available meals on: " << dayToString(currentDay) << ", day #" << std::to_string(dayNumber) << std::endl;
				}
				currentDay = nextDay(currentDay); // go to next day
			} // for: calculates a meal for every day 

			// reset meals' scheduledDays
			for (auto mealIter : meals)
			{
				std::vector<unsigned int> resetDays;
				mealIter->setDaysScheduled(resetDays);
			}
		} // for: calculates an entire plan
	}
	
	// select the plan that had the least number of errors and met budget
	std::vector<std::vector<Meal*>> finalPlan; // plan with least errors
	double finalCost = 0; // cost of all the meals in the plan
	unsigned int finalErrors = 0;

	if (!insufficientData)
	{
		bool planFound = false;
		std::map<unsigned int, std::pair<std::vector<std::vector<Meal*>>, double>> errorsToPlans; // maps errors to scheduled plans with total cost for plan
		unsigned int lowestErrors = 0;

		int planIndex = 0;
		// while a perfect plan was not found, and can check for more plans
		do
		{
			// if plan has no errors
			if (failedDays[planIndex] == 0)
			{
				// check budget
				double totalCost = 0;

				// sum the cost of all meals
				for (int index = 0; index < scheduledMeals[planIndex].size(); ++index)
				{
					// get day of meals
					std::vector<Meal*> currentDay = scheduledMeals[planIndex].at(index);

					for (auto mealIter : currentDay)
					{
						// if meal lasts multiple days, divide the cost over its duration
						if (mealIter->getMealDuration() > 1)
						{
							totalCost += (mealIter->getPrice() / mealIter->getMealDuration());
						}
						else
							totalCost += mealIter->getPrice();
					}
				}

				// if totalCost is within budget, then plan is ok to use
				if (totalCost <= budget)
				{
					planFound = true;
					lowestErrors = 0;
					errorsToPlans.emplace(lowestErrors, std::pair<std::vector<std::vector<Meal*>>, double>(scheduledMeals[planIndex], totalCost));
				}
			}
			else // set lowestErrors 
			{
				// if lowestErrors was never set, then prepare to set it to this plan
				if (errorsToPlans.size() == 0)
				{
					// check if more errors than error threshold
					unsigned int calculatedWeeks = calculationPeriod / 7;
					double allowedErrors = calculatedWeeks * ERROR_THRESHOLD_PER_WEEK;

					// only considers plans that do not exceed allowedErrors
					if (failedDays[planIndex] <= allowedErrors)
					{
						// check budget
						double totalCost = 0;

						// sum the cost of all meals
						for (int index = 0; index < scheduledMeals[planIndex].size(); ++index)
						{
							// get day of meals
							std::vector<Meal*> currentDay = scheduledMeals[planIndex].at(index);

							for (auto mealIter : currentDay)
							{
								// if meal lasts multiple days, divide the cost over its duration
								if (mealIter->getMealDuration() > 1)
								{
									totalCost += (mealIter->getPrice() / mealIter->getMealDuration());
								}
								else
									totalCost += mealIter->getPrice();
							}
						}

						// if totalCost is within budget, then plan is ok to use
						if (totalCost <= budget)
						{
							lowestErrors = failedDays[planIndex];
							errorsToPlans.emplace(lowestErrors, std::pair<std::vector<std::vector<Meal*>>, double>(scheduledMeals[planIndex], totalCost));
						}
					}
				}
				else if (failedDays[planIndex] < lowestErrors) // if a better plan was found
				{
					// check budget
					double totalCost = 0;

					// sum the cost of all meals
					for (int index = 0; index < scheduledMeals[planIndex].size(); ++index)
					{
						// get day of meals
						std::vector<Meal*> currentDay = scheduledMeals[planIndex].at(index);

						for (auto mealIter : currentDay)
						{
							// if meal lasts multiple days, divide the cost over its duration
							if (mealIter->getMealDuration() > 1)
							{
								totalCost += (mealIter->getPrice() / mealIter->getMealDuration());
							}
							else
								totalCost += mealIter->getPrice();
						}
					}

					// if totalCost is within budget, then plan is ok to use
					if (totalCost <= budget)
					{
						// attempt to insert new element
						auto iter = errorsToPlans.insert(std::pair<unsigned int, std::pair<std::vector<std::vector<Meal*>>, double>>(lowestErrors, { scheduledMeals[planIndex], totalCost }));

						// check if key already exists
						if (iter.second == false)
						{
							// replace value at key
							errorsToPlans[lowestErrors] = std::pair<std::vector<std::vector<Meal*>>, double>(scheduledMeals[planIndex], totalCost);
						}
					}
				}
			}
			++planIndex;
		} while (!planFound && planIndex < GENERATED_PLANS);

		// set finalPlan to plan with lowestErrors that met budget
		if (errorsToPlans.size() > 0)
		{
			finalPlan = errorsToPlans.at(lowestErrors).first;
			finalCost = errorsToPlans.at(lowestErrors).second;
			finalErrors = lowestErrors;
		}
		else // no suitable plans found
		{
			errorPresent = true;

			// retreive total budget and errors from the first plan generated
			failedPlanErrors = failedDays[0];

			// check budget
			failedPlanCost = 0;

			// sum the cost of all meals
			for (int index = 0; index < scheduledMeals[0].size(); ++index)
			{
				// get day of meals
				std::vector<Meal*> currentDay = scheduledMeals[0].at(index);

				for (auto mealIter : currentDay)
				{
					// if meal lasts multiple days, divide the cost over its duration
					if (mealIter->getMealDuration() > 1)
					{
						failedPlanCost += (mealIter->getPrice() / mealIter->getMealDuration());
					}
					else
						failedPlanCost += mealIter->getPrice();
				}
			}
		}
	}// if (!insufficientData)
	

	// if finalPlan is not empty
	if (finalPlan.size() > 0)
	{
		errorPresent = false;

		oFile.open(fileName);
		printSchedule(finalPlan, finalErrors, finalCost, budget, oFile);
		oFile.close();
	}

	// CLEANUP
	// checks every plan generated
	for (int attemptNumber = 0; attemptNumber < GENERATED_PLANS; ++attemptNumber)
	{
		// checks every day
		for (unsigned int dayNumber = 0; dayNumber < calculationPeriod; ++dayNumber)
		{
			// checks every meal
			for (unsigned int mealNumber = 0; mealNumber < scheduledMeals[attemptNumber].at(dayNumber).size(); ++mealNumber)
			{
				// if meal is an error meal (i.e. not a real meal), then delete it
				Meal* mealPtr = scheduledMeals[attemptNumber].at(dayNumber).at(mealNumber);

				if (mealPtr->getName() == "NO AVAILABLE MEALS" && mealPtr->getPrice() == 0.0 && mealPtr->isDisabled() == true)
				{
					delete mealPtr;
					mealPtr = nullptr;
				}
			}
		}
	} // for: checks every plan generated

	return errorPresent;
}

void MealManager::mealEditor()
{
	std::string tempStr = "";
	int lastPageVisted = -1;

	// display meals and get user's choice, or quit
	do
	{
		Meal* mealPtr = displayMeals(lastPageVisted);

		// if user did not quit
		if (mealPtr != nullptr)
		{
			// call editor
			editMeal(mealPtr);
		}
		else // force quit
			tempStr = "Q";

	} while (tempStr != "Q");
}

void MealManager::tagEditor()
{
	std::string tempStr = "";
	int lastPageVisted = -1;
	int returnStatus = -2;

	Tag* tagPtr = nullptr;

	// display tags and get user's choice, or quit
	do
	{
		returnStatus = displayTags(lastPageVisted, tagPtr);

		// if user chose a tag
		if (returnStatus == 0)
		{
			// call editor
			editTag(tagPtr);
		}
		else if (returnStatus == -1) // user quit
			tempStr = "Q";

	} while (tempStr != "Q");
}

void MealManager::multitagEditor()
{
	std::string tempStr = "";
	int lastPageVisted = -1;

	// display meals and get user's choice, or quit
	do
	{
		MultiTag* mtagPtr = displayMultiTags(lastPageVisted);

		// if user did not quit
		if (mtagPtr != nullptr)
		{
			// call editor
			editMultiTag(mtagPtr);
		}
		else // force quit
			tempStr = "Q";

	} while (tempStr != "Q");
}

void MealManager::saveState(const std::string& dataFile, std::ofstream& oFile)
{
	// open file for writing
	oFile.open(dataFile);

	// check if open
	if (oFile.is_open())
	{
		// write tags
		oFile << "<Tags>\n";

		// check if tags exist to save
		if (normalTags.size() > 0)
		{
			auto tagIter = normalTags.begin();

			// write first tag
			writeTag(*tagIter, oFile);
			++tagIter;

			// loop to write rest of tags
			while (tagIter != normalTags.end())
			{
				oFile << "\n\n";
				writeTag(*tagIter, oFile);
				++tagIter;
			}
			oFile << "\n";
		}
		oFile << "</Tags>\n\n";


		// write muliTags
		oFile << "<MultiTags>\n";

		// check if multiTags exist to save
		if (multiTags.size() > 0)
		{
			auto tagIter = multiTags.begin();

			// write first tag
			writeMultiTag(*tagIter, oFile);
			++tagIter;

			// loop to write rest of tags
			while (tagIter != multiTags.end())
			{
				oFile << "\n\n";
				writeMultiTag(*tagIter, oFile);
				++tagIter;
			}
			oFile << "\n";
		}
		oFile << "</MultiTags>\n\n";

		// write meals
		oFile << "<Meals>\n";

		// check if meals exist to save
		if (meals.size() > 0)
		{
			auto mealIter = meals.begin();

			// write first tag
			writeMeal(*mealIter, oFile);
			++mealIter;

			// loop to write rest of tags
			while (mealIter != meals.end())
			{
				oFile << "\n\n";
				writeMeal(*mealIter, oFile);
				++mealIter;
			}
			oFile << "\n";
		}
		oFile << "</Meals>";

		// close file
		oFile.close();
	}
	else
		throw std::string("Cannot open file");
}

int MealManager::loadState(const std::string& outputFile, std::ifstream& iFile)
{
	std::string tagError = "Error reading Tags";
	std::string multiTagError = "Error reading MultiTags";
	std::string mealError = "Error reading Meals";

	int returnVal = 0; // 0 = good, 1 = corrupted

	bool corruptionDetected = false;
	std::string line; // lines read from file

	// open file
	iFile.open(outputFile);

	// check if failed
	if (iFile.is_open())
	{
		// get Tags header
		std::getline(iFile, line);

		// check if header is correct
		if (line == "<Tags>")
		{
			int returnVal = readTags(iFile);

			// if tags imported with error
			if (returnVal == 1)
				throw tagError;
			else if (returnVal == 2)
				corruptionDetected = true;
		}
		else throw tagError;

		// get blank space
		std::getline(iFile, line);
		// get multiTags header
		std::getline(iFile, line);

		// check if header is correct
		if (line == "<MultiTags>")
		{
			int returnVal = readMultiTags(iFile);

			// if tags imported with error
			if (returnVal == 1)
				throw multiTagError;
			else if (returnVal == 2)
				corruptionDetected = true;
		}
		else throw multiTagError;

		// get blank space
		std::getline(iFile, line);
		// get Meals header
		std::getline(iFile, line);

		// check if header is correct
		if (line == "<Meals>")
		{
			int returnVal = readMeals(iFile);

			// if tags imported with error
			if (returnVal == 1)
				throw mealError;
			else if (returnVal == 2)
				corruptionDetected = true;
		}
		else throw mealError;

		iFile.close();
	}

	if (corruptionDetected)
		returnVal = 1;

	// organize by alphabetic order
	{
		std::sort(normalTags.begin(), normalTags.end(),
			[](Tag* lhs, Tag* rhs) { return lhs->getName() < rhs->getName(); });

		std::sort(meals.begin(), meals.end(), 
			[](Meal* lhs, Meal* rhs) { return lhs->getName() < rhs->getName(); });

		std::sort(multiTags.begin(), multiTags.end(),
			[](MultiTag* lhs, MultiTag* rhs) { return lhs->getName() < rhs->getName(); });
	}

	return returnVal;
}

DaysOfTheWeek nextDay(const DaysOfTheWeek& day)
{
	DaysOfTheWeek returnDay = MONDAY;

	switch (day)
	{
	case MONDAY:  returnDay = TUESDAY;
		break;
	case TUESDAY:  returnDay = WEDNESDAY;
		break;
	case WEDNESDAY:  returnDay = THURSDAY;
		break;
	case THURSDAY:  returnDay = FRIDAY;
		break;
	case FRIDAY:  returnDay = SATURDAY;
		break;
	case SATURDAY:  returnDay = SUNDAY;
		break;
	case SUNDAY:  returnDay = MONDAY;
		break;
	}

	return  returnDay;
}

DaysOfTheWeek previousDay(const DaysOfTheWeek& day)
{
	DaysOfTheWeek returnDay = MONDAY;

	switch (day)
	{
	case MONDAY:  returnDay = SUNDAY;
		break;
	case TUESDAY:  returnDay = MONDAY;
		break;
	case WEDNESDAY:  returnDay = TUESDAY;
		break;
	case THURSDAY:  returnDay = WEDNESDAY;
		break;
	case FRIDAY:  returnDay = THURSDAY;
		break;
	case SATURDAY:  returnDay = FRIDAY;
		break;
	case SUNDAY:  returnDay = SATURDAY;
		break;
	}

	return returnDay;
}

std::string dayToString(const DaysOfTheWeek& day)
{
	std::string returnStr = "";

	switch (day)
	{
	case MONDAY: returnStr = "MONDAY";
		break;
	case TUESDAY: returnStr = "TUESDAY";
		break;
	case WEDNESDAY: returnStr = "WEDNESDAY";
		break;
	case THURSDAY: returnStr = "THURSDAY";
		break;
	case FRIDAY: returnStr = "FRIDAY";
		break;
	case SATURDAY: returnStr = "SATURDAY";
		break;
	case SUNDAY: returnStr = "SUNDAY";
		break;
	default: returnStr = "UNKNOWN";
	}

	return returnStr;
}
