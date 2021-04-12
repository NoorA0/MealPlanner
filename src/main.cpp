#include "../headers/MealManager.hpp" // includes Tag, Meal, UIManager
#include <fstream>
#include <chrono>
#include <ctime>

void displayGeneralManual(UIManager& uim);
void displayMealManual(UIManager& uim);
void displayTagManual(UIManager& uim);
void displayMultiTagManual(UIManager& uim);
void displayExamples(UIManager& uim);

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

	const std::vector<std::string> MAINMENU_IMAGE = {
"               ,                                                      ",
"              %,           &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&  ",
"             &(,           &               Meal Plan               &  ",
"            &%*,           &---------------------------------------&  ",
"           .@#,            &Created on: Fri Aug 29 02:14:00 1997   &  ",
"           &%*,            &Duration: 3 weeks (21 days)            &  ",
"           @%*             &Budget limit: 321                      &  ",
"          &&*,             &Calculated cost: 100.25                &  ",
"          @%*              &Days with errors: 0                    &  ",
"         %&*,              &---------------------------------------&  ",
"         @%*               &---------------------------------------&  ",
"        %@/,               &          WEEK 01: cost 32.19          &  ",
"        &%*,               &                                       &  ",
"       %@#,                &            >> MONDAY <<               &  ",
"       &&*,                &           Djikstra Special            &  ",
"      #&#,                 &          price: calculating ...       &  ",
"      &&*,                 &                                       &  ",
"     #@%*,                 &           >> TUESDAY <<               &  ",
"     %@*,                  &            Hot Potatoes               &  ",
"    .@&*,                  &             price: 3.79               &  ",
"   @%@(,                   &                                       &  ",
"   &&&*.                   &          >> WEDNESDAY <<              &  ",
"  @&@%,                    &          Dehydrated Pizza             &  ",
"  @@&*.                    &            price: 1.21                &  ",
"  @@%,                     &                                       &  ",
"   @,.                     &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&  ",
"   &,                                                                 ",
"  @/                                                                  "
	};

	const std::vector<std::string> MANUAL_IMAGE = {

"                  (&&&&&&)               ",
"            %@@/************&&#         ",
"         &@*********************@%      ",
"       @@********%@@@@@@&*********&     ",
"      @*******@@@        (@@*******&    ",
"     @/******@&            @@******@#   ",
"     @******@&              @(*****&%   ",
"     @******@              @@/*****@#   ",
"     @*****@@            #@@/*****@&    ",
"      %@@@@&          (%@@/******@%     ",
"                  *#@@@(/******&%       ",
"                /@@@(******/&%          ",
"                &@&/****(&&             ",
"                @@*****&&               ",
"                @@#***@#                ",
"                @@#***@#                ",
"                @@****&#                ",
"                &#****&#                ",
"                %&&****&#               ",
"              #@*********%              ",
"              %#*********&#             ",
"               %*********%              ",
"                 #%&%%%#                "
	};

	const std::vector<std::string> CREDITS_IMAGE = {
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

	// create seed for rng
	srand(time(NULL));

	// set UI dimensions
	uim.setDimensions(UI_WIDTH, UI_HEIGHT);

	try
	{
		// load existing data
		if (mealManager.loadState(DATAFILE, iFile) == 1) // corrupted data
		{
			// write error to log
			std::ofstream errOut(LOGFILE, std::ios::app);

			if (errOut.is_open())
			{
				// get system time 
				auto now = std::chrono::system_clock::now();
				std::time_t currentTime = std::chrono::system_clock::to_time_t(now);;

				errOut << ctime(&currentTime);
				errOut << "ERROR: Saved data is corrupted.\n";
				errOut << "At least one attribute of a Meal/Tag/MultiTag is invalid in the save file.\n";
				errOut << "Corrupted values have been set to their defaults. Please check your Meals/Tags/MultiTags for any errors.\n\n";
			}
			errOut.close();

			// tell user
			uim.centeredText("Load Error");
			uim.skipLines(2);
			uim.centeredText("At least one attribute of a Meal/Tag/MultiTag is invalid in the save file.");
			uim.skipLines(1);
			uim.centeredText("Corrupted values have been set to their defaults. Please check your Meals/Tags/MultiTags for any errors.");
			uim.display();
		}
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



	while (tempStr != "Q")
	{
		// display main menu
		uim.centeredText("Meal Planner - Main Menu");
		uim.skipLines(2);
		uim.centeredText("Select an option:");

		// print ascii image
		uim.centeredText(MAINMENU_IMAGE);
		//for (int index = 0; index < MAINMENU_IMAGE.size(); ++index)
			//uim.centeredText(MAINMENU_IMAGE[index]);

		strVec = { "GGenerate a Meal Plan", "", "1Meal Menu", "2Tag Menu", "3MultiTag Menu", "", "MProgram Manual", "CCredits", "", "QQuit Program" };
		uim.prompt_List_Case_Insensitive(strVec);

		tempStr = uim.display();
		tempStr = std::toupper(tempStr.at(0));

		if (tempStr != "Q")
		{
			// convert tempStr to number 
			if (tempStr == "G") // generate plan
				tempInt = 1;
			else if (tempStr == "M") // manual
				tempInt = 5;
			else if (tempStr == "C") // credits
				tempInt = 6;
			else // is either 1, 2, or 3
			{
				tempInt = std::stoi(tempStr);
				++tempInt; // allign with switch cases
			}

			switch (tempInt)
			{
			case 1: // generate plan
			{
				// used if plan fails to generate
				unsigned int errorsInPlan = 0;
				double planCost = 0; 

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

				// attempt to generate plan
				if (mealManager.generateSchedule(tempStr, oFile, errorsInPlan, planCost) == false)
				{
					std::string fileName = tempStr;

					// success
					uim.centeredText("Success!");
					uim.skipLines(2);

					tempStr = "A Meal Plan named \"" + fileName + "\" was created in the program's directory.";
					uim.centeredText(tempStr);
					uim.display();
				}
				else
				{
					// plan failed to generate
					uim.centeredText("Error");
					uim.skipLines(2);

					uim.centeredText("A Meal Plan was unable to be generated, please try again.");
					uim.skipLines(3);

					uim.centeredText("Details of failed plan:");
					uim.skipLines(1);

					tempStr = "Number of days with errors: " + std::to_string(errorsInPlan);
					uim.centeredText(tempStr);

					tempStr = "Total cost over selected duration: " + std::to_string(planCost);
					uim.centeredText(tempStr);
					uim.skipLines(4);

					uim.centeredText("Troubleshooting tips:");
					uim.skipLines(1);
					uim.centeredText("Make sure you have enough Meals enabled for the plan's duration.");
					uim.centeredText("Tags/MultiTags may restrict the number of available Meals.");
					uim.skipLines(1);
					uim.centeredText("If the total cost displayed above is greater than your chosen budget, consider raising your budget or creating cheaper Meals.");
					uim.display();
				}
			}
				break;
			case 2: // view Meals
				mealManager.mealEditor();
				break;
			case 3: // view Tags
				mealManager.tagEditor();
				break;
			case 4: // view MultiTags
				mealManager.multitagEditor();
				break;
			case 5: // program manual
				// main menu
				while (tempStr != "Q")
				{
					uim.centeredText("Program Manual");
					uim.skipLines(2);
					uim.centeredText("Select a section to view more info:");
					uim.skipLines(2);

					// display ascii image
					uim.centeredText(MANUAL_IMAGE);

					strVec = { "1How do I use this program?", "", "2Meals", "3Tags","4MultiTags", "", "5Example Usage", "", "QQuit back to Main Menu" };

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
							displayGeneralManual(uim);
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
							displayExamples(uim);
							break;
						default:
							tempInt = 0; // useless
						}
					}
				}
				tempStr = ""; // prevent quitting program
				break;
			case 6: // credits
				uim.centeredText("Meal Planner By Noor Alaghbar");
				uim.centeredText("DEMO VERSION - NOT COMPLETE");
				uim.skipLines(1);
				uim.centeredText("Contact noorga115@gmail.com for questions/assistance.");
				//uim.centeredText("Github.com/NoorA0");
				uim.skipLines(1);
				uim.centeredText("Thanks for using my program!");

				// display galaxy
				uim.centeredText(CREDITS_IMAGE);

				uim.display();
				break;
			default:
				tempInt = 4;
			}
		}// if (tempStr != "Q")
	}// WHILE (tempInt != Q)

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

void displayGeneralManual(UIManager& uim)
{
	int currentPage = 1;
	int totalPages = 5;
	std::string tempStr = "";
	std::vector<std::string> strVec;

	// loop while user not quit
	do
	{
		switch (currentPage)
		{
		case 1:
			uim.centeredText("General Summary");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("Before you can do anything with this program, you need to create Meals.");
			uim.leftAllignedText("Meals are what gets scheduled for each day of the week, and can be as large or small as you like.");
			uim.leftAllignedText("Meals have a: Name, Price, Duration, Days Between Occurrences, and assigned Tags.");
			uim.leftAllignedText("(Check the Meals section of the manual for detailed information about these properties.)");
			uim.skipLines(1);
			uim.leftAllignedText("Use Tags to group your Meals, like \"Desserts\" that should only be served on certain days of the week.");
			uim.leftAllignedText("Many Meals can be assigned to a single Tag, and the Tag only allows meals to be scheduled on specific days of the week.");
			uim.leftAllignedText("A single Meal may be assigned to more than one Tag at the same time.");
			uim.skipLines(1);
			uim.leftAllignedText("MultiTags are an advanced feature of this program, and they group multiple Tags together.");
			uim.leftAllignedText("This is similar to how a Tag groups multiple Meals together.");
			uim.leftAllignedText("You can create very specific Meal Plans by using Meals, Tags, and MultiTags together. Be sure to understand how to use them first.");
			uim.skipLines(1);
			uim.leftAllignedText("When you want to create a Meal Plan, you will specify a file name, the plan's duration (in weeks), and a budget you want to meet.");
			uim.leftAllignedText("The program will compute a suitable plan, then create a text file with the plan written to it.");
			uim.leftAllignedText("You can find this file in the program's folder.");
			break;
		case 2:
			uim.centeredText("Usage Guidelines");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("For easiest use, Meals should be as complete as possible, meaning they should include any side dishes.");
			uim.leftAllignedText("Example: If you normally eat a sandwich and fries, you can name the Meal \"Sandwich and Fries\".");
			uim.leftAllignedText("When you want greater control over the structure of Meals, use MultiTags.");
			uim.skipLines(1);
			uim.leftAllignedText("When generating a Meal Plan that spans 4 weeks, it is recommended that you make available at least 5 Meals.");
			uim.leftAllignedText("The amount of Meals you should make available varies highly by how much you restrict their usage.");
			uim.skipLines(1);
			uim.leftAllignedText("Be mindful of the price you set for a Meal, if you add many expensive Meals, then it may be difficult to meet a low budget.");
			uim.leftAllignedText("The next page explains the process of creating a Meal Plan.");
			break;
		case 3:
			uim.centeredText("Generating a Meal Plan");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("When creating a Meal Plan, the program ignores any Meals, Tags, and MultiTags were set to disabled.");
			uim.leftAllignedText("This includes: Meals set to DISABLED, Tags/MultiTags that are not enabled on any day of the week.");
			uim.skipLines(1);
			uim.leftAllignedText("If you are having trouble creating a Meal Plan and you think you have created enough Meals, then check your Tag/MultiTag restrictions.");
			uim.leftAllignedText("Perhaps you have unintentionally set most of your Tags/MultiTags to disabled for most of the week.");
			uim.skipLines(1);
			uim.leftAllignedText("The program begins by choosing a Tag/MultiTag according to priority level.");
			uim.leftAllignedText("The highest priority are MultiTags that have been set to high priority.");
			uim.leftAllignedText("Tags, and MultiTags that were set to normal priority will be chosen only if there are no available high priority MultiTags.");
			uim.skipLines(1);
			uim.leftAllignedText("Once a priority level has been selected, the program will randomly choose a Tag/MultiTag within that priority level.");
			uim.leftAllignedText("If a MultiTag is chosen, then ALL of its linked Tags will be searched for suitable Meals to schedule on a given day.");
			uim.leftAllignedText("If a Tag is chosen, then the program will randomly choose a Meal assigned to this Tag.");
			uim.leftAllignedText("Tags/MultiTags without suitable meals will cause the program to randomly pick a different Tag/MultiTag.");
			uim.skipLines(1);
			uim.leftAllignedText("Tags linked to MultiTags can request to schedule more than one Meal on a given day.");
			uim.leftAllignedText("In this case, the program will attempt to randomly pick and schedule multiple Meals that are assigned to the same Tag.");
			uim.skipLines(1);
			uim.leftAllignedText("When creating a Meal Plan, the program will create hundreds of potential Meal Plans, then pick the best one.");
			uim.leftAllignedText("\"Best\", is defined as meeting the budget limit and containing as few errors as possible.");
			uim.leftAllignedText("An error occurs when a Meal cannot be scheduled on a given day.");
			uim.skipLines(1);
			uim.leftAllignedText("The amount of errors in a Meal Plan can be found at the top of the text file.");
			uim.leftAllignedText("Days with errors may display \"NO MEAL AVAILABLE\"");
			break;
		case 4:
			uim.centeredText("Failure to generate a Meal Plan");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("Failure to create a Meal Plan may be caused by the following:");
			uim.skipLines(1);
			uim.leftAllignedText("1. Budget is too low, or Meals are too expensive.");
			uim.leftAllignedText("2. Not enough available Meals.");
			uim.skipLines(2);
			uim.leftAllignedText("Upon failure, you will be shown the calculated cost of the Meal Plan.");
			uim.leftAllignedText("If it is significantly higher than your specified budget, then this indicates that the problem is #1.");
			uim.skipLines(1);
			uim.leftAllignedText("You will also be shown the number of errors present for the duration of the Meal Plan.");
			uim.leftAllignedText("The program will reject any proposed plan with more than 1 error per 2 weeks.");
			uim.leftAllignedText("If the number of errors is very high, then the problem is likely #2.");
			uim.skipLines(2);
			uim.leftAllignedText("The solution to #1 is to raise your budget or lower the duration of the Meal Plan.");
			uim.leftAllignedText("You may also add Meals with a lower price.");
			uim.skipLines(1);
			uim.leftAllignedText("Solving #2 requires that you check Meals and ensure they are not accidentally disabled.");
			uim.leftAllignedText("You should also check Tags and MultiTags to make sure they are not excessively restricting Meals.");
			break;
		case 5:
			uim.centeredText("Plan Generation Logic");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.centeredText("Step 1: Choose priority level");
			uim.skipLines(1);
			uim.leftAllignedText("If there are MultiTags set to high priority, then choose high priority and proceed to step 2a.");
			uim.leftAllignedText("If there are no high priority MultiTags, then flip a coin to choose between Tags and normal priority MultiTags.");
			uim.leftAllignedText("If coin says to pick normal priority MultiTags, then proceed to step 2a, otherwise proceed to step 2b.");
			uim.skipLines(2);
			uim.centeredText("Step 2a: Randomly choose a MultiTag");
			uim.skipLines(1);
			uim.leftAllignedText("Randomly choose a MultiTag according to chosen priority level.");
			uim.leftAllignedText("If chosen MultiTag is not avilable to schedule a Meal (Enabled Days restriction), then choose again.");
			uim.leftAllignedText("Once you have found a valid MultiTag, then proceed to step 2b.");
			uim.skipLines(2);
			uim.centeredText("Step 2b: Randomly choose a Tag");
			uim.skipLines(1);
			uim.leftAllignedText("Randomly choose a Tag.");
			uim.leftAllignedText("If chosen Tag is not available to schedule a Meal (Enable Days and Occurrences Limit restriction), then choose again.");
			uim.leftAllignedText("Once you have found a valid Tag, then proceed to step 3.");
			uim.skipLines(1);
			uim.leftAllignedText("Note: if you came from step 2a, then ...");
			uim.leftAllignedText("after completing step 3, you will return to this step until all Tags have been chosen.");
			uim.skipLines(2);
			uim.centeredText("Step 3: Randomly choose a Meal");
			uim.skipLines(1);
			uim.leftAllignedText("Randomly choose a Meal that is assigned to the Tag.");
			uim.leftAllignedText("If the Meal is not available to schedule (Days Between Occurrences restriction), then choose again.");
			uim.leftAllignedText("Once you have found a valid Meal, then schedule it.");
			uim.skipLines(1);
			uim.leftAllignedText("Note: if you came from step 2a, then ...");
			uim.leftAllignedText("after completing this step, you will repeat it until the number of requested meals has been fulfilled.");
			uim.leftAllignedText("Then return to step 2b.");
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
			uim.centeredText("Name of a Meal, it should be short (e.g. Tacos, Tikka Masala, Salad and Soup, etc.).");
			uim.centeredText("Names should be descriptive for you to recognise it. If you eat multiple sandwiches, then \"Sandwich\" is too vague.");
			uim.centeredText("Note that Meals cannot share the same name (case insensitive).");
			uim.skipLines(2);
			uim.centeredText("Price");
			uim.skipLines(1);
			uim.centeredText("The Meal's price, it can be the cost of its ingredients, or whatever else you wish.");
			uim.centeredText("When generating a Meal Plan, you will specify your budget over the plan's duration.");
			uim.centeredText("The program will schedule Meals so that you do not exceed your budget.");
			uim.skipLines(2);
			uim.centeredText("Meal Duration");
			uim.skipLines(1);
			uim.centeredText("How long a Meal takes to eat (typically 1 day, but can be longer).");
			uim.centeredText("For those that like to cook in large batches, set this value to how many days you expect the batch to last.");
			uim.centeredText("Note that when creating a Meal Plan, the program calculates the cost of a Meal on a per-day basis.");
			uim.centeredText("For durations of 1, this does nothing, but when the duration is greater than 1, the cost of a Meal is:");
			uim.centeredText("Cost = (Price / Duration) * Number of days scheduled");
			uim.skipLines(1);
			uim.centeredText("The price of a Meal in a Meal Plan will be displayed as its full cost.");
			uim.centeredText("However, the total cost over the entire plan is calculated on a per-day basis, as explained above.");
			uim.centeredText("If a Meal costs 20 over 5 days, but is scheduled for 2 days, the program will find the sum to be 8.");
			uim.skipLines(1);
			uim.centeredText("In short, the \"Calculated cost\", listed at the top of a Meal Plan, will always be accurate.");
			break;
		case 2:
			uim.centeredText("Meal Parameters (cont'd)");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.centeredText("Days Between Occurrences");
			uim.skipLines(1);
			uim.centeredText("The number of days must pass before a Meal can be scheduled again.");
			uim.centeredText("This is useful for Meals that you dislike to eat often, and want to wait a few days before eating again.");
			uim.skipLines(2);
			uim.centeredText("Disabled/Enabled");
			uim.skipLines(1);
			uim.centeredText("Meals can be intentionally disabled, which prevents them from appearing in Meal Plans.");
			uim.centeredText("Disable a Meal instead of deleting it altogether, unless you are sure that you don't want it anymore.");
			uim.skipLines(2);
			uim.centeredText("Assigned Tags");
			uim.skipLines(1);
			uim.centeredText("Meals can be assigned to one or more Tags. Tags are used to restrict a Meal to certain days of the week.");
			uim.centeredText("Meals without any assigned Tags will behave as if they were disabled, meaning they won't show up in a Meal Plan");
			uim.centeredText("Therefore, it's important to note that you MUST assign a Tag to a Meal in order to use the Meal in Meal Plans.");
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
			uim.centeredText("Name of a Tag, it should describe what you are categorizing in a few words.");
			uim.skipLines(1);
			uim.centeredText("Examples:");
			uim.centeredText("Meals that cater to certain tastes: Vegetarian, Chicken, ...");
			uim.centeredText("Catagorize by situation: Quick To Make, Snack, Takes Long, ...");
			uim.centeredText("By Type: Soups, Sandwiches, ...");
			uim.skipLines(1);
			uim.centeredText("The more Tags you make, the more accurate a Meal Plan will be to your needs.");
			uim.centeredText("It's up to you to decide how you want to Tag meals, keep in mind that Meals can be assigned multiple Tags at the same time.");
			uim.skipLines(2);
			uim.centeredText("Description");
			uim.skipLines(1);
			uim.centeredText("Try to describe what sort of Meals are suitable for this Tag.");
			uim.skipLines(1);
			uim.centeredText("Examples:");
			uim.centeredText("\"For Meals that take long to make.\"");
			uim.centeredText("\"James\'s favorite Dishes.\"");
			uim.centeredText("\"Lunch for work.\"");
			uim.skipLines(2);
			uim.centeredText("Enabled Days");
			uim.skipLines(1);
			uim.centeredText("Each Tag can be enabled or disabled for every day of the week (Monday, Tuesday, ..., Saturday, Sunday)");
			uim.centeredText("You can create complex rules by assigning multiple Tags to a single meal.");
			break;
		case 2:
			uim.centeredText("Tag Parameters (cont'd)");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.centeredText("Consecutive Occurrences Limit");
			uim.skipLines(1);
			uim.centeredText("Limits how many days in a row that Meals with this Tag can occur for.");
			uim.centeredText("Example: Limit = 1 day means that Meals assigned to this Tag can occur every other day at most.");
			uim.centeredText("A limit of 3 days means that Meals can occur 3 days in a row, then must wait one day, then can occur 3 days in a row ...");
			uim.centeredText("Keep in mind that if you have many Tags, then it is unlikely that the same tag will be chosen for multiple days in a row.");
			uim.skipLines(1);
			uim.centeredText("Meals with a Duration greater than Consecutive Occurrences Limit will be scheduled for their full duration.");
			uim.centeredText("This means a Meal that lasts 3 days will be scheduled for 3 days in a row, even if the occurrences limit is set to 1 or 2.");
			uim.centeredText("However, the Meal must wait a day before being rescheduled again.");
			uim.skipLines(2);
			uim.centeredText("Depends on MultiTag");
			uim.skipLines(1);
			uim.centeredText("If this value is set to TRUE, then a Tag will not cause Meals to be scheduled unless the Tag is linked to an enabled MultiTag");
			uim.centeredText("(See MuliTag manual page for more info.)");
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
	int totalPages = 4;
	std::string tempStr = "";
	std::vector<std::string> strVec;

	// loop while user not quit
	do
	{
		switch (currentPage)
		{
		case 1:
			uim.centeredText("MultiTag Parameters");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.centeredText("Name");
			uim.skipLines(1);
			uim.centeredText("Name of a MultiTag, it should describe what you are categorizing in a few words.");
			uim.skipLines(1);
			uim.centeredText("Examples:");
			uim.centeredText("\"Four-course Meal\"");
			uim.centeredText("\"Breakfast, Lunch, Dinner\"");
			uim.centeredText("\"Randomized Sandwich\" (See page 3 for explanation.)");
			uim.skipLines(2);
			uim.centeredText("Description");
			uim.skipLines(1);
			uim.centeredText("Try to describe what sort of Tags are linked to this MultiTag.");
			uim.skipLines(1);
			uim.centeredText("Examples:");
			uim.centeredText("\"Meals for each member of the family.\"");
			uim.centeredText("\"Meals for the whole day.\"");
			uim.centeredText("\"Components of a sandwich\"");
			uim.skipLines(2);
			uim.centeredText("Enabled Days");
			uim.skipLines(1);
			uim.centeredText("A MultiTag can be enabled or disabled for every day of the week (Monday, Tuesday, ..., Saturday, Sunday)");
			uim.skipLines(2);
			uim.centeredText("Linked Tags");
			uim.skipLines(1);
			uim.centeredText("MultiTags have no purpose unless linked to Tags.");
			uim.centeredText("Meals can only be enabled or disabled totally, and rely on one or more assigned Tags to determine what days a Meal is availble on.");
			uim.centeredText("MultiTags are groupings of Tags, and allows for multiple Meals to be scheduled on a single day.");
			uim.centeredText("MultiTags also allow for the use of complex rules, as both MultiTags and its Linked Tags have Enabled Days.");
			uim.skipLines(1);
			uim.centeredText("Every Tag linked to a MultiTag can request one or more Meals");
			uim.centeredText("Normally, only one Meal is scheduled from a Tag on a day of the week.");
			uim.centeredText("However, requesting more than one Meal makes the program schedule additional Meals that are assigned to the Linked Tag.");
			break;
		case 2:
			uim.centeredText("MultiTag Parameters (cont'd)");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.centeredText("High Priority");
			uim.skipLines(1);
			uim.centeredText("If this value is set to TRUE, then this MultiTag will be considered before Tags and other MultiTags with normal priority.");
			uim.centeredText("It is recommended that you set MultiTags to normal priority unless you absolutely need a MultiTag to be chosen before anything else.");
			uim.centeredText("MultiTags left on high priority WILL prevent other Tags and MultiTags from being used, which may cause bad Meal Plans.");
			uim.skipLines(2);
			uim.centeredText("Requires Fulfillment");
			uim.skipLines(1);
			uim.centeredText("Since a MultiTag can be linked to multiple Tags, it is possible that not all Tags can schedule a Meal.");
			uim.centeredText("If you require that all of a MultiTags's linked Tags schedule their requested number of Meals, set this to: REQUIRE FULFILLMENT.");
			uim.skipLines(1);
			uim.centeredText("If you're happy with at least 1 Meal in total being scheduled, then set this to: ALLOW PARTIAL FULFILLMENT");
			uim.skipLines(1);
			uim.centeredText("(For info on plan generation, see manual page about Meal Plan generation, under \"How do I use this program?\")");
			break;
		case 3:
			uim.centeredText("Advanced Usage: Sandwich Randomizer");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("Meals can be as small or large as you want them to be.");
			uim.leftAllignedText("While it is advised you make Meals as complete, or large, as possible, advanced usage of MultiTags can require small Meals.");
			uim.skipLines(2);
			uim.centeredText("Example:");
			uim.skipLines(1);
			uim.leftAllignedText("Imagine that you created a Meal called \"Slice of white bread\"");
			uim.leftAllignedText("This is a poor meal in the traditional sense, but it is a component of a sandwich, which will become useful as you shall see..");
			uim.leftAllignedText("Now imagine multiple types of bread, all as their own Meals (e.g. Slice of sourdough, Slice of whole wheat, ...).");
			uim.skipLines(1);
			uim.leftAllignedText("Next, you create Meals for the insides of a sandwich, perhaps Meals called \"Roast beef\", \"Smoked turkey\", ...");
			uim.leftAllignedText("You also create Meals for toppings: \"Mustard\", \"Ketchup\", \"Pickles\", ...");
			uim.skipLines(1);
			uim.leftAllignedText("Now, you create a Tag called \"Sandwich Buns\", and assign the slices of bread to this Tag.");
			uim.leftAllignedText("Then another Tag called \"Sandwich Core\" and assign \"Roast beef\", \"Smoked turkey\", ..., to this Tag.");
			uim.leftAllignedText("Yet again, you create a Tag, this time called \"Sandwich Toppings\", and assign it to the toppings you created as Meals.");
			uim.skipLines(1);
			uim.leftAllignedText("Finally, you create a MultiTag called \"Random Sandwich\", and link it to the newly created Tags.");
			uim.leftAllignedText("You set the number of requested meals for \"Sandwich Buns\" to 2 (for a top and bottom), while the other Linked Tags request 1 Meal each.");
			uim.leftAllignedText("You have also set the Tags to depend on this MultiTag, so that they only get chosen for Meal scheduling if \"Random Sandwhich\" is chosen.");
			uim.leftAllignedText("Since you want a complete sandwich, you set Requires Fulfillment to REQUIRE FULFILLMENT, so that incomplete sandwiches are not scheduled.");
			uim.skipLines(2);
			uim.leftAllignedText("What's the point this? Well, if the Meals and Tags are all enabled, and the program chooses \"Random Sandwhich\"");
			uim.leftAllignedText("then all of the components of a sandwich will be scheduled on the same day of the week.");
			uim.skipLines(1);
			uim.leftAllignedText("So you will have effectively created a sandwich creator/randomizer!");
			uim.skipLines(1);
			uim.centeredText("(for more practical examples, see the next page.)");
			break;
		case 4:
			uim.centeredText("Advanced Usage: Part 2");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("Hopefully the previous page illustrated the utility of using MultiTags, though a sandwich randomizer is not the most practical use of one.");
			uim.skipLines(1);
			uim.leftAllignedText("In reality, you might want to use MultiTags to plan multiple Meals for each member of a household.");
			uim.leftAllignedText("In this case, you will assign such Meals to one or more Tags, then link the Tag(s) to a MultiTag.");
			uim.leftAllignedText("When linking the Tag(s), you must specify how many Meals you want to schedule from the Tag being linked.");
			uim.skipLines(1);
			uim.leftAllignedText("A Linked Tag's Enabled Days will always be respected, so if a Linked Tag is disabled on every day of the week");
			uim.leftAllignedText("but the MultiTag is enabled on all days, then program will NEVER schedule any meals from the Linked Tag.");
			uim.leftAllignedText("This is because the Linked Tag is effectively disabled, despite the MultiTag being enabled on every day.");
			uim.skipLines(1);
			uim.leftAllignedText("Thus, it is important to be mindful of the restrictions set for a Tag.");
			uim.leftAllignedText("Only limit the Enabled Days of a MultiTag when you are absolutely sure you want to restrict it.");
			uim.skipLines(1);
			uim.leftAllignedText("If a Tag is enabled on every day, but the MultiTag that is linked to it is disabled on every day,");
			uim.leftAllignedText("then the result is no Tags will EVER be chosen from the MultiTag.");
			uim.leftAllignedText("This is becuase the MultiTag, which contains Linked Tags, is effectively disabled.");
			uim.leftAllignedText("So if the MultiTag is not enabled, the state of its Linked Tags do not matter at all, they will never be chosen.");
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

void displayExamples(UIManager& uim)
{
	int currentPage = 1;
	int totalPages = 4;
	std::string tempStr = "";
	std::vector<std::string> strVec;

	// loop while user not quit
	do
	{
		switch (currentPage)
		{
		case 1:
			uim.centeredText("Basic Usage - Minimal Tags");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("You want to create a Meal Plan for yourself and you can cook 5 Meals. You're not a picky eater, so you don't mind when they occur.");
			uim.leftAllignedText("Meals must be linked to Tags in order to work, but you don't want to create a bunch of Tags and MultiTags.");
			uim.skipLines(1);
			uim.leftAllignedText("The solution is simple, create a single Tag, since you want a Meal for every day of the week, make sure the Tag is enabled on all days.");
			uim.leftAllignedText("You could name the Tag something like \"Always Enabled\" and add a description if you want.");
			uim.leftAllignedText("You don't want to use MultiTags, so set MultiTag Dependency to NO. Also, set the number of Consecutive Days to the maximum, 1000 days.");
			uim.leftAllignedText("As long as the value is greater than the number of days you want to create a Meal Plan on, it will always allow Meals to be scheduled.");
			uim.skipLines(1);
			uim.leftAllignedText("Now, create your 5 Meals. Since you're not picky, set each Meal's Days Between Occurrences to 0 days.");
			uim.leftAllignedText("This allows the Meals to occur whenever a Tag requests it, without waiting between occurrences.");
			uim.skipLines(1);
			uim.leftAllignedText("Finally, assign this Tag to each of your 5 Meals. You're done!");
			uim.leftAllignedText("Create a Meal Plan for whatever duration you wish and set your budget appropriately.");
			break;
		case 2:
			uim.centeredText("Basic Usage - Tags as Filters");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("Continuing from the previous example, you have 5 Meals and one Tag that allows Meals on every day of the week.");
			uim.leftAllignedText("Now you've become much busier than before, and some of your Meals take too long to make on a weekday.");
			uim.leftAllignedText("What you want to do is to eat 3 Meals during the weekdays, and 2 Meals on the weekends.");
			uim.skipLines(1);
			uim.leftAllignedText("Start by creating a Tag, perhaps called \"Weekdays Only\", and only enable it on the weekdays.");
			uim.leftAllignedText("Then create another Tag called \"Weekends Only\", and only enable it on the weekend.");
			uim.leftAllignedText("You're not that picky about which Meals you eat, so you keep the Consecutive Days to 1000 days.");
			uim.skipLines(1);
			uim.leftAllignedText("Finally, unassign the Tag called \"Always Enabled\" from all of your meals, because it can cause Meals to be scheduled on any day.");
			uim.leftAllignedText("You pick 3 of your Meals and assign them to \"Weekdays Only\", and the last 2 are assigned to \"Weekends Only\".");
			uim.skipLines(1);
			uim.leftAllignedText("Done! Now only 3 of the 5 Meals will be scheduled on the weekdays, and the other 2 will only occur on the weekends.");
			uim.skipLines(1);
			uim.leftAllignedText("If you wanted more than 3 Meals on the weekends, then feel free to assign Meals to BOTH of your Tags.");
			uim.leftAllignedText("That way, those Meals can be scheduled on both the weekdends and weekdays.");
			break;
		case 3:
			uim.centeredText("Advanced Usage - Multi-Course Meal");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("You want to make a multi-course Meal with the following courses: Main, Soup, Appetizer, Salad, and Dessert.");
			uim.leftAllignedText("Create each Course as its own Tag, and enable them for all days, with Consecutive Days set to 1000.");
			uim.leftAllignedText("Set each Tag to depend on a MultiTag, since you don't want these courses to be scheduled on their own.");
			uim.skipLines(1);
			uim.leftAllignedText("Next, create Meals for all of the possible things you might serve, like 2-3 soups, 5 appetizers, etc.");
			uim.leftAllignedText("If you want this Multi-Course meal to occur every 2 weeks, the set each Meal's Days Between Occurrences to 14 days.");
			uim.leftAllignedText("Assign each Meal to the course it is a part of, so soups should only be assigned to the \"Soup\" Tag., and so on.");
			uim.skipLines(1);
			uim.leftAllignedText("Since Tags only schedule a single Meal on a day, and you need more than 1 Meal to create a Multi-Course meal");
			uim.leftAllignedText("You will create a MultiTag called \"Multi-Course Meal\", and link it to each of the courses: \"Main\", \"Soup\", etc.");
			uim.leftAllignedText("Set the Requested Meals for each linked Tag to 1, unless you want something like 2 Appetizers, then go for it!");
			uim.leftAllignedText("If you want a Multi-Course meal to occur as soon as possible, and bypass the random choice that the program uses");
			uim.leftAllignedText("then set the MultiTag to HIGH PRIORITY, otherwise, it will have to be randomly chosen among other Tags before it can be scheduled.");
			uim.skipLines(1);
			uim.leftAllignedText("A Multi-Course meal is useless if it's incomplete. So set Require Fulfillment to REQUIRES FULFILLMENT.");
			uim.skipLines(1);
			uim.leftAllignedText("Lastly, enable the MultiTag on whichever days of the week you wish.");
			uim.leftAllignedText("Now you're done! Enjoy that multi-course meal!");
			break;
		case 4:
			uim.centeredText("Advanced Usage - Sandwich Randomizer");
			// display "Page x/y"
			tempStr = "[Page " + std::to_string(currentPage) + "/" + std::to_string(totalPages) + "]";
			uim.centeredText(tempStr);
			uim.skipLines(2);
			uim.leftAllignedText("Imagine that you created a Meal called \"Slice of white bread\"");
			uim.leftAllignedText("This is a poor meal in the traditional sense, but it is a component of a sandwich, which will become useful as you shall see..");
			uim.leftAllignedText("Now imagine multiple types of bread, all as their own Meals (e.g. Slice of sourdough, Slice of whole wheat, ...).");
			uim.skipLines(1);
			uim.leftAllignedText("Next, you create Meals for the insides of a sandwich, perhaps Meals called \"Roast beef\", \"Smoked turkey\", ...");
			uim.leftAllignedText("You also create Meals for toppings: \"Mustard\", \"Ketchup\", \"Pickles\", ...");
			uim.skipLines(1);
			uim.leftAllignedText("Now, you create a Tag called \"Sandwich Buns\", and assign the slices of bread to this Tag.");
			uim.leftAllignedText("Then another Tag called \"Sandwich Core\" and assign \"Roast beef\", \"Smoked turkey\", ..., to this Tag.");
			uim.leftAllignedText("Yet again, you create a Tag, this time called \"Sandwich Toppings\", and assign it to the toppings you created as Meals.");
			uim.skipLines(1);
			uim.leftAllignedText("Finally, you create a MultiTag called \"Random Sandwich\", and link it to the newly created Tags.");
			uim.leftAllignedText("You set the number of requested meals for \"Sandwich Buns\" to 2 (for a top and bottom), while the other Linked Tags request 1 Meal each.");
			uim.leftAllignedText("You have also set the Tags to depend on this MultiTag, so that they only get chosen for Meal scheduling if \"Random Sandwhich\" is chosen.");
			uim.leftAllignedText("Since you want a complete sandwich, you set Requires Fulfillment to REQUIRE FULFILLMENT, so that incomplete sandwiches are not scheduled.");
			uim.skipLines(2);
			uim.leftAllignedText("What's the point this? Well, if the Meals and Tags are all enabled, and the program chooses \"Random Sandwhich\"");
			uim.leftAllignedText("then all of the components of a sandwich will be scheduled on the same day of the week.");
			uim.skipLines(1);
			uim.leftAllignedText("So you will have effectively created a sandwich creator/randomizer!");
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
