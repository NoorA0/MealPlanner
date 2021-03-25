/*
* Copied from UCR Winter 2021 CS100 final project
* Created by Noor Alaghbar for UCR CS100 Class Project
* Adapted for Personal Use
*/

#include "../headers/UIManager.hpp"

/*INSERTION CODES LIST - INTERNAL USE ONLY

first 4 chars of a string are reserved for determining format type 

0CT_ - CENTERED TEXT
1LT_ - LEFT ALLIGNED TEXT
2SL_ - SKIP LINE
*/

UIManager::UIManager()
{
	promptType = UNSET;
	screenWidth = 37;
	screenHeight = 5;
	lowerLimit = 0;
	upperLimit = 0;
	minInputLength = 0;
	maxInputLength = 0;
}

UIManager::~UIManager()
{
}

void UIManager::resetConfiguration()
{
	bodyBuffer.clear();
	promptBuffer.clear();
	lowerLimit = 0;
	upperLimit = 0;
	minInputLength = 0;
	maxInputLength = 0;
	promptType = UNSET;
}

void UIManager::printBody(const int& availableHeight, std::ostream& out)
{
	int lineCount = 0; // counts number of line output

	//pt1: while buffer needs print
	auto bufferIter = bodyBuffer.begin();
	// makes sure printing buffer does not overflow page, if it does, then stops output
	while (bufferIter != bodyBuffer.end() && lineCount < availableHeight)
	{
		out << "|";
		// grab 4 chars for format type
		std::string format = bufferIter->substr(0, 4);

		//check format type
		if (format == "0CT_") // centered
		{
			printCenteredText(bufferIter->substr(4), screenWidth - 2, out);
		}
		else if (format == "1LT_" || format == "2SL_") // left-alligned/skip line
		{
			printLeftAllignedText(bufferIter->substr(4), screenWidth - 2, out);
		}
		else // case if format code was not found, default to leftAlligned
		{
			printLeftAllignedText(*bufferIter, screenWidth - 2, out);
		}
		out << "|" << std::endl;
		bufferIter++;
		lineCount++;
	}//END WHILE

	//pt2: buffer is printed, now fill space
	for (lineCount; lineCount < availableHeight; lineCount++)
	{
		out << "|";
		//fill empty spaces
		for (int index = 0; index < screenWidth - 2; index++)
		{
			out << " ";
		}
		out << "|" << std::endl;
	}
}

void UIManager::printPrompt(std::ostream& out)
{
	// prompt format depends on prompt type that was selected
	switch (promptType)
	{
	case PROMPT_NONE:
	case PROMPT_FREEINT:
	case PROMPT_FREESTRING:
	case PROMPT_FREEDOUBLE:
		out << "|";
		printLeftAllignedText(promptBuffer.at(0), screenWidth - 2, out);
		out << "|\n";
		break;
	case PROMPT_LIST:
	case PROMPT_LIST_CASE_INSENSITIVE:
		// output a list, using 1st char in string as the key
		for (auto promptIter = promptBuffer.begin(); promptIter != promptBuffer.end(); promptIter++)
		{
			out << "|";
			// grab first 4 chars for format type
			std::string format = promptIter->substr(0, 4);

			// check format type 
			if (format == "2SL_") // skip line
			{
				printLeftAllignedText(format.substr(4), screenWidth - 2, out);
			}
			else // no other format types to check
			{
				// construct string for prompt
				std::string tempStr = "[" + promptIter->substr(0, 1) + "] - " + promptIter->substr(1);

				// print prompt left-alligned
				printLeftAllignedText(tempStr, screenWidth - 2, out);
			}
			out << "|\n";
		}
		break;
	default: out << "(enter a value)\n";
	}// END SWITCH
}

void UIManager::printCenteredText(const std::string& input, const int& width, std::ostream& out)
{
	int emptySpaces; // number of empty spaces needed to center text

	// calculate starting point
	emptySpaces = (width / 2) - (input.length() / 2);

	// place empty spaces before text
	for (int index = 0; index < emptySpaces; index++)
		out << " ";
	out << input;
	emptySpaces += input.length(); // use emptySpaces as an index

	// place empty spaces after text
	for (emptySpaces; emptySpaces < width; emptySpaces++)
		out << " ";
}

void UIManager::printLeftAllignedText(const std::string& input, const int& width, std::ostream& out)
{
	int remainingWidth = width - input.length();
	out << input;

	//fill empty spaces
	for (int index = 0; index < remainingWidth; index++)
		out << " ";
}

std::string UIManager::validateInput(bool& isValid, std::ostream& out, std::istream& in)
{
	std::string userInput = ""; // stores user's input
	isValid = false;

	out << ':'; // prompts the user
	std::getline(in, userInput); // catch input
	
	// validates based on promptType
	switch (promptType)
	{
	case PROMPT_NONE:
		// don't care what it was
		isValid = true;
		break;
	case PROMPT_FREEINT:
		// check if empty string
		if (userInput.length() > 0)
		{
			try
			{
				// convert userInput into integer and validate
				if (std::stoi(userInput, nullptr, 10) >= int(lowerLimit) &&
					std::stoi(userInput, nullptr, 10) <= int(upperLimit))
					isValid = true;
			}
			catch (const std::invalid_argument& exception)
			{
				// bad conversion: userInput does not begin with integer. regard as false match
			}
		}

		// if input invalid due to 0 length or failed conversion
		if (!isValid)
		{
			out << "Invalid input! Enter a value between " << int(lowerLimit)
				<< " and " << int(upperLimit) << ".\n(Press <enter> to continue)\n";
			std::getline(in, userInput);
		}
		break;
	case PROMPT_FREEDOUBLE:
		// check if empty string
		if (userInput.length() > 0)
		{
			try
			{
				// convert userInput into integer and validate
				if (std::stod(userInput) >= lowerLimit &&
					std::stod(userInput) <= upperLimit)
					isValid = true;
			}
			catch (const std::invalid_argument& exception)
			{
				// bad conversion: userInput does not begin with integer. regard as false match
			}
		}

		// if input invalid due to 0 length or failed conversion
		if (!isValid)
		{
			out << "Invalid input! Enter a value between " << lowerLimit
				<< " and " << upperLimit << ".\n(Press <enter> to continue)\n";
			std::getline(in, userInput);
		}

		break;
	case PROMPT_FREESTRING:
		if (userInput.length() >= minInputLength && userInput.length() <= maxInputLength)
			isValid = true;
		else if (userInput.length() < minInputLength)
		{
			out << "Invalid input! Your input was too short.\n(Press <enter> to continue)\n";
			std::getline(in, userInput);
		}
		else
		{
			out << "Invalid input! Your input was too long.\n(Press <enter> to continue)\n";
			std::getline(in, userInput);
		}
		break;
	case PROMPT_LIST:
		// check if user's input is of correct length
		if (userInput.length() == 1)
		{
			// generate a list of keys which are mapped to the user's available choices
			std::vector<char> keys;
			for (auto promptIter = promptBuffer.begin(); promptIter != promptBuffer.end(); promptIter++)
			{
				keys.push_back(promptIter->at(0));
			}

			// loop through keys to check if input is valid
			auto keysIter = keys.begin();
			while (!isValid && keysIter != keys.end())
			{
				// if key is an integer character (casts to int)
				if (*keysIter <= 57 && *keysIter >= 48)
				{
					// compare userInput's 1st char to *keysIter
					if (userInput.at(0) == *keysIter)
						isValid = true;
				}
				else // compare chars
				{
					if (userInput.at(0) == *keysIter)
						isValid = true;
				}
				keysIter++;
			}// END WHILE
			// if no matches, tell user to retry
			if (!isValid)
			{
				out << "Invalid input! Please try again.\n(Press <enter> to continue)\n";
				std::getline(in, userInput);
			}
		}// END IF
		else if (userInput.length() == 0) // tell user that input was empty
		{
			out << "Invalid input! Your didn't choose anything.\n(Press <enter> to continue)\n";
			std::getline(in, userInput);
		}
		else // tell user that their unput was too long
		{
			out << "Invalid input! Your input was too long.\n(Press <enter> to continue)\n";
			std::getline(in, userInput);
		}
		break;
	case PROMPT_LIST_CASE_INSENSITIVE:
		// check if user's input is of correct length
		if (userInput.length() == 1)
		{
			// generate a list of keys which are mapped to the user's available choices
			std::vector<char> keys;
			for (auto promptIter = promptBuffer.begin(); promptIter != promptBuffer.end(); promptIter++)
			{
				// convert key to uppercase (no effect if key is a number)
				keys.push_back(std::toupper(promptIter->at(0)));
			}

			// loop through keys to check if input is valid
			auto keysIter = keys.begin();
			while (!isValid && keysIter != keys.end())
			{
				// if key is an integer character (casts to int)
				if (*keysIter <= 57 && *keysIter >= 48)
				{
					// compare userInput's 1st char to *keysIter
					if (userInput.at(0) == *keysIter)
						isValid = true;
				}
				else // compare chars
				{
					if (std::toupper(userInput.at(0)) == *keysIter)
						isValid = true;
				}
				keysIter++;
			}// END WHILE
			// if no matches, tell user to retry
			if (!isValid)
			{
				out << "Invalid input! Please try again.\n(Press <enter> to continue)\n";
				std::getline(in, userInput);
			}
		}// END IF
		else if (userInput.length() == 0) // tell user that input was empty
		{
			out << "Invalid input! Your didn't choose anything.\n(Press <enter> to continue)\n";
			std::getline(in, userInput);
		}
		else // tell user that their unput was too long
		{
			out << "Invalid input! Your input was too long.\n(Press <enter> to continue)\n";
			std::getline(in, userInput);
		}
		break;
	default: isValid = false;
	}
	return userInput;
}

void UIManager::centeredText(const std::vector<std::string>& textBlock)
{
	// modify strings in textBlock to include format code
	for (auto& iter : textBlock)
	{
		std::string modifyStr = "0CT_" + iter;
		bodyBuffer.push_back(modifyStr);
	}
}

void UIManager::centeredText(const std::string& newLine)
{
	// modify newLine to include format code
	std::string modifyStr = "0CT_" + newLine;
	bodyBuffer.push_back(modifyStr);
}

void UIManager::leftAllignedText(const std::vector<std::string>& textBlock)
{
	// modify strings in textBlock to include format code
	for (auto& iter : textBlock)
	{
		std::string modifyStr = "1LT_" + iter;
		bodyBuffer.push_back(modifyStr);
	}
}

void UIManager::leftAllignedText(const std::string& newLine)
{
	// modify newLine to include format code
	std::string modifyStr = "1LT_" + newLine;
	bodyBuffer.push_back(modifyStr);
}

void UIManager::skipLines(const unsigned int& linesToSkip)
{
	// create entry in bodyBuffer to skip line
	for (unsigned int insertSkip = 0; insertSkip < linesToSkip; insertSkip++)
	{
		std::string tempStr = "2SL_";
		bodyBuffer.push_back(tempStr);
	}
}

/* ==== UIManager::prompt_List ====
	Each element in the vector is considered ONE choice. Format is as follows:
		to display: [1] - optionOneDesc
		string looks like: 1optionOneDesc

	The first char in the string is placed within brackets to indicate to the player what key they should use
		so to display: [A] - thisOption
					   [R] - that Option
					   [3] - yet another option

		the strings loaded in the vector look like: AthisOption
													Rthat Option
													3yet another option

	since the first char indicates an option's corresponding key, the UIManager knows all of the valid
	 values that can be entered, thus it can validate input easily.

	 The passed vector is cleared after processing, to prevent reusing choices accidently.
*/
void UIManager::prompt_List(std::vector<std::string>& prompts)
{
	// check if prompts has anything, else default to prompt_None()
	if (prompts.size() > 0)
	{
		promptType = PROMPT_LIST;
		promptBuffer.clear();
		// load strings in vector onto prompt vector
		for (auto& promptsIter : prompts)
		{
			promptBuffer.push_back(promptsIter);
		}
		prompts.clear();
	}
	else
		prompt_None();
}

void UIManager::prompt_List_Case_Insensitive(std::vector<std::string>& prompts)
{
	// check if prompts has anything, else default to prompt_None()
	if (prompts.size() > 0)
	{
		promptType = PROMPT_LIST_CASE_INSENSITIVE;
		promptBuffer.clear();
		// load strings in vector onto prompt vector
		for (auto& promptsIter : prompts)
		{
			promptBuffer.push_back(promptsIter);
		}
		prompts.clear();
	}
	else
		prompt_None();
}

void UIManager::prompt_List_Case_Insensitive(const std::string& choice)
{
	// check if choice has anything, else default to prompt_None()
	if (choice.length() > 0)
	{
		promptType = PROMPT_LIST_CASE_INSENSITIVE;
		// add choice to promptBuffer
		promptBuffer.push_back(choice);
	}
	else
		prompt_None();
}

void UIManager::prompt_FreeInt(const int& min, const int& max)
{
	promptType = PROMPT_FREEINT;
	lowerLimit = min;
	upperLimit = max;
	promptBuffer.clear(); // clear buffer to insert prompt
	std::string tempStr = "(Enter a value between " + std::to_string(int(lowerLimit)) +
						  " and " + std::to_string(int(upperLimit)) + ")";

	promptBuffer.push_back(tempStr);
}

void UIManager::prompt_FreeDouble(const double& min, const double& max)
{
	promptType = PROMPT_FREEDOUBLE;
	lowerLimit = min;
	upperLimit = max;
	std::string lowerStr; // string of lowerLimit
	std::string upperStr; // string of upperLimit
	promptBuffer.clear(); // clear buffer to insert prompt

	// format lowerLimit for output
	// convert to string
	lowerStr = std::to_string(lowerLimit);

	// erase trailing zeros, leaves '.' at end of string
	lowerStr.erase(lowerStr.find_last_not_of("0") + 1, std::string::npos);

	// erase '.' if exists
	if (lowerStr[lowerStr.length() - 1] == '.')
		lowerStr.pop_back();

	// format upperLimit for output
	// convert to string
	upperStr = std::to_string(upperLimit);

	// erase trailing zeros, leaves '.' at end of string
	upperStr.erase(upperStr.find_last_not_of("0") + 1, std::string::npos);

	// erase '.' if exists
	if (upperStr[upperStr.length() - 1] == '.')
		upperStr.pop_back();

	std::string tempStr = "(Enter a value between " + lowerStr + " and " + upperStr + ")";

	promptBuffer.push_back(tempStr);
}

void UIManager::prompt_FreeString(const unsigned int& minLength, const unsigned int& maxLength)
{
	promptType = PROMPT_FREESTRING;
	minInputLength = minLength;
	maxInputLength = maxLength;
	promptBuffer.clear();
	promptBuffer.push_back("(Enter between " + std::to_string(minInputLength) + " and " + std::to_string(maxInputLength) + " characters)");
}

void UIManager::prompt_skipLines(const unsigned int& linesToSkip)
{
	// create entry in promptBuffer to skip line
	for (unsigned int insertSkip = 0; insertSkip < linesToSkip; insertSkip++)
	{
		std::string tempStr = "2SL_";
		promptBuffer.push_back(tempStr);
	}
}

void UIManager::prompt_None()
{
	promptType = PROMPT_NONE;
	promptBuffer.clear();
	promptBuffer.push_back("(Press <enter> to continue)");
}

void UIManager::setDimensions(const unsigned int& width, const unsigned int& height)
{
	if (width <= 250 && width >= 37)
		screenWidth = width;
	if (height <= 250 && height >= 5)
		screenHeight = height;
}

unsigned int UIManager::getHeight() const
{
	return screenHeight;
}

unsigned int UIManager::getWidth() const
{
	return screenWidth;
}

std::string UIManager::display(std::ostream& out, std::istream& in)
{
	int availBodyHeight; // available height to print bodyBuffer
	bool validInput = false; // determines if user's input was valid
	std::string userInput;

	do // display's screen of info to user, gets input. repeats until input is valid
	{
		// print top border of window
		for (int index = 0; index < screenWidth; index++)
			out << "-";
		out << '\n';

		// calc available buffer height
		availBodyHeight = screenHeight - 2; // does NOT include space for prompts
		switch (promptType)
		{
		case UNSET:
			// set to prompt_None
			prompt_None();
		case PROMPT_NONE:
		case PROMPT_FREEINT:
		case PROMPT_FREEDOUBLE:
		case PROMPT_FREESTRING:
			availBodyHeight -= 1; // need room to tell user to press enter, or to enter values
			break;
		case PROMPT_LIST:
		case PROMPT_LIST_CASE_INSENSITIVE:
			availBodyHeight -= promptBuffer.size(); // needs a line per entry in promptBuffer
			break;
		default:
			availBodyHeight -= promptBuffer.size(); // hope for the best
		}

		// add extra space between end of body and beginning of prompt
		skipLines(1);

		// print body
		printBody(availBodyHeight, out);

		// print prompt
		printPrompt(out);

		// bottom border
		for (int index = 0; index < screenWidth; index++)
			out << "-";
		out << '\n';

		//PROCESS INPUT
		userInput = validateInput(validInput, out, in);

		if (validInput) // input valid, resets config to prevent re-printing same page
			resetConfiguration();
		else // input invalid, makes room to re-print everything
			out << "\n\n";

	} while (!validInput);
	return userInput;
}
