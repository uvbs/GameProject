////////////////////////////////////////////////
///\file FCSMove.cpp
///\brief This file contains the implementatino of classes dealing with move objects
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "FCSMove.h"
#include "FCSDebugMove.h"
#include "FCSCompactMove.h"
#include "FCState.h"
#include "FCTalonState.h"
#include "FCHelpingAlgorithms.h"

FCSMove* FCSMove::Create()
{
	switch (GlobalStateType)
	{
	case FC_DEBUG_STATE:
	case FC_TALON_DEBUG_STATE:
		return new FCSDebugMove();
	case FC_COMPACT_STATE:
	case FC_INDIRECT_STATE:
	case FC_TALON_COMPACT_STATE:
	case FC_TALON_INDIRECT_STATE:
		return new FCSCompactMove();
	default:
		return NULL;
	}
}

FCSMove* FCSMove::CreateArray(int Size)
{
	switch (GlobalStateType)
	{
	case FC_DEBUG_STATE:
	case FC_TALON_DEBUG_STATE:
		return new FCSDebugMove[Size];
	case FC_COMPACT_STATE:
	case FC_INDIRECT_STATE:
	case FC_TALON_COMPACT_STATE:
	case FC_TALON_INDIRECT_STATE:
		return new FCSCompactMove[Size];
	default:
		return NULL;
	}
}

void FCSMove::MoveAsString(char* String)
{
	switch(GetType())
	{
		case FCS_MOVE_TYPE_STACK_TO_STACK:
			sprintf(String, "Move %i cards from stack %i to stack %i",
					GetNumberOfCardsInSequence(),
					GetSourceStack(),
					GetDestStack() );
			return;
		case FCS_MOVE_TYPE_FREECELL_TO_STACK:
			sprintf(String, "Move a card from freecell %i to stack %i",
					GetSourceFreecell(),
					GetDestStack());
			return;
		case FCS_MOVE_TYPE_FREECELL_TO_FREECELL:
			sprintf(String, "Move a card from freecell %i to freecell %i",
					GetSourceFreecell(),
					GetDestFreecell());
			return;
		case FCS_MOVE_TYPE_STACK_TO_FREECELL:
			sprintf(String, "Move a card from stack %i to freecell %i",
					GetSourceStack(),
					GetDestFreecell());
			return;
		case FCS_MOVE_TYPE_STACK_TO_FOUNDATION:
			sprintf(String, "Move a card from stack %i to the foundations", GetSourceStack());
			return;
		case FCS_MOVE_TYPE_FREECELL_TO_FOUNDATION:
			sprintf(String, "Move a card from freecell %i to the foundations", GetSourceFreecell());
			return;
		case FCS_MOVE_TYPE_SEQ_TO_FOUNDATION:
			sprintf(String, "Move the sequence on top of Stack %i to the foundations", GetSourceStack());
			return;
		case FCS_MOVE_TYPE_KLONDIKE_TALON_TO_STACK:
			sprintf(String, "Move top talon card to Stack %i", GetDestStack());
			return;
		case FCS_MOVE_TYPE_KLONDIKE_TALON_TO_FOUNDATION:
			sprintf(String, "Move top talon card to the foundations");
			return;
		case FCS_MOVE_TYPE_KLONDIKE_FLIP_TALON:
//			if (GetNumberOfCardsFlipped() > 0)
				sprintf(String, "Flip %i cards from the talon deck", GetNumberOfCardsFlipped());
			return;
		case FCS_MOVE_TYPE_KLONDIKE_REDEAL_TALON:
			sprintf(String, "Restart the talon deck");
			return;
		case FCS_MOVE_TYPE_FLIP_CARD:
			sprintf(String, "Flip top card from Stack %i", GetSourceStack());
			return;
		default:
			String = NULL;
			return;
	}

	//this shouldn't happen
	assert(false);
}

void FCSMove::MoveAsStringStandardNotation(char* String)
{
	switch(GetType())
	{
		case FCS_MOVE_TYPE_STACK_TO_STACK:
			sprintf(String, "%i%i", 1+GetSourceStack(), 1+GetDestStack());
			return;
		case FCS_MOVE_TYPE_FREECELL_TO_STACK:
			sprintf(String, "%c%i", 'a'+GetSourceFreecell(), 1+GetDestStack());
			return;
		case FCS_MOVE_TYPE_FREECELL_TO_FREECELL:
			sprintf(String, "%c%c", 'a'+GetSourceFreecell(), 'a'+GetDestFreecell());
			return;
		case FCS_MOVE_TYPE_STACK_TO_FREECELL:
			sprintf(String, "%i%c", 1+GetSourceStack(), 'a'+GetDestFreecell());
			return;
		case FCS_MOVE_TYPE_STACK_TO_FOUNDATION:
			sprintf(String, "%ih", 1+GetSourceStack());
			return;
		case FCS_MOVE_TYPE_FREECELL_TO_FOUNDATION:
			sprintf(String, "%ch", 'a'+GetSourceFreecell());
			return;
		case FCS_MOVE_TYPE_SEQ_TO_FOUNDATION:
			sprintf(String, "%ih", GetSourceStack());
			return;
		default:
			String = NULL;
			return;
	}

	//this shouldn't happen
	assert(false);
}

FCSMoveStack::FCSMoveStack()
{
	m_MaxNumberOfMoves = FCS_MOVE_STACK_GROW_BY;
	m_NumberOfMoves = 0;
}

FCSMoveStack::~FCSMoveStack()
{
}

void FCSMoveStack::Normalize(FCSStateWithLocations* InitialState, 
							 int NumberOfFreecells, int NumberOfStacks, int NumberOfDecks)
{
	FCSMoveStack* TempMoves = CreateMoveStack();
	FCSMove *InMove = FCSMove::Create(),
			*OutMove = FCSMove::Create();

	FCSStateWithLocations* DynamicState = CreateStateWithLocations();
	DynamicState->Copy(InitialState);

	while (Pop(&InMove) == 0)
	{
		ApplyMove(DynamicState, InMove, NumberOfFreecells, NumberOfStacks, NumberOfDecks);

		if (InMove->GetType() != FCS_MOVE_TYPE_CANONIZE)
		{
			OutMove->SetType(InMove->GetType());

			switch(InMove->GetType())
			{
			case FCS_MOVE_TYPE_STACK_TO_STACK:
				OutMove->SetSourceStack(DynamicState->m_StackLocations[InMove->GetSourceStack()]);
				OutMove->SetDestStack(DynamicState->m_StackLocations[InMove->GetDestStack()]);
				OutMove->SetNumberOfCardsInSequence(InMove->GetNumberOfCardsInSequence());
				break;
			case FCS_MOVE_TYPE_STACK_TO_FREECELL:
				OutMove->SetSourceStack(DynamicState->m_StackLocations[InMove->GetSourceStack()]);
				OutMove->SetDestFreecell(DynamicState->m_FreecellLocations[InMove->GetDestFreecell()]);
				break;
			case FCS_MOVE_TYPE_STACK_TO_FOUNDATION:
				OutMove->SetSourceStack(DynamicState->m_StackLocations[InMove->GetSourceStack()]);
				OutMove->SetFoundation(InMove->GetFoundation());
				break;
			case FCS_MOVE_TYPE_SEQ_TO_FOUNDATION:
				OutMove->SetSourceStack(DynamicState->m_StackLocations[InMove->GetSourceStack()]);
				OutMove->SetFoundation(InMove->GetFoundation());
				break;
			case FCS_MOVE_TYPE_FREECELL_TO_STACK:
				OutMove->SetSourceFreecell(DynamicState->m_FreecellLocations[InMove->GetSourceFreecell()]);
				OutMove->SetDestStack(DynamicState->m_StackLocations[InMove->GetDestStack()]);
				break;
			case FCS_MOVE_TYPE_FREECELL_TO_FREECELL:
				OutMove->SetSourceFreecell(DynamicState->m_FreecellLocations[InMove->GetSourceFreecell()]);
				OutMove->SetDestFreecell(DynamicState->m_FreecellLocations[InMove->GetDestFreecell()]);
				break;
			case FCS_MOVE_TYPE_FREECELL_TO_FOUNDATION:
				OutMove->SetSourceFreecell(DynamicState->m_FreecellLocations[InMove->GetSourceFreecell()]);
				OutMove->SetFoundation(InMove->GetFoundation());
				break;
			case FCS_MOVE_TYPE_KLONDIKE_TALON_TO_STACK:
				OutMove->SetDestStack(DynamicState->m_StackLocations[InMove->GetDestStack()]);
				break;
			case FCS_MOVE_TYPE_KLONDIKE_TALON_TO_FOUNDATION:
				OutMove->SetFoundation(InMove->GetFoundation());
				break;
			case FCS_MOVE_TYPE_KLONDIKE_FLIP_TALON:
				OutMove->SetNumberOfCardsFlipped(InMove->GetNumberOfCardsFlipped());
				break;
			case FCS_MOVE_TYPE_KLONDIKE_REDEAL_TALON:
				break;
			case FCS_MOVE_TYPE_FLIP_CARD:
				OutMove->SetSourceStack(DynamicState->m_StackLocations[InMove->GetSourceStack()]);
				break;
			default:
				assert(false);
			}

			TempMoves->Push(OutMove);
		}
	}

	// TempMoves contain the needed moves in reverse order. So let's use
	// SwallowStack to put them in the original in the correct order.

	Reset();

	SwallowStack(TempMoves);

	DynamicState->CleanState();

	delete DynamicState;
	delete InMove;
	delete OutMove;
}

void FCSMoveStack::Reset()
{
	m_NumberOfMoves = 0;
}

void FCSMoveStack::SwallowStack(FCSMoveStack* SrcStack)
{
	FCSMove *Move = FCSMove::Create();
	while (!SrcStack->Pop(&Move))
		Push(Move);

	delete Move;
	delete SrcStack;
}

int FCSMoveStack::GetNumberOfMoves()
{
	return m_NumberOfMoves;
}

FCSMoveStack* CreateMoveStack()
{
	switch(GlobalStateType)
	{
	case FC_DEBUG_STATE:
	case FC_TALON_DEBUG_STATE:
		return new FCSDebugMoveStack();
	case FC_COMPACT_STATE:
	case FC_INDIRECT_STATE:
	case FC_TALON_COMPACT_STATE:
	case FC_TALON_INDIRECT_STATE:
		return new FCSCompactMoveStack();
	default:
		return NULL;
	}
}

void ReallocMoveStackArray(FCSMoveStack*** Array, int OldSize, int NewSize)
{
	switch (GlobalStateType)
	{
	case FC_DEBUG_STATE:
	case FC_TALON_DEBUG_STATE:
		Realloc<FCSDebugMoveStack*>((FCSDebugMoveStack***)Array, OldSize, NewSize);
		break;
	case FC_COMPACT_STATE:
	case FC_INDIRECT_STATE:
	case FC_TALON_COMPACT_STATE:
	case FC_TALON_INDIRECT_STATE:
		Realloc<FCSCompactMoveStack*>((FCSCompactMoveStack***)Array, OldSize, NewSize);
		break;
	default:
		//this shouldn't happen
		assert(false);
	}
}

void ApplyMove(FCSStateWithLocations* StateWithLocations, FCSMove* Move, 
					int NumberOfFreecells, int NumberOfStacks, int NumberOfDecks)
{
	FCSCard* TempCard;
	int SourceStack = Move->GetSourceStack(),
		DestStack = Move->GetDestStack(),
		SourceFreecell = Move->GetSourceFreecell(), 
		DestFreecell = Move->GetDestFreecell(),
		SourceStackLength,
		a;

	switch(Move->GetType())
	{
		case FCS_MOVE_TYPE_STACK_TO_STACK:
			SourceStackLength = StateWithLocations->GetStackLength(SourceStack);

			TempCard = FCSCard::Create();
			for(a=0; a<Move->GetNumberOfCardsInSequence(); a++)
			{
				StateWithLocations->PushStackCardIntoStack(DestStack, SourceStack,
					SourceStackLength - Move->GetNumberOfCardsInSequence() + a);
			}
			
			for(a=0; a<Move->GetNumberOfCardsInSequence(); a++)
			{
				StateWithLocations->PopStackCard(SourceStack, TempCard);
			}

			delete TempCard;
			break;
		case FCS_MOVE_TYPE_FREECELL_TO_STACK:
			StateWithLocations->PushCardIntoStack(DestStack, StateWithLocations->GetFreecellCard(SourceFreecell));
			StateWithLocations->EmptyFreecell(SourceFreecell);
			break;
		case FCS_MOVE_TYPE_FREECELL_TO_FREECELL:
			TempCard = StateWithLocations->GetFreecellCard(SourceFreecell);
			StateWithLocations->PutCardInFreecell(DestFreecell, TempCard);
			StateWithLocations->EmptyFreecell(SourceFreecell);
			break;
		case FCS_MOVE_TYPE_STACK_TO_FREECELL:
			TempCard = FCSCard::Create();
			StateWithLocations->PopStackCard(SourceStack, TempCard);
			StateWithLocations->PutCardInFreecell(DestFreecell, TempCard);
			delete TempCard;
			break;
		case FCS_MOVE_TYPE_STACK_TO_FOUNDATION:
			TempCard = FCSCard::Create();
			StateWithLocations->PopStackCard(SourceStack, TempCard);
			StateWithLocations->IncrementFoundation(Move->GetFoundation());
			delete TempCard;
			break;
		case FCS_MOVE_TYPE_FREECELL_TO_FOUNDATION:
			StateWithLocations->EmptyFreecell(SourceFreecell);
			StateWithLocations->IncrementFoundation(Move->GetFoundation());
			break;
		case FCS_MOVE_TYPE_SEQ_TO_FOUNDATION:
			TempCard = FCSCard::Create();
			for(a=0;a<13;a++)
			{
				StateWithLocations->PopStackCard(SourceStack, TempCard);
				StateWithLocations->IncrementFoundation(Move->GetFoundation());
			}
			delete TempCard;
			break;
		case FCS_MOVE_TYPE_FLIP_CARD:
			StateWithLocations->FlipStackCard(SourceStack, 
						StateWithLocations->GetStackLength(SourceStack) - 1);
			break;
		case FCS_MOVE_TYPE_CANONIZE:
			StateWithLocations->CanonizeState(NumberOfFreecells, NumberOfStacks);
			break;
		case FCS_MOVE_TYPE_KLONDIKE_TALON_TO_STACK:
			StateWithLocations->PushCardIntoStack(DestStack, ((FCSTalonStateWithLocations*)StateWithLocations)->GetTalonData()->GetKlondikeTalonTopCard());
			((FCSTalonStateWithLocations*)StateWithLocations)->DecrementKlondikeTalonStack();
			break;
		case FCS_MOVE_TYPE_KLONDIKE_TALON_TO_FOUNDATION:
			StateWithLocations->IncrementFoundation(Move->GetFoundation());
			((FCSTalonStateWithLocations*)StateWithLocations)->DecrementKlondikeTalonStack();
			break;
		case FCS_MOVE_TYPE_KLONDIKE_FLIP_TALON:
			for (a=0;a<Move->GetNumberOfCardsFlipped();a++)
				((FCSTalonStateWithLocations*)StateWithLocations)->GetTalonData()->KlondikeTalonQueueToStack();
			break;
		case FCS_MOVE_TYPE_KLONDIKE_REDEAL_TALON:
			((FCSTalonStateWithLocations*)StateWithLocations)->GetTalonData()->KlondikeTalonRedealBare();
			break;
		default:
		{
			assert(false);
		}
	}
}
