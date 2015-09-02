////////////////////////////////////////////////
///\file FCSDerivedStates.cpp
///\brief This file contains the implementation of the FCSDerivedStatesList class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>
#include "FCSDerivedStates.h"
#include "FCHelpingAlgorithms.h"

FCSDerivedStatesList::FCSDerivedStatesList()
{
	m_NumberOfStates = 0;
	m_MaxNumberOfStates = 0;
	m_States = NULL;
}

void FCSDerivedStatesList::AddState(FCSStateWithLocations* State)
{
	if (m_NumberOfStates == m_MaxNumberOfStates)
	{
		m_MaxNumberOfStates += 16;
		ReallocStateWithLocationsArray(&m_States, m_NumberOfStates, m_MaxNumberOfStates);
	}

	m_States[m_NumberOfStates] = State;
	m_NumberOfStates++;
}