#ifndef TEST_COMMAND_LINE_H
#define TEST_COMMAND_LINE_H

////////////////////////////////////////////////
///\file TestCommandLine.h
///\brief This file contains the TestCommandLineArguments class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <fstream>
#include "FCCommandLineArguments.h"

///\brief Test class of FCCommandLineArguments
class TestCommandLineArguments : public FCCommandLineArguments
{
public:
	///Constructor
	TestCommandLineArguments();
	///Destructor
	virtual ~TestCommandLineArguments();

	///\brief make sure everything is initialized correctly for FCCommandLineArguments
	void TestConstructor();

	///\brief make sure each command line argument is used
	void TestParse();

	///\brief make sure each command line argument is used
	void TestVerify();

protected:
	///\brief Test an integer member from FCCommandLineArguments
	void TestParseValue(int* Member, const char* MemberName, const char* Argument, 
						const char* ArgumentValue, int ExpectedMemberValue);	
	
	///\brief Test an string member from FCCommandLineArguments
	void TestParseString(char* Member, const char* MemberName, const char* Argument,
						const char* ArgumentValue);

	///\brief Test an bool member from FCCommandLineArguments
	void TestParseBool(bool* Member, const char* MemberName, const char* Argument);

	///\brief Test bool flags from DebugDisplayOption member of DebugDisplayObject
	void TestDisplayDebugOptionsBool(int Option, const char* MemberName, const char* Argument);

	///\brief Test for the FCSDebugDisplayInfo object
	void TestParseDebugDisplayObject();

	///Writes output of the tests
	std::ofstream Write;
};


#endif