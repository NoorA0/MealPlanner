#include "../headers/MealManager.hpp" // includes Tag, Meal, UIManager

int main()
{
	const std::string DATAFILE = "SavedData.txt"; // name of file that stores program data
	const std::string OUTPUTFILE = "Generated_List.txt"; // name of file that stores generated meal list
	const int UI_WIDTH = 100;
	const int UI_HEIGHT = 45;
	const double MINIMUM_PRICE = 0; // minimum acceptable price for Meal price
	const double MAXIMUM_PRICE = 99; // maximum acceptable price for Meal price 
	const unsigned int NAME_LENGTH = 40; // char limit for Tag and Meal names 
	const unsigned int DESC_LENGTH = 80; // char limit for Tag descriptions

	UIManager uim; 
	MealManager mealManager(MINIMUM_PRICE, MAXIMUM_PRICE, NAME_LENGTH, DESC_LENGTH);

	// used to construct UI elements, get user input, etc.
	std::string tempStr;
	int tempInt = 0;
	std::vector<std::string> strVec;

	// set UI dimensions
	uim.setDimensions(UI_WIDTH, UI_HEIGHT);

	// load existing data
	mealManager.loadState(DATAFILE);

	while (tempInt != 4)
	{
		// display main menu
		uim.centeredText("Main Menu");
		uim.skipLines(2);
		uim.centeredText("Select an option.");
		strVec = { "1Generate a Meal Schedule", "2Meal Menu", "3Tag Menu", "QQuit Program" };
		uim.prompt_List_Case_Insensitive(strVec);

		tempStr = uim.display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr != "Q")
			tempInt = std::stoi(tempStr);
		else
			tempInt = 4;

		switch (tempInt)
		{
		case 1: // generate schedule
			mealManager.generateSchedule(OUTPUTFILE);
			break;
		case 2: // view Meals
			mealManager.mealEditor(uim);
			break;
		case 3: // view Tags
			mealManager.tagEditor(uim);
			break;
		case 4: // save and quit
		default:
			tempInt = 4;
		}
	}// WHILE (tempInt != 4)

	// save data before exit
	mealManager.saveState(DATAFILE);
	return 0;
}