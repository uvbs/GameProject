#ifndef FCS_DERIVEDSTATES_H
#define FCS_DERIVEDSTATES_H

////////////////////////////////////////////////
///\file FCSDerivedStates.h
///\brief This file contains the FCSDerivedStatesList class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCState.h"

///This class was formed to avoid several levels of indirection in keeping track of states
class FCSDerivedStatesList
{
public:
	///Constructor
	FCSDerivedStatesList();

	///\brief Add a state to the list
	///
	///\param State to be added
	void AddState(FCSStateWithLocations* State);

	///Number of States in the list
	int m_NumberOfStates;

	///Upper limit of states for the list
	int m_MaxNumberOfStates;
	
	///States in the list
	FCSStateWithLocations** m_States;
};

#endif