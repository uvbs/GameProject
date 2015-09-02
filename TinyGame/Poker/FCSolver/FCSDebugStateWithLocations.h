#ifndef MMANN_FCS_DEBUG_STATE_WITH_LOCATIONS_H
#define MMANN_FCS_DEBUG_STATE_WITH_LOCATIONS_H

////////////////////////////////////////////////
///\file FCSDebugStateWithLocations.h
///\brief This file contains the FCSDebugStateWithLocations class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

class FCSDebugState;

///\brief Abstract class representing a debug state in a card game with additional data 
///to process and store it
template <class StateWithLocation>
class FCSDebugStateWithLocations : public StateWithLocation
{
public:
	///Constructor
	FCSDebugStateWithLocations();

	///Destructor
	virtual ~FCSDebugStateWithLocations();

	///\brief Copy FCSDebugStateWithLocations object
	///
	///\param State is the FCSDebugStateWithLocations to be copied
	virtual void Copy(FCSStateWithLocations* State);

	///\brief Compare a FCSDebugStateWithLocations to another
	///
	///\param State to be compared to
	///\return -1 if this FCSDebugStateWithLocations < State, 1 if FCSDebugStateWithLocations > State, 
	///0 if FCSDebugStateWithLocations = State
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

	///\brief Get a pointer to the debug state
	///
	///\return A pointer to the debug state at this location
	FCSState* GetState();

protected:
	///State in debug form
	FCSDebugState  m_State;
};

template <class StateWithLocation>
FCSDebugStateWithLocations<StateWithLocation>::FCSDebugStateWithLocations() : StateWithLocation()
{
}

template <class StateWithLocation>
FCSDebugStateWithLocations<StateWithLocation>::~FCSDebugStateWithLocations()
{
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::Copy(FCSStateWithLocations* State)
{
	StateWithLocation::Copy(State);

	m_State = ((FCSDebugStateWithLocations*)State)->m_State;
}

template <class StateWithLocation>
int FCSDebugStateWithLocations<StateWithLocation>::Compare(const FCSStateWithLocations *State)
{
	return m_State.Compare(&(((FCSDebugStateWithLocations*)State)->m_State));
}

template <class StateWithLocation>
FCSCard* FCSDebugStateWithLocations<StateWithLocation>::GetStackCard(int Stack, int Card)
{
	return &(m_State.m_Stacks[Stack].m_Cards[Card]);
}

template <class StateWithLocation>
char FCSDebugStateWithLocations<StateWithLocation>::GetStackCardSuit(int Stack, int Card)
{
	return m_State.m_Stacks[Stack].m_Cards[Card].GetSuit();
}

template <class StateWithLocation>
char FCSDebugStateWithLocations<StateWithLocation>::GetStackCardNumber(int Stack, int Card)
{
	return m_State.m_Stacks[Stack].m_Cards[Card].GetCardNumber();
}

template <class StateWithLocation>
int FCSDebugStateWithLocations<StateWithLocation>::GetStackLength(int Stack)
{
	return m_State.m_Stacks[Stack].m_NumberOfCards;
}

template <class StateWithLocation>
FCSCard* FCSDebugStateWithLocations<StateWithLocation>::GetFreecellCard(int Freecell)
{
	return &(m_State.m_Freecells[Freecell]);
}

template <class StateWithLocation>
char FCSDebugStateWithLocations<StateWithLocation>::GetFreecellCardSuit(int Freecell)
{
	return m_State.m_Freecells[Freecell].GetSuit();
}

template <class StateWithLocation>
char FCSDebugStateWithLocations<StateWithLocation>::GetFreecellCardNumber(int Freecell)
{
	return m_State.m_Freecells[Freecell].GetCardNumber();
}

template <class StateWithLocation>
char FCSDebugStateWithLocations<StateWithLocation>::GetFoundation(int Foundation)
{
	return m_State.m_Foundations[Foundation];
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::IncrementFoundation(int Foundation)
{
	m_State.m_Foundations[Foundation]++;
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::SetFoundation(int Foundation, char Value)
{
	m_State.m_Foundations[Foundation] = Value;
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::PopStackCard(int Stack, FCSCard* Card)
{
	Card->Copy(&m_State.m_Stacks[Stack].m_Cards[m_State.m_Stacks[Stack].m_NumberOfCards-1]);
	m_State.m_Stacks[Stack].m_Cards[m_State.m_Stacks[Stack].m_NumberOfCards-1].EmptyCard();
	m_State.m_Stacks[Stack].m_NumberOfCards--;
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::PushStackCardIntoStack(int DestStack, int SrcStack, int SrcCard)
{
	m_State.m_Stacks[DestStack].m_Cards[m_State.m_Stacks[DestStack].m_NumberOfCards].Copy(&m_State.m_Stacks[SrcStack].m_Cards[SrcCard]);
	m_State.m_Stacks[DestStack].m_NumberOfCards++;
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::PushCardIntoStack(int DestStack, FCSCard* Card)
{
	m_State.m_Stacks[DestStack].m_Cards[m_State.m_Stacks[DestStack].m_NumberOfCards].Copy(Card);
	m_State.m_Stacks[DestStack].m_NumberOfCards++;
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::PutCardInFreecell(int Freecell, FCSCard* Card)
{
	m_State.m_Freecells[Freecell].Copy(Card);
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::EmptyFreecell(int Freecell)
{
	m_State.m_Freecells[Freecell].m_CardNumber = 0;
	m_State.m_Freecells[Freecell].m_Suit = 0;
	m_State.m_Freecells[Freecell].m_Flags = 0;
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::FlipStackCard(int Stack, int Card)
{
	m_State.m_Stacks[Stack].m_Cards[Card].SetFlipped(0);
}

template <class StateWithLocation>
void FCSDebugStateWithLocations<StateWithLocation>::CanonizeState(int NumberOfFreecells, int NumberOfStacks)
{
	int b,c;

	FCSDebugStack TempStack;
	FCSDebugCard TempFeecell;
	char TempLocation;

	/* Insertion-sort the stacks */
	for(b=1;b<NumberOfStacks;b++)
	{
		c = b;
		while((c>0) && (m_State.StackCompare(c, c-1) < 0))
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
		while((c>0) && (m_State.m_Freecells[c].Compare(&m_State.m_Freecells[c-1]) < 0))
		{
			TempFeecell = m_State.m_Freecells[c];
			m_State.m_Freecells[c] = m_State.m_Freecells[c-1];
			m_State.m_Freecells[c-1] = TempFeecell;

			TempLocation = m_FreecellLocations[c];
			m_FreecellLocations[c] = m_FreecellLocations[c-1];
			m_FreecellLocations[c-1] = TempLocation;

			c--;
		}
	}   
}

template <class StateWithLocation>
FCSState* FCSDebugStateWithLocations<StateWithLocation>::GetState()
{
	return &m_State;
}

#endif
