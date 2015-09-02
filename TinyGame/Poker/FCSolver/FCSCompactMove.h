#ifndef FCS_COMPACT_MOVE_H
#define FCS_COMPACT_MOVE_H

////////////////////////////////////////////////
///\file FCSCompactMove.h
///\brief This file contains classes dealing with compact move objects
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSMove.h"

///Position of the move type in the array
#define FCS_MOVE_TYPE				0
///Position of the source stack in the array
#define FCS_MOVE_SRC				1
///Position of the destination stack in the array
#define FCS_MOVE_DEST				2
///Position of the number of cards in the sequence in the array
#define FCS_MOVE_NUM_CARDS_IN_SEQ	3
///Position of the number of cards flipped in the array
#define FCS_MOVE_NUM_CARDS_FLIPPED	3

///\brief Stores information about a move during a game
///
///This is the compact form of a move
class FCSCompactMove : public FCSMove
{
public:
	///Constructor
	FCSCompactMove();
	
	///Destructor
	virtual ~FCSCompactMove();

	///\brief Copy a compact move object.
	///
	///\param Move is the compact move object to be copied
	virtual void Copy(FCSMove* Move);

	///\brief Get the source stack used in the move
	///
	///\return Return source stack of move
	unsigned char GetSourceStack();

	///\brief Get the source freecell used in the move
	///
	///\return Return source freecell of move
	unsigned char GetSourceFreecell();

	///\brief Get the destination stack used in the move
	///
	///\return Return destination stack of move
	unsigned char GetDestStack();

	///\brief Get the destination freecell used in the move
	///
	///\return Return destination freecell of move
	unsigned char GetDestFreecell();

	///\brief Get the foundation used in the move
	///
	///\return Return foundation of move
	unsigned char GetFoundation();

	///\brief Get the type of the move
	///
	///\return Return the type of move
	FCSMoveTypes GetType();

	///\brief Get the number of cards in a sequence that moved (from stack to stack)
	///
	///\return Return the number of cards in a sequence that moved
	unsigned char GetNumberOfCardsInSequence();

	///\brief Get the number of cards that were flipped during the move
	///
	///\return Return number of cards flipped
	unsigned char GetNumberOfCardsFlipped();

	///\brief Set the source stack used in the move
	///
	///\param StackNumber is the number of the source stack
	void SetSourceStack(unsigned char StackNumber);

	///\brief Set the source freecell used in the move
	///
	///\param FreecellNumber is the number (position) of the freecell 
	void SetSourceFreecell(unsigned char FreecellNumber);

	///\brief Set the destination stack used in the move
	///
	///\param StackNumber is the number of the destination stack
	void SetDestStack(unsigned char StackNumber);
	
	///\brief Set the destination freecell used in the move
	///
	///\param FreecellNumber is the number (position) of the freecell
	void SetDestFreecell(unsigned char FreecellNumber);

	///\brief Set the foundation used in the move
	///
	///\param FoundationNumber is the number (position) of the foundation
	void SetFoundation(unsigned char FoundationNumber);

	///\brief Set the type of the move
	///
	///\param MoveType is the type of move
	void SetType(FCSMoveTypes MoveType);

	///\brief Set the number of cards in a sequence that moved (from stack to stack)
	///
	///\param NumberOfCardsInSequence is the number of cards in a sequence that moved
	void SetNumberOfCardsInSequence(unsigned char NumberOfCardsInSequence);

	///\brief Set the number of cards that were flipped during the move
	///
	///\param NumberOfCardsFlipped is the number of cards flipped in the move
	void SetNumberOfCardsFlipped(unsigned char NumberOfCardsFlipped);

protected:
	///data about the move
	unsigned char m_Data[4];
};

///\brief This class stores a stack of moves
///
///This is the compact form of a move stack
class FCSCompactMoveStack : public FCSMoveStack
{
public:
	///Constructor
	FCSCompactMoveStack();

	///Destructor
	virtual ~FCSCompactMoveStack();

	///\brief Make a new copy of the move stack
	///
	///\return A pointer to the new copy of the move stack
	FCSMoveStack* Copy();

	///\brief Push a move onto the stack
	///
	///\param Move is the move being pushed onto the stack
	void Push(FCSMove* Move);

	///\brief Pop a move off the stack
	///
	///\param Move is the move being popped off the stack
	///\return Return 0 if no moves were on the stack
	int Pop(FCSMove** Move);

protected:
	///The moves on the stack
	FCSCompactMove* m_Moves;
};


#endif

