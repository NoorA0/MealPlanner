#include "../headers/MealManager.hpp"
#include <fstream>

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
		uim->centeredText("Enter a name.");
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
				uim->leftAllignedText("The name you entered already exists. (case insensitive)");
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
	uim->centeredText("(1 day is typical, Meals prepared in large batches may last longer)");
	uim->prompt_FreeInt(1, 30);
	tempInt = std::stoi(uim->display());

	mealptr->setMealDuration(tempInt);

	// daysBetweenOccurrences
	tempStr = "New Meal: \"" + mealptr->getName() + "\"";
	uim->centeredText(tempStr);
	uim->centeredText("Days Between Occurrences");
	uim->skipLines(2);
	uim->centeredText("When this Meal is scheduled, how many days should pass before it can be scheduled again?");
	uim->centeredText("(value of 0 will not delay scheduling, 1 will make it occur every other day)");
	uim->prompt_FreeInt(0, 30);
	tempInt = std::stoi(uim->display());

	mealptr->setDaysBetweenOccurrences(tempInt);

	// enable meal
	mealptr->setDisabled(false);

	// assign tags to meal
	editMealTags(mealptr);
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
		uim->leftAllignedText("Examples:"); 
		uim->skipLines(1);
		uim->leftAllignedText("Chicken");
		uim->leftAllignedText("Vegetarian");
		uim->leftAllignedText("Takes long to cook");
		uim->leftAllignedText("Dessert");
		uim->leftAllignedText("Weekends Only");
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
				// set name and place at back of normalTags
				tagPtr->setName(tempStr);
				normalTags.push_back(tagPtr);
				doneCreatingName = true;
			}
			else if (!inputValid) // invalid name, inform user
			{
				uim->centeredText("Error: Invalid Name");
				uim->skipLines(2);
				uim->leftAllignedText("The name you entered already exists. (case insensitive)");
				uim->display();
			}
		}
		else // no tags to compare so push to back of meals
		{
			tagPtr->setName(tempStr);
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
	uim->prompt_FreeInt(0, 1000);
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
	tagPtr->setEnabledDays(enabledDays);
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
				uim->leftAllignedText("The name you entered already exists. (case insensitive)");
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
}

void MealManager::editMealTags(Meal* mealPtr)
{
	std::string tempStr = "";
	int tempInt;
	std::vector<std::string> strVec;

	// if meal has no tags, ask if user wants to assign some
	if (mealPtr->getTags().size() == 0)
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

		// bypass display tags
		if (tempStr != "Y")
			tempStr = "Q";
	}

	// if no tags exist and user wants to assign them, then ask to create one
	if (normalTags.size() == 0 && tempStr != "Q")
	{
		// ui title
		uim->centeredText("Assign Tags");
		uim->skipLines(2);

		uim->centeredText("There are no saved Tags, do you want to create one?");

		strVec = { "YYes", "NNo" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// bypass editing tags
		if (tempStr != "Y")
			tempStr = "Q";
		else
		{
			// create a tag
			Tag* tagPtr = new Tag;
			createTag(tagPtr);

			// add tag to normalTags, link to Meal, and link Meal to Tagl
			normalTags.push_back(tagPtr);
			mealPtr->addTag(tagPtr);
			tagPtr->addMeal(mealPtr);
			tagPtr = nullptr;
		}
	}
	else if (tempStr != "Q") // display tags and get input if user wants to assign tags
	{
		// prompt for tag, or quit
		do
		{
			int lastPageVisited = -1;
			tempInt = displayTags(lastPageVisited);

			// if user did not choose to quit
			if (tempInt != -1)
			{
				// check if tag already added
				std::vector<Tag*> mealTags = mealPtr->getTags();
				Tag* compare = normalTags.at(tempInt);

				bool isValid = true;
				auto mealTagsIter = mealTags.begin();

				while (isValid && mealTagsIter != mealTags.end())
				{
					// if match then tag already exists in meal
					if (compare == *mealTagsIter)
						isValid = false;
					++mealTagsIter;
				}

				if (!isValid)
				{
					// tell user the tag exists
					uim->centeredText("Error");
					uim->skipLines(2);
					tempStr = "\"" + compare->getName() + "\" is already assigned to \"" + mealPtr->getName() + "\".";
					uim->leftAllignedText(tempStr);
					uim->display();
				}
				else
				{
					// add tag to meal and meal to tag
					mealPtr->addTag(compare);
					compare->addMeal(mealPtr);

					// display confirmation
					uim->centeredText("Success!");
					uim->skipLines(2);
					tempStr = "\"" + compare->getName() + "\" assigned to \"" + mealPtr->getName() + "\".";
					uim->centeredText(tempStr);
					uim->display();
				}
			}
			else // user chose to quit
			{
				tempStr = "Q";
			}
		} while (tempStr != "Q");
		
	}// else if (tempStr != "Q")
}

void MealManager::editMultiTagTags(MultiTag* mtagPtr)
{
	std::string tempStr = "";
	int tempInt;
	std::vector<std::string> strVec;

	// if multitag has no tags, ask if user wants to assign some
	if (mtagPtr->getLinkedTags().size() == 0)
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

		// bypass display tags
		if (tempStr != "Y")
			tempStr = "Q";
	}

	// if no tags exist and user wants to assign them, then ask to create one
	if (normalTags.size() == 0 && tempStr != "Q")
	{
		// ui title
		uim->centeredText("Link Tags");
		uim->skipLines(2);

		uim->centeredText("There are no saved Tags, do you want to create one?");

		strVec = { "YYes", "NNo" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// bypass editing tags
		if (tempStr != "Y")
			tempStr = "Q";
		else
		{
			// create a tag
			Tag* tagPtr = new Tag;
			createTag(tagPtr);

			// add tag to saved tags
			normalTags.push_back(tagPtr);

			// prompt for number of Meals with this tag
			uim->centeredText("Link Tags");
			uim->skipLines(2);
			uim->centeredText("How many unique Meals from this Tag do you want to eat in one day?");
			uim->prompt_FreeInt(0, 100);
			tempInt = std::stoi(uim->display());

			// link Tag to MultiTag
			mtagPtr->addLinkedTag(tagPtr, tempInt);

			tagPtr = nullptr;
		}
	}
	else if (tempStr != "Q") // display tags and get input if user wants to assign tags
	{
		// prompt for tag, or quit
		do
		{
			int lastPageVisited = -1;
			tempInt = displayTags(lastPageVisited);

			// if user did not choose to quit
			if (tempInt != -1)
			{
				// check if tag already added
				std::map<Tag*, unsigned int> linkedTags = mtagPtr->getLinkedTags();
				Tag* compare = normalTags.at(tempInt);

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
					uim->prompt_FreeInt(0, 100);
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

	}// else if (tempStr != "Q")
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
		strVec = { "1Edit Name", "2Edit Description", "3Set Enabled Days",
					"4Set priority level", "5View/Edit Linked Tags", "QQuit Selection" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr != "Q")
		{
			// get choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1: // edit name
				tempStr = "Editing \"" + mtagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->centeredText("Name");
				uim->skipLines(2);

				// prompt and get input
				uim->leftAllignedText("Enter a new name: ");
				uim->prompt_FreeString(1, NAME_LENGTH);

				tempStr = uim->display();

				//set new name
				mtagPtr->setName(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Name changed to " + mtagPtr->getName();
				uim->leftAllignedText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			case 2: // edit description
				tempStr = "Editing \"" + mtagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->centeredText("Description");
				uim->skipLines(2);

				// display original description
				tempStr = "Original description: " + mtagPtr->getDescription();
				uim->leftAllignedText(tempStr);
				uim->skipLines(1);

				// prompt and get input
				uim->leftAllignedText("Enter a new description: ");
				uim->centeredText("(Press <enter> to skip)");
				uim->prompt_FreeString(0, DESC_LENGTH);

				tempStr = uim->display();

				//set new description
				mtagPtr->setDescription(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Description changed to " + mtagPtr->getDescription();
				uim->leftAllignedText(tempStr);
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
					tempStr = "Editing " + mtagPtr->getName();
					uim->centeredText(tempStr);
					uim->centeredText("Enable/Disable Days");
					uim->skipLines(2);

					uim->centeredText("Choose the days that \"" + mtagPtr->getName() + "\" should be active on:");
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
				uim->centeredText("Priority Level");
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
					tempStr += "give this MultiTag priority over Tags?";
				else
					tempStr += "lower priority to be the same as Tags?";

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
					tempStr += "SAME AS TAGS.";
				else
					tempStr += "PRIORITY OVER TAGS.";

				uim->leftAllignedText(tempStr);
				uim->prompt_None();
				uim->display();
			}
			break;
			case 5: // view/edit linked tags
				editMultiTagTags(mtagPtr);
				tempStr = ""; // make sure menu will loop
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
		strVec = { "1Edit Name", "2Edit Price", "3Edit Duration", "4Edit Days Between Occurrences", "5Enable/Disable Meal", "6Edit Tags", "QQuit Selection" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr != "Q")
		{
			// get choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1: // name
				tempStr = "Editing \"" + mealPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->centeredText("Name");
				uim->skipLines(2);

				// prompt and get input
				uim->leftAllignedText("Enter a new name: ");
				uim->prompt_FreeString(1, NAME_LENGTH);

				tempStr = uim->display();

				//set new name
				mealPtr->setName(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Name changed to " + mealPtr->getName();
				uim->leftAllignedText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			case 2: // price
				tempStr = "Editing " + mealPtr->getName();
				uim->centeredText(tempStr);
				uim->centeredText("Price");
				uim->skipLines(2);

				// display original price
				tempStr = "Original price: " + formatPrice(mealPtr->getPrice());

				uim->leftAllignedText(tempStr);
				uim->skipLines(1);

				// prompt and get input
				uim->leftAllignedText("Enter a new price: ");
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
				uim->centeredText("Duration");
				uim->skipLines(2);
				
				tempStr = "\"" + mealPtr->getName() + "\" has a duration of " + std::to_string(mealDuration) + " days.";
				uim->centeredText(tempStr);
				uim->centeredText("Enter a new Duration:");
				uim->prompt_FreeInt(1, 30);
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
				uim->centeredText("Days Between Occurrences");
				uim->skipLines(2);

				tempStr = "When this Meal is scheduled, it must wait " + std::to_string(occurrences) + " days before it can occur again.";
				uim->centeredText(tempStr);
				uim->centeredText("Enter a new delay time:");
				uim->prompt_FreeInt(0, 30);
				tempInt = std::stoi(uim->display());

				mealPtr->setMealDuration(tempInt);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Delay time changed to: " + std::to_string(mealPtr->getDaysBetweenOccurrences()) + " days.";

				uim->centeredText(tempStr);
				uim->display();
			}
				break;
			case 5: // enable/disable
			{
				// toggle state
				if (mealPtr->isDisabled())
					mealPtr->setDisabled(false);
				else
					mealPtr->setDisabled(true);
			}
			break;
			case 6: // edit tags
				editMealTags(mealPtr);
				tempStr = ""; // make sure menu will loop
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
		strVec = { "1Edit Name", "2Edit Description", "3Set Enabled Days", 
					"4Set Dependency on MultiTags", "5Set Consecutive Occurrences", "QQuit Selection" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr != "Q")
		{
			// get choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1: // edit name
				tempStr = "Editing \"" + tagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->centeredText("Name");
				uim->skipLines(2);

				// prompt and get input
				uim->leftAllignedText("Enter a new name: ");
				uim->prompt_FreeString(1, NAME_LENGTH);

				tempStr = uim->display();

				//set new name
				tagPtr->setName(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Name changed to " + tagPtr->getName();
				uim->leftAllignedText(tempStr);
				uim->prompt_None();
				uim->display();
				break;
			case 2: // edit description
				tempStr = "Editing \"" + tagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->centeredText("Description");
				uim->skipLines(2);

				// display original description
				tempStr = "Original description: " + tagPtr->getDescription();
				uim->leftAllignedText(tempStr);
				uim->skipLines(1);

				// prompt and get input
				uim->centeredText("Enter a new description: ");
				uim->centeredText("(Press <enter> to skip)");
				uim->prompt_FreeString(0, DESC_LENGTH);

				tempStr = uim->display();

				//set new description
				tagPtr->setDescription(tempStr);

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Description changed to " + tagPtr->getDescription();
				uim->leftAllignedText(tempStr);
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
					uim->centeredText("Enable/Disable Days");
					uim->skipLines(2);

					uim->centeredText("Choose days that Meals assigned to this Tag may occur on:");
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
				uim->centeredText("Dependency");
				uim->skipLines(2);

				// report status
				if (dependsOnMultitag)
					tempStr = "This Tag is only active when linked with a MuliTag.";
				else
					tempStr = "This Tag does not rely on a MultiTag (default behavior).";

				uim->centeredText(tempStr);

				// prompt user
				tempStr = "Do you want to ";

				if (dependsOnMultitag)
					tempStr += "use this Tag without being linked to a MultiTag? (default behavior)";
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

				tempStr = "Dependency changed to: ";

				if (tagPtr->getDependency())
					tempStr += "DEPENDS ON MULTITAG.";
				else
					tempStr += "DOES NOT REQUIRE MULTITAG.";

				uim->leftAllignedText(tempStr);
				uim->prompt_None();
				uim->display();
			}
				break;
			case 5: // set consecutive occurrences
				tempStr = "Editing \"" + tagPtr->getName() + "\"";
				uim->centeredText(tempStr);
				uim->centeredText("Consecutive Occurrences");
				uim->skipLines(2);

				// display original value
				tempStr = "Original value: " + std::to_string(tagPtr->getConsecutiveLimit())
					 + " consecutive occurrences allowed.";
				uim->leftAllignedText(tempStr);
				uim->skipLines(1);

				// prompt and get input
				uim->leftAllignedText("How many consecutive days can a Meal with this Tag occur for?");
				uim->prompt_FreeInt(0, 1000);

				// set new value
				tagPtr->setConsecutiveLimit(std::stoi(uim->display()));

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);

				tempStr = "Consecutive occurrences set to " + std::to_string(tagPtr->getConsecutiveLimit());
				uim->leftAllignedText(tempStr);
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
	std::map<DaysOfTheWeek, std::vector<Tag*>>& normalPriorityTags, 
	std::map<Tag*, std::vector<Meal*>>& availableMeals)
{
	DaysOfTheWeek day = MONDAY; // adds elements to function's params for each day of the week

	// ensure params are clear
	highPriorityMultiTags.clear();
	normalPriorityMultiTags.clear();
	normalPriorityTags.clear();
	availableMeals.clear();

	// populates all but availableMeals according to each day
	do
	{
		// vectors to add to params
		std::vector<MultiTag*> highPriorityMT;
		std::vector<MultiTag*> normalPriorityMT;
		std::vector<Tag*> normalPriorityT;

		// check all multiTags
		for (auto multiTagIter : multiTags)
		{
			// check if enabled for current day
			if (multiTagIter->getEnabledDays().at(day))
			{
				// check if multiTagsIter is disabled by its linked tags
				bool isDisabled = true;
				std::map<Tag*, unsigned int> linkedTags = multiTagIter->getLinkedTags();
				auto linkedTagIter = linkedTags.begin();

				while (isDisabled && linkedTagIter != linkedTags.end())
				{
					// if linked tag is enabled, then quit loop
					if (linkedTagIter->first->getEnabledDays().at(day))
						isDisabled = false;

					++linkedTagIter;
				}

				// if multitag is still enabled
				if (!isDisabled)
				{
					// check if multiTag is high or normal priority
					if (multiTagIter->getPriority())
						highPriorityMT.push_back(multiTagIter);
					else
						normalPriorityMT.push_back(multiTagIter);
				}
			}
		}

		// checks all tags and their meals
		for (auto tagIter : normalTags)
		{
			std::vector<Meal*> enabledMeals; // meals linked to this tag that are not disabled

			// check if enabled for current day
			if (tagIter->getEnabledDays().at(day))
			{
				// if tag depends on MultiTag, then exclude from normalPriorityTags
				if (!tagIter->getDependency())
				{
					// add to vector
					normalPriorityT.push_back(tagIter);
				}

				// regardless of dependency, check if meals are enabled
				std::vector<Meal*> linkedMeals = tagIter->getLinkedMeals();
				for (auto mealIter : linkedMeals)
				{
					// if enabled, add to enabledMeals
					if (!mealIter->isDisabled())
						enabledMeals.push_back(mealIter);
				}
			}
			// add element to availableMeals
			availableMeals.emplace(tagIter, enabledMeals);
		}

		// create elemets in main params
		highPriorityMultiTags.emplace(day, highPriorityMT);
		normalPriorityMultiTags.emplace(day, normalPriorityMT);
		normalPriorityTags.emplace(day, normalPriorityT);

		day = nextDay(day); // go to next day
	} while (day != MONDAY);
}

int MealManager::displayMeals(int& lastPageVisited)
{
	const int MEALS_PER_PAGE = 5; 
	std::string tempStr = "";
	std::vector<std::string> strVec;
	int tempInt = -1;

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
				uim->skipLines(2);
				uim->centeredText("Select a Meal to View:");

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

				// check if choice is N, P, or Q
				if (tempStr == "N")
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
				uim->centeredText("Viwing Meals");
				uim->skipLines(2);
				uim->centeredText("Select a Meal to View:");

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

				tempStr = "QQuit Selection";
				strVec.push_back(tempStr);

				uim->prompt_List_Case_Insensitive(strVec);
				tempStr = uim->display();
				tempStr = std::toupper(tempStr.at(0));

				// user chose a meal to edit
				if (tempStr != "Q")
				{
					// get index of meal
					tempInt = std::stoi(tempStr);
					--tempInt; // user's choices started at 1

					// exit loop
					tempStr = "Q";
				}
			}// while (tempStr != "Q")
		} // else display all meals
	}
	else // display error
	{
		uim->centeredText("Error");
		uim->skipLines(2);
		uim->centeredText("You have no Meals to display.");
		uim->prompt_None();
		uim->display();
	}
	
	return tempInt;
}

int MealManager::displayTags(int& lastPageVisited)
{
	const int TAGS_PER_PAGE = 5;
	std::string tempStr = "";
	std::vector<std::string> strVec;
	int tempInt = -1;

	// check if tags exist
	if (normalTags.size() > 0)
	{
		if (normalTags.size() <= TAGS_PER_PAGE) // can print all tags in 1 page
		{
			uim->centeredText("Viewing Tags");
			uim->skipLines(2);
			uim->centeredText("Select a Tag to View:");
			uim->skipLines(1);

			// display tags with description and create prompt
			int count = 1;
			for (auto tagIter : normalTags)
			{
				displayTagInfo(tagIter);
				uim->skipLines(1);

				tempStr = std::to_string(count) + tagIter->getName();
				strVec.push_back(tempStr);
				++count;
			}

			// add quit option
			tempStr = "";
			strVec.push_back(tempStr);

			tempStr = "QQuit Selection";
			strVec.push_back(tempStr);

			// prompt and get input
			uim->prompt_List_Case_Insensitive(strVec);
			tempStr = uim->display();
			tempStr = std::toupper(tempStr.at(0));

			// if user did not quit
			if (tempStr != "Q")
			{
				// get choice
				tempInt = std::stoi(tempStr);

				--tempInt; // user's choices start at 1
			}
		}
		else // print tags in pages
		{
			tempStr = "";

			int currentPage = 1;
			int totalPages = normalTags.size() / TAGS_PER_PAGE; // remainder may be present

			// if remainder, then add an extra page
			if (normalTags.size() % TAGS_PER_PAGE > 0)
				++totalPages;

			// is lastPageVisited is valid, then replace currentPage with lastPageVisited
			if (lastPageVisited >= 1 && lastPageVisited <= totalPages)
				currentPage = lastPageVisited;

			// loop while user has not quit menu
			while (tempStr != "Q")
			{
				uim->centeredText("Viewing Tags");
				// display "Page x/y"
				tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
				uim->centeredText(tempStr);
				uim->skipLines(2);
				uim->centeredText("Select a Tag to View:");

				// start index for the current page
				int startIndex = (TAGS_PER_PAGE * (currentPage - 1));

				//if displaying a page that is NOT last page, then display TAGS_PER_PAGE items
				if (currentPage < totalPages)
				{
					// create iterator at index's position
					auto tagIter = normalTags.begin() + startIndex;

					// display tag with description and create prompt
					for (int count = 1; count <= TAGS_PER_PAGE; ++count)
					{
						displayTagInfo(*tagIter);
						/*uim->centeredText((*tagIter)->getName());
						uim->leftAllignedText((*tagIter)->getDescription());*/
						uim->skipLines(1);

						tempStr = std::to_string(count) + (*tagIter)->getName();
						strVec.push_back(tempStr);

						++tagIter;
					}
				}
				else // the current page is the last page, so there may be less than TAGS_PER_PAGE elements to display
				{
					// create iterator at index's position
					auto tagIter = normalTags.begin() + startIndex;
					int count = 1; // used to print prompt number

					// create prompts for the rest of the elements
					do
					{
						displayTagInfo(*tagIter);
						/*uim->centeredText((*tagIter)->getName());
						uim->leftAllignedText((*tagIter)->getDescription());*/
						uim->skipLines(1);

						tempStr = std::to_string(count) + (*tagIter)->getName();
						strVec.push_back(tempStr);

						++tagIter;
						++count;
					} while (tagIter != normalTags.end());
				}

				// add prompts to go to next page, previous, and quit
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

				// check if choice is N, P, or Q
				if (tempStr == "N")
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
					--tempInt; // user's prompts started at 1
					tempInt += startIndex;

					// end loop
					tempStr = "Q";
					lastPageVisited = currentPage;
				}
			} // while (tempStr != "Q")
		} // else print tag in pages
	} // if more than 0 tags
	else // display error 
	{
		uim->centeredText("Error");
		uim->skipLines(2);
		uim->centeredText("No Tags to display.");
		uim->display();
	}
	return tempInt;
}

int MealManager::displayMultiTags(int& lastPageVisited)
{
	const int MULTITAGS_PER_PAGE = 5;
	std::string tempStr = "";
	std::vector<std::string> strVec;
	int tempInt = -1;

	// check if multitags exist
	if (multiTags.size() > 0)
	{
		// can print all multitags in 1 page
		if (multiTags.size() <= MULTITAGS_PER_PAGE)
		{
			uim->centeredText("Viewing MultiTags");
			uim->skipLines(2);
			uim->centeredText("Select a MultiTag to View:");
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

			// add quit option
			tempStr = "";
			strVec.push_back(tempStr);

			tempStr = "QQuit Selection";
			strVec.push_back(tempStr);

			// prompt and get input
			uim->prompt_List_Case_Insensitive(strVec);
			tempStr = uim->display();
			tempStr = std::toupper(tempStr.at(0));

			// if user did not quit
			if (tempStr != "Q")
			{
				// get choice
				tempInt = std::stoi(tempStr);
				--tempInt; // choices start at 1
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
				uim->skipLines(2);
				uim->centeredText("Select a MultiTag to View:");

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

				// check if choice is N, P, or Q
				if (tempStr == "N")
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

					// end loop
					tempStr = "Q";
					lastPageVisited = currentPage;
				}
			} // while (tempStr != "Q")
		} // else print tag in pages
	} // if more than 0 tags
	else // display error 
	{
		uim->centeredText("Error");
		uim->skipLines(2);
		uim->centeredText("No MultiTags to display.");
		uim->display();
	}
	return tempInt;
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

	// tags
	tempStr = std::to_string(mealPtr->getTags().size()) + " Tags assigned.";
	uim->leftAllignedText(tempStr);

	// enabledDays
	if (mealPtr->isDisabled())
		uim->leftAllignedText("Is disabled.");
	else
	{
		tempStr = "Enabled on: " + formatEnabledDays(mealPtr->getEnabledDays());

		// check if disabled by tags
		if (tempStr == "Enabled on: []")
			uim->leftAllignedText("Is disabled by Tags.");
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
	tempStr = "Description: " + tagPtr->getDescription();
	uim->leftAllignedText(tempStr);

	// consecutiveLimit
	tempStr = "Consecutive Meals allowed: " + std::to_string(tagPtr->getConsecutiveLimit());
	uim->leftAllignedText(tempStr);

	// hasPriority
	tempStr = "Depends on MultiTag: ";

	if (tagPtr->getDependency())
		tempStr += "YES";
	else
		tempStr += "NO";
	uim->leftAllignedText(tempStr);

	// enabledDays
	tempStr = "Enabled on: " + formatEnabledDays(tagPtr->getEnabledDays());

	// check if disabled by tags
	if (tempStr == "Enabled on: []")
		uim->leftAllignedText("Tag disabled all days.");
	else
		uim->leftAllignedText(tempStr);
}

void MealManager::displayMultiTagInfo(const MultiTag* mtagPtr)
{
	std::string tempStr = "";

	// name
	tempStr = "Name: " + mtagPtr->getName();
	uim->leftAllignedText(tempStr);

	// description
	tempStr = "Description: " + mtagPtr->getDescription();
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

bool MealManager::readMeals(std::ifstream& iFile)
{
	bool error = false; // true when encountering a read error
	bool done = false; // when read all meals
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
				// set price
				mealPtr->setPrice(std::stod(tempStr));

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
				// set value
				mealPtr->setDisabled(std::stoi(tempStr));

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
				// set value
				mealPtr->setMealDuration(std::stoi(tempStr));

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
				// set value
				mealPtr->setDaysBetweenOccurrences(std::stoi(tempStr));

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

	return error;
}

bool MealManager::readTags(std::ifstream& iFile)
{
	bool error = false; // true when encountering a read error
	bool done = false; // when read all tags
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
			// set name
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
				// set description
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
				// set value
				tagPtr->setDependency(std::stoi(tempStr));

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
				// set value
				tagPtr->setConsecutiveLimit(std::stoi(tempStr));

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

	return error;
}

bool MealManager::readMultiTags(std::ifstream& iFile)
{
	bool error = false; // true when encountering a read error
	bool done = false; // when read all tags

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
			// set name
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
				// set description
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
				// set value
				mtagPtr->setHighestPriority(std::stoi(tempStr));

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

	return error;
}

void MealManager::generateSchedule(const std::string& fileName, std::ofstream& oFile)
{
	const unsigned int MIN_CALCULATION_LENGTH = 1; // 1 week
	const unsigned int MAX_CALCULATION_LENGTH = 52; // just under a year (in weeks)
	const unsigned int MIN_BUDGET = 0;
	const unsigned int MAX_BUDGET = 100000;

	double budget = 0; // budget for the calculated period
	unsigned int calculationPeriod = 0; // period of calculation
	unsigned int currentDayNum = 0; // current day's number
	DaysOfTheWeek currentDay = MONDAY; // start of the week
	std::string tempStr = "";
	std::vector<std::string> strVec;

	std::map<DaysOfTheWeek, std::vector<MultiTag*>> highPriorityMultiTags; // multiTags with elevated priority, sorted by available day
	std::map<DaysOfTheWeek, std::vector<MultiTag*>> normalPriorityMultiTags; // multiTags with priority of Tag, sorted by available day
	std::map<DaysOfTheWeek, std::vector<Tag*>> normalPriorityTags; // Tags, sorted by available day
	std::map<Tag*, std::vector<Meal*>> availableMeals; // links tag to its meals

	// loop until user is ok with settings
	while (tempStr != "Q")
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
		uim->leftAllignedText(tempStr);
		
		tempStr = "SCHEDULE LENGTH: " + std::to_string(calculationPeriod) + " weeks";
		uim->leftAllignedText(tempStr);

		strVec = { "YYes", "NNo" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// exit loop if user accepts settings
		if (tempStr == "Y")
			tempStr = "Q";
	}

	// convert weeks into days
	calculationPeriod *= 7;

	// OPTIMIZATION
	std::cout << "\n\nBeginning Optimizations ...";
	optimizeData(highPriorityMultiTags, normalPriorityMultiTags, normalPriorityTags, availableMeals);
	std::cout << "\nDone!\n\n";

	std::cout << "size of HPMT: " << highPriorityMultiTags.size()
		<< "\nsize of NPMT: " << normalPriorityMultiTags.size()
		<< "\nsize of NPT: " << normalPriorityTags.size()
		<< "\nsize of AM: " << availableMeals.size();


}

void MealManager::mealEditor()
{
	std::string tempStr;
	int tempInt = 0;
	std::vector<std::string> strVec;
	Meal* mealPtr = nullptr;

	while (tempInt != 4)
	{
		uim->centeredText("Meal Menu");
		uim->skipLines(2);

		tempStr = "You have " + std::to_string(meals.size()) + " meals saved.";
		uim->centeredText(tempStr);

		strVec = { "1View/Edit Meals", "2Create Meal", "3Delete Meal", "QQuit to Main Menu" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// check if user wants to quit
		if (tempStr == "Q")
			tempInt = 4;
		else
			tempInt = std::stoi(tempStr);

		switch (tempInt)
		{
		case 1: // list all meals and allow editing
		{
			int lastPageVisted = -1;

			// display meals and get user's choice, or quit
			do
			{
				tempInt = displayMeals(lastPageVisted);

				// if user did not quit
				if (tempInt != -1)
				{
					// get meal and call editor
					Meal* mealPtr = meals.at(tempInt);
					editMeal(mealPtr);
				}
				else // force quit
					tempStr = "Q";

			} while (tempStr != "Q");
		}
			break;
		case 2: // create a new meal
			mealPtr = new Meal;

			createMeal(mealPtr);

			mealPtr = nullptr;

			// confirm to user
			uim->centeredText("Success!");
			uim->skipLines(2);
			uim->centeredText("Meal creation successful!");
			uim->display();
			break;
		case 3: // delete meals
			// display meals and get user's choice, or quit
			do
			{
				int lastPageVisited = -1; // start at first page
				tempInt = displayMeals(lastPageVisited);

				// if user did not quit
				if (tempInt != -1)
				{
					// get meal
					mealPtr = meals.at(tempInt);

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
						auto mealsIter = meals.begin();
						bool found = false;

						// get iterator to same element as mealPtr
						while (!found && mealsIter != meals.end())
						{
							// if names match, then found
							if ((*mealsIter)->getName() == mealPtr->getName())
								found = true;
							else
								++mealsIter;
						}

						// remove references to Tags
						std::vector<Tag*> emptyvec = { nullptr };
						mealPtr->setTags(emptyvec);

						// delete meal
						delete mealPtr;
						meals.erase(mealsIter);
						mealPtr = nullptr;
						
						// confirm to user
						uim->centeredText("Success!");
						uim->skipLines(2);
						uim->centeredText("Meal deleted!");
						uim->display();

						// exit loop
						tempStr = "Q";
					}
				}
				else // force quit
					tempStr = "Q";

			} while (tempStr != "Q");
			break;
		default:
			tempInt = 4;
		}
	}
}

void MealManager::tagEditor()
{
	std::string tempStr = "";
	int tempInt = 0;
	std::vector<std::string> strVec;
	Tag* tagPtr = nullptr;
	MultiTag* mtagPtr = nullptr;


	// while user has not quit menu
	while (tempInt != 10)
	{
		uim->centeredText("Tag Menu");
		uim->skipLines(2);

		tempStr = "You have " + std::to_string(normalTags.size()) + " normal Tags and "
			+ std::to_string(multiTags.size()) + " MultiTags saved.";
		uim->centeredText(tempStr);
		uim->skipLines(2);

		// prompt menu
		strVec = { "1View/Edit Tags", "2View/Edit MultiTags","3Create a Tag", "4Delete a Tag", "QQuit to Main Menu" };
		uim->prompt_List_Case_Insensitive(strVec);

		tempStr = uim->display();
		tempStr = std::toupper(tempStr.at(0));

		// check if user wants to quit
		if (tempStr == "Q")
			tempInt = 10;
		else
			tempInt = std::stoi(tempStr);

		switch (tempInt)
		{
		case 1: // view/edit tags
		{
			int lastPageVisited = -1;
			//display tags and get user's choice or quit
			do
			{
				tempInt = displayTags(lastPageVisited);

				// if usser did not quit
				if (tempInt != -1)
				{
					// get tag and call editor
					tagPtr = normalTags.at(tempInt);
					editTag(tagPtr);
				}

			} while (tempInt != -1);
		}
			break;
		case 2: // view/edit multitags
		{
			int lastPageVisited = -1;
			//display multitags and get user's choice or quit
			do
			{
				tempInt = displayMultiTags(lastPageVisited);

				// if user did not quit
				if (tempInt != -1)
				{
					// get tag and call editor
					mtagPtr = multiTags.at(tempInt);
					editMultiTag(mtagPtr);
				}

			} while (tempInt != -1);
		}
			break;
		case 3: // create tag
			uim->centeredText("Select a type of Tag to create:");
			
			strVec = { "1Tag", "2MultiTag" };
			uim->prompt_List_Case_Insensitive(strVec);

			tempStr = uim->display();
			
			// if user chose to create a Tag
			if (tempStr == "1")
			{
				tagPtr = new Tag;

				createTag(tagPtr);

				tagPtr = nullptr;

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);
				uim->centeredText("Tag creation successful!");
				uim->display();
			}
			else // user wants to create a MultiTag
			{
				mtagPtr = new MultiTag;

				createMultiTag(mtagPtr);

				mtagPtr = nullptr;

				// confirm to user
				uim->centeredText("Success!");
				uim->skipLines(2);
				uim->centeredText("MultiTag creation successful!");
				uim->display();
			}
			break;
		case 4: // delete tag
			uim->centeredText("Select a type of Tag to delete:");

			strVec = { "1Tag", "2MultiTag" };
			uim->prompt_List_Case_Insensitive(strVec);

			tempStr = uim->display();

			// user wants to delete a Tag
			if (tempStr == "1")
			{
				// display tags and get user's choice, or quit
				do
				{
					int lastPageVisited = -1;
					tempInt = displayTags(lastPageVisited);

					// if user did not quit
					if (tempInt != -1)
					{
						// get tag
						tagPtr = normalTags.at(tempInt);

						// confirm deletion
						uim->centeredText("Confirm Deletion");
						uim->skipLines(2);
						uim->centeredText("Are you sure you want to delete this Tag?");
						uim->skipLines(1);

						// display meal info
						displayTagInfo(tagPtr);

						strVec = { "YYes", "NNo" };

						uim->prompt_List_Case_Insensitive(strVec);
						tempStr = uim->display();
						tempStr = std::toupper(tempStr.at(0));

						// check user's choice
						if (tempStr == "Y")
						{
							auto tagsIter = normalTags.begin();
							bool found = false;

							// get iterator to same element as tagsPtr
							while (!found && tagsIter != normalTags.end())
							{
								// if names match, then found
								if ((*tagsIter)->getName() == tagPtr->getName())
									found = true;
								else
									++tagsIter;
							}

							// delete Tag
							delete tagPtr;
							normalTags.erase(tagsIter);
							tagPtr = nullptr;

							// confirm to user
							uim->centeredText("Success!");
							uim->skipLines(2);
							uim->centeredText("Tag deleted!");
							uim->display();

							// exit loop
							tempStr = "Q";
						}
					}
					else // force quit
						tempStr = "Q";

				} while (tempStr != "Q");
			}
			else // user wants to delete a MultiTag
			{
				// display multitags and get user's choice, or quit
				do
				{
					int lastPageVisited = -1;
					tempInt = displayMultiTags(lastPageVisited);

					// if user did not quit
					if (tempInt != -1)
					{
						// get multitag
						mtagPtr = multiTags.at(tempInt);

						// confirm deletion
						uim->centeredText("Confirm Deletion");
						uim->skipLines(2);
						uim->centeredText("Are you sure you want to delete this MultiTag?");
						uim->skipLines(1);

						// display meal info
						displayMultiTagInfo(mtagPtr);

						strVec = { "YYes", "NNo" };

						uim->prompt_List_Case_Insensitive(strVec);
						tempStr = uim->display();
						tempStr = std::toupper(tempStr.at(0));

						// check user's choice
						if (tempStr == "Y")
						{
							auto tagsIter = multiTags.begin();
							bool found = false;

							// get iterator to same element as mtagPtr
							while (!found && tagsIter != multiTags.end())
							{
								// if names match, then found
								if ((*tagsIter)->getName() == mtagPtr->getName())
									found = true;
								else
									++tagsIter;
							}

							// remove all references to Tags
							std::map<Tag*, unsigned int> emptyVec = { {nullptr, 0} };
							mtagPtr->setLinkedTags(emptyVec);

							// delete MultiTag
							delete mtagPtr;
							multiTags.erase(tagsIter);
							mtagPtr = nullptr;

							// confirm to user
							uim->centeredText("Success!");
							uim->skipLines(2);
							uim->centeredText("MultiTag deleted!");
							uim->display();

							// exit loop
							tempStr = "Q";
						}
					}
					else // force quit
						tempStr = "Q";

				} while (tempStr != "Q");
			}
			break;
		default:
			tempInt = 10;
		}// switch (inputInt)
	}// while (tempInt != 10)
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

void MealManager::loadState(const std::string& outputFile, std::ifstream& iFile)
{
	std::string tagError = "Error reading Tags";
	std::string multiTagError = "Error reading MultiTags";
	std::string mealError = "Error reading Meals";
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
			// if tags imported with error
			if (readTags(iFile) != 0)
				throw tagError;
		}
		else throw tagError;

		// get blank space
		std::getline(iFile, line);
		// get multiTags header
		std::getline(iFile, line);

		// check if header is correct
		if (line == "<MultiTags>")
		{
			// if MultiTags imported with error
			if (readMultiTags(iFile) != 0)
				throw multiTagError;
		}
		else throw multiTagError;

		// get blank space
		std::getline(iFile, line);
		// get Meals header
		std::getline(iFile, line);

		// check if header is correct
		if (line == "<Meals>")
		{
			// if meals imported with error
			if (readMeals(iFile) != 0)
				throw mealError;
		}
		else throw mealError;

		iFile.close();
	}
}

DaysOfTheWeek nextDay(const DaysOfTheWeek& day)
{
	switch (day)
	{
	case MONDAY: return TUESDAY;
	case TUESDAY: return WEDNESDAY;
	case WEDNESDAY: return THURSDAY;
	case THURSDAY: return FRIDAY;
	case FRIDAY: return SATURDAY;
	case SATURDAY: return SUNDAY;
	case SUNDAY: return MONDAY;
	}
}

DaysOfTheWeek previousDay(const DaysOfTheWeek& day)
{
	switch (day)
	{
	case MONDAY: return SUNDAY;
	case TUESDAY: return MONDAY;
	case WEDNESDAY: return TUESDAY;
	case THURSDAY: return WEDNESDAY;
	case FRIDAY: return THURSDAY;
	case SATURDAY: return FRIDAY;
	case SUNDAY: return SATURDAY;
	}
}
