#ifndef MMANN_FCS_INDIRECT_STATE_WITH_LOCATIONS_H
#define MMANN_FCS_INDIRECT_STATE_WITH_LOCATIONS_H

////////////////////////////////////////////////
///\file FCSIndirectStateWithLocations.h
///\brief This file contains the FCSIndirectStateWithLocations class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <malloc.h>
#include "FCSStackStorage.h"
#include "FCHelpingAlgorithms.h"

class FCSIndirectState;

///\brief Abstract class representing a indirect state in a card game with additional data 
///to process and store it
template <class StateWithLocation>
class FCSIndirectStateWithLocations : public StateWithLocation
{
public:
	///Constructor
	FCSIndirectStateWithLocations();

	///Destructor
	virtual ~FCSIndirectStateWithLocations();

	///\brief Copy FCSIndirectStateWithLocations object
	///
	///\param State is the FCSIndirectStateWithLocations to be copied
	virtual void Copy(FCSStateWithLocations* State);

	///\brief Compare a FCSIndirectStateWithLocations to another
	///
	///\param State to be compared to
	///\return -1 if this FCSIndirectStateWithLocations < State, 1 if FCSIndirectStateWithLocations > State, 
	///0 if FCSIndirectStateWithLocations = State
	int Compare(const FCSStateWithLocations* State);

	///\brief Get a card from the state
	///
	///\param Stack the card is in
	///\param Card is the position of the card
	///\return A pointer to the card
	FCSCard* GetStackCard(int Stack, int Card);

	///\brief Get a suit from a card in the state
	///
	///\param Stack the card is in
	///\param Card is the position of the card
	///\return The card's suit
	char GetStackCardSuit(int Stack, int Card);

	///\brief Get a card number from a card in the state
	///
	///\param Stack the card is in
	///\param Card is the position of the card
	///\return The card's number
	char GetStackCardNumber(int Stack, int Card);

	///\brief Get the number of cards in the stack
	///
	///\param Stack is the stack to get the length from
	///\return The number of cards in the stack
	int GetStackLength(int Stack);

	///\brief Get a freecell card from the state
	///
	///\param Freecell the card is in
	///\return A pointer to the card
	FCSCard* GetFreecellCard(int Freecell);

	///\brief Get a suit from a freecell card in the state
	///
	///\param Freecell the card is in
	///\return The card's suit
	char GetFreecellCardSuit(int Freecell);

	///\brief Get a card number from a freecell card in the state
	///
	///\param Freecell the card is in
	///\return The card's number
	char GetFreecellCardNumber(int Freecell);

	///\brief Get a card number from a foundation in the state
	///
	///\param Foundation the card is in
	///\return The card's number
	char GetFoundation(int Foundation);

	///\brief Add the next card to a foundation in the state
	///
	///\param Foundation the card is in
	void IncrementFoundation(int Foundation);

	///\brief Put a card into a foundation in the state
	///
	///\param Foundation the card is in
	///\param Value is the card number
	void SetFoundation(int Foundation, char Value);

	///\brief Take a card off a stack in the state
	///
	///\param Stack the card is in
	///\param Card a pointer to the card being removed
	void PopStackCard(int Stack, FCSCard* Card);

	///\brief Move a card from one stack to another
	///
	///\param DestStack is the stack the card is going to
	///\param SrcStack is the stack the card is coming from
	///\param SrcCard is the position of the card in the source stack
	void PushStackCardIntoStack(int DestStack, int SrcStack, int SrcCard);

	///\brief Move a card onto a stack
	///
	///\param DestStack is the stack the card is going to
	///\param Card a pointer to the card being added
	void PushCardIntoStack(int DestStack, FCSCard* Card);

	///\brief Move a card onto a freecll
	///
	///\param Freecell the card is going to be put in
	///\param Card a pointer to the card being added
	void PutCardInFreecell(int Freecell, FCSCard* Card);

	///\brief Clear a freecell
	///
	///\param Freecell to be cleared
	void EmptyFreecell(int Freecell);

	///\brief Make a stack card flipped over (visible)
	///
	///\param Stack the card is in
	///\param Card is the position of the card
	void FlipStackCard(int Stack, int Card);

	///\brief Initialize the state and stacks
	void Initialize(int NumberOfStacks);

	///\brief Clear the stacks of the state
	void CleanState();

	///\brief Cache the stacks of the state
	///
	///\param Storage is the storage mechanism for caching the stacks
	///\param StackNumber is the stack being cached
	void CacheStacks(AFCSGenericStackStorage* Storage, int StackNumber);

	///\brief Canonize the state - put it into the internal representation
	///
	///\param NumberOfFreecells is the number of freecells in the card game
	///\param NumberOfStacks is the number of stacks in the card game
	void CanonizeState(int NumberOfFreecells, int NumberOfStacks);

	///\brief Get a pointer to the indirect state
	///
	///\return A pointer to the indirect state at this location
	FCSState* GetState();

public:
	///State in compact form
	FCSIndirectState  m_State;	
};

template <class StateWithLocation>
FCSIndirectStateWithLocations<StateWithLocation>::FCSIndirectStateWithLocations() : StateWithLocation()
{
}

template <class StateWithLocation>
FCSIndirectStateWithLocations<StateWithLocation>::~FCSIndirectStateWithLocations()
{
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::Copy(FCSStateWithLocations* State)
{
	StateWithLocation::Copy(State);
	int StackLength;

	m_State.Copy(&(((FCSIndirectStateWithLocations*)State)->m_State));

	for(int a=0;a<MAX_NUM_STACKS;a++)
	{
		if (((FCSIndirectStateWithLocations*)State)->m_State.m_Stacks[a] != NULL)
		{
			StackLength = State->GetStackLength(a);
			
			m_State.m_Stacks[a] = new FCSIndirectCard[StackLength+53];
			for (int b = 0;b<StackLength+1;b++)
				m_State.m_Stacks[a][b].m_Card = ((FCSIndirectStateWithLocations*)State)->m_State.m_Stacks[a][b].m_Card;
		}
	}
}

template <class StateWithLocation>
int FCSIndirectStateWithLocations<StateWithLocation>::Compare(const FCSStateWithLocations *State)
{
	return m_State.Compare(&(((FCSIndirectStateWithLocations*)State)->m_State));
}

template <class StateWithLocation>
FCSCard* FCSIndirectStateWithLocations<StateWithLocation>::GetStackCard(int Stack, int Card)
{
	return &(m_State.m_Stacks[Stack][Card+1]);
}

template <class StateWithLocation>
char FCSIndirectStateWithLocations<StateWithLocation>::GetStackCardSuit(int Stack, int Card)
{
	return m_State.m_Stacks[Stack][Card+1].GetSuit();
}

template <class StateWithLocation>
char FCSIndirectStateWithLocations<StateWithLocation>::GetStackCardNumber(int Stack, int Card)
{
	return m_State.m_Stacks[Stack][Card+1].GetCardNumber();
}

template <class StateWithLocation>
int FCSIndirectStateWithLocations<StateWithLocation>::GetStackLength(int Stack)
{
	return (int)m_State.m_Stacks[Stack][0].m_Card;
}

template <class StateWithLocation>
FCSCard* FCSIndirectStateWithLocations<StateWithLocation>::GetFreecellCard(int Freecell)
{
	return &(m_State.m_Freecells[Freecell]);
}

template <class StateWithLocation>
char FCSIndirectStateWithLocations<StateWithLocation>::GetFreecellCardSuit(int Freecell)
{
	return m_State.m_Freecells[Freecell].GetSuit();
}

template <class StateWithLocation>
char FCSIndirectStateWithLocations<StateWithLocation>::GetFreecellCardNumber(int Freecell)
{
	return m_State.m_Freecells[Freecell].GetCardNumber();
}

template <class StateWithLocation>
char FCSIndirectStateWithLocations<StateWithLocation>::GetFoundation(int Foundation)
{
	return m_State.m_Foundations[Foundation];
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::IncrementFoundation(int Foundation)
{
	m_State.m_Foundations[Foundation]++;
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::SetFoundation(int Foundation, char Value)
{
	m_State.m_Foundations[Foundation] = Value;
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::PopStackCard(int Stack, FCSCard* Card)
{
	FCSCard* Temp = GetStackCard(Stack, GetStackLength(Stack)-1);
	Card->Copy(Temp);

	m_State.m_Stacks[Stack][GetStackLength(Stack)].EmptyCard();
	m_State.m_Stacks[Stack][0].m_Card--;
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::PushStackCardIntoStack(int DestStack, int SrcStack, int SrcCard)
{
	FCSCard* Temp = GetStackCard(SrcStack, SrcCard);
	PushCardIntoStack(DestStack, Temp);
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::PushCardIntoStack(int DestStack, FCSCard* Card)
{
	m_State.m_Stacks[DestStack][GetStackLength(DestStack)+1].Copy(Card);
	m_State.m_Stacks[DestStack][0].m_Card++;
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::PutCardInFreecell(int Freecell, FCSCard* Card)
{
	m_State.m_Freecells[Freecell].Copy(Card);
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::EmptyFreecell(int Freecell)
{
	m_State.m_Freecells[Freecell].m_Card = 0;
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::FlipStackCard(int Stack, int Card)
{
	m_State.m_Stacks[Stack][Card+1].SetFlipped(0);
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::Initialize(int NumberOfStacks)
{
	for(int a=0;a<NumberOfStacks;a++)
		m_State.m_Stacks[a] = new FCSIndirectCard[MAX_NUM_DECKS*52+1];
}


template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::CleanState()
{
	for(int s=0;s<MAX_NUM_STACKS;s++)
		if (m_State.m_Stacks[s] != NULL)
			delete [] m_State.m_Stacks[s];
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::CacheStacks(AFCSGenericStackStorage* Storage, int StackNumber)
{
	FCSIndirectCard *CacheStack;

	for (int a = 0;a<StackNumber;a++)
	{
		Realloc<FCSIndirectCard>(&m_State.m_Stacks[a], GetStackLength(a)+1, GetStackLength(a)+1);

		CacheStack = Storage->Insert(m_State.m_Stacks[a]);
		if (CacheStack != NULL)
		{
			delete [] m_State.m_Stacks[a];
			m_State.m_Stacks[a] = CacheStack;
		}
	}
}

template <class StateWithLocation>
void FCSIndirectStateWithLocations<StateWithLocation>::CanonizeState(int NumberOfFreecells, int NumberOfStacks)
{
	int b,c;
	FCSIndirectCard *TempStack,
					TempFreecell;
	char TempLocation;

	/* Insertion-sort the stacks */
	for(b=1;b<NumberOfStacks;b++)
	{
		c = b;
		while(
			(c>0) && 
			(
#if MAX_NUM_DECKS > 1
				m_State.StackCompare
#else
				m_State.StackSortCompare
#endif
				(
				c, 
				c-1
				) 
				< 0
			)
		)
		{
			TempStack = m_State.m_Stacks[c];
			m_State.m_Stacks[c] = m_State.m_Stacks[c-1];
			m_State.m_Stacks[c-1] = TempStack;

			TempLocation = m_StackLocations[c];
			m_StackLocations[c] = m_StackLocations[c-1];
			m_StackLocations[c-1] = TempLocation;

			c--;
		}
	}

	// Insertion sort the freecells
	for(b=1;b<NumberOfFreecells;b++)
	{
		c = b;
		while((c>0) && (m_State.m_Freecells[c].Compare(&(m_State.m_Freecells[c-1])) < 0) )
		{
			TempFreecell.m_Card = m_State.m_Freecells[c].m_Card;
			m_State.m_Freecells[c].m_Card = m_State.m_Freecells[c-1].m_Card;
			m_State.m_Freecells[c-1].m_Card = TempFreecell.m_Card;

			TempLocation = m_FreecellLocations[c];
			m_FreecellLocations[c] = m_FreecellLocations[c-1];
			m_FreecellLocations[c-1] = TempLocation;

			c--;
		}
	}   
}

template <class StateWithLocation>
FCSState* FCSIndirectStateWithLocations<StateWithLocation>::GetState()
{
	return &m_State;
}

#endif