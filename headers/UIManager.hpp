#ifndef __UIMANAGER_HPP__
#define __UIMANAGER_HPP__

#include <string>
#include <vector>
#include <iostream>


/* 
* <<<<<<< UIManager >>>>>>>
* Copied from UCR Winter 2021 CS100 final project
* Created by Noor Alaghbar for UCR CS100 Class Project
* Adapted for Personal Use
* 
* ------- PARAMS -------
* bodyBuffer: 
*	treated as a queue, stores strings to print into the body portion of the screen output, 
*	
*	new strings are pushed to the back. enqueue print operations using BODY OPERATIONS methods, 
*	can print centered, then left-alligned, then centered text.
* 
*	strings are output in the order that BODY OPERATIONS were called in.
*	
* promptBuffer:
*	similar to bodyBuffer, stores strings to print.
*	NOTE: only the latest PROMPT OPERATIONS method is used, you cannot queue different prompt types
* 
* screenWidth:  sets screen size during output, minVal = 37, max = 250
* screenHeight: sets screen size during output, minval = 5, max = 250
* 
* lowerLimit: used for prompt_FreeInt, minimum acceptable input value
* upperLimit: used for prompt_FreeInt, maximum acceptable input value
* 
* inputLength: used for prompt_FreeString, maximum acceptable input string length
* 
* promptType: internal use, lets UIManager know how to handle output/verification
* 
* 
* ------- HELPER FUNCTIONS -------
* resetConfiguration: sets params to default state, called within a successful display()
* 
* printBody(const int& availableHeight): 
*	prints body section of an output screen, uses availableHeight to know how many lines to fill
*
* printPrompt: prints prompt section of an output screen
* 
* printCenteredText(string input, int width):     prints input with center allignment, uses width to format the output
* printLeftAllignedText(string input, int width): prints input string with left allignment, uses witdth to format the output
* 
* string validateInput(bool isValid): 
*	gets and validates user's input according to promptType, modifies isValid and returns valid/invalid input
*
* 
* ------- BODY OPERATIONS -------
* centeredText(vector<string> textBlock): queues strings within textBlock.
*	DO NOT INCLUDE '\n' WITHIN STRINGS, THIS IS HANDLED AUTOMATICALLY
*
* centeredText(string newLine): same as above, but for a single string 
* 
* leftAllignedText(vector<string> textBlock): similar to centeredText but for left allignment
* leftAllignedText(string newLine): same as above, but for single string
* 
* skipLines(int linesToSkip): 
*	queues one or more newline operations, intended to use this over loading multiple strings containing '\n'
* 
* 
* ------- PROMPT OPERATIONS -------
* prompt_List(vector<string> prompts):
*	displays a list of prompts for the user that look like: [<key>] - description
*	at the bottom of the window. <key> is a single char that the user must type to choose an option.
*	strings within prompts vector should follow this format: <key_char><description_text>.
*	example, to show the user "[1] - choice one", the string must look like: "1choice one".
* 
*	you may use characters and symbols as the <key>, they will be validated accordingly during output
*	if <key> is a character, then user's input is case sensitive
*	if the vector prompts is empty, UIManager will handle as if prompt_None() was called
* 
*	if one of prompt's strings is "", then it will be treated as a prompt_skipLine(1)
* 
*	The passed in vector is cleared of data after processing.
* 
* prompt_List_Case_Insensitive(vector<string> prompts):
*	same as above, but character <keys> are case insensitive.
* 
* prompt_List_Case_Insensitive(string choice):
*	adds choice to promptBuffer, does not clear promptBuffer before adding choice
*	therefore, it is possible to create many prompt choices by repeatedly calling this function
* 
*	choices must follow the format of Prompt_List and prompt_List_Case_Insensitive
* 
* prompt_FreeInt(int min, int max):
*	lets the user type freely, accepted values are integers between and including min and max
*		if they type an empty string, it is regarded as an incorrect input
*		if they type "10abc", it will be regarded as the integer 10
*		if they type "abc10", is is regarded as an incorrect input
* 
* * prompt_FreeDouble(double min, double max):
*	lets the user type freely, accepted values are doubles between and including min and max
*		if they type an empty string, it is regarded as an incorrect input
*		if they type "10.20abc", it will be regarded as the double 10.20
*		if they type "abc10", is is regarded as an incorrect input
* 
* prompt_FreeString(unsigned int minLength, unsigned int maxLength):
*	lets the user type freely, as long as their input length is >= minLength and <= maxLength
* 
* prompt_SkipLines(unsigned int linesToSkip):
*	similar to skipLines, creates an empty line in the prompt section
*	used to space user choices when using prompt_List
* 
* prompt_None: tells the user to "(press <enter> to continue)" and waits for newline input
* 
* 
* ------- OTHER OPERATIONS -------
* setDimensions(int width, int height): sets character dimensions of the output window
*
* unsigned int getHeight(): returns screenHeight
* unsigned int getWidth(): returns screenWidth
*
* 
* string display(ostream, istream): 
*	outputs body text and prompt text to user with correct formatting,
*	then gets user input according to the prompt type that was selected most recently.
* 
*	ostream and istream are used for testing, using no params defaults to cout and cin, respectively.
* 
*	user's input is then verified. 
*		if input is not valid, an error will display and the window will re-display
*		if input is valid, function returns string containing user's input
*	if you did not perform a PROMPT OPERATIONS method before calling display(), 
*	then the default prompt type is prompt_None()
* 
* 
* 
* >>>> FORMAT REFERENCE <<<<
* output windows have borders surrounding it
* when specifying a screen width of 20, there are 18 usable spaces, 2 are used for borders
* 
* prompts will always end at the bottom of the screen (but above the border line)
* ------------------------------
* |		  CENTERED TEXT		   |		
* |left-alligned text		   |
* |							   |
* |							   |
* |							   |
* |[1] - prompt_list_choice1   |
* |[A] - prompt_list_choice2   |
* |[m] - prompt_list_choice3   |
* ------------------------------
* 
* 
* NOTE: 
*	='s indicate the space where BODY OPERATIONS apply
*	<'s indicates an empty line, this cannot be removed from the output,
*		and it always occurs just above the start of the prompts
* 
* ------------------------------
* |============================|		
* |============================|
* |============================|
* |============================|
* |<<<<<<<<<<<<<<<<<<<<<<<<<<<<|
* |[1] - prompt_list_choice1   |
* |[A] - prompt_list_choice2   |
* |[m] - prompt_list_choice3   |
* ------------------------------
* 
* to calculate how much room you have for BODY OPERATIONS, it is:
*		(screenHeight - 3 - <number of strings contained in prompt>)
* 
* if prompt selected was NOT prompt_List, then <number of strings contained in prompt> ALWAYS = 1

* 
*/
class UIManager
{
	enum Prompt
	{
		UNSET,
		PROMPT_NONE,
		PROMPT_FREEINT,
		PROMPT_FREEDOUBLE,
		PROMPT_FREESTRING,
		PROMPT_LIST,
		PROMPT_LIST_CASE_INSENSITIVE
	};


	std::vector<std::string> bodyBuffer;
	std::vector<std::string> promptBuffer;
	int screenWidth;
	int screenHeight;
	double lowerLimit;
	double upperLimit;
	unsigned int minInputLength;
	unsigned int maxInputLength;
	Prompt promptType;

	// HELPER FUNCTIONS
	void resetConfiguration();
	void printBody(const int& availableHeight, std::ostream& out);
	void printPrompt(std::ostream& out);
	void printCenteredText(const std::string& input, const int& width, std::ostream& out);
	void printLeftAllignedText(const std::string& input, const int& width, std::ostream& out);
	std::string validateInput(bool& isValid, std::ostream& out, std::istream& in);
	

public:
	UIManager();
	~UIManager();

	// BODY OPERATIONS
	void centeredText(const std::vector<std::string>& textBlock);
	void centeredText(const std::string& newLine); 
	void leftAllignedText(const std::vector<std::string>& textBlock);
	void leftAllignedText(const std::string& newLine); 
	void skipLines(const unsigned int& linesToSkip); 

	// PROMPT OPERATIONS
	void prompt_List(std::vector<std::string>& prompts); 
	void prompt_List_Case_Insensitive(std::vector<std::string>& prompts);
	void prompt_List_Case_Insensitive(const std::string& choice);
	void prompt_FreeInt(const int& min, const int& max); 
	void prompt_FreeDouble(const double& min, const double& max);
	void prompt_FreeString(const unsigned int& minLength, const unsigned int& maxLength);
	void prompt_skipLines(const unsigned int& linesToSkip);
	void prompt_None();

	// OTHER OPERATIONS
	void setDimensions(const unsigned int& width, const unsigned int& height);
	unsigned int getHeight() const;
	unsigned int getWidth() const;
	std::string display(std::ostream& out = std::cout, std::istream& in = std::cin);
};

#endif // !__UIMANAGER_HPP__
