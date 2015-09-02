#ifndef MMANN_FCTALONSTATE_H
#define MMANN_FCTALONSTATE_H

////////////////////////////////////////////////
///\file FCTalonState.h
///\brief This file contains the base freecell state classes that have talons
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCState.h"
#include "FCEnums.h"
#include "CompareAlgorithms.h"
#include "HashAlgorithms.h"
#include "AGenericTree.h"
#include "FCInternalHash.h"

class MD5TalonStateDataHashAlgorithm;

///Position of the talon parameter length
#define FC_TALON_PARAM_LENGTH							0
///Position of the talon parameter position
#define	FC_TALON_PARAM_POSITION							1
///Position of the talon parameter klondike stack position
#define FC_TALON_PARAM_KLONDIKE_STACK_POSITION			0
///Position of the talon parameter klondike queue position
#define FC_TALON_PARAM_KLONDIKE_QUEUE_POSITION			1
///Position of the talon parameter klondike number of redeals left
#define FC_TALON_PARAM_KLONDIKE_NUMBER_OF_REDEALS_LEFT	2
///Size of talon parameter array
#define FC_TALON_NUMBER_OF_TALON_PARAMETERS				3

///Talon data associated with a state
class FCSTalonStateData
{
public:
	///Constructor
	FCSTalonStateData();

	///Destructor
	virtual ~FCSTalonStateData();

	friend class MD5TalonStateDataHashAlgorithm;

	///\brief Copy FCSTalonStateData object
	///
	///\param Data is the FCSTalonStateData to be copied
	virtual void Copy(FCSTalonStateData* Data);

	///\brief Compare data of two talon states
	///
	///\return -1 if this talon data < TalonData, 1 if talon data > TalonData, 0 if talon data = TalonData
	virtual int Compare(const FCSTalonStateData* TalonData);

	///\brief Set the number of klondike redeals left
	///
	///\param RedealsLeft is the number of redeals left
	void SetKlondikeNumberOfRedealsLeft(char RedealsLeft);

	///\brief Get the number of klondike redeals left
	///
	///\return The number of redeals left
	char GetKlondikeNumberOfRedealsLeft();

	///\brief Set the talon position
	///
	///\param Position is the new talon position
	void SetTalonPosition(char Position);

	///\brief Get the talon position
	///
	///\return Position of the talon
	char GetTalonPosition();
	
	///\brief Set the number of talons
	///
	///\param Length is the number of talons
	void SetTalonLength(char Length);

	///\brief Get the number of talons
	///
	///\return Number of talons
	char GetTalonLength();

	///\brief Set the number of talons
	///
	///\param Length is the number of talons
	void SetKlondikeTalonLength(char Length);

	///\brief Get the number of talons
	///
	///\return the number of talons
	char GetKlondikeTalonLength();

	///\brief Set the talon klondike stack position
	///
	///\param Position to set the talon klondike stack
	void SetKlondikeTalonStackPosition(char Position);

	///\brief Get the talon klondike stack position
	///
	///\return Position of the talon klondike stack
	char GetKlondikeTalonStackPosition();

	///\brief Set the talon klondike queue position
	///
	///\param Position to set the talon klondike queue
	void SetKlondikeTalonQueuePosition(char Position);

	///\brief Get the talon klondike queue position
	///
	///\return Position of the talon klondike queue
	char GetKlondikeTalonQueuePosition();

	///\brief Make sure there are no more talons left to deal
	void KlondikeTalonRedealBare();

	///\brief Move a card from the talon queue to the talon stack
	virtual void KlondikeTalonQueueToStack() = 0;

	///\brief Get the top card from the talon
	///
	///\return Card at the top of the talon
	virtual FCSCard* GetKlondikeTalonTopCard() = 0;

	///\brief Get a talon card
	///
	///\param Position is the position of the talon card
	///\return A pointer to the talon card
	virtual FCSCard* GetTalon(int Position) = 0;

protected:
	///Information for talons
	char m_TalonParams[FC_TALON_NUMBER_OF_TALON_PARAMETERS];

	///Number of talon cards
	char m_KlondikeTalonLength;

};


///Prime number needed for talon hash algorithm
#define TALON_PARAM_HASH_VALUE							72211

///\brief Hash algorithm specifically for FCSTalonStateData classes
///
///This hash algorithm uses the MD5 hash algorithm
///to handle the dynamic allocation of the FCSTalonStateData classes
class MD5TalonStateDataHashAlgorithm : public MD5HashAlgorithm<FCSTalonStateData>
{
public:
	///\brief Hash function
	///\param key is the value to be hashed
	///\return Returns hash value
	virtual int Hash(const FCSTalonStateData* key);
};


///Data representation of a state with locations for a card game with talons
class FCSTalonStateWithLocations : public FCSStateWithLocations
{
public:
	///Constructor
	FCSTalonStateWithLocations();

	///Destructor
	virtual ~FCSTalonStateWithLocations();

	///\brief Convert a string representation of a state into a FCSTalonStateWithLocations object
	///
	///\param String is the string to be converted
	///\param NumberOfFreecells is the number of freecells in the card game
	///\param NumberOfStacks is the number of stacks in the card game
	///\param NumberOfDecks is the number of decks in the card game
	///\param TalonType is the talon type of the game
	///\return A pointer to the newly created FCSTalonStateWithLocations object
	static FCSStateWithLocations* CreateInitialState(const char *String, int NumberOfFreecells, 
													int NumberOfStacks, int NumberOfDecks, FCSTalonType TalonType);
	///\brief Validiate that this state is valid
	///
	///\param NumberOfFreecells is the number of freecells in the card game
	///\param NumberOfStacks is the number of stacks in the card game
	///\param NumberOfDecks is the number of decks in the card game
	///\param MisplacedCard will contain a misplaced card if there is one
	///\param TalonType is the talon type of the game
	///\return Returns 0 if state is valid, non-zero if state is invalid
	virtual int CheckStateValidity(int NumberOfFreecells, int NumberOfStacks, 
									int NumberOfDecks, FCSCard** MisplacedCard, FCSTalonType TalonType);

	///\brief Represent a FCSTalonStateWithLocations object as a string
	///
	///\param DebugInfo contains debug information that can be included in the string
	///\param String is the pointer to the string
	virtual void StateAsString(char* String, FCSDebugDisplayInfo* DebugInfo);

	///\brief Get a talon card
	///
	///\param Position is the position of the talon card
	///\return A pointer to the talon card
	virtual FCSCard* GetTalon(int Position);
	
	///\brief Cache the talon cards stack
	///
	///\param TalonHash is the hash table holding the stacks of talon cards
	virtual void CacheTalonStacks(FCInternalHash<FCSTalonStateData, void>* TalonHash) = 0;

	///\brief Create a number of talon cards
	///
	///\param NumberOfTalons is the number of talon cards to create
	virtual void CreateTalons(int NumberOfTalons) = 0;

	///\brief Get a pointer to all the talon data
	///
	///\return A pointer to all the talon data
	virtual FCSTalonStateData* GetTalonData() = 0;

	///\brief Take a card off of the talon stack
	virtual void DecrementKlondikeTalonStack() = 0;
};

///\brief Create a FCSTalonStateData object
///
///\return Pointer to the newly created FCSTalonStateData object
FCSTalonStateData* CreateTalonStateData();

#endif