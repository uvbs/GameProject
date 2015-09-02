#ifndef MMANN_FCS_COMPACT_STATE_WITH_LOCATIONS_H
#define MMANN_FCS_COMPACT_STATE_WITH_LOCATIONS_H

////////////////////////////////////////////////
///\file FCSCompactStateWithLocations.h
///\brief This file contains the FCSCompactStateWithLocations class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

/*
 * Stack: 0 - Number of cards 
 *        1-19 - Cards
 * Stacks: stack_num*20 where stack_num >= 0 and 
 *         stack_num <= (MAX_NUM_STACKS-1)
 * Bytes: (MAX_NUM_STACKS*20) to 
 *      (MAX_NUM_STACKS*20+MAX_NUM_FREECELLS-1) 
 *      are Freecells.
 * Bytes: (MAX_NUM_STACKS*20+MAX_NUM_FREECELLS) to
 *      MAX_NUM_STACKS*20+MAX_NUM_FREECELLS+3
 *      are Foundations.
 * */

/*  ===== Depracated Information =====
 * Stack: 0 - Number of cards 1-19 - Cards
 * Stacks: stack_num*20 where stack_num >= 0 and stack_num <= 7
 * Bytes 160-163 - Freecells
 * Bytes 164-167 - Decks
 */

//class FCSCompactState;
#include "FCCompactState.h"


///Offset of the freecells in the array of cards
#define FCS_FREECELLS_OFFSET (MAX_NUM_STACKS*(MAX_NUM_CARDS_IN_A_STACK+1))
///Offset of the foundataions in the array of cards
#define FCS_FOUNDATIONS_OFFSET (MAX_NUM_STACKS*(MAX_NUM_CARDS_IN_A_STACK+1)+MAX_NUM_FREECELLS) 

///\brief Abstract class representing a compact state in a card game with additional data 
///to process and store it
template <class StateWithLocation>
class FCSCompactStateWithLocations : public StateWithLocation
{
public:

	///Constructor
	FCSCompactStateWithLocations();

	///Destructor
	virtual ~FCSCompactStateWithLocations();

	///\brief Copy FCSCompactStateWithLocations object
	///
	///\param State is the FCSCompactStateWithLocations to be copied
	virtual void Copy(FCSStateWithLocations* State);

	///\brief Compare a FCSCompactStateWithLocations to another
	///
	///\param State to be compared to
	///\return -1 if this FCSCompactStateWithLocations < State, 1 if FCSCompactStateWithLocations > State, 
	///0 if FCSCompactStateWithLocations = State
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

	///\brief Canonize the state - put it into the internal representation
	///
	///\param NumberOfFreecells is the number of freecells in the card game
	///\param NumberOfStacks is the number of stacks in the card game
	void CanonizeState(int NumberOfFreecells, int NumberOfStacks);

	///\brief Get a pointer to the compact state
	///
	///\return A pointer to the compact state at this location
	FCSState* GetState();

protected:
	///State in compact form
	FCSCompactState  m_State;
};

template <class StateWithLocation>
FCSCompactStateWithLocations<StateWithLocation>::FCSCompactStateWithLocations() : StateWithLocation()
{
}

template <class StateWithLocation>
FCSCompactStateWithLocations<StateWithLocation>::~FCSCompactStateWithLocations()
{
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::Copy(FCSStateWithLocations* State)
{
	StateWithLocation::Copy(State);

	m_State = ((FCSCompactStateWithLocations*)State)->m_State;
}

template <class StateWithLocation>
int FCSCompactStateWithLocations<StateWithLocation>::Compare(const FCSStateWithLocations *State)
{
	return m_State.Compare(&(((FCSCompactStateWithLocations*)State)->m_State));
}

template <class StateWithLocation>
FCSCard* FCSCompactStateWithLocations<StateWithLocation>::GetStackCard(int Stack, int Card)
{
	return &(m_State.m_Data[Stack*(MAX_NUM_CARDS_IN_A_STACK+1)+Card+1]);
}

template <class StateWithLocation>
char FCSCompactStateWithLocations<StateWithLocation>::GetStackCardSuit(int Stack, int Card)
{
	return m_State.m_Data[Stack*(MAX_NUM_CARDS_IN_A_STACK+1)+Card+1].GetSuit();
}

template <class StateWithLocation>
char FCSCompactStateWithLocations<StateWithLocation>::GetStackCardNumber(int Stack, int Card)
{
	return m_State.m_Data[Stack*(MAX_NUM_CARDS_IN_A_STACK+1)+Card+1].GetCardNumber();
}

template <class StateWithLocation>
int FCSCompactStateWithLocations<StateWithLocation>::GetStackLength(int Stack)
{
	return (int)m_State.m_Data[Stack*(MAX_NUM_CARDS_IN_A_STACK+1)].m_Card;
}

template <class StateWithLocation>
FCSCard* FCSCompactStateWithLocations<StateWithLocation>::GetFreecellCard(int Freecell)
{
	return &(m_State.m_Data[FCS_FREECELLS_OFFSET + Freecell]);
}

template <class StateWithLocation>
char FCSCompactStateWithLocations<StateWithLocation>::GetFreecellCardSuit(int Freecell)
{
	return m_State.m_Data[FCS_FREECELLS_OFFSET + Freecell].GetSuit();
}

template <class StateWithLocation>
char FCSCompactStateWithLocations<StateWithLocation>::GetFreecellCardNumber(int Freecell)
{
	return m_State.m_Data[FCS_FREECELLS_OFFSET + Freecell].GetCardNumber();
}

template <class StateWithLocation>
char FCSCompactStateWithLocations<StateWithLocation>::GetFoundation(int Foundation)
{
	return m_State.m_Data[FCS_FOUNDATIONS_OFFSET + Foundation].m_Card;
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::IncrementFoundation(int Foundation)
{
	m_State.m_Data[FCS_FOUNDATIONS_OFFSET + Foundation].m_Card++;
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::SetFoundation(int Foundation, char Value)
{
	m_State.m_Data[FCS_FOUNDATIONS_OFFSET + Foundation].m_Card = Value;
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::PopStackCard(int Stack, FCSCard* Card)
{
	FCSCard* Temp = GetStackCard(Stack, GetStackLength(Stack)-1);
	Card->Copy(Temp);

	m_State.m_Data[Stack*(MAX_NUM_CARDS_IN_A_STACK+1)+1+GetStackLength(Stack)-1].EmptyCard();
	m_State.m_Data[Stack*(MAX_NUM_CARDS_IN_A_STACK+1)].m_Card--;
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::PushStackCardIntoStack(int DestStack, int SrcStack, int SrcCard)
{
	FCSCard* Temp = GetStackCard(SrcStack, SrcCard);
	PushCardIntoStack(DestStack, Temp);
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::PushCardIntoStack(int DestStack, FCSCard* Card)
{
	m_State.m_Data[DestStack*(MAX_NUM_CARDS_IN_A_STACK+1)+1+GetStackLength(DestStack)].Copy(Card);
	m_State.m_Data[DestStack*(MAX_NUM_CARDS_IN_A_STACK+1)].m_Card++;
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::PutCardInFreecell(int Freecell, FCSCard* Card)
{
	m_State.m_Data[FCS_FREECELLS_OFFSET+Freecell].Copy(Card);
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::EmptyFreecell(int Freecell)
{
	m_State.m_Data[FCS_FREECELLS_OFFSET+Freecell].m_Card = 0;
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::FlipStackCard(int Stack, int Card)
{
	m_State.m_Data[Stack*(MAX_NUM_CARDS_IN_A_STACK+1)+Card+1].SetFlipped(0);
}

template <class StateWithLocation>
void FCSCompactStateWithLocations<StateWithLocation>::CanonizeState(int NumberOfFreecells, int NumberOfStacks)
{
	int b,c;

	FCSCompactCard TempStack[(MAX_NUM_CARDS_IN_A_STACK+1)];
	FCSCompactCard TempFeecell;
	char TempLocation;

	/* Insertion-sort the stacks */

	for(b=1;b<NumberOfStacks;b++)
	{
		c = b;
		while((c>0) && (m_State.StackCompare(c*(MAX_NUM_CARDS_IN_A_STACK+1), (c-1)*(MAX_NUM_CARDS_IN_A_STACK+1)) < 0))
		{
			for (int a1 = 0;a1<(MAX_NUM_CARDS_IN_A_STACK+1);a1++)
				TempStack[a1] = m_State.m_Data[a1+c*(MAX_NUM_CARDS_IN_A_STACK+1)];

			for (int a2 = 0;a2<(MAX_NUM_CARDS_IN_A_STACK+1);a2++)
				m_State.m_Data[a2+c*(MAX_NUM_CARDS_IN_A_STACK+1)] = m_State.m_Data[a2+ (c-1)*(MAX_NUM_CARDS_IN_A_STACK+1)];

			for (int a3 = 0;a3<(MAX_NUM_CARDS_IN_A_STACK+1);a3++)
				m_State.m_Data[a3+(c-1)*(MAX_NUM_CARDS_IN_A_STACK+1)] = TempStack[a3];

			TempLocation = m_StackLocations[c];
			m_StackLocations[c] = m_StackLocations[c-1];
			m_StackLocations[c-1] = TempLocation;

			c--;
		}
	}

	// Insertion-sort the freecells
	for(b=1;b<NumberOfFreecells;b++)
	{
		c = b;

		while( (c>0) && (m_State.m_Data[FCS_FREECELLS_OFFSET+c].Compare(&(m_State.m_Data[FCS_FREECELLS_OFFSET+c-1])) < 0) )
		{
			TempFeecell = m_State.m_Data[FCS_FREECELLS_OFFSET+c];
			m_State.m_Data[FCS_FREECELLS_OFFSET+c] = m_State.m_Data[FCS_FREECELLS_OFFSET+c-1];
			m_State.m_Data[FCS_FREECELLS_OFFSET+c-1] = TempFeecell;

			TempLocation = m_FreecellLocations[c];
			m_FreecellLocations[c] = m_FreecellLocations[c-1];
			m_FreecellLocations[c-1] = TempLocation;

			c--;
		}
	}
}

template <class StateWithLocation>
FCSState* FCSCompactStateWithLocations<StateWithLocation>::GetState()
{
	return &m_State;
}

#endif