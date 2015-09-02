#ifndef MMANN_FCS_ASTAR_SOLVING_ALGORITHM_H
#define MMANN_FCS_ASTAR_SOLVING_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSAStarSolvingAlgorithm.h
///\brief This file contains the FCSAStarSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include "FCSFreecellData.h"
#include "FCSFreecellAlgorithm.h"
#include "PriorityQueue.h"

///Parameter for state rating
#define FCS_A_STAR_CARDS_UNDER_SEQUENCES_EXPONENT 1.3
///Parameter for state rating
#define FCS_A_STAR_SEQS_OVER_RENEGADE_CARDS_EXPONENT 1.3

///The defualt weight values for the a-star ratings
static const double FCSAStarDefaultWeights[5] = {0.5,0,0.3,0,0.2};

/// An enum that specifies the meaning of each A* weight.
enum AStarWeightEnum {FCS_A_STAR_WEIGHT_CARDS_OUT  = 0,
					  FCS_A_STAR_WEIGHT_MAX_SEQUENCE_MOVE  = 1,
					  FCS_A_STAR_WEIGHT_CARDS_UNDER_SEQUENCES = 2,
					  FCS_A_STAR_WEIGHT_SEQS_OVER_RENEGADE_CARDS = 3,
					  FCS_A_STAR_WEIGHT_DEPTH = 4};

///A-star method solver
template <class SolvingAlgorithmType>
class FCSAStarSolvingAlgorithm : public SolvingAlgorithmType
{
public:
	///Constructor with command line interface
	FCSAStarSolvingAlgorithm(FCCommandLineArguments* CommandLine);

	///Destructor
	virtual ~FCSAStarSolvingAlgorithm();

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
	FCSAStarSolvingAlgorithm();

	///\brief The "real" default constructor
	void InitFCSAStarSolvingAlgorithm();

	///\brief Initialize the a-star values
	///
	///\param StateWithLocations is the start state to initialize the values
	void InitAStar(FCSStateWithLocations* StateWithLocations);

	///\brief Rate the state
	///
	///\param StateWithLocations is the state to rate
	///\return State rating
	int AStarRateState(FCSStateWithLocations* StateWithLocations);

private:
	///Put the states in a priority queue based on their rating
	PriorityQueue<FCSStateWithLocations>* m_AStarQueue;

	///Part of rating the states
	double m_AStarInitialCardsUnderSequence;

	///Value of the a-star weights
	double m_AStarWeights[5];

	///Saved state for when a-star scan is resumed
	FCSStateWithLocations* m_FirstStateToCheck;

};

template <class SolvingAlgorithmType>
FCSAStarSolvingAlgorithm<SolvingAlgorithmType>::FCSAStarSolvingAlgorithm() : SolvingAlgorithmType()
{
	InitFCSAStarSolvingAlgorithm();
}

template <class SolvingAlgorithmType>
void FCSAStarSolvingAlgorithm<SolvingAlgorithmType>::InitFCSAStarSolvingAlgorithm()
{
	m_AStarQueue = NULL;

	// Set the default A* weigths
	for(int a=0;a<(sizeof(m_AStarWeights)/sizeof(m_AStarWeights[0]));a++)
	{
		m_AStarWeights[a] = FCSAStarDefaultWeights[a];
	}

	m_AStarInitialCardsUnderSequence = 0;
	m_FirstStateToCheck = NULL;
}

template <class SolvingAlgorithmType>
FCSAStarSolvingAlgorithm<SolvingAlgorithmType>::FCSAStarSolvingAlgorithm(FCCommandLineArguments* CommandLine) : SolvingAlgorithmType(CommandLine)
{
unsigned int a;
double sum = 0;

	InitFCSAStarSolvingAlgorithm();

	m_AStarQueue = new PriorityQueue<FCSStateWithLocations>(1024, INT_MAX, 256, true);

	//Initialize the priotity queue of the A* scan
	if (CommandLine->GetAStarWeights() != NULL)
	{
		for (a=0; a < sizeof(m_AStarWeights)/sizeof(m_AStarWeights[0]);a++)
		{
			if (CommandLine->GetAStarWeightValues(a) < 0)
				m_AStarWeights[a] = FCSAStarDefaultWeights[a];
			else
				m_AStarWeights[a] = CommandLine->GetAStarWeightValues(a);
		}

		// Normalize the A* Weights, so the sum of all of them would be 1.
		for(a=0; a < (sizeof(m_AStarWeights)/sizeof(m_AStarWeights[0]));a++)
			sum += m_AStarWeights[a];

		if (sum == 0)
			sum = 1;

		for(a=0;a<(sizeof(m_AStarWeights)/sizeof(m_AStarWeights[0]));a++)
			m_AStarWeights[a] /= sum;
	}
}

template <class SolvingAlgorithmType>
FCSAStarSolvingAlgorithm<SolvingAlgorithmType>::~FCSAStarSolvingAlgorithm()
{
	if (m_AStarQueue != NULL)
		delete m_AStarQueue;
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSAStarSolvingAlgorithm<SolvingAlgorithmType>::Solve(FCSStateWithLocations* StateWithLocations, int Depth)
{
	int NumberOfFreeStacks = 0,
		NumberOfFreecells = 0,
		DerivedIndex, a;

	FCSStateSolvingReturnCodes Check;

	FCSDerivedStatesList Derived;
 
	//initializes part of the StateWithLocations
	InitSolve(StateWithLocations);

	InitAStar(StateWithLocations);

	// Initialize the first element to indicate it is the first
	StateWithLocations->m_Parent = NULL;
	StateWithLocations->m_MovesToParent = NULL;
	StateWithLocations->m_Depth = 0;

	// Continue as long as there are states in the queue or priority queue.
	while (StateWithLocations != NULL)
	{
		if (StateWithLocations->m_Visited & FCS_VISITED_VISITED)
			goto NextState;

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
		// done for BFS and A*

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
			m_AStarQueue->Push(Derived.m_States[DerivedIndex], AStarRateState(Derived.m_States[DerivedIndex]));

		StateWithLocations->m_Visited |= FCS_VISITED_VISITED;

		StateWithLocations->m_VisitIterations = m_NumberOfCheckedStates-1;

NextState:

		//  Extract the next item in the queue/priority queue.
		StateWithLocations = m_AStarQueue->Pop();
	}

	// Free the memory that was allocated by derived.
	DeleteDerived(&Derived);

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

template <class SolvingAlgorithmType>
FCSStateSolvingReturnCodes FCSAStarSolvingAlgorithm<SolvingAlgorithmType>::Resume(int Depth)
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
		//this is what the code would be without the optimize stuff
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

		// Insert all the derived states into the PQ or Queue
		for(DerivedIndex = 0; DerivedIndex < Derived.m_NumberOfStates; DerivedIndex++)
			m_AStarQueue->Push(Derived.m_States[DerivedIndex], AStarRateState(Derived.m_States[DerivedIndex]));

		StateWithLocations->m_Visited |= FCS_VISITED_VISITED;
		StateWithLocations->m_VisitIterations = m_NumberOfCheckedStates-1;

NextState:
		//	Extract the next item in the queue/priority queue.
		// It is an A* scan
		StateWithLocations = m_AStarQueue->Pop();
	}

	// Free the memory that was allocated by derived.
	DeleteDerived(&Derived);

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

template <class SolvingAlgorithmType>
void FCSAStarSolvingAlgorithm<SolvingAlgorithmType>::InitAStar(FCSStateWithLocations* StateWithLocations)
{
	int NumberOfCards, c;
	FCSCard *ThisCard = FCSCard::Create(), 
			*PreviousCard = FCSCard::Create();
	double CardsUnderSequences = 0;

	for(int a=0;a<m_NumberOfStacks;a++)
	{
		NumberOfCards = StateWithLocations->GetStackLength(a);

		if (NumberOfCards <= 1)
			continue;

		c = NumberOfCards-2;
		ThisCard->Copy(StateWithLocations->GetStackCard(a, c+1));
		PreviousCard->Copy(StateWithLocations->GetStackCard(a, c));

		while (IsParentCard(ThisCard, PreviousCard) && (c >= 0))
		{
			c--;
			ThisCard->Copy(PreviousCard);
			if (c>=0)
				PreviousCard->Copy(StateWithLocations->GetStackCard(a, c));
		}
		CardsUnderSequences += pow(c+1, FCS_A_STAR_CARDS_UNDER_SEQUENCES_EXPONENT);
	}

	m_AStarInitialCardsUnderSequence = CardsUnderSequences;

	delete ThisCard;
	delete PreviousCard;
}

template <class SolvingAlgorithmType>
int FCSAStarSolvingAlgorithm<SolvingAlgorithmType>::AStarRateState(FCSStateWithLocations* StateWithLocations)
{
	FCSCard *ThisCard = FCSCard::Create(), 
		*PreviousCard = FCSCard::Create();

	int NumberOfCards, 
		NumberOfCardsInFounds = 0,
		NumberOfFreeStacks = 0,
		NumberOfFreecells = 0,
		a, c;

	double CardsUnderSequences = 0,
		   SequencesOverRenegadeCards = 0,
		   ReturnValue = 0,
		   Temp;

	for(a=0;a<m_NumberOfStacks;a++)
	{
		NumberOfCards = StateWithLocations->GetStackLength(a);
		if (NumberOfCards == 0)
			NumberOfFreeStacks++;

		if (NumberOfCards <= 1)
			continue;

		c = NumberOfCards-2;
		ThisCard->Copy(StateWithLocations->GetStackCard(a, c+1));
		PreviousCard->Copy(StateWithLocations->GetStackCard(a, c));
		while ((c >= 0) && IsParentCard(ThisCard, PreviousCard))
		{
			c--;
			ThisCard->Copy(PreviousCard);
			if (c>=0)
				PreviousCard->Copy(StateWithLocations->GetStackCard(a, c));
		}

		CardsUnderSequences += pow(c+1, FCS_A_STAR_CARDS_UNDER_SEQUENCES_EXPONENT);

		if (c >= 0)
		{
			SequencesOverRenegadeCards += ((m_IsUnlimitedSequenceMove) ? 
				1 :  pow(NumberOfCards-c-1, FCS_A_STAR_SEQS_OVER_RENEGADE_CARDS_EXPONENT));
		}
	}

	ReturnValue += ((m_AStarInitialCardsUnderSequence - CardsUnderSequences) /
				m_AStarInitialCardsUnderSequence) * m_AStarWeights[FCS_A_STAR_WEIGHT_CARDS_UNDER_SEQUENCES];

	ReturnValue += (SequencesOverRenegadeCards / 
			   pow(m_NumberOfDecks*52, FCS_A_STAR_SEQS_OVER_RENEGADE_CARDS_EXPONENT) )
		   * m_AStarWeights[FCS_A_STAR_WEIGHT_SEQS_OVER_RENEGADE_CARDS];

	for(a=0;a<m_NumberOfDecks*4;a++)
	{
		NumberOfCardsInFounds += StateWithLocations->GetFoundation(a);
	}

	ReturnValue += ((double)NumberOfCardsInFounds/(m_NumberOfDecks*52)) * m_AStarWeights[FCS_A_STAR_WEIGHT_CARDS_OUT];

	for(a=0;a<m_NumberOfFreecells;a++)
	{
		if (StateWithLocations->GetFreecellCardNumber(a) == 0)
			NumberOfFreecells++;
	}

	if (m_EmptyStacksFill == FCS_ES_FILLED_BY_ANY_CARD)
	{
		if (m_IsUnlimitedSequenceMove)
		{
			Temp = (((double)NumberOfFreecells+NumberOfFreeStacks)/(m_NumberOfFreecells+m_NumberOfStacks));
		}
		else
		{
			Temp = (((double)((NumberOfFreecells+1)<<NumberOfFreeStacks)) / ((m_NumberOfFreecells+1)<<(m_NumberOfStacks)));
		}
	}
	else
	{
		if (m_IsUnlimitedSequenceMove)
		{
			Temp = (((double)NumberOfFreecells)/m_NumberOfFreecells);
		}
		else
		{
			Temp = 0;
		}
	}

	ReturnValue += (Temp * m_AStarWeights[FCS_A_STAR_WEIGHT_MAX_SEQUENCE_MOVE]);

	if (StateWithLocations->m_Depth <= 20000)
		ReturnValue += ((20000 - StateWithLocations->m_Depth)/20000.0) * m_AStarWeights[FCS_A_STAR_WEIGHT_DEPTH];

	delete ThisCard;
	delete PreviousCard;

	return (int)(ReturnValue*INT_MAX);
}

#endif