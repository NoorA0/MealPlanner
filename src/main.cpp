#include "../headers/MealManager.hpp" // includes Tag, Meal, UIManager

int main()
{
	const int UI_WIDTH = 140;
	const int UI_HEIGHT = 45;
	const double MINIMUM_PRICE = 0; // minimum acceptable price for Meal price
	const double MAXIMUM_PRICE = 99; // maximum acceptable price for Meal price 
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
	MealManager mealManager(MINIMUM_PRICE, MAXIMUM_PRICE, NAME_LENGTH, DESC_LENGTH);

	// used to construct UI elements, get user input, etc.
	std::string tempStr;
	int tempInt = 0;
	std::vector<std::string> strVec;

	// set UI dimensions
	uim.setDimensions(UI_WIDTH, UI_HEIGHT);

	// load existing data
	mealManager.loadState();

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
		case 1: // generate schedule
			mealManager.generateSchedule();
			break;
		case 2: // view Meals
			mealManager.mealEditor(uim);
			break;
		case 3: // view Tags
			mealManager.tagEditor(uim);
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
						uim.leftAllignedText("Use Tags to catagorize your meals, such as Deserts that should only be served on certain days of the week.");
						uim.leftAllignedText("Sometimes a meal may take too long to make, and some days you only have time for a Quick Food.");
						uim.leftAllignedText("In this case, create Tags called \"Takes Long\" or \"Quick to Make\", then enable the tag on the days that you want that food.");
						uim.leftAllignedText("If you disabled Meals with \"Takes Long\" on Mondays, then foods assigned to that Tag will not occur on Mondays.");
						uim.skipLines(1);
						uim.leftAllignedText("When you want to create a Meal Plan, it will generate a text file in the same place that you ran this program from.");
						uim.leftAllignedText("If you can't find the file, you can tell the program to output the Meal Plan to your screen.");
						uim.leftAllignedText("But be warned, it will probably look really ugly, it's best to use it as last-resort.");
						uim.display();
						break;
					case 2: // Meal info
						uim.centeredText("Meal Information");
						uim.skipLines(2);
						uim.leftAllignedText("Meals should be complete, meaning they should describe a full meal that you would eat.");
						uim.leftAllignedText("It is possible to create very large meals using MultiTags, however, it get complicated to manage.");
						uim.leftAllignedText("If you normally eat a sandwich and fries, then name the meal \"Sandwich and Fries\", so that you know what it includes.");
						uim.skipLines(1);
						uim.leftAllignedText("All Meals have a given name, the Tags that are assigned to them, and their price.");
						uim.leftAllignedText("If you want to stop a Meal from sppearing in a Meal Plan, then you can also DISABLE them.");
						uim.leftAllignedText("Note that DISABLED Meals will never show up in Meal Plans unless you ENABLE them.");
						uim.leftAllignedText("All of these attributes can be changed in the Meal Menu.");
						uim.display();
						break;
					case 3: // Tag info
						uim.centeredText("Tag Information");
						uim.skipLines(2);
						uim.leftAllignedText("Tags are catagories as vague or specific as you want them to be.");
						uim.leftAllignedText("Use Tags to group meals, or assign very difficult/rare meals to a specific Tag.");
						uim.leftAllignedText("For example, You can create a Tag for a type of food (sandwiches, soups, etc.)");
						uim.leftAllignedText("And you can also create a Tag for a main ingredient (or lack of one) e.g. Chicken, Beef, Vegetarian, etc.");
						uim.skipLines(1);
						uim.leftAllignedText("Having guests over? Assign Meals suitable for many people by creating a \"Guests\" or \"Large Meal\" Tag.");
						uim.leftAllignedText("Then set the Tag to enable only on the day(s) of the week you'll have guests over.");
						uim.leftAllignedText("Tags can only be enabled/disabled according to the day of the week (Monday, Tuesday, ..., Saturday, Sunday).");
						uim.leftAllignedText("So if you'll have guests only for a single day, then you'll need to generate a Meal Plan for that specific week.");
						uim.skipLines(1);
						uim.leftAllignedText("Tags can be set to \"Rely on MultiTag\"");
						uim.leftAllignedText("When this is set, Meals assigned to the Tag will NOT be used for Meal Plans, UNLESS a MuliTag uses the Meal.");
						uim.leftAllignedText("This is always true, even if the Tag allows Meals to occurr on days of the week.");
						uim.leftAllignedText("(see MuliTag manual page for more info.");
						uim.display();
						break;
					case 4: // MultiTag info
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
			uim.skipLines(2);
			uim.centeredText("Thanks for using this program, here's a cool galaxy:");
			uim.leftAllignedText(CREDITS_IMAGE);
			uim.display();
			break;
		default:
			tempInt = 4;
		}
	}// WHILE (tempInt != 4)

	// save data before exit
	mealManager.saveState();
	return 0;
}