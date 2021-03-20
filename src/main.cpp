#include "../headers/MealManager.hpp" // includes Tag, Meal, UIManager

int main()
{
	const std::string DATAFILE = "SavedData.txt"; // name of file that stores program data
	const std::string OUTPUTFILE = "Generated_List.txt"; // name of file that stores generated meal list
	const int UI_WIDTH = 100;
	const int UI_HEIGHT = 45;

	UIManager uim; 
	MealManager mealManager;

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
		uim.leftAllignedText("Select an option.");
		strVec = { "1Generate a Meal Schedule", "2View Meals", "3View Tags", "4Quit Program" };
		uim.prompt_List(strVec);

		tempInt = std::stoi(uim.display());

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