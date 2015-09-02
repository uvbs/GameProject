#ifndef MMANN_COMPACT_STATE_H
#define MMANN_COMPACT_STATE_H

////////////////////////////////////////////////
///\file FCCompactState.h
///\brief This file contains classes dealing with Freecell Solver Compact States
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCState.h"
#include "FCTalonState.h"

template <class StateWithLocation> 
class FCSCompactStateWithLocations;

/*
 * Card:
 * Bits 0-3 - Card Number
 * Bits 4-5 - Suit
 * 
 */


///\brief Compact representation of a card
class FCSCompactCard : public FCSCard
{
public:
	///\brief friend of FCSCompactCard
	///
	///It's not worth making this a template class too, so I'm including the 
	//classes currently created by the template
	friend FCSCompactStateWithLocations<FCSTalonStateWithLocations>;

	///\brief friend of FCSCompactCard
	///
	///It's not worth making this a template class too, so I'm including the 
	//classes currently created by the template
	friend FCSCompactStateWithLocations<FCSStateWithLocations>;

	///Constructor
	FCSCompactCard();

	///Destructor
	virtual ~FCSCompactCard();

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

	///\brief Get the size of FCSCompactCard class
	///
	///\return Size of FCSCompactCard
	int GetClassSize();

private:
	///card data
	char m_Card;
};

///Compact representation of a state
class FCSCompactState : public FCSState
{
public:
	///\brief Get the size of FCSCompactState class
	///
	///\return Size of FCSCompactState
	int GetClassSize();	

	///\brief Compare two cards on the stack
	///
	///\param StackPosition1 is the stack position of the first card
	///\param StackPosition2 is the stack position of the second card
	///\return -1 if this card1 < card2, 1 if card1 > card2, 0 if card1 = Card2
	int StackCompare(int StackPosition1, int StackPosition2);

	///\brief Compare two FCSCompactStates
	///
	///\param State to be compared to
	///\return -1 if this state < State, 1 if state > State, 0 if state = State
	int Compare(FCSCompactState* State);

	///State data - a large array of cards
	FCSCompactCard m_Data[MAX_NUM_STACKS*(MAX_NUM_CARDS_IN_A_STACK+1)+MAX_NUM_FREECELLS+4*MAX_NUM_DECKS];
};

#endif