#ifndef MMANN_FCPRESETS_H
#define MMANN_FCPRESETS_H

////////////////////////////////////////////////
///\file FCPresets.h
///\brief This file contains preset card game information
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSFreecellSolvingAlgorithm.h"

class FCCommandLineArguments;

///The maximum size of a preset card game name
#define FC_SIZE_OF_PRESET_NAME	32

///Identification of preset games supported
enum FCSPresetID 
{
	FCS_PRESET_NONE = -1,
	FCS_PRESET_BAKERS_DOZEN,
	FCS_PRESET_BAKERS_GAME,
	FCS_PRESET_CRUEL,
	FCS_PRESET_DER_KATZENSCHWANZ,
	FCS_PRESET_DIE_SCHLANGE, 
	FCS_PRESET_EIGHT_OFF,
	FCS_PRESET_FORECELL, 
	FCS_PRESET_FREECELL,
	FCS_PRESET_GOOD_MEASURE,
	FCS_PRESET_KLONDIKE,
	FCS_PRESET_KINGS_ONLY_BAKERS_GAME,
	FCS_PRESET_RELAXED_FREECELL,
	FCS_PRESET_RELAXED_SEAHAVEN_TOWERS,
	FCS_PRESET_SEAHAVEN_TOWERS,
	FCS_PRESET_SIMPLE_SIMON,
	FCS_PRESET_YUKON,
	FCS_PRESET_BELEAGUERED_CASTLE
};

///Class holding preset information
class FCSPreset
{
public:
	///\brief Given a game name, get the preset game information
	///
	///\param Name is the name fo the game
	///\return A pointer to the preset game information
	static FCSPreset* GetPresetInfo(const char* Name);

	///\brief Given a preset ID, fill in the preset game information
	///
	///\param Id is the preset ID for the game
	void ApplyPresetID(FCSPresetID Id);

	///preset ID for a game
	FCSPresetID m_PresetID;

	///Number of freecells in the game
	int m_NumberOfFreecells;

	///Number of stacks in the game
	int m_NumberOfStacks;

	///Number of decks in the game
	int m_NumberOfDecks;

	///How sequences are built in the game
	BuildSequenceByEnum m_SequencesAreBuiltBy;

	///Whether sequence moves are unlimited in the game
	bool m_UnlimitedSequenceMove;

	///How empty stacks are filled in the game
	FCEmptyStacksFillType m_EmptyStacksFill;

	///Type of talon, if any
	FCSTalonType m_TalonType;

	///The order to do tests to solve the game
	char m_TestOrder[FCS_TESTS_NUM*3+1];
};

///Matches a preset ID with a game name
class FCSPresetName
{
public:

	///\brief Given a game name, get the preset ID
	///
	///\param Name is the name fo the game
	///\return The preset ID for the game
	static FCSPresetID GetPresetID(const char* Name);

	///Name of the game
	char m_Name[FC_SIZE_OF_PRESET_NAME];

	///Preset ID for the game
	FCSPresetID m_PresetID;
};

///\brief Take a test order string and turn it into a test order array
///
///\param TestOrderArray is the outputted test order
///\param TestOrderString is the test order string
///\param TestOrderCount is teh number of tests found in the string
///\param GameName is the name of the game being solved
///\param ErrorString contains error information if any
///\return Return 0 if successful, non-zero otherwise
int FCSApplyTestOrder(int* TestOrderArray, const char* TestOrderString, 
					  int* TestOrderCount, char* GameName, char** ErrorString);

///Storage for all the preset games
extern const FCSPresetName FCSPresetNames[];

#endif