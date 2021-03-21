#include "../headers/MealManager.hpp"

MealManager::MealManager(const double& MINIMUM_PRICE, const double& MAXIMUM_PRICE, 
						 const unsigned int& NAME_LENGTH, const unsigned int& DESC_LENGTH)
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
}

MealManager::~MealManager()
{
	for (auto tagsIter : normalTags)
	{
		delete tagsIter;
	}

	for (auto tagsIter : globalTags)
	{
		delete tagsIter;
	}

	for (auto mealsIter : meals)
	{
		delete mealsIter;
	}
}

void MealManager::createMeal(Meal* mealptr, UIManager& uim)
{
	bool doneCreating = false;
	bool inputValid = false;
	std::string tempStr = "";
	int tempInt;
	double tempDouble;
	std::vector<std::string> strVec;

	// loop while user is not done choosing a name
	while (!doneCreating)
	{
		uim.centeredText("New Meal - Name");
		uim.skipLines(2);

		// prompt and get name
		uim.centeredText("Enter a name.");
		uim.prompt_FreeString(NAME_LENGTH);
		tempStr = uim.display();

		// if there are other meals stored, check if names conflict (case insensitive)
		if (meals.size() > 0)
		{
			// get uppercase version of tempStr
			std::string upperStr = tempStr;
			for (int i = 0; i < upperStr.size(); ++i)
			{
				char makeUpper = std::toupper(upperStr.at(i));
				upperStr.at(i) = makeUpper;
			}

			// compare with all names of all meals until a match is found
			inputValid = true;
			auto mealsIter = meals.begin();
			while (inputValid && mealsIter != meals.end())
			{
				// get uppercase version of meal's name
				std::string mealName = (*mealsIter)->getName();
				for (int i = 0; i < mealName.size(); ++i)
				{
					char makeUpper = std::toupper(mealName.at(i));
					mealName.at(i) = makeUpper;
				}

				// check if names match
				if (upperStr == mealName)
					inputValid = false;
				++mealsIter;
			}

			// check if name is valid
			if (inputValid)
			{
				// make new meal and set name
				mealptr = new Meal;
				mealptr->setName(tempStr);
				doneCreating = true; // exits section of creation
			}
			else // invalid name, inform user
			{
				uim.centeredText("Error: Invalid Name");
				uim.skipLines(2);
				uim.leftAllignedText("The name you entered already exists. (case insensitive)");
				uim.display();
			}
		}
		else // add meal
		{
			mealptr = new Meal;
			mealptr->setName(tempStr);
			doneCreating = true;
		}

	}// while (!doneCreating)

	uim.centeredText("New Meal - Price");
	uim.skipLines(2);

	// prompt and get price
	uim.centeredText("Enter a price:");
	uim.prompt_FreeDouble(MINIMUM_PRICE, MAXIMUM_PRICE);
	tempDouble = std::stod(uim.display());

	mealptr->setPrice(tempDouble);

	// assign tags to meal
	editMealTags(mealptr, uim);
}

void MealManager::createTag(Tag* tagPtr, UIManager& uim)
{
	std::string tempStr = "";
	int tempInt;
	std::vector<std::string> strVec;

	// tag's params
	std::string tagName = "";
	std::string tagDesc = "";
	bool hasPriority = false;
	unsigned int consecutiveLimit = 0;
	std::map<DaysOfTheWeek, bool> enabledDays =
	{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false}, {FRIDAY, false},
		{SATURDAY, false}, {SUNDAY, false} };

	// prompt and get name
	uim.centeredText("Create Tag - Name");
	uim.skipLines(2);

	uim.centeredText("Enter a name (recommended 2-3 words).");
	uim.prompt_FreeString(NAME_LENGTH);
	tagName = uim.display();

	// prompt and get description
	uim.centeredText("Create Tag - Description");
	uim.skipLines(2);

	uim.centeredText("Enter a description.");
	uim.prompt_FreeString(DESC_LENGTH);
	tagDesc = uim.display();

	// prompt and get priority
	uim.centeredText("Create Tag - Priority Setting");
	uim.skipLines(2);

	uim.centeredText("Can this Tag override global settings? (Y/N)");
	strVec = { "YYes", "NNo" };
	uim.prompt_List_Case_Insensitive(strVec);

	tempStr = uim.display();
	tempStr = std::toupper(tempStr.at(0));

	// set hasPriority depending on user choice
	if (tempStr == "Y")
		hasPriority = true;
	// already set to false by default

	// prompt and get consecutiveLimit
	uim.centeredText("Create Tag - Consecutive Days Limit");
	uim.skipLines(2);

	uim.centeredText("How many consecutive days can a Meal with this Tag occurr in?");
	uim.prompt_FreeInt(0, 1000);
	consecutiveLimit = std::stoi(uim.display());

	tempStr = "";
	// let user set enabled days or quit
	while (tempStr != "Q")
	{
		uim.centeredText("Create Tag - Enabled Days");
		uim.skipLines(2);

		uim.leftAllignedText("Choose days that Meals with this Tag may occurr on:");

		tempStr = "MONDAY: ";
		if (enabledDays.find(MONDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim.leftAllignedText(tempStr);

		tempStr = "TUESDAY: ";
		if (enabledDays.find(TUESDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim.leftAllignedText(tempStr);

		tempStr = "WEDNESDAY: ";
		if (enabledDays.find(WEDNESDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim.leftAllignedText(tempStr);

		tempStr = "THURSDAY: ";
		if (enabledDays.find(THURSDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim.leftAllignedText(tempStr);

		tempStr = "FRIDAY: ";
		if (enabledDays.find(FRIDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim.leftAllignedText(tempStr);

		tempStr = "SATURDAY: ";
		if (enabledDays.find(SATURDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim.leftAllignedText(tempStr);

		tempStr = "SUNDAY: ";
		if (enabledDays.find(SUNDAY)->second)
			tempStr += "enabled";
		else
			tempStr += "disabled";
		uim.leftAllignedText(tempStr);
		break;

		// print options
		strVec = { "1Toggle MONDAY", "2Toggle TUESDAY", "3Toggle WEDNESDAY", "4Toggle THURSDAY",
				  "5Toggle FRIDAY", "6Toggle SATURDAY", "7Toggle SUNDAY",
				  "EEnable ALL", "DDisable ALL", "QConfirm Selection" };

		// prompt and get input
		uim.prompt_List_Case_Insensitive(strVec);
		tempStr = uim.display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr == "E")
		{
			// set all days to TRUE
			std::map<DaysOfTheWeek, bool> enabledDays =
			{ {MONDAY, true}, {TUESDAY, true}, {WEDNESDAY, true}, {THURSDAY, true},
			{FRIDAY, true}, {SATURDAY, true}, {SUNDAY, true} };
		}
		else if (tempStr == "D")
		{
			// set all days to FALSE
			std::map<DaysOfTheWeek, bool> enabledDays =
			{ {MONDAY, false}, {TUESDAY, false}, {WEDNESDAY, false}, {THURSDAY, false},
			{FRIDAY, false}, {SATURDAY, false}, {SUNDAY, false} };
		}
		else if (tempStr != "Q")
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

	// create a tag with collected data
	tagPtr = new Tag(tagName, tagDesc, enabledDays, hasPriority, consecutiveLimit);
}

void MealManager::editMealTags(Meal* mealPtr, UIManager& uim)
{
	std::string tempStr = "";
	int tempInt;
	std::vector<std::string> strVec;

	// if meal has no tags, ask if user wants to assign some
	if (mealPtr->getTags().size() == 0)
	{
		// ui title
		uim.centeredText("Assign Tags");
		uim.skipLines(2);

		uim.centeredText("Meal has no Tags, do you want to assign some?");
		
		strVec = { "YYes", "NNo" };
		uim.prompt_List_Case_Insensitive(strVec);

		tempStr = uim.display();
		tempStr = std::toupper(tempStr.at(0));

		// bypass display tags
		if (tempStr != "Y")
			tempStr = "Q";
	}

	// while user not doing assigning tags
	while (tempStr != "Q")
	{
		// if no tags exist, ask to create one
		if (normalTags.size() == 0)
		{
			// ui title
			uim.centeredText("Assign Tags");
			uim.skipLines(2);

			uim.centeredText("There are no saved Tags, do you want to create one?");

			strVec = { "YYes", "NNo" };
			uim.prompt_List_Case_Insensitive(strVec);

			tempStr = uim.display();
			tempStr = std::toupper(tempStr.at(0));

			// bypass editing tags
			if (tempStr != "Y")
				tempStr = "Q";
			else 
			{
				// create a tag
				Tag* tagPtr = nullptr;
				createTag(tagPtr, uim);

				// add tag to saved tags and to meal
				normalTags.push_back(tagPtr);
				mealPtr->addTag(tagPtr);
			}
		}
		else if (normalTags.size() <= 10) // can print all tags in 1 page
		{
			uim.centeredText("Assign Tags");
			uim.skipLines(2);
			uim.leftAllignedText("Choose a Tag to assign to the Meal:");
			uim.skipLines(1);

			// display tag with description and create prompt
			int count = 0;
			for (auto tagIter : normalTags)
			{
				uim.centeredText(tagIter->getName());
				uim.leftAllignedText(tagIter->getDescription());
				uim.skipLines(1);

				tempStr = std::to_string(count) + tagIter->getName();
				strVec.push_back(tempStr);
				++count;
			}

			// add quit option
			tempStr = "QQuit Selection";
			strVec.push_back(tempStr);

			// prompt and get input
			uim.prompt_List_Case_Insensitive(strVec);
			tempStr = uim.display();
			tempStr = std::toupper(tempStr.at(0));

			// check if user chose to quit
			if (tempStr != "Q")
			{
				// get choice
				tempInt = std::stoi(tempStr);

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
					uim.centeredText("Error");
					uim.skipLines(2);
					uim.leftAllignedText("Tag is already assigned to Meal.");
					uim.display();
				}
				else
				{
					// add tag to meal
					mealPtr->addTag(compare);

					// display confirmation
					uim.centeredText("Success!");
					uim.skipLines(2);
					uim.leftAllignedText("Tag assigned to Meal.");
					uim.display();
				}
			}
		}
		else // print tags in pages
		{
			tempStr = "";

			int currentPage = 1;
			int totalPages = normalTags.size() / 10; // remainder may be present

			// if remainder, then add an extra page
			if (normalTags.size() % 10 > 0)
				++totalPages;

			// loop while user has not quit menu
			while (tempStr != "Q")
			{
				uim.centeredText("Assign Tags");
				// display "Page x/y"
				tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
				uim.centeredText(tempStr);
				uim.skipLines(2);
				uim.leftAllignedText("Choose a Tag to assign to the Meal:");

				// start index for the current page
				int startIndex = (10 * (currentPage - 1));

				//if displaying a page that is NOT last page, then display 10 items
				if (currentPage < totalPages)
				{
					// create iterator at index's position
					auto tagIter = normalTags.begin() + startIndex;

					// display tag with description and create prompt
					for (int count = 0; count < 10; ++count)
					{
						uim.centeredText((*tagIter)->getName());
						uim.leftAllignedText((*tagIter)->getDescription());
						uim.skipLines(1);

						tempStr = std::to_string(count) + (*tagIter)->getName();
						strVec.push_back(tempStr);
						++count;
					}
				}
				else // the current page is the last page, so there may be less than 10 elements to display
				{
					// create iterator at index's position
					auto tagIter = normalTags.begin() + startIndex;
					int count = 0; // used to print prompt number

					// create prompts for the rest of the elements
					while (tagIter != normalTags.end())
					{
						uim.centeredText((*tagIter)->getName());
						uim.leftAllignedText((*tagIter)->getDescription());
						uim.skipLines(1);

						tempStr = std::to_string(count) + (*tagIter)->getName();
						strVec.push_back(tempStr);
						++count;
					}
				}

				// add prompts to go to next page, previous, and quit
				tempStr = "NNext Page";
				strVec.push_back(tempStr);

				tempStr = "PPrevious Page";
				strVec.push_back(tempStr);

				tempStr = "QQuit Selection";
				strVec.push_back(tempStr);

				// display UI and get input
				uim.prompt_List_Case_Insensitive(strVec);
				tempStr = uim.display();
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
					tempInt += startIndex;

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
						uim.centeredText("Error");
						uim.skipLines(2);
						uim.leftAllignedText("Tag is already assigned to Meal.");
						uim.display();
					}
					else
					{
						// add tag to meal
						mealPtr->addTag(compare);

						// display confirmation
						uim.centeredText("Success!");
						uim.skipLines(2);
						uim.leftAllignedText("Tag assigned to Meal.");
						uim.display();
					}
				}
			} // while (tempStr != "Q")
		}
	} // while (tempStr != "Q")
}

void MealManager::editMeal(Meal* mealPtr, UIManager& uim)
{
	std::string tempStr = "";
	int tempInt;
	double tempDouble;
	std::vector<std::string> strVec;

	while (tempStr != "Q")
	{
		// header
		uim.centeredText("Meal Information");
		uim.skipLines(2);

		// display name
		tempStr = "Name: " + mealPtr->getName();
		uim.leftAllignedText(tempStr);

		uim.skipLines(1);

		// display price 
		tempStr = "Price: ";

		// remove excess 0's in price
		{
			std::string formatStr = std::to_string(mealPtr->getPrice());

			// erase trailing zeros, leaves '.' at end of string
			formatStr.erase(formatStr.find_last_not_of("0") + 1, std::string::npos);

			// erase '.' if exists
			if (formatStr[formatStr.length() - 1] == '.')
				formatStr.pop_back();

			tempStr += formatStr;
		}
		uim.leftAllignedText(tempStr);
		uim.skipLines(1);


		// check if meal is disabled 
		if (mealPtr->getIsDisabled())
			uim.leftAllignedText("Meal is disabled.");
		else
		{
			// get enabled days
			std::map<DaysOfTheWeek, bool> enabledDays = mealPtr->getEnabledDays();

			bool oneDayEnabled = false; // true when at least 1 day is enabled
			tempStr = "Enabled on: [";

			// check days that are disabled by tags
			if (enabledDays.at(MONDAY))
			{
				tempStr += "M";
				oneDayEnabled = true;
			}

			if (enabledDays.at(TUESDAY))
			{
				// add spacing
				if (oneDayEnabled)
					tempStr += ", ";
				else
					oneDayEnabled = true;

				tempStr += "T";
			}

			if (enabledDays.at(WEDNESDAY))
			{
				// add spacing
				if (oneDayEnabled)
					tempStr += ", ";
				else
					oneDayEnabled = true;

				tempStr += "W";
			}

			if (enabledDays.at(THURSDAY))
			{
				// add spacing
				if (oneDayEnabled)
					tempStr += ", ";
				else
					oneDayEnabled = true;

				tempStr += "Th";
			}

			if (enabledDays.at(FRIDAY))
			{
				// add spacing
				if (oneDayEnabled)
					tempStr += ", ";
				else
					oneDayEnabled = true;

				tempStr += "F";
			}

			if (enabledDays.at(SATURDAY))
			{
				// add spacing
				if (oneDayEnabled)
					tempStr += ", ";
				else
					oneDayEnabled = true;

				tempStr += "Sa";
			}

			if (enabledDays.at(SUNDAY))
			{
				// add spacing
				if (oneDayEnabled)
					tempStr += ", ";
				else
					oneDayEnabled = true;

				tempStr += "Su";
			}

			// check if no days were enabled
			if (tempStr == "Enabled on: [")
				uim.leftAllignedText("Is disabled by assigned Tags.");
			else // close bracket
				tempStr += "]";

			uim.leftAllignedText(tempStr);
		}

		// prompt user for attribute to edit or quit
		strVec = { "1Edit Name", "2Edit Price", "3Enable/Disable Meal", "4Edit Tags", "QQuit Selection" };
		uim.prompt_List_Case_Insensitive(strVec);

		tempStr = uim.display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr != "Q")
		{
			// get choice
			tempInt = std::stoi(tempStr);

			switch (tempInt)
			{
			case 1:
				uim.centeredText("Edit Name");
				uim.skipLines(2);
				
				// display original name
				tempStr = "Original name: " + mealPtr->getName();
				uim.leftAllignedText(tempStr);
				uim.skipLines(1);

				// prompt and get input
				uim.leftAllignedText("Enter a new name: ");
				uim.prompt_FreeString(NAME_LENGTH);
				
				tempStr = uim.display();

				//set new name
				mealPtr->setName(tempStr);

				// confirm to user
				uim.centeredText("Success!");
				uim.skipLines(2);

				tempStr = "Name changed to " + mealPtr->getName();
				uim.leftAllignedText(tempStr);
				uim.prompt_None();
				uim.display();
				break;
			case 2: 
				uim.centeredText("Edit Price");
				uim.skipLines(2);

				// display original price
				tempStr = "Original price: ";

				// remove excess 0's in price
				{
					std::string formatStr = std::to_string(mealPtr->getPrice());

					// erase trailing zeros, leaves '.' at end of string
					formatStr.erase(formatStr.find_last_not_of("0") + 1, std::string::npos);

					// erase '.' if exists
					if (formatStr[formatStr.length() - 1] == '.')
						formatStr.pop_back();

					tempStr += formatStr;
				}
				uim.leftAllignedText(tempStr);
				uim.skipLines(1);

				// prompt and get input
				uim.leftAllignedText("Enter a new price: ");
				uim.prompt_FreeDouble(MINIMUM_PRICE, MAXIMUM_PRICE);

				tempDouble = std::stod(uim.display());

				// set new price
				mealPtr->setPrice(tempDouble);

				// confirm to user
				uim.centeredText("Success!");
				uim.skipLines(2);

				tempStr = "Price changed to: ";

				// remove excess 0's in price
				{
					std::string formatStr = std::to_string(mealPtr->getPrice());

					// erase trailing zeros, leaves '.' at end of string
					formatStr.erase(formatStr.find_last_not_of("0") + 1, std::string::npos);

					// erase '.' if exists
					if (formatStr[formatStr.length() - 1] == '.')
						formatStr.pop_back();

					tempStr += formatStr;
				}
				uim.leftAllignedText(tempStr);
				uim.prompt_None();
				uim.display();
				break;
			case 3:
			{
				bool mealDisabled = mealPtr->getIsDisabled();

				uim.centeredText("Enable/Disable Meal");
				uim.skipLines(2);

				// report status
				tempStr = "This meal is currently ";

				if (mealDisabled)
					tempStr += "DISABLED.";
				else
					tempStr += "ENABLED.";

				uim.leftAllignedText(tempStr);

				// prompt user
				tempStr = "Do you want to ";

				if (mealDisabled)
					tempStr += "enable";
				else
					tempStr += "disable";

				tempStr += " it?";
				uim.leftAllignedText(tempStr);

				strVec = { "YYes", "NNo" };
				uim.prompt_List_Case_Insensitive(strVec);

				tempStr = uim.display();
				tempStr = std::toupper(tempStr.at(0));

				// process user's input
				if (tempStr == "Y")
				{
					// toggle state
					if (mealDisabled)
						mealPtr->setIsDisabled(false);
					else
						mealPtr->setIsDisabled(true);
				}
			}
				break;
			case 4: // edit tags
				editMealTags(mealPtr, uim);
				tempStr = ""; // make sure menu will loop
				break;
			default:
				// do nothing
				tempInt = 0;
			}// switch (tempInt)
		} // if (tempStr != "Q")
	}// while (tempStr != "Q")
}

void MealManager::generateSchedule(std::string outputFile, std::ostream& oFile)
{
}

void MealManager::mealEditor(UIManager& uim)
{
	std::string tempStr;
	int tempInt = 0;
	double tempDouble = 0;
	std::vector<std::string> strVec;
	bool inputValid = true;
	bool doneCreating = false;
	Meal* mealptr = nullptr;

	while (tempInt != 3)
	{
		uim.centeredText("Meal Editor");
		uim.skipLines(2);

		tempStr = "You have " + std::to_string(meals.size()) + " meals saved.";
		uim.centeredText(tempStr);

		strVec = { "1View/Edit Meals", "2Create Meal", "3Exit Menu" };
		uim.prompt_List_Case_Insensitive(strVec);

		tempInt = std::stoi(uim.display());

		switch (tempInt)
		{
		case 1: // list all meals and allow editing
			
			// if there are meals to list
			if (meals.size() > 0)
			{
				// if meals > 10, then display in pages
				if (meals.size() > 10)
				{
					tempStr = "";

					int currentPage = 1;
					int totalPages = meals.size() / 10; // remainder may be present

					// if remainder, then add an extra page
					if (meals.size() % 10 > 0)
						++totalPages;

					// loop while user has not quit menu
					while (tempStr != "Q")
					{
						uim.centeredText("Saved Meals");
						// display "Page x/y"
						tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
						uim.centeredText(tempStr);
						uim.skipLines(2);
						uim.leftAllignedText("Choose a Meal to edit:");

						// start index for the current page
						int startIndex = (10 * (currentPage - 1));

						//if displaying a page that is NOT last page, then display 10 items
						if (currentPage < totalPages)
						{
							// create iterator at index's position
							auto mealIter = meals.begin() + startIndex;

							// create prompts for meals
							for (int count = 0; count < 10; ++count)
							{
								tempStr = std::to_string(count) + (*mealIter)->getName();
								strVec.push_back(tempStr);
							}
						}
						else // the current page is the last page, so there may be less than 10 elements to display
						{
							// create iterator at index's position
							auto mealIter = meals.begin() + startIndex;
							int count = 0; // used to print prompt number

							// create prompts for the rest of the elements
							while (mealIter != meals.end())
							{
								tempStr = std::to_string(count) + (*mealIter)->getName();
								strVec.push_back(tempStr);
								++count;
							}
						}

						// add prompts to go to next page, previous, and quit
						tempStr = "NNext Page";
						strVec.push_back(tempStr);

						tempStr = "PPrevious Page";
						strVec.push_back(tempStr);

						tempStr = "QQuit Selection";
						strVec.push_back(tempStr);

						// display UI and get input
						uim.prompt_List_Case_Insensitive(strVec);
						tempStr = uim.display();
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
							tempInt += startIndex;

							// get meal and call editor
							Meal* mealPtr = meals.at(tempInt);
							editMeal(mealPtr, uim);
						}
					} // while (tempStr != "Q")
				}
				else // display all
				{
					tempStr = "";
					// while user has not chosen to quit 
					while (tempStr != "Q")
					{
						uim.centeredText("Saved Meals");
						uim.skipLines(2);
						uim.leftAllignedText("Choose a Meal to edit:");

						// create prompts
						int choice = 0;
						for (auto mealIter : meals)
						{
							tempStr = std::to_string(choice) + mealIter->getName();
							strVec.push_back(tempStr);
							++choice;
						}
						// add prompts to quit
						tempStr = "QQuit Selection";
						strVec.push_back(tempStr);

						uim.prompt_List_Case_Insensitive(strVec);
						tempStr = uim.display();
						tempStr = std::toupper(tempStr.at(0));

						// user chose a meal to edit
						if (tempStr != "Q")
						{
							// get index of meal
							tempInt = std::stoi(tempStr);

							Meal* mealPtr = meals.at(tempInt);

							// display menu to edit meal
							editMeal(mealPtr, uim);
							tempStr = ""; // ensure menu will loop until user chooses to exit
						}
					}// while (tempStr != "Q")
				} // else display all meals
			}
			else // display error
			{
				uim.centeredText("Error");
				uim.skipLines(2);
				uim.centeredText("You have no Meals to display.");
				uim.prompt_None();
				uim.display();
			}
			break;
		case 2: // create a new meal and add to meals stored
			mealptr = nullptr;
			
			createMeal(mealptr, uim);

			// add meal to meals 
			meals.push_back(mealptr);
			mealptr = nullptr;
			break;
		case 3:
		default:
			tempInt = 3;
		}
	}


}

void MealManager::tagEditor(UIManager& uim)
{
	std::string tempStr;
	int tempInt;
	std::vector<std::string> strVec;

	uim.centeredText("Tag Editor");

	tempStr = "You have " + std::to_string(normalTags.size()) + " tags saved and "
		+ std::to_string(globalTags.size()) + "global tags saved.";
	uim.centeredText(tempStr);
	uim.skipLines(2);
}

void MealManager::saveState(std::string fileName, std::ostream& oFile)
{
}

void MealManager::loadState(std::string fileName, std::istream& iFile)
{
}
