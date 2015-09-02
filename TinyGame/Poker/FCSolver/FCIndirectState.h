#ifndef MMANN_INTERNAL_STATE_H
#define MMANN_INTERNAL_STATE_H

////////////////////////////////////////////////
///\file FCIndirectState.h
///\brief This file contains classes dealing with Freecell Solver Indirect States
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCState.h"
#include "FCTalonState.h"

template <class StateWithLocation> 
class FCSCompactStateWithLocations;

class FCSIndirectState;

///\brief Indirect representation of a card
class FCSIndirectCard : public FCSCard
{
public:
	///Constructor
	FCSIndirectCard();

	///Destructor
	~FCSIndirectCard();

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

	///\brief Get the size of FCSIndirectCard class
	///
	///\return Size of FCSIndirectCard
	int GetClassSize();

	///card data
	char m_Card;
};

///Indirect representation of a state
class FCSIndirectState : public FCSState
{
public:
	///Constructor
	FCSIndirectState();

	///Destructor
	virtual ~FCSIndirectState();

	///\brief Get the size of FCSIndirectState class
	///
	///\return Size of FCSIndirectState
	int GetClassSize();

	///\brief Compare two cards on the stack
	///
	///\param StackPosition1 is the stack position of the first card
	///\param StackPosition2 is the stack position of the second card
	///\return -1 if this card1 < card2, 1 if card1 > card2, 0 if card1 = Card2
	int StackCompare(int StackPosition1, int StackPosition2);

	///\brief Compare two cards on the stack for only 1 deck of cards
	///
	///\param StackPosition1 is the stack position of the first card
	///\param StackPosition2 is the stack position of the second card
	///\return -1 if this card1 < card2, 1 if card1 > card2, 0 if card1 = Card2
	int StackSortCompare(int StackPosition1, int StackPosition2);

	///\brief Compare two FCSIndirectStates
	///
	///\param State to be compared to
	///\return -1 if this state < State, 1 if state > State, 0 if state = State
	int Compare(FCSIndirectState* State);

	///\brief Copy an indirect state into an indirect state
	///
	///\param State is the indirect state to be copied
	void Copy(FCSIndirectState* State);

	///Stacks of cards in the state
	FCSIndirectCard* m_Stacks[MAX_NUM_STACKS];	

	///Freecells in the state
	FCSIndirectCard m_Freecells[MAX_NUM_FREECELLS];

	///Foundations in the state
	char m_Foundations[MAX_NUM_DECKS*4];
};


#endif