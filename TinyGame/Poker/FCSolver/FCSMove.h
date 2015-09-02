#ifndef FCS_MOVE_H
#define FCS_MOVE_H

////////////////////////////////////////////////
///\file FCSMove.h
///\brief This file contains classes dealing with move objects
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCEnums.h"

///How many move stacks are allocated at a time
#define FCS_MOVE_STACK_GROW_BY 16

#ifdef max
#undef max
#endif
///find the minimum of two values
#define max(a,b) ((a)>(b)?(a):(b))

class FCSStateWithLocations;

///\brief Stores information about a move during a game
class FCSMove
{
public:
	///\brief Create a move object
	///
	///\return A pointer to the newly created move object
	static FCSMove* Create();

	///\brief Create an array of move objects
	///
	///\return A pointer to the start of the array of move objects
	static FCSMove* CreateArray(int Size);

	///\brief Copy a move object.
	///
	///\param Move is the move object to be copied
	virtual void Copy(FCSMove* Move) = 0;

	///\brief Get the source stack used in the move
	///
	///\return Return source stack of move
	virtual unsigned char GetSourceStack() = 0;

	///\brief Get the source freecell used in the move
	///
	///\return Return source freecell of move
	virtual unsigned char GetSourceFreecell() = 0;

	///\brief Get the destination stack used in the move
	///
	///\return Return destination stack of move
	virtual unsigned char GetDestStack() = 0;

	///\brief Get the destination freecell used in the move
	///
	///\return Return destination freecell of move
	virtual unsigned char GetDestFreecell() = 0;

	///\brief Get the foundation used in the move
	///
	///\return Return foundation of move
	virtual unsigned char GetFoundation() = 0;

	///\brief Get the type of the move
	///
	///\return Return the type of move
	virtual FCSMoveTypes GetType() = 0;

	///\brief Get the number of cards in a sequence that moved (from stack to stack)
	///
	///\return Return the number of cards in a sequence that moved
	virtual unsigned char GetNumberOfCardsInSequence() = 0;

	///\brief Get the number of cards that were flipped during the move
	///
	///\return Return number of cards flipped
	virtual unsigned char GetNumberOfCardsFlipped() = 0;

	///\brief Set the source stack used in the move
	///
	///\param StackNumber is the number of the source stack
	virtual void SetSourceStack(unsigned char StackNumber) = 0;

	///\brief Set the source freecell used in the move
	///
	///\param FreecellNumber is the number (position) of the freecell 
	virtual void SetSourceFreecell(unsigned char FreecellNumber) = 0;

	///\brief Set the destination stack used in the move
	///
	///\param StackNumber is the number of the destination stack
	virtual void SetDestStack(unsigned char StackNumber) = 0;
	
	///\brief Set the destination freecell used in the move
	///
	///\param FreecellNumber is the number (position) of the freecell
	virtual void SetDestFreecell(unsigned char FreecellNumber) = 0;

	///\brief Set the foundation used in the move
	///
	///\param FoundationNumber is the number (position) of the foundation
	virtual void SetFoundation(unsigned char FoundationNumber) = 0;

	///\brief Set the type of the move
	///
	///\param MoveType is the type of move
	virtual void SetType(FCSMoveTypes MoveType) = 0;

	///\brief Set the number of cards in a sequence that moved (from stack to stack)
	///
	///\param NumberOfCardsInSequence is the number of cards in a sequence that moved
	virtual void SetNumberOfCardsInSequence(unsigned char NumberOfCardsInSequence) = 0;

	///\brief Set the number of cards that were flipped during the move
	///
	///\param NumberOfCardsFlipped is the number of cards flipped in the move
	virtual void SetNumberOfCardsFlipped(unsigned char NumberOfCardsFlipped) = 0;

	///\brief Express the move as string explaining what happened during the move
	///
	///\param String is a pointer to the string
	void MoveAsString(char* String);

	///\brief Express the move as standard notation string explaining what 
	///happened during the move
	///
	///\param String is a pointer to the string
	void MoveAsStringStandardNotation(char* String);
};

///\brief This class stores a stack of moves
class FCSMoveStack
{
public:	

	///Destructor
	virtual ~FCSMoveStack();

	///\brief Convert the moves from using the canonized positions of the stacks 
	///and freecells to a user readable position.
	///\param InitialState is the state to be converted
	///\param NumberOfFreecells is the number of freecells in the card game
	///\param NumberOfStacks is the number of stacks in the card game
	///\param NumberOfDecks is the number of decks in the card game
	void Normalize(FCSStateWithLocations* InitialState, 
					int NumberOfFreecells, int NumberOfStacks, int NumberOfDecks);

	///\brief Make a new copy of the move stack
	///
	///\return A pointer to the new copy of the move stack
	virtual FCSMoveStack* Copy() = 0;

	///\brief Push a move onto the stack
	///
	///\param Move is the move being pushed onto the stack
	virtual void Push(FCSMove* Move) = 0;

	///\brief Pop a move off the stack
	///
	///\param Move is the move being popped off the stack
	///\return Return 0 if no moves were on the stack
	virtual int Pop(FCSMove** Move) = 0;

	///\brief Clear the stack
	void Reset();

	///\brief Put the contents of a stack onto another
	///
	///\param SrcStack ist he stack being swallowed
	void SwallowStack(FCSMoveStack* SrcStack);

	///\brief Get the number of moves on the stack
	///
	///\return The number of moves on the stack
	int GetNumberOfMoves();

protected:
	///Constructor
	FCSMoveStack();

	///Upper limit of the number of moves on the stack
	int m_MaxNumberOfMoves;

	///Number of moves on the stack
	int m_NumberOfMoves;
};

///\brief Create a move stack object
///
///\return A pointer to the newly created move stack object
FCSMoveStack* CreateMoveStack();

///\brief Reallocate an array of move stack pointers
///
///The normal realloc was insufficient to handle this becuase of the different
///move types
///\param Array is a pointer to the move stack pointer array that will be modified
///\param OldSize is the old size of the array
///\param NewSize is the new desired size of the array
void ReallocMoveStackArray(FCSMoveStack*** Array, int OldSize, int NewSize);


///\brief Perform a move on a given state
///
///\param State the move will happen on
///\param Move is the move being performed
///\param NumberOfFreecells is the number of freecells in the card game
///\param NumberOfStacks is the number of stacks in the card game
///\param NumberOfDecks is the number of decks in the card game
void ApplyMove(FCSStateWithLocations* StateWithLocations, FCSMove* Move, 
				int NumberOfFreecells, int NumberOfStacks, int NumberOfDecks);

#endif