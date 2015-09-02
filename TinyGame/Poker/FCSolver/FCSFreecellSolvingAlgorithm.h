#ifndef FCS_FREECELL_SOLVING_ALGORITHM_H
#define FCS_FREECELL_SOLVING_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSFreecellSolvingAlgorithm.h
///\brief This file contains the FCSFreecellSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSFreecellData.h" 
#include "FCSFreecellAlgorithm.h"

///\brief A algorithm class to solve freecell games
class FCSFreecellSolvingAlgorithm : public FCSFreecellData, public FCSFreecellAlgorithm
{
public:

	///\brief Create a solving algorithm with CommandLine.
	///
	///It is assumed that CommandLine has already been verified.  This includes a 
	///valid start state
	///\param CommandLine is the command line object with all the information to solve a freecell game
	///\return A pointer to a freecell solving algorithm.
	static FCSFreecellSolvingAlgorithm* Create(FCCommandLineArguments* CommandLine);

	///Destructor
	virtual ~FCSFreecellSolvingAlgorithm();

protected:
	///Default constructor
	FCSFreecellSolvingAlgorithm();

	///Constructor with command line interface
	FCSFreecellSolvingAlgorithm(FCCommandLineArguments* CommandLine);

	///\brief Check if the top card in the stack is a flipped card
	///and if so flips it so its face is up.
	///
	///\param State with the stack
	///\param Stack position
	///\param Move to flip the card
	///\param MoveStack to add the move to
	inline void FlipTopCard(FCSStateWithLocations *State, int Stack, FCSMove* Move, FCSMoveStack** MoveStack);

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
													  FCSDerivedStatesList* DerivedStateList);

	///\brief This function tries to move stack cards that are present at the
	///top of stacks to the foundations.
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveTopStackCardsToFounds(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList);

	///\brief This test moves single cards that are present in the freecells to the foundations.
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveFreecellCardsToFounds(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList);

	///\brief This test moves single cards that are present in the freecells to the top of a stack.
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveFreecellCardsToTopOfStacks(FCSStateWithLocations*  StateWithLocations,
								  int Depth,
								  int NumberOfFreeStacks,
								  int NumberOfFreecells,
								  FCSDerivedStatesList* DerivedStateList);

	///\brief This test moves a single card from within a stack to a foundation
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveNonTopStackCardsToFounds(FCSStateWithLocations*  StateWithLocations,
								  int Depth,
								  int NumberOfFreeStacks,
								  int NumberOfFreecells,
								  FCSDerivedStatesList* DerivedStateList);

	///\brief This moves cards from one stack to another
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveStackCardsToDifferentStacks(FCSStateWithLocations*  StateWithLocations,
								  int Depth,
								  int NumberOfFreeStacks,
								  int NumberOfFreecells,
								  FCSDerivedStatesList* DerivedStateList);

	///\brief This test moves cards on a stack to a parent on the same stack
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveStackCardsToAParentOnTheSameStack(FCSStateWithLocations*  StateWithLocations,
										  int Depth,
										  int NumberOfFreeStacks,
										  int NumberOfFreecells,
										  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a sequence to an empty stack
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveSequencesToFreeStacks(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a (group of) freecell card(s) to an empty stack
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveFreecellCardsToEmptyStack(FCSStateWithLocations*  StateWithLocations,
								  int Depth,
								  int NumberOfFreeStacks,
								  int NumberOfFreecells,
								  FCSDerivedStatesList* DerivedStateList);

	///\brief Move cards to a parent on a different stack
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveCardsToADifferentParent(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList);

	///\brief Put an entire stack into freecell cards
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes EmptyStackIntoFreecells(FCSStateWithLocations*  StateWithLocations,
						  int Depth,
						  int NumberOfFreeStacks,
						  int NumberOfFreecells,
						  FCSDerivedStatesList* DerivedStateList);

};

#endif