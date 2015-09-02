#ifndef FCS_FREECELL_DATA_H
#define FCS_FREECELL_DATA_H

////////////////////////////////////////////////
///\file FCSFreecellData.h
///\brief This file contains the FCSFreecellData class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <limits.h>	//needed for INT_MAX
#include "FCState.h"
#include "FCSMove.h"
#include "FCSDerivedStates.h"
#include "FCCommandLineArguments.h"
#include "FCSStateStorage.h"
#include "FCSStackStorage.h"
//*** These won't be necessary if the compare/hash algorithms are moved
#include "FCSIndirectStateCompareAlgorithm.h"
#include "HashAlgorithms.h"

class FCSIndirectCard;

template <class SolvingAlgorithm>
class FCSOptimizeSolvingAlgorithm;

class FCSFreecellSolvingAlgorithm;
class FCSSimpleSimonSolvingAlgorithm;
class FCSTalonSolvingAlgorithm;

///\brief Holds all of the common data for the freecell solver
class FCSFreecellData
{
public:
	///Destructor
	virtual ~FCSFreecellData();

	///\brief Get the number of check states in the solver
	///
	///\return Number of checked states
	int GetNumberOfCheckedStates();

	///\brief Get the number states in the solver
	///
	///\return Number of states
	int GetNumberOfStatesInCollection();

	///\brief Increase the max number of checked states allowed
	void IncreaseMaxNumberOfCheckedStates();

	///\brief Delete the current solution states to create room for
	///an optimized solution
	void CleanData();

	///\brief Show the solution, step by step
	///
	///\param InitStateWithLocations is the start state of the game
	///\param DupStateWithLocations is a copy of the start state because it will get modified
	virtual void ShowSolution(FCSStateWithLocations* InitStateWithLocations, FCSStateWithLocations* DupStateWithLocations);

	///\brief Trace the solution from the final state down to the initial state
	void TraceSolution();

	/// added for every "optimized solving algorithm type" 
	friend class FCSOptimizeSolvingAlgorithm<FCSFreecellSolvingAlgorithm>;
	/// added for every "optimized solving algorithm type" 
	friend class FCSOptimizeSolvingAlgorithm<FCSSimpleSimonSolvingAlgorithm>;
	/// added for every "optimized solving algorithm type"
	friend class FCSOptimizeSolvingAlgorithm<FCSTalonSolvingAlgorithm>;

protected:
	///Default constructor
	FCSFreecellData();
	///The "real" default constructor
	void InitFCSFreecellData();
	///Constructor with command line interface
	FCSFreecellData(FCCommandLineArguments* CommandLine);

	///\brief Call this to initialize members needed for solve.  (Common between all children)
	///
	///\param InitState is the start state of the game
	void InitSolve(FCSStateWithLocations* InitState);
	
	///\brief Get the next move of a SOLVED state
	///
	///\param StateWithLocations is the state to get the moves from
	///\param Move is the move returned
	///\return 0 if there is another move, Nonzero if no more moves 
	int GetNextMove(FCSStateWithLocations* StateWithLocations, FCSMove* Move);

	///Is child class an optimize class?
	bool IsOptimizeClass;

	///State type - Debug, Compact or Indirect
	FCStateType m_StateType;

	///\brief States used to solve the game
	///
	///The State Packs variables are used by all the state cache 
	///management routines. A pack stores as many states as can fit
	///in a 64KB segment, and those variables manage an array of
	///such packs.<BR>
	///Such allocation is possible, because at the worst situation 
	///the last state is released.
	AFCSStateWithLocationsMatrix* m_StatePacks;

	///Maximum number of state packs before more memory has to be allocated for more
	int m_MaxNumberOfStatePacks;

	///Current number of state packs
	int m_NumberOfStatePacks;
	
	///Number of states in the last state pack
	int m_NumberOfStatesInLastPack;
	
	/// Number of states in each pack
	int m_StatePackLength;

	/// Number of states that were checked by the solving algorithm
	int m_NumberOfCheckedStates;

	///\brief This is the number of states in the state collection.
	///
	/// It gives a rough estimate of the memory occupied by the instance.
	int m_NumberOfStatesInCollection;

	/// A limit on the number of states in the state collection.
	int m_MaxNumberOfStatesInCollection;

	/// A vector of the states leading to the solution.
	AFCSStateWithLocationsMatrix* m_SolutionStates;

	///The number of states in m_SolutionStates.
	int m_NumberOfSolutionStates;

	///\brief A move stack that contains the moves leading to the solution.
	///
	/// It is created only after the solution was found by swallowing 
	/// all the stacks of each depth.
	FCSMoveStack* m_SolutionMoves;

	///\brief m_ProtoSolutionMoves[i] are the moves that lead from m_SolutionStates[i] to 
	///m_SolutionStates[i+1].     
	FCSMoveStack** m_ProtoSolutionMoves;

	///This is the final state that the scan recommends to the interface 
	FCSStateWithLocations* m_FinalState;

	///\brief Limits the maximal depth
	///
	/// m_MaxDepth is quite dangerous because it blocks some intermediate moves
	/// and doesn't allow a program to fully reach its solution.
	int m_MaxDepth;

	///\brief Limits the maximal number of checked states
	///
	/// m_MaxNumberOfCheckedStates is useful because it enables the process to
	/// stop before it consumes too much memory.
	int m_MaxNumberOfCheckedStates;

	///\brief m_DebugDisplayInfo provide a programmer with a programmable way
	/// to debug the algorithm while it is running.
	FCSDebugDisplayInfo* m_DebugDisplayInfo;

	/// The tests' order indicates which tests (i.e: kinds of multi-moves) to do at what order.
	int m_TestsOrder[FCS_TESTS_NUM];

	/// Number of tests in the test's order.
	int m_TestsOrderNumber;

	///Used to store the previous states in a scan
	AFCSGenericStateStorage* m_StateStorage;

	///Used to store stacks assuming Indirect Stack States are used
	AFCSGenericStackStorage* m_StackStorage;

	///Number of freecells present in the game
	int m_NumberOfFreecells;
	
	///Number of stacks present in the game
	int m_NumberOfStacks;

	///Number of decks present in the game
	int m_NumberOfDecks;

	/// What two adjacent cards in the same sequence can be
	BuildSequenceByEnum m_SequencesAreBuiltBy;

	///\brief Whether an entire sequence can be moved from one place to the
	/// other regardless of the number of unoccupied Freecells there are.
	bool m_IsUnlimitedSequenceMove;

	/// With what cards can empty stacks be filled with.
	FCEmptyStacksFillType m_EmptyStacksFill;

	/// A flag that indicates whether to optimize the solution path at the end of the scan
	bool m_OptimizeSolutionPath;

	//****** I'm not quite sure where to put these, so they're here for now *****
	//****** Since they vary, should they all be pointers and allocated only if needed ?  *****
	///Compare function used for comparing states
	CompareFunctionCompareNodesAlgorithm<FCSStateWithLocations, void> m_CompareFunction;
	///Hash function used when storing in hash tables
	MD5StateWithLocationsHashAlgorithm m_MD5Hash;
	///Compare function when storing FCSIndirectCards
	FCSIndirectCardCompareAlgorithm<FCSIndirectCard, void>* m_IndirectCompare;
	///Hash function when storing FCSIndirectCards
	MD5HashAlgorithm<FCSIndirectCard>* m_IndirectHash;

	/// Allocate a state pack
	FCSStateWithLocations* StatePackAlloc();

	/// Release a state pack
	void StatePackRelease();

	///\brief If the state is new state (not generated before), add it to storage
	///
	///\param NewState is the state to add
	///\param ExistingState becomes the state to work with (if NewState is added, ExistingState = NewState)
	///\param Depth is the current depth of the state
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes CheckAndAddState(FCSStateWithLocations* NewState,
						 FCSStateWithLocations** ExistingState,
						 int Depth);

	///\brief Prep a state to be added into storage
	///
	///\param NewStateWithLocations is the state being added to storage
	///\param StateWithLocations is the current state
	///\param Move is how to get from StateWithLocations to NewStateWithLocations
	///\param Depth is the current depth of the state
	virtual inline void CheckStateBegin(FCSStateWithLocations** NewStateWithLocations, FCSStateWithLocations* StateWithLocations,
										FCSMoveStack *Move);

	///\brief Put the last move onto the move stack for the current state
	///
	///\param NewStateWithLocations is the state being added to storage
	///\param StateWithLocations is the current state
	///\param DerivedStateList is the derived state list for the current state
	///\param Move is how to get from StateWithLocations to NewStateWithLocations
	///\param TempMove is the last move from StateWithLocations to NewStateWithLocations
	///\param Depth is the current depth of the state
	///\param ReturnCode determines if the test is done
	///\return Whether or not to return the ReturnCode
	virtual inline bool CheckStateEnd(FCSStateWithLocations** NewStateWithLocations, FCSStateWithLocations* StateWithLocations, 
										FCSDerivedStatesList** DerivedStateList, FCSMoveStack** Move, FCSMove** TempMove, int Depth, 
										FCSStateSolvingReturnCodes* ReturnCode);

	///Create a move stack of all the moves in the game
	void CreateTotalMovesStack();

	///\brief is a card a parent of another?
	///
	///\param Child is the child card
	///\param Parent is the possible parent card
	///\return Whether or not a parent relationship exists
	bool IsParentCard(FCSCard* Child, FCSCard* Parent);

	///\brief Calculate the maximum number of sequence moves given the number of
	/// freecells and free stacks
	///
	///\param FreecellNumber is the number of freecells
	///\param FreeStackNumber is the number of free stacks
	///\return The biggest sequence that can be moved
	int CalculateMaxSequenceMoves(int FreecellNumber, int FreeStackNumber);

	///\brief Move a sequence of cards
	///
	///\param NewStateWithLocations is the state the cards will move to
	///\param Card is the starting card in the sequence
	///\param MoveStack is the move stack for the state
	///\param Move is the move of the sequence
	///\param DestStack is the destination stack of the sequence
	///\param SourceStack is the source stack of the sequence
	///\param Start is the start height of the stack
	///\param End is the end height of the stack
	void MoveSequence(FCSStateWithLocations* NewStateWithLocations, FCSCard* Card, FCSMoveStack *MoveStack, FCSMove** TempMove, 
					int DestStack, int SourceStack, int Start, int End);

};

#endif