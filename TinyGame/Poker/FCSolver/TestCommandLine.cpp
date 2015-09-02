////////////////////////////////////////////////
///\file TestCommandLine.cpp
///\brief This file contains the implementation of the TestCommandLineArguments class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>
#include "TestCommandLine.h"

#include <iostream>
using std::endl;
using std::cout;

TestCommandLineArguments::TestCommandLineArguments() : FCCommandLineArguments()
{
	Write.open("TestCommandLineArguments.txt");
}

TestCommandLineArguments::~TestCommandLineArguments()
{
	//this isn't deleted in FCCommandLineArguments becuase it will be passed on
	delete m_DebugDisplayInfo;
	Write.close();
}

void TestCommandLineArguments::TestConstructor()
{
	int a;

	Write << "Size of FCCommandLineArguments: " << sizeof(FCCommandLineArguments) << endl;
	Write << "m_DisplayHelp: " << m_DisplayHelp << endl;
	Write << "m_NumberOfFreecells: " << m_NumberOfFreecells << endl;
	Write << "m_NumberOfStacks: " << m_NumberOfStacks << endl;
	Write << "m_NumberOfDecks: " << m_NumberOfDecks << endl;
	if (m_SequencesBuiltBy[0] != NULL)
		Write << "m_SequencesBuiltBy[0] should equal NULL." << endl;
	Write << "m_SequencesBuiltByType: " << m_SequencesBuiltByType << endl;
	if (m_SequenceMoveUnlimited[0] != NULL)
		Write << "m_SequenceMoveUnlimited[0] should equal NULL." << endl;
	Write << "m_IsSequenceMoveUnlimited: " << m_IsSequenceMoveUnlimited << endl;
	if (m_EmptyStacksFilledBy[0] != NULL)
		Write << "m_EmptyStacksFilledBy[0] should equal NULL" << endl;
	Write << "m_EmptyStacksFilledByType: " << m_EmptyStacksFilledByType << endl;
	if (m_GameName[0] != NULL)
		Write << "m_GameName[0] should equal NULL" << endl;
	Write << "m_MaxNumberOfIterations: " << m_MaxNumberOfIterations << endl;
	Write << "m_MaxDepth: " << m_MaxDepth << endl;
	if (m_TempTestOrder[0] != NULL)
		Write << "m_TempTestOrder should equal NULL" << endl;
	for (a=0;a<FCS_TESTS_NUM;a++)
		Write << "m_TestOrder[" << a << "]: " << m_TestOrder[a] << endl;
	Write << "m_NumberOfTests: " << m_NumberOfTests << endl;
	if (m_SolvingMethod[0] != NULL)
		Write << "m_SolvingMethod should equal NULL" << endl;
	Write << "m_SolvingMethodType: " << m_SolvingMethodType << endl;
	if (m_AStarWeights[0] != NULL)
		Write << "m_AStarWeights should equal NULL" << endl;
	for (a=0;a<FC_NUMBER_OF_ASTAR_WEIGHTS;a++)
		Write << "m_AStarWeightValues[" << a << "]: " << m_AStarWeightValues[a] << endl;
	Write << "m_Seed: " << m_Seed << endl;
	Write << "m_OptimizeSolution: " << m_OptimizeSolution << endl;
	if (m_StateTypeArray[0] != NULL)
		Write << "m_StateTypeArray should equal NULL" << endl;
	Write << "m_StateType: " << m_StateType << endl;
	if (m_StateStorage[0] != NULL)
		Write << "m_StateStorage should equal NULL" << endl;
	Write << "m_StateStorageType: " << m_StateStorageType << endl;
	if (m_StackStorage[0] != NULL)
		Write << "m_StackStorage should equal NULL" << endl;
	Write << "m_StackStorageType: " << m_StackStorageType << endl;
	if (m_Talon[0] != NULL)
		Write << "m_Talon should equal NULL" << endl;
	Write << "m_TalonType: " << m_TalonType << endl;
	if (m_UserState[0] != NULL)
		Write << "m_UserState should equal NULL" << endl;
	if (m_InitialState != NULL)
		Write << "m_InitialState should equal NULL" << endl;
	if (sizeof(*m_DebugDisplayInfo) != sizeof (FCSDebugDisplayInfo))
		Write << "m_DebugDisplayInfo needs to be initialized" << endl;

}

void TestCommandLineArguments::TestParse()
{
	TestParseDebugDisplayObject();
	TestParseValue(&m_NumberOfFreecells, "m_NumberOfFreecells", "--freecells-num", "11", 11);
	TestParseValue(&m_NumberOfStacks, "m_NumberOfStacks", "--stacks-num", "7", 7);
	TestParseValue(&m_NumberOfDecks, "m_NumberOfDecks", "--decks-num", "5", 5);
	TestParseString(m_SequencesBuiltBy, "m_SequencesBuiltBy", "--sequences-are-built-by", "suit");
	TestParseString(m_SequenceMoveUnlimited, "m_SequenceMoveUnlimited", "--sequence-move", "limited");
	TestParseString(m_EmptyStacksFilledBy, "m_EmptyStacksFilledBy", "--empty-stacks-filled-by", "kings");
	TestParseString(m_GameName, "m_GameName", "--game", "freecell");
	TestParseString(m_GameName, "m_GameName", "--preset", "simple_simon");
	TestParseString(m_GameName, "m_GameName", "-g", "solitare");
	TestParseValue(&m_MaxNumberOfIterations, "m_MaxNumberOfIterations", "-mi", "13", 13);
	TestParseValue(&m_MaxNumberOfIterations, "m_MaxNumberOfIterations", "--max-iters", "15", 15);
	TestParseValue(&m_MaxDepth, "m_MaxDepth", "-md", "14", 14);
	TestParseValue(&m_MaxDepth, "m_MaxDepth", "--max-depth", "15", 15);
	TestParseString(m_TempTestOrder, "m_TempTestOrder", "-to", "12345");
	TestParseString(m_TempTestOrder, "m_TempTestOrder", "--tests-order", "54321");
	TestParseString(m_SolvingMethod, "m_SolvingMethod", "-me", "dfs");
	TestParseString(m_SolvingMethod, "m_SolvingMethod", "--method", "a-star");
	TestParseString(m_AStarWeights, "m_AStarWeights", "-asw", "0,1,2,3,4");
	TestParseString(m_AStarWeights, "m_AStarWeights", "--a-star-weights", "4,3,2,1,0");
	TestParseValue(&m_Seed, "m_Seed", "-seed", "35", 35);
	m_OptimizeSolution = false;
	TestParseBool(&m_OptimizeSolution, "m_OptimizeSolution", "-opt");
	m_OptimizeSolution = false;
	TestParseBool(&m_OptimizeSolution, "m_OptimizeSolution", "--optimize-solution");
	TestParseString(m_StateTypeArray, "m_StateTypeArray", "-st", "compact");
	TestParseString(m_StateTypeArray, "m_StateTypeArray", "--state-type", "compact");
	TestParseString(m_StateStorage, "m_StateStorage", "-stsr", "dfs");
	TestParseString(m_StateStorage, "m_StateStorage", "--state-storage-type", "dfs");
	TestParseString(m_StackStorage, "m_StackStorage", "-sksr", "dfs");
	TestParseString(m_StackStorage, "m_StackStorage", "--stack-storage-type", "dfs");
}

void TestCommandLineArguments::TestVerify()
{
}

void TestCommandLineArguments::TestParseValue(int* Member, const char* MemberName, 
											  const char* Argument, const char* ArgumentValue, int ExpectedMemberValue)
{
	char **CommandLineArgument;
	
	CommandLineArgument = new char*[4];
	for (int a = 0;a<4;a++)
		CommandLineArgument[a] = new char[35];

	strcpy(CommandLineArgument[0], "");
	strcpy(CommandLineArgument[1], Argument);
	strcpy(CommandLineArgument[2], ArgumentValue);
	strcpy(CommandLineArgument[3], "DummyTestFile.txt");

	if (!Parse(4, CommandLineArgument))
		Write << "Error!! Parsing CommandLine argument '"<< MemberName << "'" << endl;
	else if (*Member != ExpectedMemberValue)
		Write << "Error!! " << MemberName << ":\tActualValue: " << *Member << "\tExpectedValue: " << ExpectedMemberValue << endl;

	for (int b = 0;b<4;b++)
		delete [] CommandLineArgument[b];

	delete [] CommandLineArgument;
}

void TestCommandLineArguments::TestParseString(char* Member, const char* MemberName, const char* Argument,
						const char* ArgumentValue)
{
	char **CommandLineArgument;
	
	CommandLineArgument = new char*[4];
	for (int a = 0;a<4;a++)
		CommandLineArgument[a] = new char[35];

	strcpy(CommandLineArgument[0], "");
	strcpy(CommandLineArgument[1], Argument);
	strcpy(CommandLineArgument[2], ArgumentValue);
	strcpy(CommandLineArgument[3], "DummyTestFile.txt");

	if (!Parse(4, CommandLineArgument))
		Write << "Error!! Parsing CommandLine argument '"<< MemberName << "'" << endl;
	else if (strcmp(Member, ArgumentValue))
		Write << "Error!! " << MemberName << ":\tActualValue: " << *Member << "\tExpectedValue: " << ArgumentValue << endl;

	for (int b = 0;b<4;b++)
		delete [] CommandLineArgument[b];

	delete [] CommandLineArgument;
}

void TestCommandLineArguments::TestParseBool(bool* Member, const char* MemberName, const char* Argument)
{
	char** CommandLineArgument = new char*[3];
	int a;

	for (a = 0;a<3;a++)
		CommandLineArgument[a] = new char[35];

	strcpy(CommandLineArgument[0], "");
	strcpy(CommandLineArgument[1], Argument);
	strcpy(CommandLineArgument[2], "DummyTestFile.txt");

	if (!Parse(4, CommandLineArgument))
		Write << "Error!! Parsing CommandLine argument '"<< MemberName << "'" << endl;
	else if (*Member != true)
		Write << "Error!! " << MemberName << ":\tActualValue: " << *Member << "\tExpectedValue: " << true << endl;

	for (a = 0;a<3;a++)
		delete [] CommandLineArgument[a];

	delete [] CommandLineArgument;

}

void TestCommandLineArguments::TestDisplayDebugOptionsBool(int Option, const char* MemberName, const char* Argument)
{
	char** CommandLineArgument = new char*[3];
	int a;

	for (a = 0;a<3;a++)
		CommandLineArgument[a] = new char[35];

	strcpy(CommandLineArgument[0], "");
	strcpy(CommandLineArgument[1], Argument);
	strcpy(CommandLineArgument[2], "DummyTestFile.txt");

	if (!Parse(4, CommandLineArgument))
		Write << "Error!! Parsing CommandLine argument '"<< MemberName << "'" << endl;
	else if (!(m_DebugDisplayInfo->m_DisplayDebugOptions & Option))
		Write << "Error!! Flag " << MemberName << ":\tActualValue: " 
			  << (m_DebugDisplayInfo->m_DisplayDebugOptions & Option) 
			  << "\tExpectedValue: " << true << endl;

	for (a = 0;a<3;a++)
		delete [] CommandLineArgument[a];

	delete [] CommandLineArgument;

}

void TestCommandLineArguments::TestParseDebugDisplayObject()
{
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_ITERATIVE_STATE_OUTPUT;
	TestDisplayDebugOptionsBool(DEBUG_ITERATIVE_STATE_OUTPUT, "DEBUG_ITERATIVE_STATE_OUTPUT", "--state-output");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_ITERATIVE_STATE_OUTPUT;
	TestDisplayDebugOptionsBool(DEBUG_ITERATIVE_STATE_OUTPUT, "DEBUG_ITERATIVE_STATE_OUTPUT", "-s");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_IS_OUTPUT_PARSEABLE;
	TestDisplayDebugOptionsBool(DEBUG_IS_OUTPUT_PARSEABLE, "DEBUG_IS_OUTPUT_PARSEABLE", "--parseable-output");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_IS_OUTPUT_PARSEABLE;
	TestDisplayDebugOptionsBool(DEBUG_IS_OUTPUT_PARSEABLE, "DEBUG_IS_OUTPUT_PARSEABLE", "-p");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_10_AS_T;
	TestDisplayDebugOptionsBool(DEBUG_DISPLAY_10_AS_T, "DEBUG_DISPLAY_10_AS_T", "--display-10-as-t");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_10_AS_T;
	TestDisplayDebugOptionsBool(DEBUG_DISPLAY_10_AS_T, "DEBUG_DISPLAY_10_AS_T", "-t");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_CANONIZED_ORDER_OUTPUT;
	TestDisplayDebugOptionsBool(DEBUG_CANONIZED_ORDER_OUTPUT, "DEBUG_CANONIZED_ORDER_OUTPUT", "--canonized-order-output");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_CANONIZED_ORDER_OUTPUT;
	TestDisplayDebugOptionsBool(DEBUG_CANONIZED_ORDER_OUTPUT, "DEBUG_CANONIZED_ORDER_OUTPUT", "-c");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_MOVES;
	TestDisplayDebugOptionsBool(DEBUG_DISPLAY_MOVES, "DEBUG_DISPLAY_MOVES", "--display-moves");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_MOVES;
	TestDisplayDebugOptionsBool(DEBUG_DISPLAY_MOVES, "DEBUG_DISPLAY_MOVES", "-m");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_USE_STANDARD_NOTATION;
	TestDisplayDebugOptionsBool(DEBUG_USE_STANDARD_NOTATION, "DEBUG_USE_STANDARD_NOTATION", "--standard-notation");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_USE_STANDARD_NOTATION;
	TestDisplayDebugOptionsBool(DEBUG_USE_STANDARD_NOTATION, "DEBUG_USE_STANDARD_NOTATION", "-sn");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_MOVES;
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_STATES;
	TestDisplayDebugOptionsBool(DEBUG_DISPLAY_MOVES, "DEBUG_DISPLAY_MOVES & DEBUG_DISPLAY_STATES", "--display-states-and-moves");
	//also verify m_DisplayState was changed
	if (!(m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_STATES))
		Write << "Error!! Flag DEBUG_DISPLAY_STATES:\tActualValue: " << false << "\tExpectedValue: " << true << endl;
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_MOVES;
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_STATES;
	TestDisplayDebugOptionsBool(DEBUG_DISPLAY_MOVES, "DEBUG_DISPLAY_MOVES & DEBUG_DISPLAY_STATES", "-sam");
	//also verify m_DisplayState was changed
	if (!(m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_STATES))
		Write << "Error!! Flag DEBUG_DISPLAY_STATES:\tActualValue: " << false << "\tExpectedValue: " << true << endl;
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_PARENT_ITERATIONS;
	TestDisplayDebugOptionsBool(DEBUG_DISPLAY_PARENT_ITERATIONS, "DEBUG_DISPLAY_PARENT_ITERATIONS", "--display-parent-iter");
	m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_DISPLAY_PARENT_ITERATIONS;
	TestDisplayDebugOptionsBool(DEBUG_DISPLAY_PARENT_ITERATIONS, "DEBUG_DISPLAY_PARENT_ITERATIONS", "-pi");
	m_DebugDisplayInfo->m_DisplayDebug = false;
	TestParseBool(&m_DebugDisplayInfo->m_DisplayDebug, "m_DebugDisplayInfo->m_DisplayDebug", "--display-debug");
	m_DebugDisplayInfo->m_DisplayDebug = false;
	TestParseBool(&m_DebugDisplayInfo->m_DisplayDebug, "m_DebugDisplayInfo->m_DisplayDebug", "-dd");
}