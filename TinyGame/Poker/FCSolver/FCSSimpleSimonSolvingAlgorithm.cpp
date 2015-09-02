////////////////////////////////////////////////
///\file FCSSimpleSimonSolvingAlgorithm.cpp
///\brief This file contains the implementation of the FCSSimpleSimonSolvingAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSSimpleSimonSolvingAlgorithm.h"
#include "FCSAStarSolvingAlgorithm.h"
#include "FCSBFSSolvingAlgorithm.h"
#include "FCSDFSSolvingAlgorithm.h"
#include "FCSRandomDFSSolvingAlgorithm.h"
#include "FCSSoftDFSSolvingAlgorithm.h"


FCSSimpleSimonSolvingAlgorithm::FCSSimpleSimonSolvingAlgorithm() : FCSFreecellData(), FCSFreecellAlgorithm()
{
}

FCSSimpleSimonSolvingAlgorithm::FCSSimpleSimonSolvingAlgorithm(FCCommandLineArguments* CommandLine) : FCSFreecellData(CommandLine), FCSFreecellAlgorithm()
{
}

FCSSimpleSimonSolvingAlgorithm::~FCSSimpleSimonSolvingAlgorithm()
{
}

FCSSimpleSimonSolvingAlgorithm* FCSSimpleSimonSolvingAlgorithm::Create(FCCommandLineArguments* CommandLine)
{

	if (CommandLine == NULL)
		return NULL;

	//determine with solving algorithm to create
	switch(CommandLine->GetSolvingMethodType())
	{
	case FCS_METHOD_HARD_DFS:
		return new FCSDFSSolvingAlgorithm<FCSSimpleSimonSolvingAlgorithm>(CommandLine);
		break;
	case FCS_METHOD_SOFT_DFS:
		return new FCSSoftDFSSolvingAlgorithm<FCSSimpleSimonSolvingAlgorithm>(CommandLine);
		break;
	case FCS_METHOD_BFS:
		return new FCSBFSSolvingAlgorithm<FCSSimpleSimonSolvingAlgorithm>(CommandLine);
		break;
	case FCS_METHOD_A_STAR:
		return new FCSAStarSolvingAlgorithm<FCSSimpleSimonSolvingAlgorithm>(CommandLine);
		break;
	case FCS_METHOD_RANDOM_DFS:
		return new FCSRandomDFSSolvingAlgorithm<FCSSimpleSimonSolvingAlgorithm>(CommandLine);
		break;
	default:
		return NULL;
	}

	return NULL;
}

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::RunTest(int TestNumber, FCSStateWithLocations*  StateWithLocations,
													  int Depth, int NumberOfFreeStacks,
													  int NumberOfFreecells,
													  FCSDerivedStatesList* DerivedStateList)
{
	switch(TestNumber)
	{
	case 0:
		return MoveSequenceToFounds(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 1:
		return MoveSequenceToTrueParent(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 2:
		return MoveWholeStackSequenceToFalseParent(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 3:
		return MoveSequenceToTrueParentWithSomeCardsAbove(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 4:
		return MoveSequenceWithSomeCardsAboveToTrueParent(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 5:
		return MoveSequenceWithJunkSequenceAboveToTrueParentWithSomeCardsAbove(StateWithLocations, Depth, 
										NumberOfFreeStacks, NumberOfFreecells, DerivedStateList);
	case 6:
		return MoveWholeStackSequenceToFalseParentWithSomeCardsAbove(StateWithLocations, Depth, 
										NumberOfFreeStacks, NumberOfFreecells, DerivedStateList);
	case 7:
		return MoveSequenceToParentOnTheSameStack(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	case 8:
		return MoveSequenceToFalseParent(StateWithLocations, Depth, NumberOfFreeStacks,
										NumberOfFreecells, DerivedStateList);
	default:
		return FCS_STATE_IS_NOT_SOLVEABLE;
	}
}

bool FCSSimpleSimonSolvingAlgorithm::IsSimpleSimonTrueParent(FCSCard* Parent, FCSCard* Child)
{
	return (IsSimpleSimonFalseParent(Parent, Child) && 
			IsSimpleSimonTrueParentSuit(Parent, Child) );
}

bool FCSSimpleSimonSolvingAlgorithm::IsSimpleSimonTrueParentSuit(FCSCard* Parent, FCSCard* Child)
{
	return (Parent->GetSuit() == Child->GetSuit());
}

bool FCSSimpleSimonSolvingAlgorithm::IsSimpleSimonFalseParent(FCSCard* Parent, FCSCard* Child)
{
	return (Parent->GetCardNumber() == Child->GetCardNumber()+1);
}

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveSequenceToFounds(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();

	int NumberOfCardsInStack, HeightOfCard;
	FCSStateSolvingReturnCodes Check;

	FCSCard *TempCard = FCSCard::Create(),
			*Card, *AboveCard;

	for(int StackIndex=0;StackIndex<m_NumberOfStacks;StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack < 13)
			continue;

		Card = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);

		// Check if the top 13 cards are a sequence
		for(HeightOfCard=2;HeightOfCard<=13;HeightOfCard++)
		{
			AboveCard = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-HeightOfCard);
			if (!IsSimpleSimonTrueParent(AboveCard, Card))
				break;

			Card = AboveCard;
		}

		if (HeightOfCard <= 13)
			continue;

		// We can move this sequence up there
		CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

		for(HeightOfCard=0;HeightOfCard<13;HeightOfCard++)
		{
			NewStateWithLocations->PopStackCard(StackIndex, TempCard);
			NewStateWithLocations->IncrementFoundation(Card->GetSuit());
		}

		TempMove->SetType(FCS_MOVE_TYPE_SEQ_TO_FOUNDATION);
		TempMove->SetSourceStack(StackIndex);
		TempMove->SetFoundation(Card->GetSuit());
		Moves->Push(TempMove);

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

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveSequenceToTrueParent(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{

	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();

	int NumberOfCardsInStack, NumberOfTrueSequences, NumberOfCardsInDestStack;
	FCSStateSolvingReturnCodes Check;

	FCSCard *TempCard = FCSCard::Create(),
			*Card, *NextCard;

	for(int StackIndex=0;StackIndex<m_NumberOfStacks;StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack <= 0)
			continue;

		// Loop on the cards in the stack and try to look for a true
		// parent on top one of the stacks
		Card = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);
		NumberOfTrueSequences = 1;

		for(int HeightOfStack=NumberOfCardsInStack-2;HeightOfStack>=-1;HeightOfStack--)
		{
			for(int DestStack = 0;DestStack<m_NumberOfStacks;DestStack++)
			{
				if (DestStack == StackIndex)
					continue;

				NumberOfCardsInDestStack = StateWithLocations->GetStackLength(DestStack);
				if ((NumberOfCardsInDestStack > 0) &&
					(IsSimpleSimonTrueParent(StateWithLocations->GetStackCard(DestStack, NumberOfCardsInDestStack-1), Card)) &&
					// This is a suitable parent - let's check if we
					// have enough empty stacks to make the move feasible
					(CalculateMaxSequenceMoves(0, NumberOfFreeStacks) >= NumberOfTrueSequences))
				{

					// We can do it - so let's move
					CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

					MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
					DestStack, StackIndex, HeightOfStack+1, NumberOfCardsInStack-1);

					if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
								  &Moves, &TempMove, Depth, &Check))
					{
						delete TempCard;
						delete TempMove;
						return Check;
					}
				}
			}

			//***** Can this line be rearranged for better logic?
			// Stop if we reached the bottom of the stack
			if (HeightOfStack == -1)
				break;

			NextCard = StateWithLocations->GetStackCard(StackIndex, HeightOfStack);
			// If this is no longer a sequence - move to the next stack
			if (!IsSimpleSimonFalseParent(NextCard, Card))
				break;

			if (!IsSimpleSimonTrueParentSuit(NextCard, Card))
				NumberOfTrueSequences++;

			Card = NextCard;
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveWholeStackSequenceToFalseParent(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();

	int NumberOfCardsInStack, HeightOfStack,
		NumberOfTrueSequences, NumberOfCardsInDestStack;
	FCSStateSolvingReturnCodes Check;

	FCSCard *TempCard = FCSCard::Create(),
			*Card, *NextCard;

	for(int StackIndex=0;StackIndex<m_NumberOfStacks;StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack <= 0)
			continue;

		Card = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);
		NumberOfTrueSequences = 1;

		// Stop if we reached the bottom of the stack
		for(HeightOfStack=NumberOfCardsInStack-2;HeightOfStack>-1;HeightOfStack--)
		{
			NextCard = StateWithLocations->GetStackCard(StackIndex, HeightOfStack);
			// If this is no longer a sequence - move to the next stack
			if (!IsSimpleSimonFalseParent(NextCard, Card))
				break;

			if (!IsSimpleSimonTrueParentSuit(NextCard, Card))
				NumberOfTrueSequences++;

			Card = NextCard;
		}
		// This means that the loop exited prematurely and the stack does
		// not contain a sequence.
		if (HeightOfStack != -1)
			continue;

		for(int DestStack=0;DestStack<m_NumberOfStacks;DestStack++)
		{
			NumberOfCardsInDestStack = StateWithLocations->GetStackLength(DestStack);

			if (NumberOfCardsInDestStack <= 0)
				continue;

			if (!IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(DestStack, NumberOfCardsInDestStack-1), Card))
				continue;

			// This is a suitable parent - let's check if we
			// have enough empty stacks to make the move feasible
			if (CalculateMaxSequenceMoves(0, NumberOfFreeStacks) < NumberOfTrueSequences)
				continue;

			// We can do it - so let's move
			CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

			MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
							DestStack, StackIndex, HeightOfStack+1, NumberOfCardsInStack-1);

			if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
			  &Moves, &TempMove, Depth, &Check))
			{
				delete TempCard;
				delete TempMove;
				return Check;
			}
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveSequenceToTrueParentWithSomeCardsAbove(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();

	FCSStateSolvingReturnCodes Check;

	int NumberOfCardsInStack, NumberOfTrueSequences, NumberOfCardsInDestStack,
		NumberOfSeparateFalseSequences,	FalseSequencesIndex, 
		AfterJunkNumberOfFreeStacks;

	int AboveNumberOfTrueSequences[MAX_NUM_CARDS_IN_A_STACK],
		SeparatePointOfFalseSequences[MAX_NUM_CARDS_IN_A_STACK],
		StacksMap[MAX_NUM_STACKS],
		JunkMoveToStacks[MAX_NUM_STACKS];

	FCSCard *TempCard = FCSCard::Create(),
			*Card, *NextCard;

	/*
	 * stack - the source stack index
	 * cards_num - the number of cards in "stack"
	 * h - the height of the current card in "stack"
	 * card - the card in height "h"
	 * suit - its suit
	 * card_num - its card number
	 * ds - the destionation stack index
	 * dest_cards_num - the number of cards in "ds"
	 * dc - the index of the current card in "ds".
	 * num_separate_false_seqs - this variable tells how many distinct false
	 *      sequences exist above the true parent
	 * above_num_true_seqs[] - the number of true sequences in each false
	 *      sequence
	 * seq_points[] - the separation points of the false sequences (i.e: where
	 *      they begin and end)
	 * stacks_map[] - a boolean map that indicates if one can place a card
	 *      on this stack or is it already taken.
	 * junk_move_to_stacks[] - the stacks to move each false sequence of the
	 *      junk to.
	 * FalseSequencesIndex - an iterator to hold the index of the current false
	 *      sequence.
	 * after_junk_num_freestacks - this variable holds the number of stacks
	 *      that remained unoccupied during and after the process of moving
	 *      the junk sequences to different stacks.
	 *
	 *
	 */
	for(int StackIndex=0;StackIndex<m_NumberOfStacks;StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack <= 0)
			continue;

		Card = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);
		NumberOfTrueSequences = 1;

		for(int HeightOfStack = NumberOfCardsInStack-2;HeightOfStack>=-1;HeightOfStack--)
		{
			for(int DestStack=0;DestStack<m_NumberOfStacks;DestStack++)
			{
				if (DestStack == StackIndex)
					continue;

				NumberOfCardsInDestStack = StateWithLocations->GetStackLength(DestStack);
				if (NumberOfCardsInDestStack <= 0)
					continue;

				for(int DestStackCardIndex=NumberOfCardsInDestStack-1;DestStackCardIndex>=0;DestStackCardIndex--)
				{
					if (!IsSimpleSimonTrueParent(StateWithLocations->GetStackCard(DestStack, DestStackCardIndex), Card))
						continue;

					// This is a suitable parent - let's check if there's a sequence above it.
					int AboveCardHeight;
					FCSCard *AboveCard, *UpAboveCard;

					NumberOfSeparateFalseSequences = 0;
					AboveCard = StateWithLocations->GetStackCard(DestStack, NumberOfCardsInDestStack-1);
					AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
					for(AboveCardHeight = NumberOfCardsInDestStack-2; AboveCardHeight > DestStackCardIndex; AboveCardHeight--)
					{
						UpAboveCard = StateWithLocations->GetStackCard(DestStack, AboveCardHeight);
						if (!IsSimpleSimonFalseParent(UpAboveCard, AboveCard))
						{
							SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;
							AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
						}
						AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] += ! (UpAboveCard->GetSuit() == AboveCard->GetSuit());
						AboveCard = UpAboveCard;
					}

					if (DestStackCardIndex < NumberOfCardsInDestStack - 1)
						SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;

					for(int a=0;a<m_NumberOfStacks;a++)
						StacksMap[a] = 0;

					StacksMap[StackIndex] = 1;
					StacksMap[DestStack] = 1;

					AfterJunkNumberOfFreeStacks = NumberOfFreeStacks;

					for(FalseSequencesIndex=0;FalseSequencesIndex<NumberOfSeparateFalseSequences;FalseSequencesIndex++)
					{
						// Find a suitable place to put it

						// ClearJunkDestStack is the stack to move this particular junk sequence to.
						int ClearJunkDestStack;

						// Let's try to find a suitable parent on top one of the stacks
						for(ClearJunkDestStack=0; ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
						{
							int ClearJunkStackLength = StateWithLocations->GetStackLength(ClearJunkDestStack);

							if ( (ClearJunkStackLength > 0) && (StacksMap[ClearJunkDestStack] == 0) &&
								 (IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(ClearJunkDestStack, ClearJunkStackLength-1), StateWithLocations->GetStackCard(DestStack, SeparatePointOfFalseSequences[FalseSequencesIndex]))) &&
								 (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks) >= AboveNumberOfTrueSequences[FalseSequencesIndex]) )
							{
								StacksMap[ClearJunkDestStack] = 1;
								break;
							}
						}

						if (ClearJunkDestStack == m_NumberOfStacks)
						{
							ClearJunkDestStack = -1;
							// Check if there is a vacant stack
							if (NumberOfFreeStacks > 0)
							{
								if (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks-1) >= AboveNumberOfTrueSequences[FalseSequencesIndex])
								{
									// Find an empty stack and designate it as the destination for the junk
									for(ClearJunkDestStack = 0;	ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
									{
										if ((StateWithLocations->GetStackLength(ClearJunkDestStack) == 0) && 
											(StacksMap[ClearJunkDestStack] == 0))
										{
											StacksMap[ClearJunkDestStack] = 1;
											break;
										}
									}
								}
								AfterJunkNumberOfFreeStacks--;
							}

							if (ClearJunkDestStack == -1)
								break;
						}

						JunkMoveToStacks[FalseSequencesIndex] = ClearJunkDestStack;
					}

					if (FalseSequencesIndex != NumberOfSeparateFalseSequences)
						continue;

					if (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks) < NumberOfTrueSequences)
						continue;

					// We can do it - so let's move everything.
					// Notice that we only put the child in a different stack
					// then the parent and let it move to the parent in the 
					// next iteration of the program 

					CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

					// Move the junk cards to their place

					for(FalseSequencesIndex=0; FalseSequencesIndex<NumberOfSeparateFalseSequences; FalseSequencesIndex++)
					{
						// start and end are the start and end heights of the sequence that is to be moved.
						int End = ((FalseSequencesIndex == 0) ? (NumberOfCardsInDestStack-1) : (SeparatePointOfFalseSequences[FalseSequencesIndex-1]-1));

						MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
									JunkMoveToStacks[FalseSequencesIndex], DestStack, SeparatePointOfFalseSequences[FalseSequencesIndex], End);
					}

					// Move the source seq on top of the dest seq										
					MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
									DestStack, StackIndex, HeightOfStack+1, NumberOfCardsInStack-1);

					if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList, 
										 &Moves, &TempMove, Depth, &Check))
					{
						delete TempCard;
						delete TempMove;
						return Check;
					}
				}
			}

			//**** Can this line be rearranged for better logic?
			// Stop if we reached the bottom of the stack
			if (HeightOfStack == -1)
				break;

			// If this is no longer a sequence - move to the next stack
			if (!IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(StackIndex, HeightOfStack), Card))
				break;

			NextCard = StateWithLocations->GetStackCard(StackIndex, HeightOfStack);
			if (!IsSimpleSimonTrueParentSuit(NextCard, Card))
				NumberOfTrueSequences++;

			Card = NextCard;
		}
	}

	delete [] Moves;
	delete TempMove;
	delete TempCard;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveSequenceWithSomeCardsAboveToTrueParent(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();

	FCSStateSolvingReturnCodes Check;

	int NumberOfCardsInStack, NumberOfTrueSequences, NumberOfCardsInDestStack,
		NumberOfSeparateFalseSequences,	FalseSequencesIndex, 
		AfterJunkNumberOfFreeStacks;

	int AboveNumberOfTrueSequences[MAX_NUM_CARDS_IN_A_STACK],
		SeparatePointOfFalseSequences[MAX_NUM_CARDS_IN_A_STACK],
		StacksMap[MAX_NUM_STACKS],
		JunkMoveToStacks[MAX_NUM_STACKS];

	FCSCard *TempCard = FCSCard::Create(),
			*Card, *SavedCard;

	for(int StackIndex=0;StackIndex<m_NumberOfStacks;StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack <= 0)
			continue;

		for(int StackCard = NumberOfCardsInStack-1 ; StackCard >= 0 ; StackCard-- )
		{
			int AboveCardHeight, EndOfSourceSequence;
			FCSCard *AboveCard, *UpAboveCard;

			SavedCard = Card = StateWithLocations->GetStackCard(StackIndex, StackCard);
			NumberOfTrueSequences = 1;

			for (EndOfSourceSequence = StackCard+1; EndOfSourceSequence < NumberOfCardsInStack ; EndOfSourceSequence++)
			{
				AboveCard = StateWithLocations->GetStackCard(StackIndex, EndOfSourceSequence);
				if (!IsSimpleSimonFalseParent(Card, AboveCard))
					break;

				if (!IsSimpleSimonTrueParentSuit(Card, AboveCard))
					NumberOfTrueSequences++;

				Card = AboveCard;
			}

			if (EndOfSourceSequence == NumberOfCardsInStack)
				continue;

			// Split the cards above it into false sequences
			NumberOfSeparateFalseSequences = 0;
			AboveCard = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);
			AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
			for(AboveCardHeight = NumberOfCardsInStack-2 ; AboveCardHeight > EndOfSourceSequence-1; AboveCardHeight--)
			{
				UpAboveCard = StateWithLocations->GetStackCard(StackIndex, AboveCardHeight);
				if (!IsSimpleSimonFalseParent(UpAboveCard, AboveCard))
				{
					SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;
					AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
				}
				AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] += ! (UpAboveCard->GetSuit() == AboveCard->GetSuit());
				AboveCard = UpAboveCard;
			}

			if (EndOfSourceSequence-1 < NumberOfCardsInStack-1)
				SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;

			for(int DestStack=0;DestStack<m_NumberOfStacks;DestStack++)
			{
				if (DestStack == StackIndex)
					continue;

				NumberOfCardsInDestStack = StateWithLocations->GetStackLength(DestStack);

				if (NumberOfCardsInDestStack <= 0)
					continue;

				if (!IsSimpleSimonTrueParent(StateWithLocations->GetStackCard(DestStack, NumberOfCardsInDestStack-1), SavedCard))
					continue;

				// This is a suitable parent - let's check if we
				// have enough empty stacks to make the move feasible

				for(int a=0;a<m_NumberOfStacks;a++)
					StacksMap[a] = 0;

				StacksMap[StackIndex] = 1;
				StacksMap[DestStack] = 1;

				AfterJunkNumberOfFreeStacks = NumberOfFreeStacks;

				for(FalseSequencesIndex=0;FalseSequencesIndex<NumberOfSeparateFalseSequences;FalseSequencesIndex++)
				{
					// Find a suitable place to put it
					int ClearJunkDestStack;

					// Let's try to find a suitable parent on top one of the stacks
					for(ClearJunkDestStack=0; ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
					{
						int ClearJunkStackLength = StateWithLocations->GetStackLength(ClearJunkDestStack);

						if ((ClearJunkStackLength > 0) && (StacksMap[ClearJunkDestStack] == 0) && 
							(IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(ClearJunkDestStack, ClearJunkStackLength-1), StateWithLocations->GetStackCard(StackIndex, SeparatePointOfFalseSequences[FalseSequencesIndex]))) && 
							(CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks) >= AboveNumberOfTrueSequences[FalseSequencesIndex]))
						{
							StacksMap[ClearJunkDestStack] = 1;
							break;
						}
					}

					if (ClearJunkDestStack == m_NumberOfStacks)
					{
						ClearJunkDestStack = -1;

						// Check if there is a vacant stack
						if (NumberOfFreeStacks > 0)
						{
							if (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks-1) >= AboveNumberOfTrueSequences[FalseSequencesIndex])
							{
								// Find an empty stack and designate it as the destination for the junk
								for(ClearJunkDestStack = 0; ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
								{
									if ((StateWithLocations->GetStackLength(ClearJunkDestStack) == 0) && (StacksMap[ClearJunkDestStack] == 0))
									{
										StacksMap[ClearJunkDestStack] = 1;
										break;
									}
								}
							}
							AfterJunkNumberOfFreeStacks--;
						}

						if (ClearJunkDestStack == -1)
							break;
					}

					JunkMoveToStacks[FalseSequencesIndex] = ClearJunkDestStack;
				}

				if (FalseSequencesIndex != NumberOfSeparateFalseSequences)
					continue;

				if (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks) <= NumberOfTrueSequences)
					continue;

				CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

				// Let's boogie - we can move everything
				// Move the junk cards to their place
				for(FalseSequencesIndex=0; FalseSequencesIndex<NumberOfSeparateFalseSequences; FalseSequencesIndex++)
				{
					int End = ((FalseSequencesIndex == 0) ? (NumberOfCardsInStack-1) : (SeparatePointOfFalseSequences[FalseSequencesIndex-1]-1));

					MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
									JunkMoveToStacks[FalseSequencesIndex], StackIndex, SeparatePointOfFalseSequences[FalseSequencesIndex], End);
				}

				MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
							DestStack, StackIndex, StackCard, EndOfSourceSequence-1);

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

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveSequenceWithJunkSequenceAboveToTrueParentWithSomeCardsAbove(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();

	FCSStateSolvingReturnCodes Check;

	/*
	 * stack - the source stack index
	 * cards_num - the number of cards in "stack"
	 * h - the height of the current card in "stack".
	 * card - the current card in "stack"
	 * ds - the index of the destination stack
	 * dest_cards_num - the number of cards in "ds".
	 * dc - the height of the current card in "ds".
	 * num_separate_false_seqs - the number of false sequences
	 * seq_points[] - the places in which the false sequences of the junk begin
	 *      and end
	 * false_seq_index - an iterator that marks the index of the current 
	 *      false sequence
	 * stacks_map[] - a map of booleans that indicates if one can place a card
	 *      on this stack or is already taken.
	 * above_num_true_seqs[] - the number of true sequences in each false
	 *      sequence
	 * num_src_junk_true_seqs - the number of true seqs in the false seq above
	 *      the source card.
	 * end_of_junk - the height marking the end of the source junk.
	 * num_true_seqs - the number of true sequences in the false seq which we
	 *      wish to move.
	 * */
	int NumberOfCardsInStack, HeightOfCardInStack,
		NumberOfTrueSequences, NumberOfCardsInDestStack,
		NumberOfSourceJunkTrueSequences, NumberOfSeparateFalseSequences,	
		FalseSequencesIndex, AfterJunkNumberOfFreeStacks, EndOfJunk;

	int AboveNumberOfTrueSequences[MAX_NUM_CARDS_IN_A_STACK],
		SeparatePointOfFalseSequences[MAX_NUM_CARDS_IN_A_STACK],
		StacksMap[MAX_NUM_STACKS],
		JunkMoveToStacks[MAX_NUM_STACKS];

	FCSCard *TempCard = FCSCard::Create(),
			*Card, *SavedCard;

	for(int StackIndex=0;StackIndex<m_NumberOfStacks;StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack <= 0)
			continue;

		SavedCard = Card = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);
		NumberOfSourceJunkTrueSequences = 1;

		//Count the number of junk true sequences in source stack
		for(HeightOfCardInStack=NumberOfCardsInStack-2;HeightOfCardInStack>-1;HeightOfCardInStack--)
		{
			Card = StateWithLocations->GetStackCard(StackIndex, HeightOfCardInStack);
			if (!IsSimpleSimonFalseParent(Card, SavedCard))
				break;

			if (!IsSimpleSimonTrueParentSuit(Card, SavedCard))
				NumberOfSourceJunkTrueSequences++;

			SavedCard = Card;
		}

		if (HeightOfCardInStack == -1)
			continue;

		EndOfJunk = HeightOfCardInStack;
		NumberOfTrueSequences = 1;

		for(;HeightOfCardInStack>-1;HeightOfCardInStack--)
		{
			Card = StateWithLocations->GetStackCard(StackIndex, HeightOfCardInStack);
			if (!IsSimpleSimonFalseParent(Card, SavedCard))
				break;

			if (!IsSimpleSimonTrueParentSuit(Card, SavedCard))
				NumberOfTrueSequences++;

			SavedCard = Card;
		}

		SavedCard = Card;

		for(int DestStack =0;DestStack<m_NumberOfStacks;DestStack++)
		{
			if (DestStack == StackIndex)
				continue;

			NumberOfCardsInDestStack = StateWithLocations->GetStackLength(DestStack);
			// At least 2 cards in the stack
			if (NumberOfCardsInDestStack <= 1)
				continue;

			// Start at the card below the top one, so we will
			// make sure there's at least some junk above it
			for(int DestStackCardIndex=NumberOfCardsInDestStack-2;DestStackCardIndex>=0;DestStackCardIndex--)
			{
				if (!IsSimpleSimonTrueParent(StateWithLocations->GetStackCard(DestStack, DestStackCardIndex), SavedCard))
					continue;
				
				// This is a suitable parent - let's check if there's a sequence above it.
				int AboveCardHeight;
				FCSCard *AboveCard, *UpAboveCard;

				NumberOfSeparateFalseSequences = 0;
				AboveCard = StateWithLocations->GetStackCard(DestStack, NumberOfCardsInDestStack-1);
				AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
				for(AboveCardHeight = NumberOfCardsInDestStack-2; AboveCardHeight > DestStackCardIndex; AboveCardHeight--)
				{
					UpAboveCard = StateWithLocations->GetStackCard(DestStack, AboveCardHeight);
					if (!IsSimpleSimonFalseParent(UpAboveCard, AboveCard))
					{
						SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;
						AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
					}
					AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] += ! (IsSimpleSimonTrueParentSuit(UpAboveCard, AboveCard));
					AboveCard = UpAboveCard;
				}

				if (DestStackCardIndex < NumberOfCardsInDestStack - 1)
					SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;

				for(int a=0;a<m_NumberOfStacks;a++)
					StacksMap[a] = 0;

				StacksMap[StackIndex] = 1;
				StacksMap[DestStack] = 1;

				AfterJunkNumberOfFreeStacks = NumberOfFreeStacks;

				for(FalseSequencesIndex=0;FalseSequencesIndex<NumberOfSeparateFalseSequences+1;FalseSequencesIndex++)
				{
					// Find a suitable place to put it
					int ClearJunkDestStack;

					FCSCard *TheCard = (FalseSequencesIndex == NumberOfSeparateFalseSequences) ? 
						StateWithLocations->GetStackCard(StackIndex, EndOfJunk+1) :
						StateWithLocations->GetStackCard(DestStack, SeparatePointOfFalseSequences[FalseSequencesIndex]);

					int TheNumberOfTrueSequences = (FalseSequencesIndex == NumberOfSeparateFalseSequences) ?
								NumberOfSourceJunkTrueSequences :
								AboveNumberOfTrueSequences[FalseSequencesIndex];

					// Let's try to find a suitable parent on top one of the stacks
					for(ClearJunkDestStack=0; ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
					{
						int ClearJunkStackLength = StateWithLocations->GetStackLength(ClearJunkDestStack);

						if ((ClearJunkStackLength > 0) && (StacksMap[ClearJunkDestStack] == 0) &&
							(IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(ClearJunkDestStack, ClearJunkStackLength-1), TheCard)) && 
							(CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks) >= TheNumberOfTrueSequences))
						{
							StacksMap[ClearJunkDestStack] = 1;
							break;
						}
					}

					if (ClearJunkDestStack == m_NumberOfStacks)
					{
						ClearJunkDestStack = -1;

						// Check if there is a vacant stack
						if (NumberOfFreeStacks > 0)
						{
							if (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks-1) >= TheNumberOfTrueSequences)
							{
								// Find an empty stack and designate it as the destination for the junk
								for(ClearJunkDestStack = 0; ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
								{
									if ((StateWithLocations->GetStackLength(ClearJunkDestStack) == 0) && 
										(StacksMap[ClearJunkDestStack] == 0))
									{
										StacksMap[ClearJunkDestStack] = 1;
										break;
									}
								}
							}
							AfterJunkNumberOfFreeStacks--;
						}

						if (ClearJunkDestStack == -1)
							break;
					}

					JunkMoveToStacks[FalseSequencesIndex] = ClearJunkDestStack;
				}

				if (FalseSequencesIndex != NumberOfSeparateFalseSequences+1)
					continue;

				if (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks) < NumberOfTrueSequences)
					continue;

				// We can do it - so let's move everything
				CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

				// Move the junk cards to their place 

				for(FalseSequencesIndex=0; FalseSequencesIndex<NumberOfSeparateFalseSequences+1; FalseSequencesIndex++)
				{
					int Start, End, SourceStack; 

					if (FalseSequencesIndex == NumberOfSeparateFalseSequences)
					{
						Start = EndOfJunk+1;
						End = NumberOfCardsInStack-1;
						SourceStack = StackIndex;
					}
					else
					{
						Start = SeparatePointOfFalseSequences[FalseSequencesIndex];
						End = ((FalseSequencesIndex == 0) ? (NumberOfCardsInDestStack-1) : (SeparatePointOfFalseSequences[FalseSequencesIndex-1]-1));
						SourceStack = DestStack;
					}

					MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
								JunkMoveToStacks[FalseSequencesIndex], SourceStack, Start, End);
				}

				// Move the source seq on top of the dest seq
				MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
						DestStack, StackIndex, HeightOfCardInStack, EndOfJunk);

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

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveWholeStackSequenceToFalseParentWithSomeCardsAbove(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();

	FCSStateSolvingReturnCodes Check;

	/* 
	 * stack - the source stack index
	 * cards_num - the number of cards in "stack" 
	 * h - the height of the current card in stack
	 * card - the current card
	 * suit - its suit
	 * card_num - its card number
	 * ds - the destination stack index.
	 * dest_cards_num - the number of cards in it.
	 * dc - the height of the card in "ds".
	 * num_separate_false_seqs - this variable tells how many distinct false
	 *      sequences exist above the false parent
	 * above_num_true_seqs[] - the number of true sequences in each false 
	 *      sequence
	 * seq_points[] - the separation points of the false sequences (i.e: where
	 *      they begin and end)
	 * stacks_map[] - a boolean map that indicates if one can place a card
	 *      on this stack or is it already taken.
	 * junk_move_to_stacks[] - the stacks to move each false sequence of the
	 *      junk to.
	 * false_seq_index - an iterator to hold the index of the current false
	 *      sequence.
	 * after_junk_num_freestacks - a variable that holds the number of stacks
	 *      that are left unoccupied as part of the junk disposal process.
	 * 
	 * */
	int NumberOfCardsInStack, HeightOfCardInStack,
		NumberOfTrueSequences, NumberOfCardsInDestStack,
		NumberOfSeparateFalseSequences,	FalseSequencesIndex, AfterJunkNumberOfFreeStacks;

	int AboveNumberOfTrueSequences[MAX_NUM_CARDS_IN_A_STACK],
		SeparatePointOfFalseSequences[MAX_NUM_CARDS_IN_A_STACK],
		StacksMap[MAX_NUM_STACKS],
		JunkMoveToStacks[MAX_NUM_STACKS];

	FCSCard *TempCard = FCSCard::Create(),
			*Card, *SavedCard;

	for(int StackIndex=0;StackIndex<m_NumberOfStacks;StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack <= 0)
			continue;

		SavedCard = Card = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);
		NumberOfTrueSequences = 1;

		for(HeightOfCardInStack=NumberOfCardsInStack-2;HeightOfCardInStack>-1;HeightOfCardInStack--)
		{
			Card = StateWithLocations->GetStackCard(StackIndex, HeightOfCardInStack);
			if (!IsSimpleSimonFalseParent(Card, SavedCard))
				break;

			if (!IsSimpleSimonTrueParentSuit(Card, SavedCard))
				NumberOfTrueSequences++;

			SavedCard = Card;
		}

		if (HeightOfCardInStack != -1)
			continue;

		for(int DestStack=0;DestStack<m_NumberOfStacks;DestStack++)
		{
			NumberOfCardsInDestStack = StateWithLocations->GetStackLength(DestStack);
			if (NumberOfCardsInDestStack <= 0)
				continue;

			for(int DestStackCardIndex=NumberOfCardsInDestStack-1;DestStackCardIndex>=0;DestStackCardIndex--)
			{
				if (!IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(DestStack, DestStackCardIndex), SavedCard))
					continue;

				// This is a suitable parent - let's check if there's a sequence above it.
				int AboveCardHeight;
				FCSCard *AboveCard, *UpAboveCard;

				NumberOfSeparateFalseSequences = 0;
				AboveCard = StateWithLocations->GetStackCard(DestStack, NumberOfCardsInDestStack-1);
				AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
				for(AboveCardHeight = NumberOfCardsInDestStack-2 ; AboveCardHeight > DestStackCardIndex; AboveCardHeight--)
				{
					UpAboveCard = StateWithLocations->GetStackCard(DestStack, AboveCardHeight);
					if (!IsSimpleSimonFalseParent(UpAboveCard, AboveCard))
					{
						SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;
						AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
					}
					AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] += ! (IsSimpleSimonTrueParentSuit(UpAboveCard, AboveCard));
					AboveCard = UpAboveCard;
				}

				if (DestStackCardIndex < NumberOfCardsInDestStack - 1)
					SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;

				for(int a=0;a<m_NumberOfStacks;a++)
					StacksMap[a] = 0;

				StacksMap[StackIndex] = 1;
				StacksMap[DestStack] = 1;

				AfterJunkNumberOfFreeStacks = NumberOfFreeStacks;

				for(FalseSequencesIndex=0;FalseSequencesIndex<NumberOfSeparateFalseSequences;FalseSequencesIndex++)
				{
					// Find a suitable place to put it
					int ClearJunkDestStack,
						TheNumberOfTrueSequences = AboveNumberOfTrueSequences[FalseSequencesIndex];

					// Let's try to find a suitable parent on top one of the stacks
					for(ClearJunkDestStack=0; ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
					{
						int ClearJunkStackLength = StateWithLocations->GetStackLength(ClearJunkDestStack);

						if ((ClearJunkStackLength > 0) && (StacksMap[ClearJunkDestStack] == 0) &&
							(IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(ClearJunkDestStack, ClearJunkStackLength-1), 
									StateWithLocations->GetStackCard(DestStack, SeparatePointOfFalseSequences[FalseSequencesIndex]))) &&
							(CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreeStacks) >= TheNumberOfTrueSequences))
						{
							StacksMap[ClearJunkDestStack] = 1;
							break;
						}
					}

					if (ClearJunkDestStack == m_NumberOfStacks)
						break;

					JunkMoveToStacks[FalseSequencesIndex] = ClearJunkDestStack;
				} 

				if (FalseSequencesIndex != NumberOfSeparateFalseSequences)
					continue;

				// This is a suitable parent - let's check if we
				// have enough empty stacks to make the move feasible
				if (CalculateMaxSequenceMoves(0, NumberOfFreeStacks) < NumberOfTrueSequences)
					continue;

				// We can do it - so let's move 
				CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

				// Move the junk cards to their place */
				for(FalseSequencesIndex=0; FalseSequencesIndex<NumberOfSeparateFalseSequences; FalseSequencesIndex++)
				{
					int End = ((FalseSequencesIndex == 0) ? (NumberOfCardsInDestStack-1) : (SeparatePointOfFalseSequences[FalseSequencesIndex-1]-1));

					MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
							JunkMoveToStacks[FalseSequencesIndex], DestStack, SeparatePointOfFalseSequences[FalseSequencesIndex], End);
				}

				
				MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
						DestStack, StackIndex, HeightOfCardInStack+1, NumberOfCardsInStack-1);

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

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveSequenceToParentOnTheSameStack(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();

	FCSStateSolvingReturnCodes Check;

	FCSCard *ParentCard, *ChildCard, 
		*TempCard = FCSCard::Create(); 
	int NumberOfCardsInStack,
		AfterJunkNumberOfFreestacks,
		FalseSequencesIndex,
		ChildSequencesIndex;

	for(int StackIndex=0 ; StackIndex < m_NumberOfStacks ; StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack <= 2)
			continue;

		// Search for a parent card
		for(int ParentCardIndex=0; ParentCardIndex < NumberOfCardsInStack-1 ; ParentCardIndex++)
		{
			ParentCard = StateWithLocations->GetStackCard(StackIndex, ParentCardIndex);
			if (IsSimpleSimonTrueParent(ParentCard, StateWithLocations->GetStackCard(StackIndex, ParentCardIndex+1)))
				continue;

			for(int ChildCardIndex = ParentCardIndex + 2 ; ChildCardIndex < NumberOfCardsInStack ; ChildCardIndex++)
			{
				ChildCard = StateWithLocations->GetStackCard(StackIndex, ChildCardIndex);
				if (!IsSimpleSimonTrueParent(ParentCard, ChildCard))
					continue;

				// We have a matching parent and child cards
				// Now let's try to find stacks to place the cards above the child card.
				int AboveNumberOfTrueSequences[MAX_NUM_CARDS_IN_A_STACK],
					SeparatePointOfFalseSequences[MAX_NUM_CARDS_IN_A_STACK],
					StacksMap[MAX_NUM_STACKS],
					JunkMoveToStacks[MAX_NUM_STACKS];

				FCSCard *AboveCard, *UpAboveCard;
				int NumberOfSeparateFalseSequences,
					AboveCardHeight,
					EndOfChildSequence = ChildCardIndex,
					ChildNumberOfTrueSequences = 1;

				while ((EndOfChildSequence+1 < NumberOfCardsInStack) &&
						(IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(StackIndex, EndOfChildSequence),
								StateWithLocations->GetStackCard(StackIndex, EndOfChildSequence+1))))
				{
					ChildNumberOfTrueSequences += (!IsSimpleSimonTrueParent(
								StateWithLocations->GetStackCard(StackIndex, EndOfChildSequence),
								StateWithLocations->GetStackCard(StackIndex, EndOfChildSequence+1)));
					EndOfChildSequence++;
				}

				NumberOfSeparateFalseSequences = 0;
				AboveCard = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);
				AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
				for(AboveCardHeight = NumberOfCardsInStack-2; AboveCardHeight > EndOfChildSequence ; AboveCardHeight--)
				{
					UpAboveCard = StateWithLocations->GetStackCard(StackIndex, AboveCardHeight);
					if (!IsSimpleSimonFalseParent(UpAboveCard, AboveCard))
					{

						SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;
						AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
					}
					AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] += ! (IsSimpleSimonTrueParentSuit(UpAboveCard, AboveCard));
					AboveCard = UpAboveCard;
				}

				if (EndOfChildSequence < NumberOfCardsInStack - 1)
				{
					SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;
				}

				// Add the child to the SeparatePointOfFalseSequences
				ChildSequencesIndex = NumberOfSeparateFalseSequences;
				AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = ChildNumberOfTrueSequences;
				SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = ChildCardIndex;

				// Add the cards between the parent and the child to the SeparatePointOfFalseSequences
				AboveCard = StateWithLocations->GetStackCard(StackIndex, ChildCardIndex-1);
				AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
				for(AboveCardHeight = ChildCardIndex-2;	AboveCardHeight > ParentCardIndex ; AboveCardHeight--)
				{
					UpAboveCard = StateWithLocations->GetStackCard(StackIndex, AboveCardHeight);
					if (!IsSimpleSimonFalseParent(UpAboveCard, AboveCard))
					{
						SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;
						AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] = 1;
					}
					AboveNumberOfTrueSequences[NumberOfSeparateFalseSequences] += ! (IsSimpleSimonTrueParentSuit(UpAboveCard, AboveCard));
					AboveCard = UpAboveCard;
				}

				if (ParentCardIndex < ChildCardIndex - 1)
					SeparatePointOfFalseSequences[NumberOfSeparateFalseSequences++] = AboveCardHeight+1;

				for(int a = 0 ; a < m_NumberOfStacks ; a++)
					StacksMap[a] = 0;

				StacksMap[StackIndex] = 1;

				AfterJunkNumberOfFreestacks = NumberOfFreeStacks;

				for(FalseSequencesIndex=0;FalseSequencesIndex<NumberOfSeparateFalseSequences;FalseSequencesIndex++)
				{
					// Find a suitable place to put it
					int ClearJunkDestStack;

					// Let's try to find a suitable parent on top one of the stacks
					for(ClearJunkDestStack=0; ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
					{
						int ClearJunkStackLength = StateWithLocations->GetStackLength(ClearJunkDestStack);

						if ((ClearJunkStackLength > 0) && (StacksMap[ClearJunkDestStack] == 0) &&
							(IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(ClearJunkDestStack, ClearJunkStackLength-1), StateWithLocations->GetStackCard(StackIndex, SeparatePointOfFalseSequences[FalseSequencesIndex]))) &&
							(CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreestacks) >= AboveNumberOfTrueSequences[FalseSequencesIndex]))
						{
							StacksMap[ClearJunkDestStack] = 1;
							break;
						}
					}

					if (ClearJunkDestStack == m_NumberOfStacks)
					{
						ClearJunkDestStack = -1;
						
						// Check if there is a vacant stack
						if (NumberOfFreeStacks > 0)
						{
							if (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreestacks-1) >= AboveNumberOfTrueSequences[FalseSequencesIndex])
							{
								// Find an empty stack and designate it as the destination for the junk
								for(ClearJunkDestStack = 0; ClearJunkDestStack < m_NumberOfStacks; ClearJunkDestStack++)
								{
									if ((StateWithLocations->GetStackLength(ClearJunkDestStack) == 0) &&
										(StacksMap[ClearJunkDestStack] == 0))
									{
										StacksMap[ClearJunkDestStack] = 1;
										break;
									}
								}
							}
							AfterJunkNumberOfFreestacks--;
						}

						if (ClearJunkDestStack == -1)
							break;
					}

					JunkMoveToStacks[FalseSequencesIndex] = ClearJunkDestStack;
				}

				if (FalseSequencesIndex != NumberOfSeparateFalseSequences)
					continue;

				// Let's check if we can move the child after we are done moving all the junk cards
				if (CalculateMaxSequenceMoves(0, AfterJunkNumberOfFreestacks) < ChildNumberOfTrueSequences)
					continue;

				// We can do it - so let's move everything
				CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

				// Move the junk cards to their place
				for(FalseSequencesIndex=0; FalseSequencesIndex<NumberOfSeparateFalseSequences; FalseSequencesIndex++)
				{
					int End = ((FalseSequencesIndex == 0) ? (NumberOfCardsInStack-1) : (SeparatePointOfFalseSequences[FalseSequencesIndex-1]-1));

					MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
									JunkMoveToStacks[FalseSequencesIndex], StackIndex, SeparatePointOfFalseSequences[FalseSequencesIndex], End);
				}

				int End2 = NewStateWithLocations->GetStackLength(JunkMoveToStacks[ChildSequencesIndex])-1;
				int Start = End2 - (EndOfChildSequence - ChildCardIndex);

				MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove, 
									StackIndex, JunkMoveToStacks[ChildSequencesIndex], Start, End2);

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

FCSStateSolvingReturnCodes FCSSimpleSimonSolvingAlgorithm::MoveSequenceToFalseParent(FCSStateWithLocations*  StateWithLocations,
												  int Depth,
												  int NumberOfFreeStacks,
												  int NumberOfFreecells,
												  FCSDerivedStatesList* DerivedStateList)
{
	FCSStateWithLocations* NewStateWithLocations;
	FCSMoveStack* Moves = CreateMoveStack();
	FCSMove* TempMove = FCSMove::Create();
	FCSStateSolvingReturnCodes Check;

	FCSCard *TempCard = FCSCard::Create(),
			*Card, *NextCard;
	int NumberOfCardsInStack, SequenceSize, 
		NumberOfTrueSequences, HeightOfStack,
		NumberOfCardsInDestStack;


	for (int StackIndex=0;StackIndex<m_NumberOfStacks;StackIndex++)
	{
		NumberOfCardsInStack = StateWithLocations->GetStackLength(StackIndex);
		if (NumberOfCardsInStack <= 0)
			continue;

		Card = StateWithLocations->GetStackCard(StackIndex, NumberOfCardsInStack-1);
		NumberOfTrueSequences = 1;
		SequenceSize = 1;

		for (HeightOfStack=NumberOfCardsInStack-2;HeightOfStack>-1;HeightOfStack--)
		{
			NextCard = StateWithLocations->GetStackCard(StackIndex, HeightOfStack);
			//Go until there is no longer a sequence
			if (!IsSimpleSimonFalseParent(NextCard, Card))
				break;
			
			SequenceSize++;

			if (!IsSimpleSimonTrueParentSuit(NextCard, Card))
				NumberOfTrueSequences++;

			Card = NextCard;
		}

		/******
		Now take the sequence and try and put it on another stack
		For now, the sequence will not be put on empty stacks or the same stack
		********/

		for (int DestStack=0;DestStack<m_NumberOfStacks;DestStack++)
		{
			if (DestStack == StackIndex)
				continue;

			NumberOfCardsInDestStack = StateWithLocations->GetStackLength(DestStack);
			if (NumberOfCardsInDestStack <= 0)
				continue;

			if (!IsSimpleSimonFalseParent(StateWithLocations->GetStackCard(DestStack, NumberOfCardsInDestStack-1), Card))
				continue;

			//This is a suitable parent
			//Do we have enough empty stacks to make the move feasible
			if (CalculateMaxSequenceMoves(0, NumberOfFreeStacks) < NumberOfTrueSequences)
				continue;

			//We can do it
			CheckStateBegin(&NewStateWithLocations, StateWithLocations, Moves);

			MoveSequence(NewStateWithLocations, TempCard, Moves, &TempMove,
						  DestStack, StackIndex, HeightOfStack+1, HeightOfStack+SequenceSize);

			if (CheckStateEnd(&NewStateWithLocations, StateWithLocations, &DerivedStateList,
				&Moves, &TempMove, Depth, &Check))
			{
				delete TempCard;
				delete TempMove;
				return Check;
			}
		}
	}

	delete [] Moves;
	delete TempCard;
	delete TempMove;

	return FCS_STATE_IS_NOT_SOLVEABLE;
}