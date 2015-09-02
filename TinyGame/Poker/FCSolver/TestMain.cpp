////////////////////////////////////////////////
///\file TestMain.cpp
///\brief This file contains the main test function
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <iostream>

#ifdef _DEBUG
#include <afx.h>
#endif

#include "TestAVLRedBlackTree.h"
#include "TestTrees.h"
#include "TestCommandLine.h"
#include "TestSolvingAlgorithms.h"

#include "SearchAlgorithms.h"

#include <iostream>
using std::endl;
using std::cout;

///\brief Main test function
int main(int argc, char **argv)
{
#ifdef _DEBUG
	CMemoryState oldMemState, newMemState, diffMemState;
	oldMemState.Checkpoint();
#endif

	//test the trees
	TestRedBlackTree();
	TestAVLRedBlackTree(argc, argv);
	TestAVLTree(argc, argv);
	TestGLIBTree(argc, argv);

	//test CommandLineArguments
	TestCommandLineArguments* TestCommandLine = new TestCommandLineArguments();
	TestCommandLine->TestConstructor();
	TestCommandLine->TestParse();
	TestCommandLine->TestVerify();
	delete TestCommandLine;

	//test the solving algorithms
//	TestAllSolvingAlgorithms();
	
	//test each game
	TestEachGame();

#ifdef _DEBUG
	newMemState.Checkpoint();
	if( diffMemState.Difference( oldMemState, newMemState ) )
	{
		diffMemState.DumpStatistics();
		TRACE( "Memory leaked detected!!!\n" );
		cout << "Memory leaked detected!!!" << endl;
		_CrtDumpMemoryLeaks();
	}
#endif

	return 0;
}