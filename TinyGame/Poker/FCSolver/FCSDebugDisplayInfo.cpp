////////////////////////////////////////////////
///\file FCSDebugDisplayInfo.cpp
///\brief This file contains the implementation of the FCSDebugDisplayInfo class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include "FCSDebugDisplayInfo.h"
#include "FCState.h"

using namespace std;

FCSDebugDisplayInfo::FCSDebugDisplayInfo()
{
	m_DisplayDebug = false;
	m_DisplayDebugOptions = 0;
	m_NumberOfFreecells = 0;
	m_NumberOfStacks = 0;
	m_NumberOfDecks = 0;
	m_TalonType = FCS_TALON_NONE;

	Write.open("StateOutput.txt");
}

FCSDebugDisplayInfo::~FCSDebugDisplayInfo()
{
	Write.close();
}

void FCSDebugDisplayInfo::Display(FCStateType StateType, int NumberOfIterations, 
								  int Depth, FCSStateWithLocations* StateWithLocations, 
								  int ParentNumberOfIterations, int StoredStates)
{
	if (m_DisplayDebug)
	{
		Write << "Iteration: " << NumberOfIterations << endl;
		Write << "Depth: " << Depth << endl;
		Write << "Stored-States: " << StoredStates << endl << endl;

		if (m_DisplayDebugOptions & DEBUG_DISPLAY_PARENT_ITERATIONS)
			Write << "Parent Iteration: " << ParentNumberOfIterations << endl;

		if (m_DisplayDebugOptions & DEBUG_ITERATIVE_STATE_OUTPUT)
		{
			char* AsString = new char[STATE_STRING_SIZE];
			StateWithLocations->StateAsString(AsString, this);
			Write << AsString << endl << "---------------" << endl << endl << endl;
			delete [] AsString;
		}

		FCSCard* Card = NULL;

		int Valid = StateWithLocations->CheckStateValidity(m_NumberOfFreecells, m_NumberOfStacks,
															m_NumberOfDecks, &Card, m_TalonType);

		if (Valid != 0)
		{
			char CardString[10];

			Card->Perl2User(CardString, (m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T), m_DisplayDebug);

			switch(Valid)
			{
			case 1:
				cerr << "Error!  There's a missing card: " << CardString << endl;
				break;
			case 2:
				cerr << "Error!  There's an extra card: " << CardString << endl;
				break;
			case 3:
				cerr << "Error!  There's an empty slot in one of the stacks" << endl;
				break;
			}

			exit(-1);
		}
	}
}