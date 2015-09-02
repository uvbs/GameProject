#ifndef MMANN_DEBUG_STATE_H
#define MMANN_DEBUG_STATE_H

////////////////////////////////////////////////
///\file FCDebugState.h
///\brief This file contains classes dealing with Freecell Solver Debug States
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCState.h"
#include "FCTalonState.h"

template <class StateWithLocation> 
class FCSDebugStateWithLocations;

///\brief Debug representation of a card
class FCSDebugCard : public FCSCard
{
public:

	///\brief friend of FCSDebugCard
	///
	///It's not worth making this a template class too, so I'm including the 
	//classes currently created by the template
	friend FCSDebugStateWithLocations<FCSTalonStateWithLocations>;

	///\brief friend of FCSDebugCard
	///
	///It's not worth making this a template class too, so I'm including the 
	//classes currently created by the template
	friend FCSDebugStateWithLocations<FCSStateWithLocations>;

	///Constructor
	FCSDebugCard();

	///Destructor
	virtual ~FCSDebugCard();

	///\brief Copy the value of a card into a card
	///
	///\param Card is the card to be copied
	void Copy(FCSCard* Card);

	///\brief Make card have no value
	void EmptyCard();

	///\brief Determines whether or not card is empty
	bool IsEmptyCard();

	///\brief Retrieves card number
	///
	///\return Card Number of card
	char GetCardNumber();

	///\brief Retrieves suit
	///
	///\return Suit of card
	char GetSuit();

	///\brief Retrieves whether or not card is flipped (visible)
	///
	///\return Visibility of card
	char GetFlipped();

	///\brief Set suit of card
	///
	///\param New suit of card
	void SetSuit(char Suit);

	///\brief Set card number of card
	///
	///\param New card number of card
	void SetCardNumber(char CardNumber);

	///\brief Set flippedness (visibility) of card
	///
	///\param New visibility of card
	void SetFlipped(char Flipped);

	///\brief Get the size of FCSDebugCard class
	///
	///\return Size of FCSDebugCard
	int GetClassSize();

private:
	///Card number
	char m_CardNumber;

	///Card suit
	char m_Suit;

	///Any flags that may apply to the card (e.g. flipped)
	char m_Flags;
};

///\brief Debug representation of a stack of card
class FCSDebugStack
{
public:
	///Constructor
	FCSDebugStack();

	///\brief Compare a stack to another
	///
	///\param Stack to be compared to
	///\return -1 if this stack < Stack, 1 if stack > Stack, 0 if stack = Stack
	int Compare(FCSDebugStack* Stack);

	///Number of cards in the stack
	int m_NumberOfCards;

	///Cards in the stack
	FCSDebugCard m_Cards[MAX_NUM_CARDS_IN_A_STACK];
};

///Debug representation of a state
class FCSDebugState : public FCSState
{
public:
	///Constructor
	FCSDebugState();

	///\return Size of FCSDebugState
	int GetClassSize();	

	///\brief Compare two cards on the stack
	///
	///\param StackPosition1 is the stack position of the first card
	///\param StackPosition2 is the stack position of the second card
	///\return -1 if this card1 < card2, 1 if card1 > card2, 0 if card1 = Card2
	int StackCompare(int StackPosition1, int StackPosition2);

	///\brief Compare two FCSDebugStates
	///
	///\param State to be compared to
	///\return -1 if this state < State, 1 if state > State, 0 if state = State	
	int Compare(FCSDebugState* State);

	///Stacks of cards in the state
	FCSDebugStack m_Stacks[MAX_NUM_STACKS];
	
	///Freecells in the state
	FCSDebugCard m_Freecells[MAX_NUM_FREECELLS];
	
	///Foundations in the state
	char m_Foundations[MAX_NUM_DECKS*4];
};


#endif