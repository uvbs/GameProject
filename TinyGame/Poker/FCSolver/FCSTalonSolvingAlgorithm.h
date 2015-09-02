#ifndef FCS_TALON_SOLVING_ALGORITHM_H
#define FCS_TALON_SOLVING_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSTalonSolvingAlgorithm.h
///\brief This file contains the FCSTalonSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date November 2002
////////////////////////////////////////////////

#include "FCSFreecellSolvingAlgorithm.h"

///\brief A algorithm class to solve solitare games with talons
class FCSTalonSolvingAlgorithm : public FCSFreecellSolvingAlgorithm
{
public:
	///\brief Create a solving algorithm with CommandLine.
	///
	///It is assumed that CommandLine has already been verified.  This includes a 
	///valid start state
	///\param CommandLine is the command line object with all the information to solve a freecell game
	///\return A pointer to a talon solving algorithm.
	static FCSTalonSolvingAlgorithm* Create(FCCommandLineArguments* CommandLine);

	///Constructor with command line interface
	FCSTalonSolvingAlgorithm(FCCommandLineArguments* CommandLine);

	///Destructor
	virtual ~FCSTalonSolvingAlgorithm();

protected:
	///Default constructor
	FCSTalonSolvingAlgorithm();

	///The "real" default constructor
	void InitFCSTalonSolvingAlgorithm();

/****** Note: This is not used in code, Temporarily removing it.

	// The talon for Gypsy-like games. Since only the position changes from
	// state to state.  We can keep it here.
	FCSCard* m_GypsyTalon;

	//The length of the Gypsy talon 
	int m_GypsyTalonLength;
**************************/

	///Type of talon used for the game
	FCSTalonType m_TalonType;

	/// The Klondike Talons' Cache in Hash storage
	FCInternalHash<FCSTalonStateData, void>* m_TalonsHash; 
	/// The compare function used for the Klondike Talons' Cache
	CompareFunctionCompareNodesAlgorithm<FCSTalonStateData, void> m_TalonCompareFunction;
	/// The hash function used for the Klondike Talons' Cache
	MD5TalonStateDataHashAlgorithm m_TalonHashFunction;

	///\brief Call this to initialize members needed for solve.  (Common between all children)
	///
	///\param InitState is the start state of the game
	virtual void InitSolve(FCSStateWithLocations* InitState);

	///\brief If the state is new state (not generated before), add it to storage
	///
	///\param NewState is the state to add
	///\param ExistingState becomes the state to work with (if NewState is added, ExistingState = NewState)
	///\param Depth is the current depth of the state
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes CheckAndAddState(FCSStateWithLocations* NewState,
						 FCSStateWithLocations** ExistingState,
						 int Depth);

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

	///\brief This test moves a single cards from the talon to a stack.
	///
	///\param StateWithLocations is the state to run the test on
	///\param Depth is the current depth of the state
	///\param NumberOfFreeStacks is the current number of free stacks in the state
	///\param NumberOfFreecells is the current number of freecells in the state
	///\param DerivedStateList is the current derived state list 
	///\return Solving return code
	FCSStateSolvingReturnCodes GetCardFromKlondikeTalon(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList);
};

#endif