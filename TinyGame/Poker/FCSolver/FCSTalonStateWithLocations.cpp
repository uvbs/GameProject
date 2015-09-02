////////////////////////////////////////////////
///\file FCSTalonStateWithLocations.cpp
///\brief This file contains the implementation of classes dealing with TalonStateWithLocations
///\author Michael Mann
///\version 1.0
///\date November 2002
////////////////////////////////////////////////

#include "FCTalonState.h"
#include "FCSTalonStateWithLocations.h"

FCSDebugTalonStateData::FCSDebugTalonStateData()
{
	m_Talon = NULL;
}

FCSDebugTalonStateData::~FCSDebugTalonStateData()
{
	if (m_Talon != NULL)
		delete [] m_Talon;
}

void FCSDebugTalonStateData::Copy(FCSTalonStateData* Data)
{
	FCSTalonStateData::Copy(Data);

	if (m_Talon == NULL)
		m_Talon = new FCSDebugCard[m_KlondikeTalonLength];

	for (int i = 0; i < m_KlondikeTalonLength; i++)
		m_Talon[i].Copy(&((FCSDebugTalonStateData*)Data)->m_Talon[i]);
}

int FCSDebugTalonStateData::Compare(const FCSTalonStateData* TalonData)
{
	int CompareValue = FCSTalonStateData::Compare(TalonData);
	if (CompareValue != 0)
		return CompareValue;

	for (int a = 0;a<m_KlondikeTalonLength;a++)
		if ((CompareValue = m_Talon[a].Compare(&((FCSDebugTalonStateData*)TalonData)->m_Talon[a])) != 0)
			return CompareValue;

	return 0;
};


FCSCard* FCSDebugTalonStateData::GetTalon(int Position)
{
	return &m_Talon[Position];
}

void FCSDebugTalonStateData::KlondikeTalonQueueToStack()
{
	m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]++;
	while ((m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION] < m_KlondikeTalonLength-1) &&
		   (m_Talon[m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]].IsEmptyCard()))
	{
		m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]++;
	}

	m_TalonParams[FC_TALON_PARAM_KLONDIKE_QUEUE_POSITION]++;
}

FCSCard* FCSDebugTalonStateData::GetKlondikeTalonTopCard()
{
	return &m_Talon[m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]];
}



FCSDebugTalonStateWithLocations::FCSDebugTalonStateWithLocations() : FCSDebugStateWithLocations<FCSTalonStateWithLocations>()
{
	m_TalonData = NULL;
}

FCSDebugTalonStateWithLocations::~FCSDebugTalonStateWithLocations()
{
}

void FCSDebugTalonStateWithLocations::Copy(FCSStateWithLocations* State)
{
	FCSDebugStateWithLocations<FCSTalonStateWithLocations>::Copy(State);
	m_TalonData = new FCSDebugTalonStateData();
	m_TalonData->Copy(((FCSDebugTalonStateWithLocations*)State)->m_TalonData);
}

void FCSDebugTalonStateWithLocations::Initialize(int NumberOfStacks)
{
	m_TalonData = new FCSDebugTalonStateData();
}

void FCSDebugTalonStateWithLocations::CleanState()
{
	if (m_TalonData != NULL)
		delete m_TalonData;
}

void FCSDebugTalonStateWithLocations::CacheTalonStacks(FCInternalHash<FCSTalonStateData, void>* TalonHash)
{
	FCSDebugTalonStateData* CachedTalonData;

	Realloc<FCSDebugCard>(&m_TalonData->m_Talon, 
							(m_TalonData->m_Talon == NULL) ? 0 : m_TalonData->GetKlondikeTalonLength()+1,
							m_TalonData->GetKlondikeTalonLength()+1);

	CachedTalonData = (FCSDebugTalonStateData*)TalonHash->Insert(m_TalonData, true);

    if (CachedTalonData != NULL)
    {
		delete m_TalonData;
		m_TalonData = CachedTalonData;
    }
}

void FCSDebugTalonStateWithLocations::CreateTalons(int NumberOfTalons)
{
	m_TalonData->m_Talon = new FCSDebugCard[NumberOfTalons];
}

FCSTalonStateData* FCSDebugTalonStateWithLocations::GetTalonData()
{
	return m_TalonData;
}

void FCSDebugTalonStateWithLocations::DecrementKlondikeTalonStack()
{
	m_TalonData->m_Talon[m_TalonData->m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]--].EmptyCard();
}


FCSCompactTalonStateData::FCSCompactTalonStateData()
{
	m_Talon = NULL;
}

FCSCompactTalonStateData::~FCSCompactTalonStateData()
{
	if (m_Talon != NULL)
		delete [] m_Talon;
}

void FCSCompactTalonStateData::Copy(FCSTalonStateData* Data)
{
	FCSTalonStateData::Copy(Data);

	if (m_Talon == NULL)
		m_Talon = new FCSCompactCard[m_KlondikeTalonLength];

	for (int i = 0; i < m_KlondikeTalonLength; i++)
		m_Talon[i].Copy(&((FCSCompactTalonStateData*)Data)->m_Talon[i]);
}

int FCSCompactTalonStateData::Compare(const FCSTalonStateData* TalonData)
{
	int CompareValue = FCSTalonStateData::Compare(TalonData);
	if (CompareValue != 0)
		return CompareValue;

	for (int a = 0;a<m_KlondikeTalonLength;a++)
		if ((CompareValue = m_Talon[a].Compare(&((FCSCompactTalonStateData*)TalonData)->m_Talon[a])) != 0)
			return CompareValue;

	return 0;
};

FCSCard* FCSCompactTalonStateData::GetTalon(int Position)
{
	return &m_Talon[Position];
}

void FCSCompactTalonStateData::KlondikeTalonQueueToStack()
{
	m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]++;
	while ((m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION] < m_KlondikeTalonLength-1) &&
		   (m_Talon[m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]].IsEmptyCard()))
	{
		m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]++;
	}

	m_TalonParams[FC_TALON_PARAM_KLONDIKE_QUEUE_POSITION]++;
}

FCSCard* FCSCompactTalonStateData::GetKlondikeTalonTopCard()
{
	return &m_Talon[m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]];
}

FCSCompactTalonStateWithLocations::FCSCompactTalonStateWithLocations() : FCSCompactStateWithLocations<FCSTalonStateWithLocations>()
{
	m_TalonData = NULL;
}

FCSCompactTalonStateWithLocations::~FCSCompactTalonStateWithLocations()
{
}

void FCSCompactTalonStateWithLocations::Copy(FCSStateWithLocations* State)
{
	FCSCompactStateWithLocations<FCSTalonStateWithLocations>::Copy(State);
	m_TalonData = new FCSCompactTalonStateData();
	m_TalonData->Copy(((FCSCompactTalonStateWithLocations*)State)->m_TalonData);
}

void FCSCompactTalonStateWithLocations::Initialize(int NumberOfStacks)
{
	m_TalonData = new FCSCompactTalonStateData();
}

void FCSCompactTalonStateWithLocations::CleanState()
{
	if (m_TalonData != NULL)
		delete m_TalonData;
}

void FCSCompactTalonStateWithLocations::CacheTalonStacks(FCInternalHash<FCSTalonStateData, void>* TalonHash)
{
	FCSCompactTalonStateData* CachedTalonData;

	Realloc<FCSCompactCard>(&m_TalonData->m_Talon, 
							(m_TalonData->m_Talon == NULL) ? 0 : m_TalonData->GetKlondikeTalonLength()+1,
							m_TalonData->GetKlondikeTalonLength()+1);

	CachedTalonData = (FCSCompactTalonStateData*)TalonHash->Insert(m_TalonData, true);

    if (CachedTalonData != NULL)
    {
		delete m_TalonData;
		m_TalonData = CachedTalonData;
    }
}

void FCSCompactTalonStateWithLocations::CreateTalons(int NumberOfTalons)
{
	m_TalonData->m_Talon = new FCSCompactCard[NumberOfTalons];
}

FCSTalonStateData* FCSCompactTalonStateWithLocations::GetTalonData()
{
	return m_TalonData;
}

void FCSCompactTalonStateWithLocations::DecrementKlondikeTalonStack()
{
	m_TalonData->m_Talon[m_TalonData->m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]--].EmptyCard();
}



FCSIndirectTalonStateData::FCSIndirectTalonStateData()
{
	m_Talon = NULL;
}

FCSIndirectTalonStateData::~FCSIndirectTalonStateData()
{
	if (m_Talon != NULL)
		delete [] m_Talon;
}

void FCSIndirectTalonStateData::Copy(FCSTalonStateData* Data)
{
	FCSTalonStateData::Copy(Data);

	if (m_Talon == NULL)
		m_Talon = new FCSIndirectCard[m_KlondikeTalonLength];

	for (int i = 0; i < m_KlondikeTalonLength; i++)
		m_Talon[i].Copy(&((FCSIndirectTalonStateData*)Data)->m_Talon[i]);
}

int FCSIndirectTalonStateData::Compare(const FCSTalonStateData* TalonData)
{
	int CompareValue = FCSTalonStateData::Compare(TalonData);
	if (CompareValue != 0)
		return CompareValue;

	for (int a = 0;a<m_KlondikeTalonLength;a++)
		if ((CompareValue = m_Talon[a].Compare(&((FCSIndirectTalonStateData*)TalonData)->m_Talon[a])) != 0)
			return CompareValue;

	return 0;
};

FCSCard* FCSIndirectTalonStateData::GetTalon(int Position)
{
	return &m_Talon[Position];
}

void FCSIndirectTalonStateData::KlondikeTalonQueueToStack()
{
	m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]++;
	while ((m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION] < m_KlondikeTalonLength-1) &&
		   (m_Talon[m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]].IsEmptyCard()))
	{
		m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]++;
	}

	m_TalonParams[FC_TALON_PARAM_KLONDIKE_QUEUE_POSITION]++;
}

FCSCard* FCSIndirectTalonStateData::GetKlondikeTalonTopCard()
{
	return &m_Talon[m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]];
}

FCSIndirectTalonStateWithLocations::FCSIndirectTalonStateWithLocations() : FCSIndirectStateWithLocations<FCSTalonStateWithLocations>()
{
	m_TalonData = NULL;
}

FCSIndirectTalonStateWithLocations::~FCSIndirectTalonStateWithLocations()
{
}

void FCSIndirectTalonStateWithLocations::Copy(FCSStateWithLocations* State)
{
	FCSIndirectStateWithLocations<FCSTalonStateWithLocations>::Copy(State);
	m_TalonData = new FCSIndirectTalonStateData();
	m_TalonData->Copy(((FCSIndirectTalonStateWithLocations*)State)->m_TalonData);
}

void FCSIndirectTalonStateWithLocations::Initialize(int NumberOfStacks)
{
	FCSIndirectStateWithLocations<FCSTalonStateWithLocations>::Initialize(NumberOfStacks);
	m_TalonData = new FCSIndirectTalonStateData();
}

void FCSIndirectTalonStateWithLocations::CleanState()
{
	FCSIndirectStateWithLocations<FCSTalonStateWithLocations>::CleanState();
	if (m_TalonData != NULL)
		delete m_TalonData;
}

void FCSIndirectTalonStateWithLocations::CacheTalonStacks(FCInternalHash<FCSTalonStateData, void>* TalonHash)
{
	FCSIndirectTalonStateData* CachedTalonData;

	Realloc<FCSIndirectCard>(&m_TalonData->m_Talon, 
							(m_TalonData->m_Talon == NULL) ? 0 : m_TalonData->GetKlondikeTalonLength()+1,
							m_TalonData->GetKlondikeTalonLength()+1);

	CachedTalonData = (FCSIndirectTalonStateData*)TalonHash->Insert(m_TalonData, true);

    if (CachedTalonData != NULL)
    {
		delete m_TalonData;
		m_TalonData = CachedTalonData;
    }
}

void FCSIndirectTalonStateWithLocations::CreateTalons(int NumberOfTalons)
{
	m_TalonData->m_Talon = new FCSIndirectCard[NumberOfTalons];
}

FCSTalonStateData* FCSIndirectTalonStateWithLocations::GetTalonData()
{
	return m_TalonData;
}

void FCSIndirectTalonStateWithLocations::DecrementKlondikeTalonStack()
{
	m_TalonData->m_Talon[m_TalonData->m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION]--].EmptyCard();
}
