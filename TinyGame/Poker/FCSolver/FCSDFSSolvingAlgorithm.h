#ifndef MMANN_FCS_DFS_SOLVING_ALGORITHM_H
#define MMANN_FCS_DFS_SOLVING_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSDFSSolvingAlgorithm.h
///\brief This file contains the FCSDFSSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include "FCSFreecellData.h"
#include "FCSFreecellAlgorithm.h"

///Depth First Search method solver
template <class SolvingAlgorithmType>
class FCSDFSSolvingAlgorithm : public SolvingAlgorithmType
{
public:
	///Constructor with command line interface
	FCSDFSSolvingAlgorithm(FCCommandLineArguments* CommandLine);

	///Destructor
	virtual ~FCSDFSSolvingAlgorithm();

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
	///Default constructor
	FCSDFSSolvingAlgorithm();
	
	///The "real" default constructor
	void InitFCSDFSSolvingAlgorithm();

	///Determines if the DFS solver has been initialized
	bool m_IsInitialized;
};

template <class SolvingAlgorithmType>
FCSDFSSolvingAlgorithm<SolvingAlgorithmType>::FCSDFSSolvingAlgorithm() : SolvingAlgorithmType()
{
	InitFCSDFSSolvingAlgorithm();
}

template <class SolvingAlgorithmType>
void FCSDFSSolvingAlgorithm<SolvingAlgorithmType>::InitFCSDFSSolvingAlgorithm()
{
	m_IsInitialized = false;
}

template <class SolvingAlgorithmType>
FCSDFSSolvingAlgorithm<SolvingAlgorithmType>::FCSDFSSolvingAlgorithm(FCCommandLineArguments* CommandLine) : SolvingAlgorithmType(CommandLine)
{
	InitFCSDFSSolvingAlgorithm();
}

template <class SolvingAlgorithmType>
FCSDFSSolvingAlgorithm<SolvingAlgorithmType>::~FCSDFSSolvingAlgorithm()
{
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSDFSSolvingAlgorithm<SolvingAlgorithmType>::Solve(FCSStateWithLocations* StateWithLocations, int Depth)
{
	FCSStateSolvingReturnCodes Check;

	int NumberOfIterations = m_NumberOfCheckedStates,
		NumberOfFreeStacks = 0, 
		NumberOfFreecells = 0,
		a;

	FCSDerivedStatesList Derived;

	//DFS is the only solving method that calls Solve() recursively and
	//InitSolve is meant to be only called once.
	if (!m_IsInitialized)
	{
		InitSolve(StateWithLocations);
		m_IsInitialized = true;
	}

	/*
	 * If this state has not been visited before - increase the number of
	 * iterations this program has seen, and output this state again.
	 *
	 * I'm doing this in order to make the output of a stopped and
	 * resumed run consistent with the output of a normal (all-in-one-time)
	 * run.
	 * */
	if (!(StateWithLocations->m_Visited & FCS_VISITED_VISITED))
	{
		m_DebugDisplayInfo->Display(m_StateType, m_NumberOfCheckedStates, Depth, 
									StateWithLocations, 0, m_NumberOfStatesInCollection);

		// Increase the number of iterations
		m_NumberOfCheckedStates++;
		StateWithLocations->m_VisitIterations = NumberOfIterations;
	}

	// Mark this state as visited, so it won't be recursed into again.
	StateWithLocations->m_Visited |= FCS_VISITED_VISITED;

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


	// Let's check if this state is finished, and if so return 0;
	if ((NumberOfFreeStacks == m_NumberOfStacks) && (NumberOfFreecells == m_NumberOfFreecells))
	{
		m_FinalState = StateWithLocations;
		DeleteDerived(&Derived);
		return FCS_STATE_WAS_SOLVED;
	}

	for(a=0; a < m_TestsOrderNumber; a++)
	{
		Derived.m_NumberOfStates = 0;

		Check = RunTest(m_TestsOrder[a] & FCS_TEST_ORDER_NO_FLAGS_MASK, StateWithLocations, Depth, NumberOfFreeStacks,
						NumberOfFreecells, &Derived);

		if ((Check == FCS_STATE_BEGIN_SUSPEND_PROCESS) ||
			(Check == FCS_STATE_SUSPEND_PROCESS))
		{
			if (Check == FCS_STATE_BEGIN_SUSPEND_PROCESS)
			{
				m_NumberOfSolutionStates = Depth+1;

				m_SolutionStates = CreateStateWithLocationsMatrix(m_NumberOfSolutionStates);
				m_ProtoSolutionMoves = new FCSMoveStack*[m_NumberOfSolutionStates-1];
			}

			m_SolutionStates->Set(Depth, StateWithLocations);

			DeleteDerived(&Derived);
			return FCS_STATE_SUSPEND_PROCESS;
		}

		for(int DerivedStateIndex=0; DerivedStateIndex<Derived.m_NumberOfStates; DerivedStateIndex++)
		{
			if (!Derived.m_States[DerivedStateIndex]->m_Visited)
			{
				Check = Solve(Derived.m_States[DerivedStateIndex], Depth+1);

				if ((Check == FCS_STATE_SUSPEND_PROCESS) ||
					(Check == FCS_STATE_BEGIN_SUSPEND_PROCESS))
				{
					m_SolutionStates->Set(Depth, StateWithLocations);

					DeleteDerived(&Derived);
					return FCS_STATE_SUSPEND_PROCESS;
				}

				if (Check == FCS_STATE_WAS_SOLVED)
				{
					DeleteDerived(&Derived);
					return FCS_STATE_WAS_SOLVED;
				}
			}
		}
	}

	DeleteDerived(&Derived);

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSDFSSolvingAlgorithm<SolvingAlgorithmType>::Resume(int Depth)
{
	FCSStateWithLocations* StateWithLocations = m_SolutionStates->Get(Depth, 0);
	FCSMoveStack* Moves = NULL;

	FCSStateSolvingReturnCodes Check;

	bool UseOwnMoves = true;

	if (Depth < m_NumberOfSolutionStates-1)
	{
		Check = Resume(Depth+1);
	}
	else
	{
		delete [] m_SolutionStates;
		m_SolutionStates = NULL;
		delete [] m_ProtoSolutionMoves;
		m_ProtoSolutionMoves = NULL;
		Check = FCS_STATE_IS_NOT_SOLVEABLE;
	}

	switch(Check)
	{
	case FCS_STATE_IS_NOT_SOLVEABLE:
		Check = Solve(StateWithLocations, Depth);
		break;
	case FCS_STATE_WAS_SOLVED:
		break;
	case FCS_STATE_SUSPEND_PROCESS:
		m_SolutionStates->Set(Depth, StateWithLocations);
		break;
	}

	return Check;
}

#endif