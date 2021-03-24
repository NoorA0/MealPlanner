#include "../headers/MealManager.hpp" // includes Tag, Meal, UIManager
#include <fstream>
#include <chrono>
#include <ctime>

void displayMealManual(UIManager& uim);
void displayTagManual(UIManager& uim);
void displayMultiTagManual(UIManager& uim);

// ensures input has .txt extension, otherwise adds .txt to input and returns it
std::string verifyFileName(const std::string& input);

int main()
{
	// default file names to store and store Meal Plans, respectively
	const std::string DATAFILE = "ProgramData.txt";
	const std::string OUTPUTFILE = "Generated_Plan.txt";
	const std::string LOGFILE = "Logs.txt";

	// min filename length for generated plans, length of 0 indicates to use OUTPUTFILE
	const unsigned int MIN_OUTFILE_LENGTH = 0; 
	// max filename length for generated plans
	const unsigned int MAX_OUTFILE_LENGTH = 50; 
	const int UI_WIDTH = 140;
	const int UI_HEIGHT = 45;
	const double MINIMUM_PRICE = 0; // minimum acceptable price for Meal price
	const double MAXIMUM_PRICE = 99.99; // maximum acceptable price for Meal price 
	const unsigned int NAME_LENGTH = 40; // char limit for Tag and Meal names 
	const unsigned int DESC_LENGTH = 80; // char limit for Tag descriptions

	const std::vector<std::string> CREDITS_IMAGE = {
		"                                                                                                  ",
"                                                                                                  ",
"                                                                                                  ",
"                                                                                                  ",
"                                         . ...........                                            ",
"                                ................,,............                                    ",
"                            ...............,,,,,,,,,,,,,,,.,........                              ",
"                         .........,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,.......                          ",
"                       .....,,,,****///////////#((//****,,,,,,,,,,,,........                      ",
"                   .....,,,****//((#(#/((/((////(((((/#///***,,,,**,,,,.......                    ",
"                  ...,,,**///////////////*////////////(((/(//*****,**,,,,,.......                 ",
"                ..,,**////////////***/********/*/////////(((#((//*****,,,,,,........              ",
"              ...,**/(/(///*************////////////////////((#%((/*****,,,,,,.......   ...       ",
"            ...,,*/(///***************///////(((((((#(///////((#(((//****/*,,,,,........ ...      ",
"           ...,,**//*/**********////////((((((((((((((((((/////((/((//******,,,,......... ...     ",
"          ...,,*/***********///(////((#%###%##(#(###%#((((((////((#((//****/*,,,,,,..........     ",
"          ...,,************//////((####(((////////(((#%&%##((///((((((/******,,,,,,...........    ",
"         ...,,*************/(///(#%##(/////*/*//(/(#%%&&%&%(((///((##(/**/*/**,,,,,,.,,,......    ",
"         ...,,,***********/////####((/((/***/((#%%&&&&%%#(///(///((##(//*///**,,,,,,,,,,......    ",
"         ...,,,,*********/////(###((/(/***/((#%&@@@@&%#(#(**/////(##%(//////**,,,,,,,,,,....,.    ",
"        ...,,,,,,,*,,,***//*//(&(((/(//**/((#%&@@@&&%#(#/*//////((##(///*//***,,,,,,,,,.......    ",
"        ...,,,,,,,,,,,***//*/(###((((//*/##%&&&&&%#%(//////////((##((///((****,*,,,,,,........    ",
"         ...,,,,,,,,,,*/*//*//(##((((/(#%&&%%%%#(//////(**///((%##(/////**********,,,,........    ",
"         ....,,,,,,,,,***/***/(###((((%##%%%#(((((////////((#(&##////*//**********,,,....         ",
"          .....,,,,,,,,,*/****/(%#(((((((((##%#######%#####%#((/////(//***********,,....   ...    ",
"           ....,,,,,,,,,,**/**//(#(((((((((((##((((((((((((/////((////******//****,...........    ",
"            ....,,,,,,,,,*******//(#%#(((//(#(//(/((((#(((((/*/********////////**,,......*..      ",
"             .......,,,,,,,*/****//(((##(((////(((////////*/****//////////(//**,,.........        ",
"               ........,,,,,**//****//(#(((#(((((///#((/(/(////////(/(#(///**,,....,,,.           ",
"                 ........,,,,,**/***///////(#%((((#(((#((((((((((////#//*,,,...,....              ",
"                    ........,,,,,**********///////((((((((((/(////***,,,,.........                ",
"                      ..........,,,,,***(***********************,,,,,...,,,,...                   ",
"                          ..........,,,,,,,***,,,,,,,,,,,,*,,,,,*,,,**,,....                      ",
"                              ..............,,,.,,,..,,,,,,,...........                           ",
"                                   ..............................                                 ",
"                                            . . ......                                            ",
"                                                                                                  ",
"                                                                                                  ",
"                                                                                                  "
	};

	UIManager uim; 
	MealManager mealManager(MINIMUM_PRICE, MAXIMUM_PRICE, NAME_LENGTH, DESC_LENGTH, uim);
	
	// used to write to file, load from file
	std::ifstream iFile;
	std::ofstream oFile;

	// used to construct UI elements, get user input, etc.
	std::string tempStr;
	int tempInt = 0;
	std::vector<std::string> strVec;

	// set UI dimensions
	uim.setDimensions(UI_WIDTH, UI_HEIGHT);

	try
	{
		// load existing data
		mealManager.loadState(DATAFILE, iFile);
	}
	catch (std::string& error)
	{
		// write error to log
		std::ofstream errOut(LOGFILE, std::ios::app);

		if (errOut.is_open())
		{
			// get system time 
			auto now = std::chrono::system_clock::now();
			std::time_t currentTime = std::chrono::system_clock::to_time_t(now);;

			errOut << ctime(&currentTime);
			errOut << "ERROR: Could not load program data.\n";
			errOut << "REASON: " << error << ".\n";
			errOut << "Ignore this message if this is the program's first execution.\n\n";
		}
		errOut.close();
	}



	while (tempInt != 4)
	{
		// display main menu
		uim.centeredText("Meal Planner - Main Menu");
		uim.skipLines(2);
		uim.centeredText("Select an option.");
		strVec = { "1Generate a Meal Plan", "2Meal Menu", "3Tag Menu", "4Program Manual", "5Credits", "QQuit Program" };
		uim.prompt_List_Case_Insensitive(strVec);

		tempStr = uim.display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr != "Q")
			tempInt = std::stoi(tempStr);
		else
			tempInt = 4;

		switch (tempInt)
		{
		case 1: // generate plan
			uim.centeredText("Create a Plan");
			uim.skipLines(2);

			// prompt for filename
			uim.centeredText("Enter a file name:");
			tempStr = "(press <enter> to use \"" + OUTPUTFILE + "\")";
			uim.centeredText(tempStr);
			uim.prompt_FreeString(MIN_OUTFILE_LENGTH, MAX_OUTFILE_LENGTH);
			tempStr = uim.display();

			// check if using default name
			if (tempStr.length() == 0)
				tempStr = OUTPUTFILE;
			else // verify file extension is correct
				tempStr = verifyFileName(tempStr);

			mealManager.generateSchedule(tempStr, oFile);
			break;
		case 2: // view Meals
			mealManager.mealEditor();
			break;
		case 3: // view Tags
			mealManager.tagEditor();
			break;
		case 4: // program manual
			// main menu
			while (tempStr != "Q")
			{
				uim.centeredText("Manual - Main Menu");
				uim.skipLines(2);
				uim.centeredText("Select a section to view more info:");
				strVec = { "1How do I use this program?", "2Meals", "3Tags","4MultiTags", "5Examples", "QQuit back to Main Menu" };

				uim.prompt_List_Case_Insensitive(strVec);
				tempStr = uim.display();
				tempStr = std::toupper(tempStr.at(0));


				// if not quit
				if (tempStr != "Q")
				{
					tempInt = std::stoi(tempStr);

					switch (tempInt)
					{
					case 1: // general summary
						uim.centeredText("General Summary");
						uim.skipLines(2);
						uim.leftAllignedText("First, make sure you've created a few Meals, they're the bread and butter of this program.");
						uim.leftAllignedText("Each Meal can be assigned any number of Tags, which will define when a Meal is suitable to make on a given day.");
						uim.leftAllignedText("Meals also have a Price. When you want to create a Meal Plan, you can specify your budget for the Plan's period.");
						uim.leftAllignedText("The program will ensure that the generated plan meets this budget, if possible.");
						uim.skipLines(1);
						uim.leftAllignedText("Use Tags to catagorize your meals, such as Desserts that should only be served on certain days of the week.");
						uim.leftAllignedText("Sometimes a meal may take too long to make, and some days you only have time for a Quick Food.");
						uim.leftAllignedText("In this case, create Tags called \"Takes Long\" or \"Quick to Make\", then enable the tag on the days that you want that food.");
						uim.leftAllignedText("If you disabled Meals with \"Takes Long\" on Mondays, then foods assigned to that Tag will not occur on Mondays.");
						uim.skipLines(1);
						uim.leftAllignedText("When you want to create a Meal Plan, you will specify a file name, time period in weeks, and a budget over this period.");
						uim.leftAllignedText("The program will compute a suitable Plan, then create a file with the plan written to it.");
						uim.leftAllignedText("You can find this file in the same place as where you launched this program from.");
						uim.display();
						break;
					case 2: // Meal info
						displayMealManual(uim);
						break;
					case 3: // Tag info
						displayTagManual(uim);
						break;
					case 4: // MultiTag info
						displayMultiTagManual(uim);
						break;
					case 5: // examples
						uim.centeredText("Examples");
						uim.skipLines(2);
						uim.display();
						break;
					default:
						tempInt = 4; // useless
					}
				}
			}
			break;
		case 5: // credits
			uim.centeredText("Meal Planner By Noor Alaghbar");
			uim.centeredText("Github.com/NoorA0");
			uim.skipLines(2);
			uim.centeredText("Thanks for using my program!");
			uim.leftAllignedText(CREDITS_IMAGE);
			uim.display();
			break;
		default:
			tempInt = 4;
		}
	}// WHILE (tempInt != 4)

	try
	{
		// save data before exit
		mealManager.saveState(DATAFILE, oFile);
	}
	catch (std::string& error)
	{
		// write error to log
		std::ofstream errOut(LOGFILE, std::ios::app);

		if (errOut.is_open())
		{
			// get system time 
			auto now = std::chrono::system_clock::now();
			std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

			errOut << ctime(&currentTime);
			errOut << "ERROR: Could not save program data.\n";
			errOut << "REASON: " << error << ".\n";
			errOut << "\nThis may be caused by insufficient write permissions or lack of drive space.\n\n";
		}
		errOut.close();
	}

	return 0;
}

void displayMealManual(UIManager& uim)
{
	int currentPage = 1;
	int totalPages = 2;
	std::string tempStr = "";
	std::vector<std::string> strVec;

	// loop while user not quit
	do
	{
		switch (currentPage)
		{
		case 1:
			uim.centeredText("Meal Parameters");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.centeredText("Name");
			uim.skipLines(1);
			uim.leftAllignedText("Name of a Meal, should be short (e.g. Tacos, Tikka Masala).");
			uim.leftAllignedText("Note that Meals cannot share the same name.");
			uim.skipLines(2);
			uim.centeredText("Description");
			uim.skipLines(1);
			uim.leftAllignedText("A brief description of the meal, if necessary.");
			uim.skipLines(2);
			uim.centeredText("Price");
			uim.skipLines(1);
			uim.leftAllignedText("The Meal's price, can be the cost of its ingredients, or whatever else you wish.");
			uim.leftAllignedText("When generating a Meal Plan, you will specify your budget over the plan's duration.");
			uim.leftAllignedText("The program will schedule Meals so that you do not exceed your budget.");
			uim.skipLines(2);
			uim.centeredText("Meal Duration");
			uim.skipLines(1);
			uim.leftAllignedText("How long a Meal takes to eat (typically 1 day, but can be longer).");
			uim.leftAllignedText("For those that like to cook in large batches, set this value to how many days you expect the batch to last.");
			uim.skipLines(2);
			uim.centeredText("Days Between Occurrences");
			uim.skipLines(1);
			uim.leftAllignedText("How many days must pass before a Meal can be scheduled again.");
			uim.leftAllignedText("This is useful for Meals that you're sick of, if you have to eat them in a short span of time.");
			uim.skipLines(2);
			uim.centeredText("Flag: Disabled");
			uim.skipLines(1);
			uim.leftAllignedText("Meals can be intentionally disabled, which prevents them from appearing in Meal Plans.");
			uim.leftAllignedText("Disable a Meal instead of deleting it altogether, unless you are certain that you don't want it anymore.");
			uim.skipLines(2);
			uim.centeredText("Assigned Tags");
			uim.skipLines(1);
			uim.leftAllignedText("Meals can be assigned with one or more Tags. Tags are what will be used to find if a Meal is suitable for a given day.");
			uim.leftAllignedText("Meals without any assigned Tags will behave as if they were disabled, meaning they won't show up in a Meal Plan");
			break;
		case 2:
			uim.centeredText("Usage Guidelines");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("In most cases, Meals should be complete, meaning they should describe a full meal that you would eat.");
			uim.leftAllignedText("If you normally eat a sandwich and fries, you can name the Meal \"Sandwich and Fries\", so that you know what it includes.");
			uim.skipLines(1);
			uim.leftAllignedText("When you want greater control over the structure of Meals, you will need to use Tags and MultiTags.");
			uim.leftAllignedText("Be sure to understand how to use Tags and MultiTags before attempting to use them.");
			break;
		default:
			// do nothing
			currentPage = 1;
		}

		// add prompts to go to next page, previous, and quit
		tempStr = "NNext Page";
		strVec.push_back(tempStr);

		tempStr = "PPrevious Page";
		strVec.push_back(tempStr);

		tempStr = "QQuit to Menu";
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
	} while (tempStr != "Q");

}

void displayTagManual(UIManager& uim)
{
	int currentPage = 1;
	int totalPages = 2;
	std::string tempStr = "";
	std::vector<std::string> strVec;

	// loop while user not quit
	do
	{
		switch (currentPage)
		{
		case 1:
			uim.centeredText("Tag Parameters");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.centeredText("Name");
			uim.skipLines(1);
			uim.leftAllignedText("Name of a Tag, it should describe what you are categorizing in a few words.");
			uim.skipLines(1);
			uim.leftAllignedText("Examples:");
			uim.leftAllignedText("Meals that cater to certain tastes: Vegetarian, Chicken, ...");
			uim.leftAllignedText("Catagorize by situation: Quick To Make, Snack, Takes Long, ...");
			uim.leftAllignedText("By Type: Soups, Sandwiches, ...");
			uim.skipLines(1);
			uim.leftAllignedText("The more Tags you make, the more accurate a Meal Plan will be.");
			uim.leftAllignedText("It's up to you to decide how you want to Tag meals, keep in mind that Meals can be assigned multiple Tags at the same time.");
			uim.skipLines(2);
			uim.centeredText("Description");
			uim.skipLines(1);
			uim.leftAllignedText("Try to describe what sort of Meals are suitable for this Tag (example: \"For Meals that take long to make\"");
			uim.skipLines(2);
			uim.centeredText("Enabled Days");
			uim.skipLines(1);
			uim.leftAllignedText("Each Tag can be enabled or disabled for every day of the week (Monday, Tuesday, ..., Saturday, Sunday)");
			uim.leftAllignedText("You can create complex rules by assigning multiple Tags to a single meal.");
			uim.skipLines(2);
			uim.centeredText("Consecutive Occurrences Limit");
			uim.skipLines(1);
			uim.leftAllignedText("Limits how many days in a row that Meals with this Tag can occur for in a given week.");
			uim.leftAllignedText("Example: a limit of 1 day means that if a Meal with this Tag is used in a week, 7 days must pass before Meals with this Tag can occur again.");
			uim.skipLines(2);
			uim.centeredText("Flag: Depends on MultiTag");
			uim.skipLines(1);
			uim.leftAllignedText("If this flag is set to TRUE, then a Tag will not cause Meals to be scheduled, unless the Tag is linked to an enabled MultiTag");
			uim.leftAllignedText("(see MuliTag manual page for more info.)");
			break;
		case 2:
			uim.centeredText("Meal Information");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			break;
		default:
			// do nothing
			currentPage = 1;
		}

		// add prompts to go to next page, previous, and quit
		tempStr = "NNext Page";
		strVec.push_back(tempStr);

		tempStr = "PPrevious Page";
		strVec.push_back(tempStr);

		tempStr = "QQuit to Menu";
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
	} while (tempStr != "Q");
}

void displayMultiTagManual(UIManager& uim)
{
	int currentPage = 1;
	int totalPages = 2;
	std::string tempStr = "";
	std::vector<std::string> strVec;

	// loop while user not quit
	do
	{
		switch (currentPage)
		{
		case 1:
			uim.centeredText("Meal Information");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			break;
		case 2:
			uim.centeredText("Meal Information");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			break;
		default:
			// do nothing
			currentPage = 1;
		}

		// add prompts to go to next page, previous, and quit
		tempStr = "NNext Page";
		strVec.push_back(tempStr);

		tempStr = "PPrevious Page";
		strVec.push_back(tempStr);

		tempStr = "QQuit to Menu";
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
	} while (tempStr != "Q");

	uim.centeredText("MuliTag Information");
	uim.skipLines(2);
	uim.leftAllignedText("MultiTags are very powerful tools, but don't overuse them unless you know what you're doing.");
	uim.leftAllignedText("In short, a MultiTag behaves like a normal Tag (has days when enabled and disabled).");
	uim.leftAllignedText("The catch is that MulitTags are linked to multiple Tags, an unlimited amount of them, even.");
	uim.leftAllignedText("MultiTags will ALWAYS take priority over normal Tags.");
	uim.skipLines(2);
	uim.leftAllignedText("Example: MondayMeals is a Tag that is only enabled on Mondays. MondayMeals has a number of Meals assigned to it.");
	uim.leftAllignedText("NoTime is a MultiTag that is assigned to other tags, perhaps a Salads, a Sandwiches, etc. and it is also enabled on Monday.");
	uim.skipLines(1);
	uim.leftAllignedText("A family of 3 normally eats the same Meal, so they normally only need a meal from MondayMeals.");
	uim.leftAllignedText("However, when no one can cook for the family (because there's no time), then everyone eats their own individual Meal");
	uim.leftAllignedText("Person A wants to eat a Salad, person B wants a Sandwich, etc.");
	uim.skipLines(1);
	uim.leftAllignedText("So, instead of a single Meal from MondayMeal being planned, 3 individual meals will be planned on the days that NoTime is enabled.");
	uim.display();
}

std::string verifyFileName(const std::string& input)
{
	std::string extension = ".txt"; // extension to add
	std::string returnStr = "";

	// if input is long enough to have extension
	if (input.length() >= extension.length())
	{
		// check if last 4 chars has extension
		if (input.substr(input.length() - extension.length()) == extension)
			returnStr = input; // input is correct
		else
			returnStr = input + extension; // append extension
	}
	else // append extension
		returnStr = input + extension;

	return returnStr;
}
