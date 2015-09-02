#ifndef FCS_FREECELL_ALGORITHM_H
#define FCS_FREECELL_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSFreecellAlgorithm.h
///\brief This file contains the FCSFreecellAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCEnums.h"
#include "FCState.h"
#include "FCSDerivedStates.h"

/*
 * Conventions for use of the tests' order flags:
 * A test that should be scanned sequentially should have both flags cleared.
 * The first test in its random group should have both flags set. All the
 * other tests in the group should contain the FLAG_RANDOM flag.
 *
 * For instance: 123(45)(67)8 translates into:
 * 1 , 2, 3, 4|RANDOM|START_RANDOM_GROUP, 5|RANDOM, 
 * 6|RANDOM_START_RANDOM_GROUP, 7|RANDOM, 8
 *
 * */
///Conventions for use of the tests' order flags
enum FCS_TESTS_ORDER_FLAGS
{
	FCS_TEST_ORDER_NO_FLAGS_MASK = 0xFFFFFF,
	FCS_TEST_ORDER_FLAG_RANDOM = 0x1000000,
	FCS_TEST_ORDER_FLAG_START_RANDOM_GROUP = 0x2000000
};

///\brief Abstract class that sets up the functions needed to solve a freecell type game
class FCSFreecellAlgorithm
{
public:

	///\brief Solve the game
	///
	///\param StateWithLocations is the start state
	///\param Depth is the starting depth
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes Solve(FCSStateWithLocations* StateWithLocations, int Depth) = 0;

	///\brief Resume solving a game
	///
	///\param Depth to continue at depth
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes Resume(int Depth) = 0;

protected:
	///\brief Helper function to delete a FCSDerivedStatesList
	void DeleteDerived(FCSDerivedStatesList* Derive);

	///\brief Function to run each test on a state
	///
	///\param TestNumber to be run
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes RunTest(int TestNumber, FCSStateWithLocations*  StateWithLocations,
													  int Depth, int NumberOfFreeStacks,
													  int NumberOfFreecells,
													  FCSDerivedStatesList* DerivedStateList) = 0;

};

#endif