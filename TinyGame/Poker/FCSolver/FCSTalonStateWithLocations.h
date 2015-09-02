#ifndef FCS_TALON_STATE_WITH_LOCATIONS_H
#define FCS_TALON_STATE_WITH_LOCATIONS_H

////////////////////////////////////////////////
///\file FCSTalonStateWithLocations.h
///\brief This file contains classes dealing with TalonStateWithLocations
///\author Michael Mann
///\version 1.0
///\date October 2002
////////////////////////////////////////////////

#include "FCDebugState.h"
#include "FCCompactState.h"
#include "FCIndirectState.h"
#include "FCSDebugStateWithLocations.h"
#include "FCSCompactStateWithLocations.h"
#include "FCSIndirectStateWithLocations.h"

class FCSDebugTalonStateWithLocations;
class FCSCompactTalonStateWithLocations;
class FCSIndirectTalonStateWithLocations;

///Debug data representation of a state for a card game with talons
class FCSDebugTalonStateData : public FCSTalonStateData
{
public:
	///class that uses the FCSDebugTalonStateData data
	friend class FCSDebugTalonStateWithLocations;

	///Constructor
	FCSDebugTalonStateData();

	///Destructor
	virtual ~FCSDebugTalonStateData();

	///\brief Copy FCSDebugTalonStateData object
	///
	///\param Data is the FCSDebugTalonStateData to be copied
	virtual void Copy(FCSTalonStateData* Data);

	///\brief Compare data of two talon states
	///
	///\return -1 if this talon data < TalonData, 1 if talon data > TalonData, 0 if talon data = TalonData
	virtual int Compare(const FCSTalonStateData* TalonData);

	///\brief Get a talon card
	///
	///\param Position is the position of the talon card
	///\return A pointer to the talon card
	FCSCard* GetTalon(int Position);

	///\brief Move a card from the talon queue to the talon stack
	void KlondikeTalonQueueToStack();

	///\brief Get the top card from the talon
	///
	///\return Card at the top of the talon
	FCSCard* GetKlondikeTalonTopCard();

protected:
	///Talon cards
	FCSDebugCard* m_Talon;
};

///Debug data representation of a state with locations for a card game with talons
class FCSDebugTalonStateWithLocations : public FCSDebugStateWithLocations<FCSTalonStateWithLocations>
{
public:
	///Constructor
	FCSDebugTalonStateWithLocations();

	///Destructor
	virtual ~FCSDebugTalonStateWithLocations();

	///\brief Copy FCSDebugTalonStateWithLocations object
	///
	///\param State is the FCSDebugTalonStateWithLocations to be copied
	virtual void Copy(FCSStateWithLocations* State);

	///\brief Initialize the talon stack
	virtual void Initialize(int NumberOfStacks);

	///\brief Clear the talon of the state
	virtual void CleanState();

	///\brief Cache the talon cards stack
	///
	///\param TalonHash is the hash table holding the stacks of talon cards
	void CacheTalonStacks(FCInternalHash<FCSTalonStateData, void>* TalonHash);

	///\brief Create a number of talon cards
	///
	///\param NumberOfTalons is the number of talon cards to create
	void CreateTalons(int NumberOfTalons);

	///\brief Get a pointer to all the talon data
	///
	///\return A pointer to all the talon data
	FCSTalonStateData* GetTalonData();

	///\brief Take a card off of the talon stack
	void DecrementKlondikeTalonStack();

protected:
	///Talon data for a state
	FCSDebugTalonStateData* m_TalonData;
};


///Compact data representation of a state for a card game with talons
class FCSCompactTalonStateData : public FCSTalonStateData
{
public:
	///class that uses the FCSCompactTalonStateData data
	friend class FCSCompactTalonStateWithLocations;
	
	///Contructor
	FCSCompactTalonStateData();

	///Destructor
	virtual ~FCSCompactTalonStateData();

	///\brief Copy FCSCompactTalonStateData object
	///
	///\param Data is the FCSCompactTalonStateData to be copied
	virtual void Copy(FCSTalonStateData* Data);

	///\brief Compare data of two talon states
	///
	///\return -1 if this talon data < TalonData, 1 if talon data > TalonData, 0 if talon data = TalonData
	virtual int Compare(const FCSTalonStateData* TalonData);

	///\brief Get a talon card
	///
	///\param Position is the position of the talon card
	///\return A pointer to the talon card
	FCSCard* GetTalon(int Position);

	///\brief Move a card from the talon queue to the talon stack
	void KlondikeTalonQueueToStack();

	///\brief Get the top card from the talon
	///
	///\return Card at the top of the talon
	FCSCard* GetKlondikeTalonTopCard();

protected:
	///Talon cards
	FCSCompactCard* m_Talon;
};

///Compact data representation of a state with locations for a card game with talons
class FCSCompactTalonStateWithLocations : public FCSCompactStateWithLocations<FCSTalonStateWithLocations>
{
public:
	///Constructor
	FCSCompactTalonStateWithLocations();

	///Destructor
	virtual ~FCSCompactTalonStateWithLocations();

	///\brief Copy FCSCompactTalonStateWithLocations object
	///
	///\param State is the FCSCompactTalonStateWithLocations to be copied
	virtual void Copy(FCSStateWithLocations* State);

	///\brief Initialize the talon stack
	virtual void Initialize(int NumberOfStacks);

	///\brief Clear the talon of the state
	virtual void CleanState();

	///\brief Cache the talon cards stack
	///
	///\param TalonHash is the hash table holding the stacks of talon cards
	void CacheTalonStacks(FCInternalHash<FCSTalonStateData, void>* TalonHash);

	///\brief Create a number of talon cards
	///
	///\param NumberOfTalons is the number of talon cards to create
	void CreateTalons(int NumberOfTalons);

	///\brief Get a pointer to all the talon data
	///
	///\return A pointer to all the talon data
	FCSTalonStateData* GetTalonData();

	///\brief Take a card off of the talon stack
	void DecrementKlondikeTalonStack();


protected:
	///Talon data for a state
	FCSCompactTalonStateData* m_TalonData;
};

///Indirect data representation of a state for a card game with talons
class FCSIndirectTalonStateData : public FCSTalonStateData
{
public:
	///class that uses the FCSIndirectTalonStateData data
	friend class FCSIndirectTalonStateWithLocations;
	
	///Contructor
	FCSIndirectTalonStateData();

	///Destructor
	virtual ~FCSIndirectTalonStateData();

	///\brief Copy FCSIndirectTalonStateData object
	///
	///\param Data is the FCSIndirectTalonStateData to be copied
	virtual void Copy(FCSTalonStateData* Data);

	///\brief Compare data of two talon states
	///
	///\return -1 if this talon data < TalonData, 1 if talon data > TalonData, 0 if talon data = TalonData
	virtual int Compare(const FCSTalonStateData* TalonData);

	///\brief Get a talon card
	///
	///\param Position is the position of the talon card
	///\return A pointer to the talon card
	FCSCard* GetTalon(int Position);

	///\brief Move a card from the talon queue to the talon stack
	void KlondikeTalonQueueToStack();

	///\brief Get the top card from the talon
	///
	///\return Card at the top of the talon
	FCSCard* GetKlondikeTalonTopCard();

protected:	
	///Talon cards
	FCSIndirectCard* m_Talon;
};

///Indirect data representation of a state with locations for a card game with talons
class FCSIndirectTalonStateWithLocations : public FCSIndirectStateWithLocations<FCSTalonStateWithLocations>
{
public:
	///Constructor
	FCSIndirectTalonStateWithLocations();

	///Destructor
	virtual ~FCSIndirectTalonStateWithLocations();
	
	///\brief Copy FCSIndirectTalonStateWithLocations object
	///
	///\param State is the FCSIndirectTalonStateWithLocations to be copied
	virtual void Copy(FCSStateWithLocations* State);

	///\brief Initialize the talon stack
	virtual void Initialize(int NumberOfStacks);

	///\brief Clear the talon of the state
	virtual void CleanState();

	///\brief Cache the talon cards stack
	///
	///\param TalonHash is the hash table holding the stacks of talon cards
	void CacheTalonStacks(FCInternalHash<FCSTalonStateData, void>* TalonHash);

	///\brief Create a number of talon cards
	///
	///\param NumberOfTalons is the number of talon cards to create
	void CreateTalons(int NumberOfTalons);

	///\brief Get a pointer to all the talon data
	///
	///\return A pointer to all the talon data
	FCSTalonStateData* GetTalonData();

	///\brief Take a card off of the talon stack
	void DecrementKlondikeTalonStack();

protected:
	///Talon data for a state
	FCSIndirectTalonStateData* m_TalonData;
};


#endif