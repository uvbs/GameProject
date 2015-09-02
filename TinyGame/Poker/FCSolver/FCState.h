#ifndef MMANN_FCSTATE_H
#define MMANN_FCSTATE_H

////////////////////////////////////////////////
///\file FCState.h
///\brief This file contains the base freecell state classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCEnums.h"
#include "FCSMove.h"
#include "FCSCard.h"
#include "Config.h"
#include "FCSDebugDisplayInfo.h"
#include "HashAlgorithms.h"
#include "FCSStateWithLocationsMatrix.h"

//forward declaration
class AFCSGenericStackStorage;

///\brief Abstract class representing a state in a card game
class FCSState
{
public:
	///Destructor
	virtual ~FCSState();

	///\brief Get the size of FCSState child class
	///
	///\return Size of state class
	virtual int GetClassSize() = 0;

protected:
	///Constructor
	FCSState();
};

///\brief Abstract class representing a state in a card game with additional data 
///to process and store it
class FCSStateWithLocations
{
public:	

	///Destructor
	virtual ~FCSStateWithLocations();

	///\brief Copy StateWithLocations object
	///
	///\param State is the StateWithLocations to be copied
	virtual void Copy(FCSStateWithLocations* State);

	///\brief Compare a FCSStateWithLocations to another
	///
	///\param State to be compared to
	///\return -1 if this FCSStateWithLocations < State, 1 if FCSStateWithLocations > State, 
	///0 if FCSStateWithLocations = State
	virtual int Compare(const FCSStateWithLocations* State) = 0;

	///\brief Convert a string representation of a state into a FCSStateWithLocations object
	///
	///\param String is the string to be converted
	///\param NumberOfFreecells is the number of freecells in the card game
	///\param NumberOfStacks is the number of stacks in the card game
	///\param NumberOfDecks is the number of decks in the card game
	///\return A pointer to the newly created FCSStateWithLocations object
	static FCSStateWithLocations* CreateInitialState(const char *String, 
													int NumberOfFreecells, int NumberOfStacks, int NumberOfDecks);

	///\brief Represent a FCSStateWithLocations object as a string
	///
	///\param DebugInfo contains debug information that can be included in the string
	///\param String is the pointer to the string
	virtual void StateAsString(char* String, FCSDebugDisplayInfo* DebugInfo);

	///\brief Validiate that this state is valid
	///
	///\param NumberOfFreecells is the number of freecells in the card game
	///\param NumberOfStacks is the number of stacks in the card game
	///\param NumberOfDecks is the number of decks in the card game
	///\param MisplacedCard will contain a misplaced card if there is one
	///\param TalonType is the talon type of the game if applicable
	///\return Returns 0 if state is valid, non-zero if state is invalid
	virtual int CheckStateValidity(int NumberOfFreecells, int NumberOfStacks, 
									int NumberOfDecks, FCSCard** MisplacedCard, FCSTalonType TalonType);

	///\brief Get a card from the state
	///
	///\param Stack the card is in
	///\param Card is the position of the card
	///\return A pointer to the card
	virtual FCSCard* GetStackCard(int Stack, int Card) = 0;

	///\brief Get a suit from a card in the state
	///
	///\param Stack the card is in
	///\param Card is the position of the card
	///\return The card's suit
	virtual char GetStackCardSuit(int Stack, int Card) = 0;

	///\brief Get a card number from a card in the state
	///
	///\param Stack the card is in
	///\param Card is the position of the card
	///\return The card's number
	virtual char GetStackCardNumber(int Stack, int Card) = 0;

	///\brief Get the number of cards in the stack
	///
	///\param Stack is the stack to get the length from
	///\return The number of cards in the stack
	virtual int GetStackLength(int Stack) = 0;

	///\brief Get a freecell card from the state
	///
	///\param Freecell the card is in
	///\return A pointer to the card
	virtual FCSCard* GetFreecellCard(int Freecell) = 0;

	///\brief Get a suit from a freecell card in the state
	///
	///\param Freecell the card is in
	///\return The card's suit
	virtual char GetFreecellCardSuit(int Freecell) = 0;

	///\brief Get a card number from a freecell card in the state
	///
	///\param Freecell the card is in
	///\return The card's number
	virtual char GetFreecellCardNumber(int Freecell) = 0;

	///\brief Get a card number from a foundation in the state
	///
	///\param Foundation the card is in
	///\return The card's number
	virtual char GetFoundation(int Foundation) = 0;

	///\brief Add the next card to a foundation in the state
	///
	///\param Foundation the card is in
	virtual void IncrementFoundation(int Foundation) = 0;

	///\brief Put a card into a foundation in the state
	///
	///\param Foundation the card is in
	///\param Value is the card number
	virtual void SetFoundation(int Foundation, char Value) = 0;

	///\brief Take a card off a stack in the state
	///
	///\param Stack the card is in
	///\param Card a pointer to the card being removed
	virtual void PopStackCard(int Stack, FCSCard* Card) = 0;

	///\brief Move a card from one stack to another
	///
	///\param DestStack is the stack the card is going to
	///\param SrcStack is the stack the card is coming from
	///\param SrcCard is the position of the card in the source stack
	virtual void PushStackCardIntoStack(int DestStack, int SrcStack, int SrcCard) = 0;

	///\brief Move a card onto a stack
	///
	///\param DestStack is the stack the card is going to
	///\param Card a pointer to the card being added
	virtual void PushCardIntoStack(int DestStack, FCSCard* Card) = 0;

	///\brief Move a card onto a freecll
	///
	///\param Freecell the card is going to be put in
	///\param Card a pointer to the card being added
	virtual void PutCardInFreecell(int Freecell, FCSCard* Card) = 0;

	///\brief Clear a freecell
	///
	///\param Freecell to be cleared
	virtual void EmptyFreecell(int Freecell) = 0;

	///\brief Make a stack card flipped over (visible)
	///
	///\param Stack the card is in
	///\param Card is the position of the card
	virtual void FlipStackCard(int Stack, int Card) = 0;

	///\brief Clear the state of data
	virtual void CleanState();

	///\brief Cache the stacks of the state
	///
	///This is only implemented in the indirect state classes
	virtual void CacheStacks(AFCSGenericStackStorage* Storage, int StackNumber);

	///\brief Initialize the state of stacks (and talons if applicable)
	virtual void Initialize(int NumberOfStacks);

	///\brief Canonize the state - put it into the internal representation
	///
	///\param NumberOfFreecells is the number of freecells in the card game
	///\param NumberOfStacks is the number of stacks in the card game
	virtual void CanonizeState(int NumberOfFreecells, int NumberOfStacks) = 0;

	///\brief Get a pointer to the state
	///
	///\return A pointer to the state at this location
	virtual FCSState* GetState() = 0;

	///State of game before this state
	FCSStateWithLocations* m_Parent;
	///How to arrive at this state from parent
	FCSMoveStack* m_MovesToParent;
	///Number of states in the path to this state
	int m_Depth;
	///How this state has been visited
	int m_Visited;
	///Number of states looked through so far
	int m_VisitIterations;
	///Canonized location of the stacks of cards
	char m_StackLocations[MAX_NUM_STACKS];
	///Canonized location of the freecell cards
	char m_FreecellLocations[MAX_NUM_FREECELLS];

protected:
	///Constructor
	FCSStateWithLocations();
};

///\brief A simple linked list class for FCSStateWithLocations
class FCSStateWithLocationsLinkedList
{
public:
	///Data of the linked list
	FCSStateWithLocations* m_State;

	///Pointer to the next item in the list
	FCSStateWithLocationsLinkedList* m_Next;
};

///\brief Get the size of the child FCSStateWithLocations class
///
///\return Size of the child FCSStateWithLocations class
int GetFCSStateWithLocationsClassSize();

///\brief Create a FCSStateWithLocations object
///
///\param NumberOfStacks is the number of stack in the state (used for indirect states)
///\return Pointer to the newly created FCSStateWithLocations object
FCSStateWithLocations* CreateStateWithLocations();

///\brief Create a AFCSStateWithLocationsMatrix object
///
///\param Size is the number of FCSStateWithLocations pointers to create
///\return Pointer to the newly created array
AFCSStateWithLocationsMatrix* CreateStateWithLocationsMatrix(int Size);

///\brief Reallocate an array of FCSStateWithLocations pointers
///
///The normal realloc was insufficient to handle this becuase of the different
///FCSStateWithLocations types
///\param Array is a pointer to the FCSStateWithLocations pointer array that will be modified
///\param OldSize is the old size of the array
///\param NewSize is the new desired size of the array
void ReallocStateWithLocationsArray(FCSStateWithLocations*** Array, int OldSize, int NewSize);

///\brief Convert a string representation of a state into a FCSStateWithLocations object
///
///\param String is the string to be converted
///\param NumberOfFreecells is the number of freecells in the card game
///\param NumberOfStacks is the number of stacks in the card game
///\param NumberOfDecks is the number of decks in the card game
///\param TalonType is the talon type for the card game (if applicable)
///\return A pointer to the newly created FCSStateWithLocations object
FCSStateWithLocations* CreateInitialState(const char *String, int NumberOfFreecells, 
										  int NumberOfStacks, int NumberOfDecks, FCSTalonType TalonType);

///\brief Hash algorithm specifically for FCSStateWithLocation classes
///
///This hash algorithm uses the MD5 hash algorithm
///to handle the dynamic allocation of the FCSStateWithLocation classes
class MD5StateWithLocationsHashAlgorithm : public MD5HashAlgorithm<FCSStateWithLocations>
{
public:
	///\brief Hash function
	///\param key is the value to be hashed
	///\return Returns hash value
	virtual int Hash(const FCSStateWithLocations* key);
};

extern FCStateType GlobalStateType;

#endif