////////////////////////////////////////////////
///\file FCSTalonSolvingAlgorithm.cpp
///\brief This file contains the implementation of the FCSTalonSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date November 2002
////////////////////////////////////////////////

#include "FCSTalonSolvingAlgorithm.h"
#include "FCSAStarSolvingAlgorithm.h"
#include "FCSBFSSolvingAlgorithm.h"
#include "FCSDFSSolvingAlgorithm.h"
#include "FCSRandomDFSSolvingAlgorithm.h"
#include "FCSSoftDFSSolvingAlgorithm.h"

FCSTalonSolvingAlgorithm::FCSTalonSolvingAlgorithm() : FCSFreecellSolvingAlgorithm()
{
	InitFCSTalonSolvingAlgorithm();
}

FCSTalonSolvingAlgorithm::FCSTalonSolvingAlgorithm(FCCommandLineArguments* CommandLine) : FCSFreecellSolvingAlgorithm(CommandLine)
{
	InitFCSTalonSolvingAlgorithm();

	m_TalonType = CommandLine->GetTalonType();
}

void FCSTalonSolvingAlgorithm::InitFCSTalonSolvingAlgorithm()
{
	m_TalonType = FCS_TALON_NONE;
	m_TalonsHash = NULL;
}

FCSTalonSolvingAlgorithm::~FCSTalonSolvingAlgorithm()
{
	if (m_TalonsHash != NULL)
	{
		m_TalonsHash->DeleteItems();
		delete m_TalonsHash;
	}
}

FCSTalonSolvingAlgorithm* FCSTalonSolvingAlgorithm::Create(FCCommandLineArguments* CommandLine)
{

	if (CommandLine == NULL)
		return NULL;

	//determine with solving algorithm to create
	switch(CommandLine->GetSolvingMethodType())
	{
	case FCS_METHOD_HARD_DFS:
		return new FCSDFSSolvingAlgorithm<FCSTalonSolvingAlgorithm>(CommandLine);
	case FCS_METHOD_SOFT_DFS:
		return new FCSSoftDFSSolvingAlgorithm<FCSTalonSolvingAlgorithm>(CommandLine);
	case FCS_METHOD_BFS:
		return new FCSBFSSolvingAlgorithm<FCSTalonSolvingAlgorithm>(CommandLine);
	case FCS_METHOD_A_STAR:
		return new FCSAStarSolvingAlgorithm<FCSTalonSolvingAlgorithm>(CommandLine);
	case FCS_METHOD_RANDOM_DFS:
		return new FCSRandomDFSSolvingAlgorithm<FCSTalonSolvingAlgorithm>(CommandLine);
	default:
		return NULL;
	}

	return NULL;
}

FCSStateSolvingReturnCodes FCSTalonSolvingAlgorithm::RunTest(int TestNumber, FCSStateWithLocations*  StateWithLocations,
													  int Depth, int NumberOfFreeStacks,
													  int NumberOfFreecells,
													  FCSDerivedStatesList* DerivedStateList)
{
	switch(TestNumber)
	{
	case 0:
		return MoveTopStackCardsToFounds(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 1:
		return MoveNonTopStackCardsToFounds(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 2:
		return MoveStackCardsToDifferentStacks(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 3:
		return MoveStackCardsToAParentOnTheSameStack(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 4:
		return MoveSequencesToFreeStacks(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 5:
		return MoveCardsToADifferentParent(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 6:
		return GetCardFromKlondikeTalon(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	default:
		return FCS_STATE_IS_NOT_SOLVEABLE;
	}
}

void FCSTalonSolvingAlgorithm::InitSolve(FCSStateWithLocations* InitState)
{
	// Initialize the Talon's Cache
	if (m_TalonType == FCS_TALON_KLONDIKE)
	{
		m_TalonsHash = new FCInternalHash<FCSTalonStateData, void>(TALON_CACHE_SIZE, 
					&m_TalonCompareFunction, &m_TalonHashFunction, DELETE_HASH_ITEM);
	}

	FCSFreecellData::InitSolve(InitState);
}

FCSStateSolvingReturnCodes FCSTalonSolvingAlgorithm::CheckAndAddState(FCSStateWithLocations* NewState,
						 FCSStateWithLocations** ExistingState,
						 int Depth)
{
	if (((m_MaxNumberOfCheckedStates >= 0) &&
		(m_NumberOfCheckedStates >= m_MaxNumberOfCheckedStates)) ||
		((m_MaxNumberOfStatesInCollection >= 0) &&
		(m_NumberOfStatesInCollection >= m_MaxNumberOfStatesInCollection)))
	{
		return FCS_STATE_BEGIN_SUSPEND_PROCESS;
	}

	if ((m_MaxDepth >= 0) &&
		(m_MaxDepth <= Depth))
	{
		return FCS_STATE_EXCEEDS_MAX_DEPTH;
	}

	NewState->CanonizeState(m_NumberOfFreecells, m_NumberOfStacks);
	NewState->CacheStacks(m_StackStorage, m_NumberOfStacks);
	((FCSTalonStateWithLocations*)NewState)->CacheTalonStacks(m_TalonsHash);

	if (m_StateStorage->CheckAndInsert(ExistingState, NewState))
	{
		// The new state was not found in the cache, and it was already inserted
		m_NumberOfStatesInCollection++;
		return FCS_STATE_DOES_NOT_EXIST;
	}

	return FCS_STATE_ALREADY_EXISTS;
}

FCSStateSolvingReturnCodes FCSTalonSolvingAlgorithm::GetCardFromKlondikeTalon(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList)
{
	if (m_TalonType != FCS_TALON_KLONDIKE)
	{
		return FCS_STATE_IS_NOT_SOLVEABLE;
	}

	FCSStateWithLocations *NewStateWithLocations;
	FCSTalonStateData* TempTalonData = CreateTalonStateData();
	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	FCSStateSolvingReturnCodes Check;
	int NumberOfRedealsDone = 0,
		NumberOfCardsMoved[2] = {0,0};

	bool FirstIteration = true;

	FCSCard *CardToCheck = FCSCard::Create(),
			*TopCard = FCSCard::Create();
	
	// Duplicate the talon and its parameters into talon_temp
	TempTalonData->Copy(((FCSTalonStateWithLocations*)StateWithLocations)->GetTalonData());

	char SavedStackPosition = TempTalonData->GetKlondikeTalonStackPosition();

	// Make sure we redeal the talon only once
	int NumberOfRedealsLeft = TempTalonData->GetKlondikeNumberOfRedealsLeft();
	if (NumberOfRedealsLeft != 0)
		NumberOfRedealsLeft = 1;

	while (NumberOfRedealsLeft >= 0)
	{
		if ((TempTalonData->GetKlondikeTalonStackPosition() == -1) &&
			(TempTalonData->GetKlondikeTalonQueuePosition() == TempTalonData->GetKlondikeTalonLength()))
			break;

		if ((!FirstIteration) || (TempTalonData->GetKlondikeTalonStackPosition() == -1))
		{
			if (TempTalonData->GetKlondikeTalonStackPosition() == TempTalonData->GetKlondikeTalonLength() - 1)
			{
				if (NumberOfRedealsLeft > 0)
				{
					//no cards left to redeal
					TempTalonData->KlondikeTalonRedealBare();

					NumberOfRedealsLeft--;
					NumberOfRedealsDone++;
				}
				else
				{
					break;
				}
			}
			else if ((NumberOfRedealsLeft <= 0) &&
					 (TempTalonData->GetKlondikeTalonStackPosition() >= SavedStackPosition - 1))
			{
				break;
			}

			TempTalonData->KlondikeTalonQueueToStack();
			NumberOfCardsMoved[NumberOfRedealsDone]++;
		}

		FirstIteration = false;

		CardToCheck->Copy(TempTalonData->GetKlondikeTalonTopCard());
		if (CardToCheck->IsEmptyCard())
			continue;

		//check if the card can go on a foundation
		for(int Deck=0;Deck < m_NumberOfDecks;Deck++)
		{
			if (StateWithLocations->GetFoundation(Deck*4 + CardToCheck->GetSuit()) != CardToCheck->GetCardNumber() - 1)
				continue;

			// We can put it there
			CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

			((FCSTalonStateWithLocations*)NewStateWithLocations)->GetTalonData()->Copy(TempTalonData);

			for(int a=0;a<=NumberOfRedealsDone;a++)
			{
				TempMove->SetType(FCS_MOVE_TYPE_KLONDIKE_FLIP_TALON);
				TempMove->SetNumberOfCardsFlipped(NumberOfCardsMoved[a]);
				Moves->Push(TempMove);
				if (a != NumberOfRedealsDone)
				{
					TempMove->SetType(FCS_MOVE_TYPE_KLONDIKE_REDEAL_TALON);
					Moves->Push(TempMove);
				}
			}

			NewStateWithLocations->IncrementFoundation(Deck*4+CardToCheck->GetSuit());
			((FCSTalonStateWithLocations*)NewStateWithLocations)->DecrementKlondikeTalonStack();

			TempMove->SetType(FCS_MOVE_TYPE_KLONDIKE_TALON_TO_FOUNDATION);
			TempMove->SetFoundation(Deck*4+CardToCheck->GetSuit());
			Moves->Push(TempMove);


			// The last move needs to be FCS_MOVE_TYPE_CANONIZE
			// because it indicates that the internal order of the 
			// stacks
			// and freecells may have changed.
			TempMove->SetType(FCS_MOVE_TYPE_CANONIZE);
			Moves->Push(TempMove);

			if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
							  &Moves, &TempMove, Depth, &Check))
			{
				delete TempTalonData;
				delete [] Moves;
				delete TempMove;
				delete CardToCheck;
				delete TopCard;
				return Check;
			}

			break;
		}
		
		//check if the card can go on a stack
		for(int Stack=0; Stack<m_NumberOfStacks; Stack++)
		{
			TopCard->Copy(StateWithLocations->GetStackCard(Stack, StateWithLocations->GetStackLength(Stack)-1));

			if ((!IsParentCard(CardToCheck, TopCard)) &&
				((StateWithLocations->GetStackLength(Stack) > 0) || 
				 (m_EmptyStacksFill == FCS_ES_FILLED_BY_NONE) || 
				 ((m_EmptyStacksFill == FCS_ES_FILLED_BY_KINGS_ONLY) && (CardToCheck->GetCardNumber() != 13))
				)
			   )
				continue;

			// We have a card in the talon that we can move to the stack, so let's move it
			CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

			((FCSTalonStateWithLocations*)NewStateWithLocations)->GetTalonData()->Copy(TempTalonData);

			for(int a=0;a<=NumberOfRedealsDone;a++)
			{
				TempMove->SetType(FCS_MOVE_TYPE_KLONDIKE_FLIP_TALON);
				TempMove->SetNumberOfCardsFlipped(NumberOfCardsMoved[a]);
				Moves->Push(TempMove);
				if (a != NumberOfRedealsDone)
				{
					TempMove->SetType(FCS_MOVE_TYPE_KLONDIKE_REDEAL_TALON);
					Moves->Push(TempMove);
				}
			}

			NewStateWithLocations->PushCardIntoStack(Stack, ((FCSTalonStateWithLocations*)NewStateWithLocations)->GetTalonData()->GetKlondikeTalonTopCard());
			((FCSTalonStateWithLocations*)NewStateWithLocations)->DecrementKlondikeTalonStack();

			TempMove->SetType(FCS_MOVE_TYPE_KLONDIKE_TALON_TO_STACK);
			TempMove->SetDestStack(Stack);
			Moves->Push(TempMove);

			if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
							  &Moves, &TempMove, Depth, &Check))
			{
				delete TempTalonData;
				delete [] Moves;
				delete TempMove;
				delete CardToCheck;
				delete TopCard;
				return Check;
			}
		}
	}

	delete TempTalonData;
	delete [] Moves;
	delete TempMove;
	delete CardToCheck;
	delete TopCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}