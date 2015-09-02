#ifndef FCS_RANDOM_DFS_SOLVING_ALGORITHM_H
#define FCS_RANDOM_DFS_SOLVING_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSRandomDFSSolvingAlgorithm.h
///\brief This file contains the FCSRandomDFSSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include "FCSSoftDFSSolvingAlgorithm.h"
#include "FCSRandom.h"
#include "FCHelpingAlgorithms.h"

///Random DFS method solver
template <class SolvingAlgorithmType>
class FCSRandomDFSSolvingAlgorithm : public FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>
{
public:
	///Constructor with command line interface
	FCSRandomDFSSolvingAlgorithm(FCCommandLineArguments* CommandLine);

	///Destructor
	virtual ~FCSRandomDFSSolvingAlgorithm();

protected:
	///\brief Default constructor
	FCSRandomDFSSolvingAlgorithm();

	///\brief The "real" default constructor
	void InitFCSRandomDFSSolvingAlgorithm();

	///\brief Solve or resume solving a game
	///
	///\param Depth to continue at depth
	///\return Solving return code
	virtual FCSStateSolvingReturnCodes SolveOrResume(int Depth);

	///\brief Increase the max depth and the affected structures
	virtual void IncreaseDFSMaxDepth();

	///Array of maximum sizes for the array of random derived states
	int* m_SoftDFSDerivedStatesRandomIndexesMaxSize;
	///Array of random dervied states
	int** m_SoftDFSDerivedStatesRandomIndexes;

	///Random number generator
	FCSRandom* m_RandomGenerator;
};

template <class SolvingAlgorithmType>
FCSRandomDFSSolvingAlgorithm<SolvingAlgorithmType>::FCSRandomDFSSolvingAlgorithm() : FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>()
{
	InitFCSRandomDFSSolvingAlgorithm();
}

template <class SolvingAlgorithmType>
void FCSRandomDFSSolvingAlgorithm<SolvingAlgorithmType>::InitFCSRandomDFSSolvingAlgorithm()
{
	m_SoftDFSDerivedStatesRandomIndexesMaxSize = NULL;
	m_SoftDFSDerivedStatesRandomIndexes = NULL;
	m_RandomGenerator = NULL;
}

template <class SolvingAlgorithmType>
FCSRandomDFSSolvingAlgorithm<SolvingAlgorithmType>::FCSRandomDFSSolvingAlgorithm(FCCommandLineArguments* CommandLine) : FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>(CommandLine)
{
	InitFCSRandomDFSSolvingAlgorithm();
	m_RandomGenerator = new FCSRandom(CommandLine->GetSeed());
}

template <class SolvingAlgorithmType>
FCSRandomDFSSolvingAlgorithm<SolvingAlgorithmType>::~FCSRandomDFSSolvingAlgorithm()
{
	int Depth;

	for(Depth=0;Depth<m_NumberOfSolutionStates-1;Depth++)
		delete [] m_SoftDFSDerivedStatesRandomIndexes[Depth];

	for(;Depth<m_DFSMaxDepth;Depth++)
		if (m_SoftDFSDerivedStatesLists[Depth].m_MaxNumberOfStates)
			delete [] m_SoftDFSDerivedStatesRandomIndexes[Depth];

	delete [] m_SoftDFSDerivedStatesRandomIndexes;
	m_SoftDFSDerivedStatesRandomIndexes = NULL;
	delete [] m_SoftDFSDerivedStatesRandomIndexesMaxSize;
	m_SoftDFSDerivedStatesRandomIndexesMaxSize = NULL;

	delete m_RandomGenerator;
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSRandomDFSSolvingAlgorithm<SolvingAlgorithmType>::SolveOrResume(int Depth)
{
	FCSStateWithLocations *StateWithLocations,
						  *RecursiveStateWithLocations;

	int a, j;
	FCSStateSolvingReturnCodes Check;

	//The main loop.
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
			// freecells and stacks and check if we are done.
			if (m_SoftDFSTestIndexes[Depth] == 0)
			{
				int NumberOfFreeStacks = 0,
					NumberOfFreecells = 0;

				m_DebugDisplayInfo->Display(m_StateType, m_NumberOfCheckedStates, Depth, StateWithLocations,
					((Depth == 0) ? 0 : m_SolutionStates->Get(Depth-1, 0)->m_VisitIterations), m_NumberOfStatesInCollection);

				// Count the free-cells
				for(a=0;a<m_NumberOfFreecells;a++)
					if (StateWithLocations->GetFreecellCardNumber(a) == 0)
						NumberOfFreecells++;

				// Count the number of unoccupied stacks
				for(a=0;a<m_NumberOfStacks;a++)
					if (StateWithLocations->GetStackLength(a) == 0)
						NumberOfFreeStacks++;

				// Check if we have reached the empty state
				if ((NumberOfFreeStacks == m_NumberOfStacks) && (NumberOfFreecells == m_NumberOfFreecells))
				{
					m_FinalState = StateWithLocations;
					return FCS_STATE_WAS_SOLVED;
				}

				//	Cache num_freecells and num_freestacks in their 
				//	appropriate stacks, so they won't be calculated over and over again.
				m_SoftDFSNumberOfFreecells[Depth] = NumberOfFreecells;
				m_SoftDFSNumberOfFreestacks[Depth] = NumberOfFreeStacks;
			}

			if (m_SoftDFSTestIndexes[Depth] < m_TestsOrderNumber)
			{
				do
				{
					Check = RunTest(m_TestsOrder[m_SoftDFSTestIndexes[Depth]] & FCS_TEST_ORDER_NO_FLAGS_MASK,
									StateWithLocations, Depth,
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
				while (
					  // Make sure we do not exceed the number of tests
					  (m_SoftDFSTestIndexes[Depth] < m_TestsOrderNumber) && 
					  // We are still on a random group
					  (m_TestsOrder[m_SoftDFSTestIndexes[Depth]] & FCS_TEST_ORDER_FLAG_RANDOM) && 
					  // A new random group did not start
					  (! (m_TestsOrder[m_SoftDFSTestIndexes[Depth]] & FCS_TEST_ORDER_FLAG_START_RANDOM_GROUP)));
			}

			int SwapSave,
				*RandomArray;

			if (m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates > 
				m_SoftDFSDerivedStatesRandomIndexesMaxSize[Depth])
			{
				Realloc<int>(&m_SoftDFSDerivedStatesRandomIndexes[Depth], 
					m_SoftDFSDerivedStatesRandomIndexesMaxSize[Depth]+1,
					m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates);

				m_SoftDFSDerivedStatesRandomIndexesMaxSize[Depth] = 
					m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates;
			}

			RandomArray = m_SoftDFSDerivedStatesRandomIndexes[Depth];

			for (a=0;a<m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates;a++)
				RandomArray[a] = a;

			// If we just conducted the tests for a random group - randomize.
			// Else - keep those indexes as the unity vector.
			if (m_TestsOrder[m_SoftDFSTestIndexes[Depth]-1] & FCS_TEST_ORDER_FLAG_RANDOM)
			{
				a = m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates-1;
				while (a > 0)
				{
					j = m_RandomGenerator->GetRandomNumber() % (a+1);

					SwapSave = RandomArray[a];
					RandomArray[a] = RandomArray[j];
					RandomArray[j] = SwapSave;
					a--;
				}
			}

			// We just performed a test, so the index of the first state that
			//  ought to be checked in this depth is 0.
			m_SoftDFSCurrentStateIndexes[Depth] = 0;
		}

		while (m_SoftDFSCurrentStateIndexes[Depth] < m_SoftDFSDerivedStatesLists[Depth].m_NumberOfStates)
		{
			RecursiveStateWithLocations = m_SoftDFSDerivedStatesLists[Depth].m_States[
				m_SoftDFSDerivedStatesRandomIndexes[Depth][
					m_SoftDFSCurrentStateIndexes[Depth]]];

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
void FCSRandomDFSSolvingAlgorithm<SolvingAlgorithmType>::IncreaseDFSMaxDepth()
{
	int NewDFSMaxDepth = m_DFSMaxDepth + INCREASE_DFS_MAX_DEPTH_BY;
 
	Realloc<int*>(&m_SoftDFSDerivedStatesRandomIndexes, m_DFSMaxDepth, NewDFSMaxDepth);
	Realloc<int>(&m_SoftDFSDerivedStatesRandomIndexesMaxSize, m_DFSMaxDepth, NewDFSMaxDepth);

	for(int d=m_DFSMaxDepth; d<NewDFSMaxDepth; d++)
	{
		m_SoftDFSDerivedStatesRandomIndexes[d] = NULL;
		m_SoftDFSDerivedStatesRandomIndexesMaxSize[d] = 0;
	}

	//this is the function that saves the "new" m_DFSMaxDepth 
	FCSSoftDFSSolvingAlgorithm<SolvingAlgorithmType>::IncreaseDFSMaxDepth();
}
#endif