////////////////////////////////////////////////
///\file FCPresets.cpp
///\brief This file contains preset card game implementation
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include "FCPresets.h"

const FCSPresetName FCSPresetNames[23] =
{
	{
		"bakers_dozen",
		FCS_PRESET_BAKERS_DOZEN,
	},
	{
		"bakers_game",
		FCS_PRESET_BAKERS_GAME,
	},
	{
		"beleaguered_castle",
		FCS_PRESET_BELEAGUERED_CASTLE,
	},
	{
		"citadel",
		FCS_PRESET_BELEAGUERED_CASTLE,
	},
	{
		"cruel",
		FCS_PRESET_CRUEL,
	},
	{
		"der_katzenschwanz",
		FCS_PRESET_DER_KATZENSCHWANZ,
	},
	{
		"der_katz",
		FCS_PRESET_DER_KATZENSCHWANZ,
	},
	{
		"die_schlange",
		FCS_PRESET_DIE_SCHLANGE,
	},
	{
		"eight_off",
		FCS_PRESET_EIGHT_OFF,
	},
	{
		"forecell",
		FCS_PRESET_FORECELL,
	},
	{
		"freecell",
		FCS_PRESET_FREECELL,
	},
	{
		"good_measure",
		FCS_PRESET_GOOD_MEASURE,
	},
	{
		"klondike",
		FCS_PRESET_KLONDIKE,
	},
	{
		"kings_only_bakers_game",
		FCS_PRESET_KINGS_ONLY_BAKERS_GAME,
	},
	{
		"ko_bakers_game",
		FCS_PRESET_KINGS_ONLY_BAKERS_GAME,
	},
	{
		"relaxed_freecell",
		FCS_PRESET_RELAXED_FREECELL,
	},
	{
		"relaxed_seahaven_towers",
		FCS_PRESET_RELAXED_SEAHAVEN_TOWERS,
	},
	{
		"relaxed_seahaven",
		FCS_PRESET_RELAXED_SEAHAVEN_TOWERS,
	},
	{
		"seahaven_towers",
		FCS_PRESET_SEAHAVEN_TOWERS,
	},
	{
		"seahaven",
		FCS_PRESET_SEAHAVEN_TOWERS,
	},
	{
		"simple_simon",
		FCS_PRESET_SIMPLE_SIMON,
	},
	{
		"streets_and_alleys",
		FCS_PRESET_BELEAGUERED_CASTLE,
	},
	{
		"yukon",
		FCS_PRESET_YUKON,
	},
};

///All the game presets and their information
static const FCSPreset FCSPresets[16] =
{
	{
		FCS_PRESET_BAKERS_DOZEN,
		0,
		13,
		1,

		FCS_SEQ_BUILT_BY_RANK,
		0,
		FCS_ES_FILLED_BY_NONE,
		FCS_TALON_NONE,
		"0123456789",
	},
	{
		FCS_PRESET_BAKERS_GAME,
		4,
		8,
		1,

		FCS_SEQ_BUILT_BY_SUIT,
		0,
		FCS_ES_FILLED_BY_ANY_CARD,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_BELEAGUERED_CASTLE,
		0,
		8,
		1,

		FCS_SEQ_BUILT_BY_RANK,
		0,
		FCS_ES_FILLED_BY_ANY_CARD,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_CRUEL,
		0,
		12,
		1,

		FCS_SEQ_BUILT_BY_SUIT,
		0,
		FCS_ES_FILLED_BY_NONE,
		FCS_TALON_NONE,
		"0123456789",
	},
	{
		FCS_PRESET_DER_KATZENSCHWANZ,
		8,
		9,
		2,

		FCS_SEQ_BUILT_BY_ALTERNATE_COLOR,
		1,
		FCS_ES_FILLED_BY_NONE,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_DIE_SCHLANGE,
		8,
		9,
		2,

		FCS_SEQ_BUILT_BY_ALTERNATE_COLOR,
		0,
		FCS_ES_FILLED_BY_NONE,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_EIGHT_OFF,
		8,
		8,
		1,

		FCS_SEQ_BUILT_BY_SUIT,
		0,
		FCS_ES_FILLED_BY_KINGS_ONLY,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_FORECELL,
		4,
		8,
		1,

		FCS_SEQ_BUILT_BY_ALTERNATE_COLOR,
		0,
		FCS_ES_FILLED_BY_KINGS_ONLY,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_FREECELL,
		4,
		8,
		1,

		FCS_SEQ_BUILT_BY_ALTERNATE_COLOR,
		0,
		FCS_ES_FILLED_BY_ANY_CARD,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_GOOD_MEASURE,
		0,
		10,
		1,

		FCS_SEQ_BUILT_BY_RANK,
		0,
		FCS_ES_FILLED_BY_NONE,
		FCS_TALON_NONE,
		"0123456789",
	},
	{
		FCS_PRESET_KINGS_ONLY_BAKERS_GAME,
		4,
		8,
		1,

		FCS_SEQ_BUILT_BY_SUIT,
		0,
		FCS_ES_FILLED_BY_KINGS_ONLY,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_KLONDIKE,
		0,
		7,
		1,

		FCS_SEQ_BUILT_BY_ALTERNATE_COLOR,
		1,
		FCS_ES_FILLED_BY_KINGS_ONLY,
		FCS_TALON_KLONDIKE,
		"[0123456]",
	},
	{
		FCS_PRESET_RELAXED_FREECELL,
		4,
		8,
		1,

		FCS_SEQ_BUILT_BY_ALTERNATE_COLOR,
		1,
		FCS_ES_FILLED_BY_ANY_CARD,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_RELAXED_SEAHAVEN_TOWERS,
		4,
		10,
		1,

		FCS_SEQ_BUILT_BY_SUIT,
		1,
		FCS_ES_FILLED_BY_KINGS_ONLY,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_SEAHAVEN_TOWERS,
		4,
		10,
		1,

		FCS_SEQ_BUILT_BY_SUIT,
		0,
		FCS_ES_FILLED_BY_KINGS_ONLY,
		FCS_TALON_NONE,
		"[01][23456789]",
	},
	{
		FCS_PRESET_SIMPLE_SIMON,
		0,
		10,
		1,

		FCS_SEQ_BUILT_BY_SUIT,
		0,
		FCS_ES_FILLED_BY_ANY_CARD,
		FCS_TALON_NONE,
		"012345678",
	}
};

FCSPresetID FCSPresetName::GetPresetID(const char* Name)
{
	int NumberOfElements = sizeof(FCSPresetNames)/sizeof(FCSPresetName);

	for(int a=0;a<NumberOfElements;a++)
		if (!strcmp(Name, FCSPresetNames[a].m_Name))
			return FCSPresetNames[a].m_PresetID;

	return FCS_PRESET_NONE;
}

FCSPreset* FCSPreset::GetPresetInfo(const char* Name)
{
	FCSPresetID PresetID = FCSPresetName::GetPresetID(Name);
	if (PresetID >= 0)
	{
		FCSPreset* Preset = new FCSPreset();
		Preset->ApplyPresetID(PresetID);
		return Preset;
	}

	return NULL;
}

void FCSPreset::ApplyPresetID(FCSPresetID Id)
{
	int NumberOfElements = sizeof(FCSPresets)/sizeof(FCSPreset);

	for(int PresetIndex=0; PresetIndex < NumberOfElements ; PresetIndex++)
	{
		if (FCSPresets[PresetIndex].m_PresetID == Id)
		{
			m_NumberOfFreecells = FCSPresets[PresetIndex].m_NumberOfFreecells;
			m_NumberOfStacks = FCSPresets[PresetIndex].m_NumberOfStacks;
			m_NumberOfDecks = FCSPresets[PresetIndex].m_NumberOfDecks;
			m_SequencesAreBuiltBy = FCSPresets[PresetIndex].m_SequencesAreBuiltBy;
			m_UnlimitedSequenceMove = FCSPresets[PresetIndex].m_UnlimitedSequenceMove;
			m_EmptyStacksFill = FCSPresets[PresetIndex].m_EmptyStacksFill;
			m_TalonType = FCSPresets[PresetIndex].m_TalonType;
			strcpy(m_TestOrder, FCSPresets[PresetIndex].m_TestOrder);
		}
	}
}

int FCSApplyTestOrder(int* TestOrderArray, const char* TestOrderString, 
					  int* TestOrderCount, char* GameName, char** ErrorString)
{
	int Length = strlen(TestOrderString),
		TestIndex = 0,
		TestsOrder[FCS_TESTS_NUM],
		a;

	bool IsGroup = false,
		 IsStartGroup = false;

	//I'm using this instead of strdup becuase I think it uses malloc.
	char* LocalErrorString = new char[50];

	//The array of tests setup in the original fcs was much bigger than it 
	//will ever need, so the second condition wasn't ever true
	for(a=0;a<Length;a++)
	{
		if ((TestOrderString[a] == '(') || (TestOrderString[a] == '['))
		{
			if (IsGroup)
			{
				strcpy(LocalErrorString, "There's a nested random group.");
				*ErrorString = LocalErrorString;
				return 1;
			}
			
			IsGroup = true;
			IsStartGroup = true;
			continue;
		}

		if ((TestOrderString[a] == ')') || (TestOrderString[a] == ']'))
		{
			if (IsStartGroup)
			{
				strcpy(LocalErrorString,"There's an empty group.");
				*ErrorString = LocalErrorString;
				return 2;
			}
			if (!IsGroup)
			{
				strcpy(LocalErrorString, "There's a renegade right parenthesis or bracket.");
				*ErrorString = LocalErrorString;
				return 3;
			}
			IsGroup = false;
			IsStartGroup = false;
			continue;
		}

		//verify the test order numbers are valid
		bool TestOrderError = false;
		if (!strcmp(GameName, "simple_simon"))
		{
			if ((TestOrderString[a] < '0') || (TestOrderString[a] > '8'))
				TestOrderError = true;
		}
		else if (!strcmp(GameName, "klondike"))
		{
			if ((TestOrderString[a] < '0') || (TestOrderString[a] > '6'))
				TestOrderError = true;
		}
		else
		{
			if ((TestOrderString[a] < '0') || (TestOrderString[a] > '9'))
				TestOrderError = true;
		}

		if (TestOrderError)
		{
			sprintf(LocalErrorString, "'%c' is not a valid --test-order value.", TestOrderString[a]);
			*ErrorString = LocalErrorString;
			return 4;
		}

		if (TestIndex < FCS_TESTS_NUM)
			TestsOrder[TestIndex++] = ((TestOrderString[a]-'0')%FCS_TESTS_NUM) | (IsGroup ? FCS_TEST_ORDER_FLAG_RANDOM : 0) | (IsStartGroup ? FCS_TEST_ORDER_FLAG_START_RANDOM_GROUP : 0);

		IsStartGroup = false;
	}

	if (a != Length)
	{
		sprintf(LocalErrorString, "The number of tests exceeds the maximum of %i.\n Recomile the program if you wish to have more", FCS_TESTS_NUM);
		*ErrorString = LocalErrorString;
		return 5;
	}

	*TestOrderCount = TestIndex;
	memcpy(TestOrderArray, TestsOrder, sizeof(int)*TestIndex);
	delete [] LocalErrorString;
	*ErrorString = NULL;

	return 0;
}

