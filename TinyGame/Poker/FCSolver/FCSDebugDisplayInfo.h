#ifndef FCS_DEBUG_DISPLAY_INFO_H
#define FCS_DEBUG_DISPLAY_INFO_H

////////////////////////////////////////////////
///\file FCSDebugDisplayInfo.h
///\brief This file contains the FCSDebugDisplayInfo class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <fstream>
#include "FCEnums.h"

class FCSStateWithLocations;

//These are the debug display option flags

///Display Iterative State Output
#define DEBUG_ITERATIVE_STATE_OUTPUT	1
///Make the debug output parsible
#define DEBUG_IS_OUTPUT_PARSEABLE		2
///Display Canonized Output
#define DEBUG_CANONIZED_ORDER_OUTPUT	4
///Display "10" as a "T"
#define DEBUG_DISPLAY_10_AS_T			8
///Display parent iterations
#define DEBUG_DISPLAY_PARENT_ITERATIONS	16
///Display moves of the game
#define DEBUG_DISPLAY_MOVES				32
///Display states of the game
#define DEBUG_DISPLAY_STATES			64
///Display using standard notation
#define DEBUG_USE_STANDARD_NOTATION		128

///Displays optional debugging information
class FCSDebugDisplayInfo
{
public:
	///Constructor
	FCSDebugDisplayInfo();

	///Destructor
	virtual ~FCSDebugDisplayInfo();

	///\brief Display a state with all debugging information selected
	///
	///\param StateType is the state type used in the game
	///\param NumberOfIterations to the state
	///\param Depth of the state
	///\param StateWithLocations is the state itself
	///\param ParentNumberOfIterations to the state
	///\param StoredStates is the number of stored states
	///\param TalonType is the type of talon in the game (if applicable)
	virtual void Display(FCStateType StateType, int NumberOfIterations, int Depth, 
						 FCSStateWithLocations* StateWithLocations, 
						 int ParentNumberOfIterations, int StoredStates);

	///Flag that determines displaying debug information
	bool m_DisplayDebug;

	///Contains all the flags used for debug information
	int m_DisplayDebugOptions;

	///Number of freecells in the game
	int m_NumberOfFreecells;

	///Number of stacks in the game
	int m_NumberOfStacks;

	///Number of decks in the game
	int m_NumberOfDecks;

	///Talon type for the game
	FCSTalonType m_TalonType;

	///File being written to
	std::ofstream Write;
};

#endif