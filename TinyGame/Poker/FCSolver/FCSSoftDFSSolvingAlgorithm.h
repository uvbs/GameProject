#ifndef MMANN_FCS_SOFT_DFS_SOLVING_ALGORITHM_H
#define MMANN_FCS_SOFT_DFS_SOLVING_ALGORITHM_H

#include <stdlib.h>
#include <string.h>
#include "FCSFreecellData.h" 
#include "FCSFreecellAlgorithm.h"
#include "FCSStateStorage.h"
#include "FCHelpingAlgorithms.h"

////////////////////////////////////////////////
///\file FCSSoftDFSSolvingAlgorithm.h
///\brief This file contains the FCSSoftDFSSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////


///Soft DFS method solver
template <class SolvingAlgorithmType>
class FCSSoftDFSSolvingAlgorithm : public SolvingAlgorithmType
{
public:
	///Constructor with command line interface
	FCSSoftDFSSolvingAlgorithm(FCCommandLineArguments* CommandLine);

	///Destructor
	virtual ~FCSSoftDFSSolvingAlgorithm();

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
	FCSSoftDFSSolvingAlgorithm();

	///The "real" default constructor
	void InitFCSSoftDFSSolvingAlgorithm();


	///\brief Solve or resume solving a game
	///
	///\param Depth to continue at depth
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes SolveOrResume(int Depth);

	///\brief Increase the max depth and the affected structures
	virtual void IncreaseDFSMaxDepth();

	///Temporary max depth of the scan
	int m_DFSMaxDepth;

	/// Derived state list for the soft-dfs scans
	FCSDerivedStatesList* m_SoftDFSDerivedStatesLists;

	/// m_SoftDFSCurrentStateIndexes[i] - The index of the last checked state in depth i.
	int * m_SoftDFSCurrentStateIndexes;

	///\brief m_SoftDFSTestIndexes[i] - The index of the test that was last performed. 
	/// FCS performs each test separately, so states_to_check[i] and
	/// friends will not be overpopulated.
	int * m_SoftDFSTestIndexes;

	///\brief m_SoftDFSNumberOfFreestacks[i] - The number of unoccupied stacks that 
	///correspond to m_SolutionStates[i].
	int * m_SoftDFSNumberOfFreestacks;

	///\brief m_SoftDFSNumberOfFreecells[i] - The number of unoccupied freecells that 
	///correspond to m_SolutionStates[i].
	int * m_SoftDFSNumberOfFreecells;
};

template <class SolvingAlgorithmType>
FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::FCSSoftDFSSolvingAlgorithm() : SolvingAlgorithmType()
{
	InitFCSSoftDFSSolvingAlgorithm();
}

template <class SolvingAlgorithmType>
void FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::InitFCSSoftDFSSolvingAlgorithm()
{
	m_DFSMaxDepth = 0;

	//Initialize SoftDFS Stack
	m_SoftDFSDerivedStatesLists = NULL;
	m_SoftDFSCurrentStateIndexes = NULL;
	m_SoftDFSTestIndexes = NULL;
	m_SoftDFSNumberOfFreestacks = NULL;
	m_SoftDFSNumberOfFreecells = NULL;
}


template <class SolvingAlgorithmType>
FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::FCSSoftDFSSolvingAlgorithm(FCCommandLineArguments* CommandLine) : SolvingAlgorithmType(CommandLine)
{
	InitFCSSoftDFSSolvingAlgorithm();
}

template <class SolvingAlgorithmType>
FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::~FCSSoftDFSSolvingAlgorithm()
{
	// De-allocate the Soft-DFS specific stacks
	int Depth;
	for(Depth=0;Depth<m_NumberOfSolutionStates-1;Depth++)
		delete [] m_SoftDFSDerivedStatesLists[Depth].m_States;

	for(;Depth<m_DFSMaxDepth;Depth++)
		if (m_SoftDFSDerivedStatesLists[Depth].m_MaxNumberOfStates)
			delete [] m_SoftDFSDerivedStatesLists[Depth].m_States;

	delete [] m_SoftDFSDerivedStatesLists;
	m_SoftDFSDerivedStatesLists = NULL;
	delete [] m_SoftDFSTestIndexes;
	m_SoftDFSTestIndexes = NULL;
	delete [] m_SoftDFSCurrentStateIndexes;
	m_SoftDFSCurrentStateIndexes = NULL;
	delete [] m_SoftDFSNumberOfFreecells;
	m_SoftDFSNumberOfFreecells = NULL;
	delete [] m_SoftDFSNumberOfFreestacks;
	m_SoftDFSNumberOfFreestacks = NULL;
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::Solve(FCSStateWithLocations* StateWithLocations, int Depth)
{
	Depth = 0;

	//initializes part of the StateWithLocations
	InitSolve(StateWithLocations);

	IncreaseDFSMaxDepth();
	m_SolutionStates->Set(0, StateWithLocations);

	return SolveOrResume(Depth);
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::Resume(int Depth)
{
	Depth = m_NumberOfSolutionStates - 1;

	return SolveOrResume(Depth);
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::SolveOrResume(int Depth)
{
	FCSStateWithLocations *StateWithLocations,
						  *RecursiveStateWithLocations;

	FCSStateSolvingReturnCodes Check;

	int a;

	while (Depth >= 0)
	{
		
		//	Increase the "maximal" depth if it about to be exceeded.
		if (Depth+1 >= m_DFSMaxDepth)
			IncreaseDFSMaxDepth();

		// All the resultant states in the last test conducted were covered
		if (m_SoftDFSCurrentStateIndexes[Depth] == m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates)
		{
			if (m_SoftDFSTestIndexes[Depth] >= m_TestsOrderNumber)
			{
				// Backtrack to the previous depth.
				Depth--;
				continue; // Just to make sure depth is not -1 now
			}

			m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates = 0;
			StateWithLocations = m_SolutionStates->Get(Depth, 0);

			// If this is the first test, then count the number of unoccupied
			// freeceels and stacks and check if we are done. */
			if (m_SoftDFSTestIndexes[Depth] == 0)
			{
				int NumberOfFreeStacks = 0, 
					NumberOfFreecells = 0;

				m_DebugDisplayInfo->Display(m_StateType, m_NumberOfCheckedStates, Depth, StateWithLocations,
					((Depth == 0) ? 0 : m_SolutionStates->Get(Depth-1, 0)->m_VisitIterations), m_NumberOfStatesInCollection);

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

				// Check if we have reached the empty state
				if ((NumberOfFreeStacks == m_NumberOfStacks) && (NumberOfFreecells == m_NumberOfFreecells))
				{
					m_FinalState = StateWithLocations;
					return FCS_STATE_WAS_SOLVED;
				}

				//	Cache num_freecells and num_freestacks in their 
				//	appropriate stacks, so they won't be calculated over and over
				//	again.
				m_SoftDFSNumberOfFreecells[Depth] = NumberOfFreecells;
				m_SoftDFSNumberOfFreestacks[Depth] = NumberOfFreeStacks;
			}

			if (m_SoftDFSTestIndexes[Depth] < m_TestsOrderNumber)
			{
				Check = RunTest(m_TestsOrder[m_SoftDFSTestIndexes[Depth]] & FCS_TEST_ORDER_NO_FLAGS_MASK,
								StateWithLocations,
								Depth, 
								m_SoftDFSNumberOfFreestacks[Depth],
								m_SoftDFSNumberOfFreecells[Depth],
								&(m_SoftDFSDerivedStatesLists[Depth]));

				if ((Check == FCS_STATE_BEGIN_SUSPEND_PROCESS) ||
					(Check == FCS_STATE_EXCEEDS_MAX_NUM_TIMES) ||
					(Check == FCS_STATE_SUSPEND_PROCESS))
				{
					// Have this test be re-performed
					m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates = 0;
					m_SoftDFSCurrentStateIndexes[Depth] = 0;
					m_NumberOfSolutionStates = Depth+1;

					return FCS_STATE_SUSPEND_PROCESS;
				}

				// Move the counter to the next test
				m_SoftDFSTestIndexes[Depth]++;
			}

			// We just performed a test, so the index of the first state that
			// ought to be checked in this depth is 0.
			m_SoftDFSCurrentStateIndexes[Depth] = 0;
		}

		while (m_SoftDFSCurrentStateIndexes[Depth] < 
			   m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates)
		{
			RecursiveStateWithLocations = m_SoftDFSDerivedStatesLists[Depth].m_States[
				m_SoftDFSCurrentStateIndexes[Depth]];

			m_SoftDFSCurrentStateIndexes[Depth]++;
			if (!RecursiveStateWithLocations->m_Visited)
			{
				m_NumberOfCheckedStates++;

				RecursiveStateWithLocations->m_Visited = FCS_VISITED_VISITED;
				RecursiveStateWithLocations->m_VisitIterations = m_NumberOfCheckedStates;
				m_SolutionStates->Set(Depth+1, RecursiveStateWithLocations);
				
				//	I'm using current_state_indexes[depth]-1 because we already
				//	increased it by one, so now it refers to the next state.
				Depth++;
				m_SoftDFSTestIndexes[Depth] = 0;
				m_SoftDFSCurrentStateIndexes[Depth] = 0;
				m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates = 0;
				break;
			}
		}
	}

	m_NumberOfSolutionStates = 0;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

template <class SolvingAlgorithmType>
void FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::IncreaseDFSMaxDepth()
{
	//*** Check that the 16 could be a #define
	int NewDFSMaxDepth = m_DFSMaxDepth + 16;

	m_SolutionStates->ReallocArray(m_DFSMaxDepth, NewDFSMaxDepth);
	ReallocMoveStackArray(&m_ProtoSolutionMoves, m_DFSMaxDepth, NewDFSMaxDepth);
	Realloc<FCSDerivedStatesList>(&m_SoftDFSDerivedStatesLists, m_DFSMaxDepth, NewDFSMaxDepth);
	Realloc<int>(&m_SoftDFSCurrentStateIndexes, m_DFSMaxDepth, NewDFSMaxDepth);
 	Realloc<int>(&m_SoftDFSTestIndexes, m_DFSMaxDepth, NewDFSMaxDepth);
 	Realloc<int>(&m_SoftDFSNumberOfFreestacks, m_DFSMaxDepth, NewDFSMaxDepth);
 	Realloc<int>(&m_SoftDFSNumberOfFreecells, m_DFSMaxDepth, NewDFSMaxDepth);
	
	for(int d=m_DFSMaxDepth; d<NewDFSMaxDepth; d++)
	{
		m_SolutionStates->Set(d, NULL);
		m_ProtoSolutionMoves[d] = NULL;
		m_SoftDFSDerivedStatesLists[d].m_MaxNumberOfStates = 0;
		m_SoftDFSTestIndexes[d] = 0;
		m_SoftDFSCurrentStateIndexes[d] = 0;
		m_SoftDFSDerivedStatesLists[d].m_NumberOfStates = 0;
		m_SoftDFSDerivedStatesLists[d].m_States = NULL;
	}

	m_DFSMaxDepth = NewDFSMaxDepth;
}

#endif