#ifndef FCS_DEBUG_MOVE_H
#define FCS_DEBUG_MOVE_H

////////////////////////////////////////////////
///\file FCSDebugMove.h
///\brief This file contains classes dealing with debug move objects
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSMove.h"

///\brief Stores information about a move during a game
///
///This is the debug form of a move
class FCSDebugMove : public FCSMove
{
public:
	///Constructor
	FCSDebugMove();

	///Destructor
	virtual ~FCSDebugMove();

	///\brief Copy a debug move object.
	///
	///\param Move is the debug move object to be copied
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

private:
	/// The index of the foundation, in case there are more than one decks
	unsigned char m_Foundation;

	/// Number of cards in a sequence that moved (from stack to stack)
	unsigned char m_NumberOfCardsInSequence; 

	/// Number of cards that were flipped during the move
	unsigned char m_NumberOfCardsFlipped;

	/// Source freecell of the move
	unsigned char m_SourceFreecell;
	
	///Destination freecell of the move
	unsigned char m_DestinationFreecell;

	/// Source stack of the move
	unsigned char m_SourceStack;

	/// Destination stack of the move
	unsigned char m_DestinationStack;

	/// The type of the move
	FCSMoveTypes m_Type;
};

///\brief This class stores a stack of moves
///
///This is the debug form of a move stack
class FCSDebugMoveStack : public FCSMoveStack
{
public:
	///Constructor
	FCSDebugMoveStack();

	///Destructor
	virtual ~FCSDebugMoveStack();

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
	///Moves in the stack
	FCSDebugMove* m_Moves;
};


#endif