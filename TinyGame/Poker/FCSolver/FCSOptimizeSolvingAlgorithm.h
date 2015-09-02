#ifndef MMANN_FCS_OPTIMIZE_SOLVING_ALGORITHM_H
#define MMANN_FCS_OPTIMIZE_SOLVING_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSOptimizeSolvingAlgorithm.h
///\brief This file contains the FCSOptimizeSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSBFSSolvingAlgorithm.h"

///\brief Optimize the solution to a game
///
///Once a game has been solved, try to optimize the number of states and moves.
template <class SolvingAlgorithmType>
class FCSOptimizeSolvingAlgorithm : public FCSBFSSolvingAlgorithm<SolvingAlgorithmType>
{
public:
	///Constructor with current solving algorithm
	FCSOptimizeSolvingAlgorithm(SolvingAlgorithmType* SolvingAlgorithm);

	///\brief Destructor
	///
	///The original solving method will delete most of data,
	///we just need to worry about the solution data we create
	virtual ~FCSOptimizeSolvingAlgorithm();

	///\brief Solve the game
	///
	///\param StateWithLocations is the start state
	///\param Depth is the starting depth
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes Solve(FCSStateWithLocations* StateWithLocations, int Depth);

	///\brief Resume solving a game
	///
	///Since the solution has already been acheived, this shouldn't be necessary,
	///so it will just return FCS_STATE_IS_NOT_SOLVEABLE
	///\param Depth to continue at depth
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes Resume(int Depth);

protected:
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
										FCSDerivedStatesList** DerivedStateList, FCSMoveStack** Move, FCSMove** TempMove, int depth, 
										FCSStateSolvingReturnCodes* ReturnCode);

	///Number of state packs in first solving algorithm
	int m_SavedNumberOfStatePacks;

	///Pointer to the state packs of the first solving algorithm
	AFCSStateWithLocationsMatrix** m_SavedStatePacks;
};

template <class SolvingAlgorithmType>
FCSOptimizeSolvingAlgorithm<SolvingAlgorithmType>::FCSOptimizeSolvingAlgorithm(SolvingAlgorithmType* SolvingAlgorithm) : FCSBFSSolvingAlgorithm<SolvingAlgorithmType>()
{
	IsOptimizeClass = true;

	// Initialize the BFS queue. We have one dummy element at the beginning
	// in order to make operations simpler.
	m_BFSQueue = new FCSStateWithLocationsLinkedList();
	m_BFSQueue->m_Next = new FCSStateWithLocationsLinkedList();
	m_BFSQueueLastItem = m_BFSQueue->m_Next;
	m_BFSQueueLastItem->m_Next = NULL;

	//copy over everything needed from other solving algorithm.
	m_StateType = SolvingAlgorithm->m_StateType;
	m_StatePacks = SolvingAlgorithm->m_StatePacks;
	m_SavedStatePacks = &SolvingAlgorithm->m_StatePacks;
	m_MaxNumberOfStatePacks = SolvingAlgorithm->m_MaxNumberOfStatePacks;
	m_SavedNumberOfStatePacks = m_NumberOfStatePacks = SolvingAlgorithm->m_NumberOfStatePacks;
	m_NumberOfStatesInLastPack = SolvingAlgorithm->m_NumberOfStatesInLastPack;
	m_StatePackLength = SolvingAlgorithm->m_StatePackLength;

	m_FinalState = SolvingAlgorithm->m_FinalState;

	m_NumberOfCheckedStates = SolvingAlgorithm->m_NumberOfCheckedStates;
	m_MaxDepth = SolvingAlgorithm->m_MaxDepth;
	m_MaxNumberOfCheckedStates = SolvingAlgorithm->m_MaxNumberOfCheckedStates;

	m_DebugDisplayInfo = SolvingAlgorithm->m_DebugDisplayInfo;
	m_TestsOrderNumber = SolvingAlgorithm->m_TestsOrderNumber;
	memcpy(m_TestsOrder, SolvingAlgorithm->m_TestsOrder, sizeof(int)*FCS_TESTS_NUM);

	m_StateStorage = SolvingAlgorithm->m_StateStorage;
	m_StackStorage = SolvingAlgorithm->m_StackStorage;

	m_NumberOfFreecells = SolvingAlgorithm->m_NumberOfFreecells;
	m_NumberOfStacks = SolvingAlgorithm->m_NumberOfStacks;
	m_NumberOfDecks = SolvingAlgorithm->m_NumberOfDecks;

	m_SequencesAreBuiltBy = SolvingAlgorithm->m_SequencesAreBuiltBy;
	m_IsUnlimitedSequenceMove = SolvingAlgorithm->m_IsUnlimitedSequenceMove;
	m_EmptyStacksFill = SolvingAlgorithm->m_EmptyStacksFill;
	m_OptimizeSolutionPath = SolvingAlgorithm->m_OptimizeSolutionPath;

	m_CompareFunction = SolvingAlgorithm->m_CompareFunction;
	m_MD5Hash = SolvingAlgorithm->m_MD5Hash;
	
	m_IndirectCompare = SolvingAlgorithm->m_IndirectCompare;
	m_IndirectHash = SolvingAlgorithm->m_IndirectHash;
}

template <class SolvingAlgorithmType>
FCSOptimizeSolvingAlgorithm<SolvingAlgorithmType>::~FCSOptimizeSolvingAlgorithm()
{
	for(int p=m_SavedNumberOfStatePacks; p<m_NumberOfStatePacks;p++)
		m_StatePacks->DeleteArray(p);

	(*m_SavedStatePacks) = m_StatePacks;
}

template <class SolvingAlgorithmType>
bool FCSOptimizeSolvingAlgorithm<SolvingAlgorithmType>::CheckStateEnd(FCSStateWithLocations** NewStateWithLocations, FCSStateWithLocations* StateWithLocations, 
										FCSDerivedStatesList** DerivedStateList, FCSMoveStack** Move, FCSMove** TempMove, int depth, 
										FCSStateSolvingReturnCodes* ReturnCode)
{
	FCSStateWithLocations* ExistingState;
	FCSStateSolvingReturnCodes Check;

// The last move in a move stack should be FCS_MOVE_TYPE_CANONIZE
// because it indicates that the order of the stacks and freecells
// need to be recalculated 

	(*TempMove)->SetType(FCS_MOVE_TYPE_CANONIZE);
	(*Move)->Push(*TempMove);

	Check = CheckAndAddState(*NewStateWithLocations, &ExistingState, depth);
		
	if ((Check == FCS_STATE_BEGIN_SUSPEND_PROCESS) ||
		(Check == FCS_STATE_SUSPEND_PROCESS))
	{
		// This state is not going to be used, so
		// let's clean it.
		(*NewStateWithLocations)->CleanState();
		*ReturnCode = Check;
		return true;
	}
	else if (Check == FCS_STATE_ALREADY_EXISTS)
	{
		StatePackRelease();

		// Re-parent the existing state to this one.
		// What it means is that if the depth of the state if it
		// can be reached from this one is lower than what it
		// already have, then re-assign its parent to this state.
		if (ExistingState->m_Depth > StateWithLocations->m_Depth+1)
		{
			// Make a copy of "moves" because "moves will be destroyed
			FCSMoveStack* MovesCopy = (*Move)->Copy();

			// Destroy the old moves stack of the state, because we are going to
			// override it.
			delete [] ExistingState->m_MovesToParent;
			ExistingState->m_MovesToParent = MovesCopy;
			ExistingState->m_Parent = StateWithLocations;
			ExistingState->m_Depth = StateWithLocations->m_Depth + 1;
		}

		(*DerivedStateList)->AddState(ExistingState);
	}
	else
	{   
		// We duplicate the move stack so it won't be
		// destroyed twice as it exists in both the state
		// moves_to_parent member variable and in the derived
		// states list
		(*DerivedStateList)->AddState(*NewStateWithLocations);

		// moves is already used inside the states_list, so we
		// need to get a fresh one.
		(*Move) = CreateMoveStack();
	}

	return false;
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSOptimizeSolvingAlgorithm<SolvingAlgorithmType>::Resume(int Depth)
{
	return FCS_STATE_IS_NOT_SOLVEABLE;
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSOptimizeSolvingAlgorithm<SolvingAlgorithmType>::Solve(FCSStateWithLocations* StateWithLocations, int Depth)
{

	int NumberOfFreeStacks = 0,
		NumberOfFreecells = 0,
		DerivedIndex, a;

	FCSStateSolvingReturnCodes Check;

	FCSDerivedStatesList Derived;

	// Initialize the first element to indicate it is the first
	StateWithLocations->m_Parent = NULL;
	StateWithLocations->m_MovesToParent = NULL;
	StateWithLocations->m_Depth = 0;
  
    // Continue as long as there are states in the queue or priority queue.
    while (StateWithLocations != NULL)
    {
		if (!(StateWithLocations->m_Visited & FCS_VISITED_IN_SOLUTION_PATH))
		{
			goto NextState;
		}

		if (StateWithLocations->m_Visited & FCS_VISITED_IN_OPTIMIZED_PATH)
		{
			goto NextState;
		}

		// Count the freecells
		NumberOfFreecells = 0;
		for(a=0;a<m_NumberOfFreecells;a++)
		{
			if (StateWithLocations->GetFreecellCardNumber(a) == 0)
				NumberOfFreecells++;
		}

		// Count the number of unoccupied stacks
		NumberOfFreeStacks = 0;
		for(a=0;a<m_NumberOfStacks;a++)
		{
			if (StateWithLocations->GetStackLength(a) == 0)
				NumberOfFreeStacks++;
		}

		m_DebugDisplayInfo->Display(m_StateType, m_NumberOfCheckedStates, StateWithLocations->m_Depth, StateWithLocations,  
			((StateWithLocations->m_Parent == NULL) ? 0 : StateWithLocations->m_Parent->m_VisitIterations), m_NumberOfStatesInCollection);

		if ((NumberOfFreeStacks == m_NumberOfStacks) && (NumberOfFreecells == m_NumberOfFreecells))
		{
			m_FinalState = StateWithLocations;

			// Free the memory that was allocated by derived.
			DeleteDerived(&Derived);
			return FCS_STATE_WAS_SOLVED;
		}

		// Increase the number of iterations by one . This
		// is meant to make sure we do not entered this state before which
		// will lead to a false iterations count.
		m_NumberOfCheckedStates++;

		// Do all the tests at one go, because that the way it should be
		//  done for BFS and A*

		Derived.m_NumberOfStates = 0;
		for(a=0 ; a < m_TestsOrderNumber; a++)
		{
			Check = RunTest(m_TestsOrder[a] & FCS_TEST_ORDER_NO_FLAGS_MASK, StateWithLocations, 
							StateWithLocations->m_Depth, NumberOfFreeStacks,
							NumberOfFreecells, &Derived);

			if ((Check == FCS_STATE_BEGIN_SUSPEND_PROCESS) ||
				(Check == FCS_STATE_EXCEEDS_MAX_NUM_TIMES) ||
				(Check == FCS_STATE_SUSPEND_PROCESS))
			{
				// Save the current position in the scan
				m_FirstStateToCheck = StateWithLocations;

				DeleteDerived(&Derived);
				return FCS_STATE_SUSPEND_PROCESS;
			}
		}    

		// Insert all the derived states into the PQ or Queue
		for(DerivedIndex = 0; DerivedIndex < Derived.m_NumberOfStates; DerivedIndex++)
            BFSEnqueueState(Derived.m_States[DerivedIndex]);              

		StateWithLocations->m_Visited |= FCS_VISITED_IN_OPTIMIZED_PATH;
		StateWithLocations->m_VisitIterations = m_NumberOfCheckedStates-1;

NextState:

		// Extract the next item in the queue/priority queue.
		if (m_BFSQueue->m_Next != m_BFSQueueLastItem)
		{
			FCSStateWithLocationsLinkedList* SaveItem = m_BFSQueue->m_Next;
			StateWithLocations = SaveItem->m_State;
			m_BFSQueue->m_Next = m_BFSQueue->m_Next->m_Next;
			delete SaveItem;
		}
		else
		{
			StateWithLocations = NULL;
		}
    }

	// Free the memory that was allocated by derived.
	DeleteDerived(&Derived);

	return FCS_STATE_IS_NOT_SOLVEABLE;
}
#endif