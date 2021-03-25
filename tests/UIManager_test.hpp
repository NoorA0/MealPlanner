/*
* Copied from UCR Winter 2021 CS100 final project
* Created by Noor Alaghbar for UCR CS100 Class Project
* Adapted for Personal Use
*/

#ifndef __UIMANAGER_TEST_HPP__
#define __UIMANAGER_TEST_HPP__

#include "../headers/UIManager.hpp"



TEST(UIManagerTest, DefaultConstructor_test)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("123\n"); 

	std::string testReturn, expectedReturn;
	expectedReturn = "123"; // omits \n as it is not passed to testReturn

	testReturn = uim.display(oStream, iStream);

	EXPECT_EQ(testReturn, expectedReturn); // ensure input passed correctly
}

TEST(UIManagerTest, setDimensions_invalidDimensions)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(28, 3); // too small, should default to 37x5 window
	uim.prompt_None();
	uim.display(oStream, iStream);

	std::string reference = "-------------------------------------\n"
							"|                                   |\n"
							"|                                   |\n"
							"|(Press <enter> to continue)        |\n"
							"-------------------------------------\n"
							":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, setDimensions_validDimensions)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(50, 8);
	uim.prompt_None();
	uim.display(oStream, iStream);

	std::string reference = "--------------------------------------------------\n"
							"|                                                |\n"
							"|                                                |\n"
							"|                                                |\n"
							"|                                                |\n"
							"|                                                |\n"
							"|(Press <enter> to continue)                     |\n"
							"--------------------------------------------------\n"
							":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, setDimensions_invalidWidth)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(-1, 8); // unsigned -1 becomes very large, should default to 37
	uim.prompt_None();
	uim.display(oStream, iStream);

	std::string reference = "-------------------------------------\n"
		"|                                   |\n"
		"|                                   |\n"
		"|                                   |\n"
		"|                                   |\n"
		"|                                   |\n"
		"|(Press <enter> to continue)        |\n"
		"-------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, setDimensions_invalidHeight)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(40, 3); //minHeight = 5
	uim.prompt_None();
	uim.display(oStream, iStream);

	std::string reference = "----------------------------------------\n"
		"|                                      |\n"
		"|                                      |\n"
		"|(Press <enter> to continue)           |\n"
		"----------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

/* How to manually calculate text starting point for centered text:
*
*  1- take screen width, subtract 2, then divide by 2 (truncate if result is a float)
*  2- now take the length of the string you want to center, divide by 2 (truncate if result is a float)
*  3- take result from line 1, subtract it by the result of line 2
*   - this is the position you should start from (not including borders)
*   - add 2 to get an absolute position. in equation form:
*		int startPos = ((screenWidth - 2) / 2) - (string_to_center.length() / 2)
*/
TEST(UIManagerTest, centeredText_single)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(80, 20);
	uim.prompt_None();
	uim.centeredText("I_AM_A_TEST_99");
	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                I_AM_A_TEST_99                                |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Press <enter> to continue)                                                   |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, centeredText_multiple)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(80, 20);
	uim.prompt_None();
	std::vector<std::string> strings =
	{ "I_AM_A_TEST_99", "CENTER", "0", "00", "000" };

	uim.centeredText(strings);
	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                I_AM_A_TEST_99                                |\n"
		"|                                    CENTER                                    |\n"
		"|                                       0                                      |\n"
		"|                                      00                                      |\n"
		"|                                      000                                     |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Press <enter> to continue)                                                   |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, centeredText_unevenScreenWidth)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(79, 20);
	uim.prompt_None();
	uim.centeredText("I_AM_A_TEST_99");
	uim.display(oStream, iStream);

	std::string reference =
		"-------------------------------------------------------------------------------\n"
		"|                               I_AM_A_TEST_99                                |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|(Press <enter> to continue)                                                  |\n"
		"-------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, centeredText_unevenScreenWidthAndTextLength)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(79, 20);
	uim.prompt_None();
	uim.centeredText("I_AM_A_TEST");
	uim.display(oStream, iStream);

	std::string reference =
		"-------------------------------------------------------------------------------\n"
		"|                                 I_AM_A_TEST                                 |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|(Press <enter> to continue)                                                  |\n"
		"-------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, leftAllignedText_single)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(79, 20);
	uim.prompt_None();
	uim.leftAllignedText("TESTME");
	uim.display(oStream, iStream);

	std::string reference =
		"-------------------------------------------------------------------------------\n"
		"|TESTME                                                                       |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|(Press <enter> to continue)                                                  |\n"
		"-------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, leftAllignedText_multiple)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(79, 20);
	uim.prompt_None();
	std::vector<std::string> strings =
	{ "))(())(())(())(())((","LINE TWO", "FOO", "bar0", "somethingsomething" };
	
	uim.leftAllignedText(strings);
	uim.display(oStream, iStream);

	std::string reference =
		"-------------------------------------------------------------------------------\n"
		"|))(())(())(())(())((                                                         |\n"
		"|LINE TWO                                                                     |\n"
		"|FOO                                                                          |\n"
		"|bar0                                                                         |\n"
		"|somethingsomething                                                           |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|(Press <enter> to continue)                                                  |\n"
		"-------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, skipLines)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(79, 20);
	uim.prompt_None();
	uim.skipLines(1);
	uim.leftAllignedText("TESTME");
	uim.skipLines(2);
	uim.leftAllignedText("_what_am_i");
	uim.display(oStream, iStream);

	std::string reference =
		"-------------------------------------------------------------------------------\n"
		"|                                                                             |\n"
		"|TESTME                                                                       |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|_what_am_i                                                                   |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|                                                                             |\n"
		"|(Press <enter> to continue)                                                  |\n"
		"-------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, combinedBodyOperations)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(80, 20);
	uim.prompt_None();
	uim.skipLines(1);
	uim.centeredText("_what_am_i");
	uim.skipLines(2);
	uim.leftAllignedText("who_are_you_");
	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                  _what_am_i                                  |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|who_are_you_                                                                  |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Press <enter> to continue)                                                   |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_empty_success1)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts;
	uim.prompt_List(prompts);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Press <enter> to continue)                                                   |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_empty_success2)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream(" \n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts;
	prompts.push_back(" ");
	uim.prompt_List(prompts);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[ ] -                                                                         |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_outputFailures1)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("q\n\nQ\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "1choice one", "2choice two", "QQuit game" };

	uim.prompt_List(prompts);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - choice one                                                              |\n"
		"|[2] - choice two                                                              |\n"
		"|[Q] - Quit game                                                               |\n"
		"--------------------------------------------------------------------------------\n"
		":Invalid input! Please try again.\n(Press <enter> to continue)\n\n\n"
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - choice one                                                              |\n"
		"|[2] - choice two                                                              |\n"
		"|[Q] - Quit game                                                               |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_outputFailures2)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("11\n\n2\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "1choice one", "2choice two", "QQuit game" };

	uim.prompt_List(prompts);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - choice one                                                              |\n"
		"|[2] - choice two                                                              |\n"
		"|[Q] - Quit game                                                               |\n"
		"--------------------------------------------------------------------------------\n"
		":Invalid input! Your input was too long.\n(Press <enter> to continue)\n\n\n"
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - choice one                                                              |\n"
		"|[2] - choice two                                                              |\n"
		"|[Q] - Quit game                                                               |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_empty_success1)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts;
	uim.prompt_List_Case_Insensitive(prompts);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Press <enter> to continue)                                                   |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_empty_success2)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream(" \n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts;
	prompts.push_back(" ");
	uim.prompt_List_Case_Insensitive(prompts);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[ ] -                                                                         |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_outputFailures1)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("f\n\n1\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "1choice one", "2choice two", "QQuit game" };

	uim.prompt_List_Case_Insensitive(prompts);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - choice one                                                              |\n"
		"|[2] - choice two                                                              |\n"
		"|[Q] - Quit game                                                               |\n"
		"--------------------------------------------------------------------------------\n"
		":Invalid input! Please try again.\n(Press <enter> to continue)\n\n\n"
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - choice one                                                              |\n"
		"|[2] - choice two                                                              |\n"
		"|[Q] - Quit game                                                               |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_outputFailures2)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("11\n\n2\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "1choice one", "2choice two", "QQuit game" };

	uim.prompt_List_Case_Insensitive(prompts);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - choice one                                                              |\n"
		"|[2] - choice two                                                              |\n"
		"|[Q] - Quit game                                                               |\n"
		"--------------------------------------------------------------------------------\n"
		":Invalid input! Your input was too long.\n(Press <enter> to continue)\n\n\n"
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - choice one                                                              |\n"
		"|[2] - choice two                                                              |\n"
		"|[Q] - Quit game                                                               |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_inputValidation_correctInput)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	std::string expected, userInput;
	expected = "Q";
	iStream = std::stringstream("Q\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "1choice one", "2choice two", "QQuit game" };

	uim.prompt_List(prompts);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_List_inputValidation_correctInput2)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	std::string expected, userInput;
	expected = "a";
	iStream = std::stringstream("a\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "1choice one", "achoice two", "QQuit game" };

	uim.prompt_List(prompts);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_List_inputValidation_badInput)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	std::string expected, userInput;
	expected = "#";
	iStream = std::stringstream("1\n\nE\n\n#\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "#choice one", "2choice two", "QQuit game" };

	uim.prompt_List(prompts);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_List_inputValidation_badInput2)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	std::string expected, userInput;
	expected = "Q";
	iStream = std::stringstream("q\n\nQ\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "#choice one", "2choice two", "QQuit game" };

	uim.prompt_List(prompts);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_inputValidation_correctInput)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	std::string expected, userInput;
	expected = "q";
	iStream = std::stringstream("q\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "1choice one", "2choice two", "QQuit game" };

	uim.prompt_List_Case_Insensitive(prompts);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_inputValidation_correctInput2)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	std::string expected, userInput;
	expected = "1";
	iStream = std::stringstream("1\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "1choice one", "achoice two", "QQuit game" };

	uim.prompt_List_Case_Insensitive(prompts);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_inputValidation_badInput)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	std::string expected, userInput;
	expected = "#";
	iStream = std::stringstream("1\n\nE\n\n#\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "#choice one", "2choice two", "QQuit game" };

	uim.prompt_List_Case_Insensitive(prompts);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_inputValidation_badInput2)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	std::string expected, userInput;
	expected = "Q";
	iStream = std::stringstream("m\n\nQ\n");

	uim.setDimensions(80, 20);
	std::vector<std::string> prompts =
	{ "#choice one", "2choice two", "QQuit game" };

	uim.prompt_List_Case_Insensitive(prompts);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_FreeInt_outputSuccess)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("5\n");

	uim.setDimensions(80, 20);
	uim.prompt_FreeInt(0, 10);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter a value between 0 and 10)                                              |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_FreeInt_outputFailures)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("-1\n\n22\n");

	uim.setDimensions(80, 20);
	uim.prompt_FreeInt(0, 255);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter a value between 0 and 255)                                             |\n"
		"--------------------------------------------------------------------------------\n"
		":Invalid input! Enter a value between 0 and 255.\n"
		"(Press <enter> to continue)\n\n\n"
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter a value between 0 and 255)                                             |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_FreeInt_negativeInputSuccess)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("-100\n");

	std::string expected, userInput;
	expected = "-100";

	uim.setDimensions(80, 20);
	uim.prompt_FreeInt(-200, -99);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_FreeInt_inputFailures)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("-100\n\n70\n\n0");

	std::string expected, userInput;
	expected = "0";

	uim.setDimensions(80, 20);
	uim.prompt_FreeInt(0, 0);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_FreeInt_inputChar)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("E\n\n0");

	std::string expected, userInput;
	expected = "0";

	uim.setDimensions(80, 20);
	uim.prompt_FreeInt(0, 0);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_FreeDouble_outputSuccess)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("3.99\n");

	uim.setDimensions(80, 20);
	uim.prompt_FreeDouble(1.23, 4.56);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter a value between 1.23 and 4.56)                                         |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_FreeDouble_outputFailures)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("-1\n\n22.7\n");

	uim.setDimensions(80, 20);
	uim.prompt_FreeDouble(0, 255.5);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter a value between 0 and 255.5)                                           |\n"
		"--------------------------------------------------------------------------------\n"
		":Invalid input! Enter a value between 0 and 255.5.\n"
		"(Press <enter> to continue)\n\n\n"
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter a value between 0 and 255.5)                                           |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_FreeDouble_negativeInputSuccess)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("-100.9\n");

	std::string expected, userInput;
	expected = "-100.9";

	uim.setDimensions(80, 20);
	uim.prompt_FreeDouble(-200, -99);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_FreeDouble_inputFailures)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("-100.99\n\n70.1000000\n\n0");

	std::string expected, userInput;
	expected = "0";

	uim.setDimensions(80, 20);
	uim.prompt_FreeDouble(0, 0);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_FreeDouble_inputChar)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("E\n\n0");

	std::string expected, userInput;
	expected = "0";

	uim.setDimensions(80, 20);
	uim.prompt_FreeDouble(0, 0);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_FreeString_outputSuccess)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("so_many_tests_\n");

	uim.setDimensions(80, 20);
	uim.prompt_FreeString(0, 100);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter between 0 and 100 characters)                                          |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_FreeString_outputFailures)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("so_many_tests_\n\n\n\nthisisOk?\n");

	uim.setDimensions(80, 20);
	uim.prompt_FreeString(1, 10);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter between 1 and 10 characters)                                           |\n"
		"--------------------------------------------------------------------------------\n"
		":Invalid input! Your input was too long.\n"
		"(Press <enter> to continue)\n\n\n"
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter between 1 and 10 characters)                                           |\n"
		"--------------------------------------------------------------------------------\n"
		":Invalid input! Your input was too short.\n"
		"(Press <enter> to continue)\n\n\n"
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|(Enter between 1 and 10 characters)                                           |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_FreeString_inputSuccess)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("\n");

	std::string expected, userInput;
	expected = "";

	uim.setDimensions(80, 20);
	uim.prompt_FreeString(0, 50);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_FreeString_inputFailures)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("tooloooooooooooooong\n\n#just right\n");

	std::string expected, userInput;
	expected = "#just right";

	uim.setDimensions(80, 20);
	uim.prompt_FreeString(1, 11);

	userInput = uim.display(oStream, iStream);

	EXPECT_EQ(expected, userInput);
}

TEST(UIManagerTest, prompt_skipLines)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("1\n");

	std::vector<std::string> inputs = { "1One", "2Two" };
	uim.prompt_List(inputs);
	uim.prompt_skipLines(3);

	uim.setDimensions(80, 20);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - One                                                                     |\n"
		"|[2] - Two                                                                     |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, prompt_List_Case_Insensitive_StringVer)
{
	UIManager uim;
	std::stringstream iStream, oStream;
	iStream = std::stringstream("Q\n");

	uim.prompt_List_Case_Insensitive("1One");
	uim.prompt_List_Case_Insensitive("2Two");
	uim.prompt_skipLines(1);
	uim.prompt_List_Case_Insensitive("QQuit");

	uim.setDimensions(80, 20);

	uim.display(oStream, iStream);

	std::string reference =
		"--------------------------------------------------------------------------------\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|[1] - One                                                                     |\n"
		"|[2] - Two                                                                     |\n"
		"|                                                                              |\n"
		"|[Q] - Quit                                                                    |\n"
		"--------------------------------------------------------------------------------\n"
		":";

	EXPECT_EQ(oStream.str(), reference);
}

TEST(UIManagerTest, getHeight_getWidth)
{
	UIManager uim;
	uim.setDimensions(50, 29);

	EXPECT_EQ(uim.getHeight(), 29);
	EXPECT_EQ(uim.getWidth(), 50);
}

#endif // !__UIMANAGER_TEST_HPP__
