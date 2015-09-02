////////////////////////////////////////////////
///\file FCSFreecellSolvingAlgorithm.cpp
///\brief This file contains the implementation of the FCSFreecellSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include "FCSFreecellSolvingAlgorithm.h"
#include "FCSAStarSolvingAlgorithm.h"
#include "FCSBFSSolvingAlgorithm.h"
#include "FCSDFSSolvingAlgorithm.h"
#include "FCSRandomDFSSolvingAlgorithm.h"
#include "FCSSoftDFSSolvingAlgorithm.h"
#include "FCSStateStorage.h"

FCSFreecellSolvingAlgorithm::FCSFreecellSolvingAlgorithm() : FCSFreecellData(), FCSFreecellAlgorithm()
{
}

FCSFreecellSolvingAlgorithm::FCSFreecellSolvingAlgorithm(FCCommandLineArguments* CommandLine) : FCSFreecellData(CommandLine), FCSFreecellAlgorithm()
{
}

FCSFreecellSolvingAlgorithm::~FCSFreecellSolvingAlgorithm()
{
}

FCSFreecellSolvingAlgorithm* FCSFreecellSolvingAlgorithm::Create(FCCommandLineArguments* CommandLine)
{

	if (CommandLine == NULL)
		return NULL;

	//determine with solving algorithm to create
	switch(CommandLine->GetSolvingMethodType())
	{
	case FCS_METHOD_HARD_DFS:
		return new FCSDFSSolvingAlgorithm<FCSFreecellSolvingAlgorithm>(CommandLine);
		break;
	case FCS_METHOD_SOFT_DFS:
		return new FCSSoftDFSSolvingAlgorithm<FCSFreecellSolvingAlgorithm>(CommandLine);
		break;
	case FCS_METHOD_BFS:
		return new FCSBFSSolvingAlgorithm<FCSFreecellSolvingAlgorithm>(CommandLine);
		break;
	case FCS_METHOD_A_STAR:
		return new FCSAStarSolvingAlgorithm<FCSFreecellSolvingAlgorithm>(CommandLine);
		break;
	case FCS_METHOD_RANDOM_DFS:
		return new FCSRandomDFSSolvingAlgorithm<FCSFreecellSolvingAlgorithm>(CommandLine);
		break;
	default:
		return NULL;
	}

	return NULL;
}

void FCSFreecellSolvingAlgorithm::FlipTopCard(FCSStateWithLocations *State, int Stack,
									  FCSMove* Move, FCSMoveStack** MoveStack)
{
	if (State->GetStackLength(Stack) != 0)
	{
		if (State->GetStackCard(Stack, State->GetStackLength(Stack) - 1)->GetFlipped() == 1)
		{
			State->GetStackCard(Stack, State->GetStackLength(Stack) - 1)->SetFlipped(0);
			Move->SetType(FCS_MOVE_TYPE_FLIP_CARD);
			Move->SetSourceStack(Stack);
			(*MoveStack)->Push(Move);
		}
	}
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::RunTest(int TestNumber, FCSStateWithLocations*  StateWithLocations,
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
		return MoveFreecellCardsToFounds(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 2:
		return MoveFreecellCardsToTopOfStacks(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 3:
		return MoveNonTopStackCardsToFounds(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 4:
		return MoveStackCardsToDifferentStacks(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 5:
		return MoveStackCardsToAParentOnTheSameStack(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 6:
		return MoveSequencesToFreeStacks(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 7:
		return MoveFreecellCardsToEmptyStack(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 8:
		return MoveCardsToADifferentParent(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 9:
		return EmptyStackIntoFreecells(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	default:
		return FCS_STATE_IS_NOT_SOLVEABLE;
	}
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveTopStackCardsToFounds(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations *NewStateWithLocations;

	FCSCard *Card, 
		*TempCard = FCSCard::Create();
	FCSStateSolvingReturnCodes Check;

	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	for(int Stack=0;Stack<m_NumberOfStacks;Stack++)
	{
		if (StateWithLocations->GetStackLength(Stack) <= 0)
			continue;

		// Get the top card in the stack
		Card = StateWithLocations->GetStackCard(Stack, StateWithLocations->GetStackLength(Stack)-1);
		for(int Deck=0;Deck < m_NumberOfDecks;Deck++)
		{
			if (StateWithLocations->GetFoundation(Deck*4 + Card->GetSuit()) != Card->GetCardNumber() - 1)
				continue;

			// We can put it there
			CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

			NewStateWithLocations->PopStackCard(Stack, TempCard);
			NewStateWithLocations->IncrementFoundation(Deck*4+Card->GetSuit());

			TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FOUNDATION);
			TempMove->SetSourceStack(Stack);
			TempMove->SetFoundation(Deck*4+Card->GetSuit());

			Moves->Push(TempMove);

			FlipTopCard(NewStateWithLocations, Stack, TempMove, &Moves);

			// The last move needs to be FCS_MOVE_TYPE_CANONIZE
			// because it indicates that the internal order of the 
			// stacks
			// and freecells may have changed.
			TempMove->SetType(FCS_MOVE_TYPE_CANONIZE);
			Moves->Push(TempMove);

			if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
							  &Moves, &TempMove, Depth, &Check))
			{
				delete TempMove;
				delete TempCard;
				return Check;
			}

			break;
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveFreecellCardsToFounds(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations *NewStateWithLocations;
	FCSCard *Card;
	FCSStateSolvingReturnCodes Check;

	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	// Now check the same for the free cells
	for(int Freecell=0;Freecell<m_NumberOfFreecells;Freecell++)
	{
		Card = StateWithLocations->GetFreecellCard(Freecell);
		if (Card->GetCardNumber() <= 0)
			continue;

		for(int Deck=0;Deck<m_NumberOfDecks;Deck++)
		{
			if (StateWithLocations->GetFoundation(Deck*4+Card->GetSuit()) != Card->GetCardNumber() - 1)
				continue;

			// We can put it there
			CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

			NewStateWithLocations->EmptyFreecell(Freecell);
			NewStateWithLocations->IncrementFoundation(Deck*4+Card->GetSuit());

			TempMove->SetType(FCS_MOVE_TYPE_FREECELL_TO_FOUNDATION);
			TempMove->SetSourceFreecell(Freecell);
			TempMove->SetFoundation(Deck*4+Card->GetSuit());

			Moves->Push(TempMove);

			TempMove->SetType(FCS_MOVE_TYPE_CANONIZE);
			Moves->Push(TempMove);

			if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
							  &Moves, &TempMove, Depth, &Check))
			{
				delete TempMove;
				return Check;
			}
		}
	}

	delete TempMove;
	delete [] Moves;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveFreecellCardsToTopOfStacks(FCSStateWithLocations*  StateWithLocations,
								  int Depth,
								  int NumberOfFreeStacks,
								  int NumberOfFreecells,
								  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations *NewStateWithLocations;
	int DestinationCardNumber, NumberOfCardsToRelocate, 
		FreecellsToFill, FreeStacksToFill, a, b;
	FCSCard *DestinationCard, *SourceCard, *DestBelowCard, 
			*TempCard = FCSCard::Create();
	FCSStateSolvingReturnCodes Check;
	bool IsSequenceInDestination;

	FCSMove *TempMove = FCSMove::Create();
	FCSMoveStack *Moves = CreateMoveStack();

	// Let's try to put cards in the freecells on top of stacks
	for(int DestinationStack=0;DestinationStack<m_NumberOfStacks;DestinationStack++)
	{
		DestinationCardNumber = StateWithLocations->GetStackLength(DestinationStack);

		// If the stack is not empty we can proceed
		if (DestinationCardNumber <= 0)
			continue;

		// Let's search for a suitable card in the stack
		for(int DestCard=DestinationCardNumber-1;DestCard>=0;DestCard--)
		{
			DestinationCard = StateWithLocations->GetStackCard(DestinationStack, DestCard);

			// Scan the freecells
			for(int Freecell=0;Freecell<m_NumberOfFreecells;Freecell++)
			{
				SourceCard = StateWithLocations->GetFreecellCard(Freecell);

				// If the freecell is not empty and dest_card is its parent
				if ( (SourceCard->GetCardNumber() <= 0) || (!IsParentCard(SourceCard,DestinationCard)))
					continue;

				// Let's check if we can put it there
				// Check if the destination card is already below a suitable card
				IsSequenceInDestination = false;
				if (DestinationCardNumber - 1 > DestCard)
				{
					DestBelowCard = StateWithLocations->GetStackCard(DestinationStack, DestCard+1);
					if (IsParentCard(DestBelowCard, DestinationCard))
					{
						IsSequenceInDestination = true;
					}
				}


				if (IsSequenceInDestination)
					continue;

				NumberOfCardsToRelocate = DestinationCardNumber - DestCard - 1;
				FreecellsToFill = min(NumberOfCardsToRelocate, NumberOfFreecells);
				NumberOfCardsToRelocate -= FreecellsToFill;

				if (m_EmptyStacksFill == FCS_ES_FILLED_BY_ANY_CARD)
				{
					FreeStacksToFill = min(NumberOfCardsToRelocate, NumberOfFreeStacks);
					NumberOfCardsToRelocate -= FreeStacksToFill;
				}
				else
				{
					FreeStacksToFill = 0;
				}

				if (NumberOfCardsToRelocate != 0)
					continue;

				//	We can move it
				CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

				// Fill the freecells with the top cards
				for(a=0 ; a<FreecellsToFill ; a++)
				{
					// Find a vacant freecell
					for(b=0;b<m_NumberOfFreecells;b++)
					{
						if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
							break;
					}

					NewStateWithLocations->PopStackCard(DestinationStack, TempCard);

					NewStateWithLocations->PutCardInFreecell(b, TempCard);

					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
					TempMove->SetSourceStack(DestinationStack);
					TempMove->SetDestFreecell(b);
					Moves->Push(TempMove);
				}

				// Fill the free stacks with the cards below them
				for(a=0; a <FreeStacksToFill; a++)
				{
					// Find a vacant stack
					for(b=0;b<m_NumberOfStacks;b++)
					{
						if (NewStateWithLocations->GetStackLength(b) == 0)
							break;
					}

					NewStateWithLocations->PopStackCard(DestinationStack, TempCard);
					NewStateWithLocations->PushCardIntoStack(b, TempCard);
					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
					TempMove->SetSourceStack(DestinationStack);
					TempMove->SetDestStack(b);
					TempMove->SetNumberOfCardsInSequence(1);
					Moves->Push(TempMove);
				}

				// Now put the freecell card on top of the stack
				NewStateWithLocations->PushCardIntoStack(DestinationStack, SourceCard);
				NewStateWithLocations->EmptyFreecell(Freecell);
				TempMove->SetType(FCS_MOVE_TYPE_FREECELL_TO_STACK);
				TempMove->SetSourceFreecell(Freecell);
				TempMove->SetDestStack(DestinationStack);
				Moves->Push(TempMove);

				if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
					  &Moves, &TempMove, Depth, &Check))
				{
					delete TempMove;
					delete TempCard;
					return Check;
				}
			}
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveNonTopStackCardsToFounds(FCSStateWithLocations*  StateWithLocations,
								  int Depth,
								  int NumberOfFreeStacks,
								  int NumberOfFreecells,
								  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations *NewStateWithLocations;
	FCSCard *Card, 
		*TempCard = FCSCard::Create();
	FCSStateSolvingReturnCodes Check;

	int NumberOfCards, a, b;

	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	// Now let's check if a card that is under some other cards can be placed 
	// in the foundations.

	for(int Stack=0;Stack<m_NumberOfStacks;Stack++)
	{
		NumberOfCards = StateWithLocations->GetStackLength(Stack);

		 // We starts from cards_num-2 because the top card is already covered
		 // by move_top_stack_cards_to_founds.
		for(int c=NumberOfCards-2;c>=0;c--)
		{
			Card = StateWithLocations->GetStackCard(Stack, c);
			for(int Deck=0;Deck<m_NumberOfDecks;Deck++)
			{
				if (StateWithLocations->GetFoundation(Deck*4+Card->GetSuit()) != Card->GetCardNumber() -1)
					continue;

				// The card is foundation-able. Now let's check if we 
				// can move the cards above it to the freecells and stacks

				if ((NumberOfFreecells + 
					((m_EmptyStacksFill == FCS_ES_FILLED_BY_ANY_CARD) ? 
						NumberOfFreeStacks : 
						0
					)) 
						>= NumberOfCards-(c+1))
				{
					// We can move it
					CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

					// Fill the freecells with the top cards
					for(a=0 ; a<min(NumberOfFreecells, NumberOfCards-(c+1)) ; a++)
					{
						// Find a vacant freecell
						for(b=0; b<m_NumberOfFreecells; b++)
						{
							if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
								break;
						}

						NewStateWithLocations->PopStackCard(Stack, TempCard);
						NewStateWithLocations->PutCardInFreecell(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
						TempMove->SetSourceStack(Stack);
						TempMove->SetDestFreecell(b);
	
						Moves->Push(TempMove);

						FlipTopCard(NewStateWithLocations, Stack, TempMove, &Moves);
					}

					// Fill the free stacks with the cards below them
					for(a=0; a < NumberOfCards-(c+1) - min(NumberOfFreecells, NumberOfCards-(c+1)) ; a++)
					{
						/* Find a vacant stack */
						for(b=0;b<m_NumberOfStacks;b++)
						{
							if (NewStateWithLocations->GetStackLength(b) == 0)
								break;
						}

						NewStateWithLocations->PopStackCard(Stack, TempCard);
						NewStateWithLocations->PushCardIntoStack(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
						TempMove->SetSourceStack(Stack);
						TempMove->SetDestStack(b);
						TempMove->SetNumberOfCardsInSequence(1);

						Moves->Push(TempMove);

						FlipTopCard(NewStateWithLocations, Stack, TempMove, &Moves);
					}

					NewStateWithLocations->PopStackCard(Stack, TempCard);
					NewStateWithLocations->IncrementFoundation(Deck*4+TempCard->GetSuit());

					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FOUNDATION);
					TempMove->SetSourceStack(Stack);
					TempMove->SetFoundation(Deck*4+TempCard->GetSuit());

					Moves->Push(TempMove);

					FlipTopCard(NewStateWithLocations, Stack, TempMove, &Moves);

					if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
								  &Moves, &TempMove, Depth, &Check))
					{
						delete TempCard;
						delete TempMove;
						return Check;
					}
				}
				break;
			}
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveStackCardsToDifferentStacks(FCSStateWithLocations*  StateWithLocations,
								  int Depth,
								  int NumberOfFreeStacks,
								  int NumberOfFreecells,
								  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations *NewStateWithLocations;
	FCSCard *Card, 
			*TempCard = FCSCard::Create(), 
			*DestCard, *DestBelowCard;
	FCSStateSolvingReturnCodes Check;

	int NumberOfCards, SequenceEnd, 
		NumberOfDestinationCards, NumberOfCardsToRelocate,
		FreecellsToFill, FreeStacksToFill, a, b;

	bool IsSequenceInDestination;

	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	// Now let's try to move a card from one stack to the other
	// Note that it does not involve moving cards lower than king
	// to empty stacks
	for (int Stack=0;Stack<m_NumberOfStacks;Stack++)
	{
		NumberOfCards = StateWithLocations->GetStackLength(Stack);

		for (int c=0 ; c<NumberOfCards; c=SequenceEnd+1)
		{   
			// Check if there is a sequence here.
			for(SequenceEnd=c ; SequenceEnd<NumberOfCards-1 ; SequenceEnd++)
			{
				if (!IsParentCard(StateWithLocations->GetStackCard(Stack, SequenceEnd+1), 
								  StateWithLocations->GetStackCard(Stack, SequenceEnd)))
					break;
			}

			// Find a card which this card can be put on
			Card = StateWithLocations->GetStackCard(Stack, c);

			// Make sure the card is not flipped or else we can't move it
			if (Card->GetFlipped() != 0)
				continue;

			for(int DestinationStack=0 ; DestinationStack<m_NumberOfStacks; DestinationStack++)
			{
				if (DestinationStack == Stack)
					continue;

				NumberOfDestinationCards = StateWithLocations->GetStackLength(DestinationStack);
				for(int DestinationCard=0;DestinationCard<NumberOfDestinationCards;DestinationCard++)
				{
					DestCard = StateWithLocations->GetStackCard(DestinationStack, DestinationCard);

					if (!IsParentCard(Card, DestCard))
						continue;

					// Corresponding cards - see if it is feasible to move
					//  the source to the destination.
					IsSequenceInDestination = false;
					if (NumberOfDestinationCards - 1 > DestinationCard)
					{
						DestBelowCard = StateWithLocations->GetStackCard(DestinationStack, DestinationCard+1);
						if (IsParentCard(DestBelowCard, DestCard))
							IsSequenceInDestination = true;
					}

					if (IsSequenceInDestination)
						continue;

					NumberOfCardsToRelocate = NumberOfDestinationCards - DestinationCard - 1 + NumberOfCards - SequenceEnd - 1;
					FreecellsToFill = min(NumberOfCardsToRelocate, NumberOfFreecells);
					NumberOfCardsToRelocate -= FreecellsToFill;

					if (m_EmptyStacksFill == FCS_ES_FILLED_BY_ANY_CARD)
					{
						FreeStacksToFill = min(NumberOfCardsToRelocate, NumberOfFreeStacks);

						NumberOfCardsToRelocate -= FreeStacksToFill;
					}
					else
					{
						FreeStacksToFill = 0;
					}

					if ((NumberOfCardsToRelocate != 0) || 
						(CalculateMaxSequenceMoves(NumberOfFreecells-FreecellsToFill, NumberOfFreeStacks-FreeStacksToFill) < 
							SequenceEnd - c + 1))
						continue;

					// We can move it
					int FromWhichStack;

					CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

					// Fill the freecells with the top cards
					for(a=0 ; a<FreecellsToFill ; a++)
					{
						// Find a vacant freecell
						for(b=0;b<m_NumberOfFreecells;b++)
						{
							if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
								break;
						}

						if (NewStateWithLocations->GetStackLength(DestinationStack) == DestinationCard+1)
						{
							FromWhichStack = Stack;
						}
						else
						{
							FromWhichStack = DestinationStack;
						}
						NewStateWithLocations->PopStackCard(FromWhichStack, TempCard);
						NewStateWithLocations->PutCardInFreecell(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
						TempMove->SetSourceStack(FromWhichStack);
						TempMove->SetDestFreecell(b);

						Moves->Push(TempMove);

						FlipTopCard(NewStateWithLocations, FromWhichStack, TempMove, &Moves);
					}

					// Fill the free stacks with the cards below them
					for(a=0; a < FreeStacksToFill ; a++)
					{
						//  Find a vacant stack
						for(b=0;b<m_NumberOfStacks;b++)
						{
							if (NewStateWithLocations->GetStackLength(b) == 0)
								break;
						}

						if (NewStateWithLocations->GetStackLength(DestinationStack) == DestinationCard+1)
						{
							FromWhichStack = Stack;
						}
						else
						{
							FromWhichStack = DestinationStack;
						}

						NewStateWithLocations->PopStackCard(FromWhichStack, TempCard);
						NewStateWithLocations->PushCardIntoStack(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
						TempMove->SetSourceStack(FromWhichStack);
						TempMove->SetDestStack(b);
						TempMove->SetNumberOfCardsInSequence(1);

						Moves->Push(TempMove);

						FlipTopCard(NewStateWithLocations, FromWhichStack, TempMove, &Moves);
					}

					for(a=c ; a <= SequenceEnd ; a++)
						NewStateWithLocations->PushStackCardIntoStack(DestinationStack, Stack, a);

					for(a=0; a < SequenceEnd-c+1 ;a++)
						NewStateWithLocations->PopStackCard(Stack, TempCard);

					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
					TempMove->SetSourceStack(Stack);
					TempMove->SetDestStack(DestinationStack);
					TempMove->SetNumberOfCardsInSequence(SequenceEnd-c+1);

					Moves->Push(TempMove);

					FlipTopCard(NewStateWithLocations, Stack, TempMove, &Moves);

					if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
									  &Moves, &TempMove, Depth, &Check))
					{
						delete TempCard;
						delete TempMove;
						return Check;
					}
				}
			}
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveStackCardsToAParentOnTheSameStack(FCSStateWithLocations*  StateWithLocations,
										  int Depth,
										  int NumberOfFreeStacks,
										  int NumberOfFreecells,
										  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations *NewStateWithLocations;
	FCSCard *Card, 
			*TempCard = FCSCard::Create(), 
			*PreviousCard, *DestCard, *DestBelowCard,
			*MovedCard = FCSCard::Create();

	FCSStateSolvingReturnCodes Check;

	int NumberOfCards, NumberOfDestinationCards, NumberOfCardsToRelocate,
		FreecellsToFill, FreeStacksToFill, a, b;

	bool IsSequenceInDestination;

	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	// Now let's try to move a stack card to a parent card which is found
	// on the same stack.
	for (int Stack=0;Stack<m_NumberOfStacks;Stack++)
	{
		NumberOfCards = StateWithLocations->GetStackLength(Stack);

		for (int c=0; c<NumberOfCards; c++)
		{   
			// Find a card which this card can be put on.
			Card = StateWithLocations->GetStackCard(Stack, c);

			// Do not move cards that are already found above a suitable parent
			a = 1;
			if (c != 0)
			{
				PreviousCard = StateWithLocations->GetStackCard(Stack, c-1);
				if ((PreviousCard->GetCardNumber() == Card->GetCardNumber()+1) &&
					((PreviousCard->GetSuit() & 0x1) != (Card->GetSuit() & 0x1)))
				{
				   a = 0;  
				}
			}

			if (!a)
				continue;

			// Check if it can be moved to something on the same stack
			NumberOfDestinationCards = StateWithLocations->GetStackLength(Stack);

			for(int DestinationCard=0;DestinationCard<c-1;DestinationCard++)
			{
				DestCard = StateWithLocations->GetStackCard(Stack, DestinationCard);
				if (!IsParentCard(Card, DestCard))
					continue;
				// Corresponding cards - see if it is feasible to move
				//  the source to the destination.
				IsSequenceInDestination = false;
				DestBelowCard = StateWithLocations->GetStackCard(Stack, DestinationCard+1);
				if (IsParentCard(DestBelowCard, DestCard))
					IsSequenceInDestination = true;

				if (IsSequenceInDestination)
					continue;

				NumberOfCardsToRelocate = NumberOfDestinationCards - DestinationCard - 1;
				FreecellsToFill = min(NumberOfCardsToRelocate, NumberOfFreecells);
				NumberOfCardsToRelocate -= FreecellsToFill;

				if (m_EmptyStacksFill == FCS_ES_FILLED_BY_ANY_CARD)
				{
					FreeStacksToFill = min(NumberOfCardsToRelocate, NumberOfFreeStacks);
					NumberOfCardsToRelocate -= FreeStacksToFill;
				}
				else
				{
					FreeStacksToFill = 0;
				}

				if (NumberOfCardsToRelocate != 0)
					continue;

				// We can move it
				CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

				int SourceType, SourceIndex,
					iCardPosition = NewStateWithLocations->GetStackLength(Stack)-1;

				a = 0;
				while(iCardPosition>c)
				{
					if (a < FreecellsToFill)
					{
						for(b=0;b<m_NumberOfFreecells;b++)
						{
							if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
								break;
						}

						NewStateWithLocations->PopStackCard(Stack, TempCard);
						NewStateWithLocations->PutCardInFreecell(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
						TempMove->SetSourceStack(Stack);
						TempMove->SetDestFreecell(b);

						Moves->Push(TempMove);
					}
					else
					{

						//  Find a vacant stack
						for(b=0;b<m_NumberOfStacks;b++)
						{
							if (NewStateWithLocations->GetStackLength(b) == 0)
								break;
						}

						NewStateWithLocations->PopStackCard(Stack, TempCard);
						NewStateWithLocations->PushCardIntoStack(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
						TempMove->SetSourceStack(Stack);
						TempMove->SetDestStack(b);
						TempMove->SetNumberOfCardsInSequence(1);

						Moves->Push(TempMove);
					}
					a++;

					iCardPosition--;
				}

				NewStateWithLocations->PopStackCard(Stack, MovedCard);
				if (a < FreecellsToFill)
				{
					for(b=0;b<m_NumberOfFreecells;b++)
					{
						if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
							break;
					}

					NewStateWithLocations->PutCardInFreecell(b, MovedCard);

					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
					TempMove->SetSourceStack(Stack);
					TempMove->SetDestFreecell(b);

					Moves->Push(TempMove);

					SourceType = 0;
					SourceIndex = b;
				}
				else
				{
					for(b=0;b<m_NumberOfStacks;b++)
					{
						if (NewStateWithLocations->GetStackLength(b) == 0)
							break;
					}

					NewStateWithLocations->PushCardIntoStack(b, MovedCard);

					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
					TempMove->SetSourceStack(Stack);
					TempMove->SetDestStack(b);
					TempMove->SetNumberOfCardsInSequence(1);

					Moves->Push(TempMove);

					SourceType = 1;
					SourceIndex = b;
				}
				iCardPosition--;
				a++;

				while(iCardPosition>DestinationCard)
				{
					if (a < FreecellsToFill)
					{
						for(b=0;b<m_NumberOfFreecells;b++)
						{
							if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
								break;
						}

						NewStateWithLocations->PopStackCard(Stack, TempCard);
						NewStateWithLocations->PutCardInFreecell(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
						TempMove->SetSourceStack(Stack);
						TempMove->SetDestFreecell(b);

						Moves->Push(TempMove);
					}
					else
					{

						//  Find a vacant stack
						for(b=0;b<m_NumberOfStacks;b++)
						{
							if (NewStateWithLocations->GetStackLength(b) == 0)
								break;
						}

						NewStateWithLocations->PopStackCard(Stack, TempCard);
						NewStateWithLocations->PushCardIntoStack(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
						TempMove->SetSourceStack(Stack);
						TempMove->SetDestStack(b);
						TempMove->SetNumberOfCardsInSequence(1);

						Moves->Push(TempMove);
					}
					a++;

					iCardPosition--;
				}

				if (SourceType == 0)
				{
					FCSCard* TempMoveCard = NewStateWithLocations->GetFreecellCard(SourceIndex);
					MovedCard->Copy(TempMoveCard);
					NewStateWithLocations->EmptyFreecell(SourceIndex);

					TempMove->SetType(FCS_MOVE_TYPE_FREECELL_TO_STACK);
					TempMove->SetSourceFreecell(SourceIndex);
					TempMove->SetDestStack(Stack);
					
					Moves->Push(TempMove);
				}
				else
				{
					NewStateWithLocations->PopStackCard(SourceIndex, MovedCard);
					
					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
					TempMove->SetSourceStack(SourceIndex);
					TempMove->SetDestStack(Stack);
					TempMove->SetNumberOfCardsInSequence(1);

					Moves->Push(TempMove);
				}

				NewStateWithLocations->PushCardIntoStack(Stack, MovedCard);

				if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
								  &Moves, &TempMove, Depth, &Check))
				{
					delete TempCard;
					delete MovedCard;
					delete TempMove;
					return Check;
				}
			}
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;
	delete MovedCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveSequencesToFreeStacks(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList)
{
	if ((m_EmptyStacksFill == FCS_ES_FILLED_BY_NONE) || (NumberOfFreeStacks <= 0))
		return FCS_STATE_IS_NOT_SOLVEABLE;

	FCSStateWithLocations *NewStateWithLocations;
	FCSCard *ThisCard, 
			*TempCard = FCSCard::Create(), 
			*PreviousCard;
	FCSStateSolvingReturnCodes Check;

	int NumberOfCards, SequenceEnd,
		MaxSequenceLength = CalculateMaxSequenceMoves(NumberOfFreecells, NumberOfFreeStacks-1), 
		NumberOfCardsToRelocate, FreecellsToFill, FreeStacksToFill, 
		DestinationStack, a, b;

	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	// Now try to move sequences to empty stacks
	for(int Stack=0;Stack<m_NumberOfStacks;Stack++)
	{
		NumberOfCards = StateWithLocations->GetStackLength(Stack);

		for(int c=0; c<NumberOfCards; c=SequenceEnd+1)
		{
			// Check if there is a sequence here.
			for(SequenceEnd=c ; SequenceEnd<NumberOfCards-1; SequenceEnd++)
			{
				ThisCard = StateWithLocations->GetStackCard(Stack, SequenceEnd+1);
				PreviousCard = StateWithLocations->GetStackCard(Stack, SequenceEnd);

				if (!IsParentCard(ThisCard, PreviousCard))
					break;
			}

			if ((StateWithLocations->GetStackCardNumber(Stack, c) != 13) &&
				(m_EmptyStacksFill == FCS_ES_FILLED_BY_KINGS_ONLY))
					continue;

			if (SequenceEnd == NumberOfCards-1)
			{
				// One stack is the destination stack, so we have one
				// less stack in that case
				while ((MaxSequenceLength < NumberOfCards - c) && (c > 0))
					c--;

				if ((c <= 0) || ((m_EmptyStacksFill == FCS_ES_FILLED_BY_KINGS_ONLY) && 
					(StateWithLocations->GetStackCard(Stack, c)->GetCardNumber() != 13)))
					continue;

				CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

				for(DestinationStack=0;DestinationStack<m_NumberOfStacks;DestinationStack++)
				{
					if (StateWithLocations->GetStackLength(DestinationStack) == 0)
						break;
				}

				for(a=c; a <= NumberOfCards-1; a++)
					NewStateWithLocations->PushStackCardIntoStack(DestinationStack, Stack, a);

				for(a=0;a<NumberOfCards-c;a++)
					NewStateWithLocations->PopStackCard(Stack, TempCard);

				TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
				TempMove->SetSourceStack(Stack);
				TempMove->SetDestStack(DestinationStack);
				TempMove->SetNumberOfCardsInSequence(NumberOfCards-c);

				Moves->Push(TempMove);

				FlipTopCard(NewStateWithLocations, Stack, TempMove, &Moves);

				if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
										  &Moves, &TempMove, Depth, &Check))
					return Check;
			}
			else
			{
				NumberOfCardsToRelocate = NumberOfCards - SequenceEnd - 1;
				FreecellsToFill = min(NumberOfCardsToRelocate, NumberOfFreecells);
				NumberOfCardsToRelocate -= FreecellsToFill;

				if (m_EmptyStacksFill == FCS_ES_FILLED_BY_ANY_CARD)
				{
					FreeStacksToFill = min(NumberOfCardsToRelocate, NumberOfFreeStacks);
					NumberOfCardsToRelocate -= FreeStacksToFill;
				}
				else
				{
					FreeStacksToFill = 0;
				}

				if ((NumberOfCardsToRelocate != 0) || (NumberOfFreeStacks-FreeStacksToFill <= 0))
					continue;

				// We can move it
				int SequenceStart = c;
				while (
					(CalculateMaxSequenceMoves(
						NumberOfFreecells-FreecellsToFill,
						NumberOfFreeStacks-FreeStacksToFill-1) < SequenceEnd-SequenceStart+1)
						&&
					(SequenceStart <= SequenceEnd)
					)
				{
					SequenceStart++;
				}

				if ((SequenceStart > SequenceEnd) || ((m_EmptyStacksFill == FCS_ES_FILLED_BY_KINGS_ONLY) && 
					(StateWithLocations->GetStackCard(Stack, SequenceStart)->GetCardNumber() != 13)))
					continue;
				
				CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

				// Fill the freecells with the top cards
				for(a=0; a<FreecellsToFill ; a++)
				{
					// Find a vacant freecell
					for(b=0;b<m_NumberOfFreecells;b++)
						if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
							break;

					NewStateWithLocations->PopStackCard(Stack, TempCard);
					NewStateWithLocations->PutCardInFreecell(b, TempCard);

					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
					TempMove->SetSourceStack(Stack);
					TempMove->SetDestFreecell(b);
					Moves->Push(TempMove);
				}

				// Fill the free stacks with the cards below them
				for(a=0; a < FreeStacksToFill ; a++)
				{
					// Find a vacant stack
					for(b=0; b<m_NumberOfStacks; b++)
						if (NewStateWithLocations->GetStackLength(b) == 0)
							break;

					NewStateWithLocations->PopStackCard(Stack, TempCard);
					NewStateWithLocations->PushCardIntoStack(b, TempCard);

					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
					TempMove->SetSourceStack(Stack);
					TempMove->SetDestStack(b);
					TempMove->SetNumberOfCardsInSequence(1);
					Moves->Push(TempMove);
				}

				// Find a vacant stack
				for(b=0; b<m_NumberOfStacks; b++)
				{
					if (NewStateWithLocations->GetStackLength(b) == 0)
						break;
				}

				for(a=SequenceStart; a <= SequenceEnd; a++)
					NewStateWithLocations->PushStackCardIntoStack(b, Stack, a);

				for(a=SequenceStart; a <= SequenceEnd; a++)
					NewStateWithLocations->PopStackCard(Stack, TempCard);

				TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
				TempMove->SetSourceStack(Stack);
				TempMove->SetDestStack(b);
				TempMove->SetNumberOfCardsInSequence(SequenceEnd-SequenceStart+1);

				Moves->Push(TempMove);

				FlipTopCard(NewStateWithLocations, Stack, TempMove, &Moves);

				if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
									  &Moves, &TempMove, Depth, &Check))
				{
					delete TempCard;
					delete TempMove;
					return Check;
				}
			}
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveFreecellCardsToEmptyStack(FCSStateWithLocations*  StateWithLocations,
								  int Depth,
								  int NumberOfFreeStacks,
								  int NumberOfFreecells,
								  FCSDerivedStatesList* DerivedStateList)
{
	// Let's try to put cards that occupy freecells on an empty stack
	if (m_EmptyStacksFill == FCS_ES_FILLED_BY_NONE)
		return FCS_STATE_IS_NOT_SOLVEABLE;

	FCSStateWithLocations *NewStateWithLocations;
	FCSCard *Card;
	FCSStateSolvingReturnCodes Check;
	int Stack;
	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	for(int Freecell=0;Freecell<m_NumberOfFreecells;Freecell++)
	{
		Card = StateWithLocations->GetFreecellCard(Freecell);

		if ((m_EmptyStacksFill == FCS_ES_FILLED_BY_KINGS_ONLY) ? 
			(Card->GetCardNumber() != 13) :
			(Card->GetCardNumber() == 0))
				continue;

		for(Stack=0;Stack<m_NumberOfStacks;Stack++)
			if (StateWithLocations->GetStackLength(Stack) == 0)
				break;

		if (Stack == m_NumberOfStacks)
			continue;

		// We can move it
		CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

		NewStateWithLocations->PushCardIntoStack(Stack, Card);
		NewStateWithLocations->EmptyFreecell(Freecell);

		TempMove->SetType(FCS_MOVE_TYPE_FREECELL_TO_STACK);
		TempMove->SetSourceFreecell(Freecell);
		TempMove->SetDestStack(Stack);

		Moves->Push(TempMove);

		if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
						  &Moves, &TempMove, Depth, &Check))
		{
			delete TempMove;
			return Check;
		}
	}

	delete [] Moves;
	delete TempMove;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::MoveCardsToADifferentParent(FCSStateWithLocations*  StateWithLocations,
							  int Depth,
							  int NumberOfFreeStacks,
							  int NumberOfFreecells,
							  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations *NewStateWithLocations;
	FCSCard *Card, 
			*TempCard = FCSCard::Create(), 
			*ThisCard, *PreviousCard,
			*DestCard, *DestBelowCard;
	FCSStateSolvingReturnCodes Check;


	int NumberOfCards, NumberOfCardsToRelocate, NumberOfDestinationCards,
		FreecellsToFill, FreeStacksToFill,
		a, b;
	bool IsSequenceInSource, IsSequenceInDestination;

	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	// This time try to move cards that are already on top of a parent to a different parent
	for (int Stack=0;Stack<m_NumberOfStacks;Stack++)
	{
		NumberOfCards = StateWithLocations->GetStackLength(Stack);

		for (int c=0 ; c<NumberOfCards ; c++)
		{   
			// Check if there is a sequence here.
			IsSequenceInSource = true;
			for(a=c+1 ; a<NumberOfCards ; a++)
			{
				ThisCard = StateWithLocations->GetStackCard(Stack, a);
				PreviousCard = StateWithLocations->GetStackCard(Stack, a-1);

				if (!IsParentCard(ThisCard, PreviousCard))
				{
					IsSequenceInSource = false;
					break;
				}
			}

			// Find a card which this card can be put on;
			Card = StateWithLocations->GetStackCard(Stack, c);

			// Do not move cards that are already found above a suitable parent
			a = 1;
			if (c != 0)
			{
				PreviousCard = StateWithLocations->GetStackCard(Stack, c-1);
				if (IsParentCard(Card, PreviousCard))
				   a = 0;  
			}

			// And do not move cards that are flipped
			if (!a)
			{
				ThisCard = StateWithLocations->GetStackCard(Stack, c);
				if (ThisCard->GetFlipped())
					a = 0;
			}

			if (a)
				continue;

			for(int DestinationStack=0 ; DestinationStack<m_NumberOfStacks; DestinationStack++)
			{
				if (DestinationStack == Stack)
					continue;

				NumberOfDestinationCards = StateWithLocations->GetStackLength(DestinationStack);
				for(int DestinationCard=0;DestinationCard<NumberOfDestinationCards;DestinationCard++)
				{
					DestCard = StateWithLocations->GetStackCard(DestinationStack, DestinationCard);

					if (!IsParentCard(Card, DestCard))
						continue;

					// Corresponding cards - see if it is feasible to move
					// the source to the destination.
					IsSequenceInDestination = false;
					if (NumberOfDestinationCards - 1 > DestinationCard)
					{
						DestBelowCard = StateWithLocations->GetStackCard(DestinationStack, DestinationCard+1);
						if (IsParentCard(DestBelowCard, DestCard))
							IsSequenceInDestination = true;
					}

					if ((IsSequenceInDestination) || (!IsSequenceInSource))
						continue;

					NumberOfCardsToRelocate = NumberOfDestinationCards - DestinationCard - 1;
					FreecellsToFill = min(NumberOfCardsToRelocate, NumberOfFreecells);
					NumberOfCardsToRelocate -= FreecellsToFill;

					if (m_EmptyStacksFill == FCS_ES_FILLED_BY_ANY_CARD)
					{
						FreeStacksToFill = min(NumberOfCardsToRelocate, NumberOfFreeStacks);
						NumberOfCardsToRelocate -= FreeStacksToFill;
					}
					else
					{
						FreeStacksToFill = 0;
					}

					if ((NumberOfCardsToRelocate != 0) || (CalculateMaxSequenceMoves(NumberOfFreecells-FreecellsToFill, NumberOfFreeStacks-FreeStacksToFill) <
						NumberOfCards - c))
						continue;

					// We can move it
					CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

					// Fill the freecells with the top cards
					for(a=0 ; a<FreecellsToFill ; a++)
					{
						// Find a vacant freecell
						for(b=0;b<m_NumberOfFreecells;b++)
							if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
								break;

						NewStateWithLocations->PopStackCard(DestinationStack, TempCard);
						NewStateWithLocations->PutCardInFreecell(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
						TempMove->SetSourceStack(DestinationStack);
						TempMove->SetDestFreecell(b);

						Moves->Push(TempMove);
					}

					// Fill the free stacks with the cards below them
					for(a=0; a < FreeStacksToFill ; a++)
					{
						//  Find a vacant stack
						for(b=0;b<m_NumberOfStacks;b++)
							if (NewStateWithLocations->GetStackLength(b) == 0)
								break;

						NewStateWithLocations->PopStackCard(DestinationStack, TempCard);
						NewStateWithLocations->PushCardIntoStack(b, TempCard);

						TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
						TempMove->SetSourceStack(DestinationStack);
						TempMove->SetDestStack(b);
						TempMove->SetNumberOfCardsInSequence(1);

						Moves->Push(TempMove);
					}

					for(a=c; a <= NumberOfCards-1 ; a++)
						NewStateWithLocations->PushStackCardIntoStack(DestinationStack, Stack, a);

					for(a=0;a<NumberOfCards-c;a++)
						NewStateWithLocations->PopStackCard(Stack, TempCard);

					TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
					TempMove->SetSourceStack(Stack);
					TempMove->SetDestStack(DestinationStack);
					TempMove->SetNumberOfCardsInSequence(NumberOfCards-c);

					Moves->Push(TempMove);

					if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
									  &Moves, &TempMove, Depth, &Check))
					{
						delete TempCard;
						delete TempMove;
						return Check;
					}
				}
			}
		}
	}

	delete [] Moves;   
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSFreecellSolvingAlgorithm::EmptyStackIntoFreecells(FCSStateWithLocations*  StateWithLocations,
						  int Depth,
						  int NumberOfFreeStacks,
						  int NumberOfFreecells,
						  FCSDerivedStatesList* DerivedStateList)
{

	if ((m_EmptyStacksFill == FCS_ES_FILLED_BY_NONE) || (NumberOfFreeStacks != 0))
		return FCS_STATE_IS_NOT_SOLVEABLE;
	
	FCSStateWithLocations *NewStateWithLocations;
	FCSCard *TempCard = FCSCard::Create();
	FCSStateSolvingReturnCodes Check;

	int NumberOfCards, b;

	FCSMoveStack *Moves = CreateMoveStack();
	FCSMove *TempMove = FCSMove::Create();

	// Now, let's try to empty an entire stack into the freecells, so other cards can 
	// inhabit it
	for(int Stack=0;Stack<m_NumberOfStacks;Stack++)
	{
		NumberOfCards = StateWithLocations->GetStackLength(Stack);
		if (NumberOfCards > NumberOfFreecells)
			continue;

		// We can empty it
		CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

		for(int c=0;c<NumberOfCards;c++)
		{
			// Find a vacant freecell
			for(b=0; b<m_NumberOfFreecells; b++)
				if (NewStateWithLocations->GetFreecellCardNumber(b) == 0)
					break;

			NewStateWithLocations->PopStackCard(Stack, TempCard);
			NewStateWithLocations->PutCardInFreecell(b, TempCard);

			TempMove->SetType(FCS_MOVE_TYPE_STACK_TO_FREECELL);
			TempMove->SetSourceStack(Stack);
			TempMove->SetDestFreecell(b);

			Moves->Push(TempMove);
		}

		if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
						  &Moves, &TempMove, Depth, &Check))
		{
			delete TempCard;
			delete TempMove;
			return Check;
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}