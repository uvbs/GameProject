#ifndef FCS_SIMPLE_SIMON_SOLVING_ALGORITHM_H
#define FCS_SIMPLE_SIMON_SOLVING_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSSimpleSimonSolvingAlgorithm.h
///\brief This file contains the FCSSimpleSimonSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSFreecellData.h" 
#include "FCSFreecellAlgorithm.h"

///\brief A algorithm class to solve simple simon games
class FCSSimpleSimonSolvingAlgorithm : public FCSFreecellData, public FCSFreecellAlgorithm
{
public:
	///\brief Create a solving algorithm with CommandLine.
	///
	///It is assumed that CommandLine has already been verified.  This includes a 
	///valid start state
	///\param CommandLine is the command line object with all the information to solve a freecell game
	///\return A pointer to a freecell solving algorithm.
	static FCSSimpleSimonSolvingAlgorithm* Create(FCCommandLineArguments* CommandLine);

	///Destructor
	virtual ~FCSSimpleSimonSolvingAlgorithm();

protected:
	///Default constructor
	FCSSimpleSimonSolvingAlgorithm();

	///Constructor with command line interface
	FCSSimpleSimonSolvingAlgorithm(FCCommandLineArguments* CommandLine);

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

	///\brief Determines whether the above card is one greater in value than current card and is the same suit
	///
	///This was added for readability of the code
	///\param Parent is the card above
	///\param Child is the card below
	///\return True if the above card is one greater in value and equal in suit
	inline bool IsSimpleSimonTrueParent(FCSCard* Parent, FCSCard* Child);
	
	///\brief Determines whether or not two cards have the same suit
	///
	///This was added for readability of the code
	///\param ParentSuit is the suit of the card above
	///\param ChildSuit is the suit of the card below
	///\return True if the suits are equal, false otherwise
	inline bool IsSimpleSimonTrueParentSuit(FCSCard* Parent, FCSCard* Child);

	///\brief Determines whether the above card is one greater in value than current card
	///
	///This was added for readability of the code
	///\param Parent is the card above
	///\param Child is the card below
	///\return True if the above card is one greater in value
	inline bool IsSimpleSimonFalseParent(FCSCard* Parent, FCSCard* Child);

	///\brief This tests moves a suit of freecell cards to its foundation
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveSequenceToFounds(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a (group of) cards to its true parent (same suit)
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveSequenceToTrueParent(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a stack of cards to its false parent (different suit)
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveWholeStackSequenceToFalseParent(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a (group of) cards to its true parent (same suit) 
	///when the parent isn't at the top of a stack
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveSequenceToTrueParentWithSomeCardsAbove(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a (group of) cards that have unsequenced cards above to 
	///its true parent (same suit)
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveSequenceWithSomeCardsAboveToTrueParent(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a (group of) different suited cards to its true parent (same suit) 
	///when the parent isn't at the top of a stack
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveSequenceWithJunkSequenceAboveToTrueParentWithSomeCardsAbove(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a (group of) cards to its false parent (different suit) 
	///when the parent isn't at the top of a stack
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveWholeStackSequenceToFalseParentWithSomeCardsAbove(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a (group of) cards to its true parent (same suit) on the same stack
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveSequenceToParentOnTheSameStack(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

	///\brief This tests moves a (group of) card(s) to its false parent (different suit)
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes MoveSequenceToFalseParent(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList);

};

#endif