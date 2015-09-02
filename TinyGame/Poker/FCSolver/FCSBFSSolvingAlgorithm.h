#ifndef MMANN_FCS_BFS_SOLVING_ALGORITHM_H
#define MMANN_FCS_BFS_SOLVING_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSBFSSolvingAlgorithm.h
///\brief This file contains the FCSBFSSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include "FCSFreecellData.h"
#include "FCSFreecellAlgorithm.h"

///Breadth First Search method solver
template <class SolvingAlgorithmType>
class FCSBFSSolvingAlgorithm : public SolvingAlgorithmType
{
public:
	///Constructor with command line interface
	FCSBFSSolvingAlgorithm(FCCommandLineArguments* CommandLine);

	///Destructor
	virtual ~FCSBFSSolvingAlgorithm();

	///\brief Solve the game
	///
	///\param StateWithLocations is the start state
	///\param Depth is the starting depth
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes Solve(FCSStateWithLocations* StateWithLocations, int Depth);

	///\brief Resume solving a game
	///
	///\param Depth to continue at depth
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes Resume(int Depth);

protected:
	///\brief Default constructor
	FCSBFSSolvingAlgorithm();
	///\brief The "real" default constructor
	void InitFCSBFSSolvingAlgorithm();

	///\brief Initialize the BFS Queue
	///
	///\param StateWithLocations is the start state
	void BFSEnqueueState(FCSStateWithLocations* StateWithLocations);

	///BFS queue
	FCSStateWithLocationsLinkedList* m_BFSQueue;

	///Last item in the queue
	FCSStateWithLocationsLinkedList* m_BFSQueueLastItem;

	///Saved state for when bfs scan is resumed
	FCSStateWithLocations* m_FirstStateToCheck;
};

template <class SolvingAlgorithmType>
FCSBFSSolvingAlgorithm<SolvingAlgorithmType>::FCSBFSSolvingAlgorithm() : SolvingAlgorithmType()
{
	InitFCSBFSSolvingAlgorithm();
}

template <class SolvingAlgorithmType>
void FCSBFSSolvingAlgorithm<SolvingAlgorithmType>::InitFCSBFSSolvingAlgorithm()
{
	m_BFSQueue = NULL;
	m_BFSQueueLastItem = NULL;
	m_FirstStateToCheck = NULL;
}

template <class SolvingAlgorithmType>
FCSBFSSolvingAlgorithm<SolvingAlgorithmType>::FCSBFSSolvingAlgorithm(FCCommandLineArguments* CommandLine) : SolvingAlgorithmType(CommandLine)
{
	InitFCSBFSSolvingAlgorithm();

	// Initialize the BFS queue. We have one dummy element at the beginning
	// in order to make operations simpler.
	m_BFSQueue = new FCSStateWithLocationsLinkedList();
	m_BFSQueue->m_Next = new FCSStateWithLocationsLinkedList();
	m_BFSQueueLastItem = m_BFSQueue->m_Next;
	m_BFSQueueLastItem->m_Next = NULL;
}

template <class SolvingAlgorithmType>
FCSBFSSolvingAlgorithm<SolvingAlgorithmType>::~FCSBFSSolvingAlgorithm()
{
	// Free the BFS linked list
	FCSStateWithLocationsLinkedList *Item = m_BFSQueue, 
									*NextItem;
	while (Item != NULL)
	{
		NextItem = Item->m_Next;
		delete Item;
		Item = NextItem;
	}
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSBFSSolvingAlgorithm<SolvingAlgorithmType>::Solve(FCSStateWithLocations* StateWithLocations, int Depth)
{
	int NumberOfFreeStacks = 0,
		NumberOfFreecells = 0,
		DerivedIndex, a;

	FCSStateSolvingReturnCodes Check;

	FCSDerivedStatesList Derived;

	//initializes part of the StateWithLocations
	InitSolve(StateWithLocations);

	// Initialize the first element to indicate it is the first
	StateWithLocations->m_Parent = NULL;
	StateWithLocations->m_MovesToParent = NULL;
	StateWithLocations->m_Depth = 0;

	// Continue as long as there are states in the queue or priority queue.
	while (StateWithLocations != NULL)
	{
		if (StateWithLocations->m_Visited & FCS_VISITED_VISITED)
			goto NextState;

		// Count the free-cells
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

		//  Insert all the derived states into the PQ or Queue
		for(DerivedIndex = 0; DerivedIndex < Derived.m_NumberOfStates; DerivedIndex++)
			BFSEnqueueState(Derived.m_States[DerivedIndex]);

		StateWithLocations->m_Visited |= FCS_VISITED_VISITED;
		StateWithLocations->m_VisitIterations = m_NumberOfCheckedStates-1;

NextState:

		//  Extract the next item in the queue/priority queue.
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

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSBFSSolvingAlgorithm<SolvingAlgorithmType>::Resume(int Depth)
{
 	FCSStateWithLocations* StateWithLocations = m_FirstStateToCheck;

	int NumberOfFreeStacks = 0,
		NumberOfFreecells = 0,
		DerivedIndex, a;

	FCSStateSolvingReturnCodes Check;

	FCSDerivedStatesList Derived;

	// Continue as long as there are states in the queue or priority queue.
	while (StateWithLocations != NULL)
	{
		if (StateWithLocations->m_Visited & FCS_VISITED_VISITED)
			goto NextState;

		// Count the free-cells
		for(a=0;a<m_NumberOfFreecells;a++)
		{
			if (StateWithLocations->GetFreecellCardNumber(a) == 0)
				NumberOfFreecells++;
		}

		// Count the number of unoccupied stacks
		for(a=0;a<m_NumberOfStacks;a++)
		{
			if (StateWithLocations->GetStackLength(a) == 0)
				NumberOfFreeStacks++;
		}

		if ((NumberOfFreeStacks == m_NumberOfStacks) && (NumberOfFreecells == m_NumberOfFreecells))
		{
			m_FinalState = StateWithLocations;

			// Free the memory that was allocated by derived.
			DeleteDerived(&Derived);
			return FCS_STATE_WAS_SOLVED;
		}

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

		//  Insert all the derived states into the PQ or Queue
		for(DerivedIndex = 0; DerivedIndex < Derived.m_NumberOfStates; DerivedIndex++)
			BFSEnqueueState(Derived.m_States[DerivedIndex]);

		StateWithLocations->m_Visited |= FCS_VISITED_VISITED;
		StateWithLocations->m_VisitIterations = m_NumberOfCheckedStates-1;

NextState:

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

template <class SolvingAlgorithmType>
void FCSBFSSolvingAlgorithm<SolvingAlgorithmType>::BFSEnqueueState(FCSStateWithLocations* StateWithLocations)
{

	m_BFSQueueLastItem->m_Next = new FCSStateWithLocationsLinkedList();
	m_BFSQueueLastItem->m_State = StateWithLocations;
	m_BFSQueueLastItem->m_Next->m_Next = NULL;
	m_BFSQueueLastItem = m_BFSQueueLastItem->m_Next;
}

#endif