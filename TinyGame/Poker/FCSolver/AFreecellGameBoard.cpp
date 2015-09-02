////////////////////////////////////////////////
///\file AFreecellGameBoard.cpp
///\brief This file contains the implementation of the AFreecellGameBoard class and all the game supporting data 
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "AFreecellGameBoard.h"
#include "PysolGameBoards.h"
#include "MiscFreecellGameBoards.h"

///All the game boards that are handled by the program
const BoardName AllBoardNames[50] =
{
	{
		"bakers_dozen",
		BOARD_ID_BAKERS_DOZEN,
	},
	{
		"bakers_game",
		BOARD_ID_BAKERS_GAME,
	},
	{
		"beleaguered_castle",
		BOARD_ID_BELEAGUERED_CASTLE,
	},
	{
		"casino_klondike",
		BOARD_ID_KLONDIKE,
	},
	{
		"citadel",
		BOARD_ID_CITADEL,
	},
	{
		"cruel",
		BOARD_ID_CRUEL,
	},
	{
		"der_katzenschwanz",
		BOARD_ID_DER_KATZENSCHWANZ,
	},
	{
		"der_katz",
		BOARD_ID_DER_KATZENSCHWANZ,
	},
	{
		"die_schlange",
		BOARD_ID_DIE_SCHLANGE,
	},
	{
		"eight_off",
		BOARD_ID_EIGHT_OFF,
	},
	{
		"fan",
		BOARD_ID_FAN,
	},
	{
		"forecell",
		BOARD_ID_FORECELL,
	},
	{
		"freecell",
		BOARD_ID_FREECELL,
	},
	{
		"gnome_freecell",
		BOARD_ID_GNOME_FREECELL,
	},
	{
		"good_measure",
		BOARD_ID_GOOD_MEASURE,
	},
	{
		"gypsy",
		BOARD_ID_GYPSY,
	},
	{
		"kings_only_bakers_game",
		BOARD_ID_KINGS_ONLY_BAKERS_GAME,
	},
	{
		"klondike",
		BOARD_ID_KLONDIKE,
	},
	{
		"klondike_by_threes",
		BOARD_ID_KLONDIKE,
	},
	{
		"ko_bakers_game",
		BOARD_ID_KINGS_ONLY_BAKERS_GAME,
	},
	{
		"microsoft_freecell",
		BOARD_ID_MICROSOFT_FREECELL,
	},
	{
		"pysol_bakers_game",
		BOARD_ID_PYSOL_BAKERS_GAME,
	},
	{
		"pysol_beleaguered_castle",
		BOARD_ID_PYSOL_BELEAGUERED_CASTLE,
	},
	{
		"pysol_casino_klondike",
		BOARD_ID_PYSOL_KLONDIKE,
	},
	{
		"pysol_eight_off",
		BOARD_ID_PYSOL_EIGHT_OFF,
	},
	{
		"pysol_freecell",
		BOARD_ID_PYSOL_FREECELL,
	},
	{
		"pysol_kings_only_bakers_game",
		BOARD_ID_PYSOL_KINGS_ONLY_BAKERS_GAME,
	},
	{
		"pysol_klondike",
		BOARD_ID_PYSOL_KLONDIKE,
	},
	{
		"pysol_klondike_by_threes",
		BOARD_ID_PYSOL_KLONDIKE,
	},
	{
		"pysol_ko_bakers_game",
		BOARD_ID_PYSOL_KINGS_ONLY_BAKERS_GAME,
	},
	{
		"pysol_relaxed_freecell",
		BOARD_ID_PYSOL_RELAXED_FREECELL,
	},
	{
		"pysol_relaxed_seahaven",
		BOARD_ID_PYSOL_RELAXED_SEAHAVEN_TOWERS,
	},
	{
		"pysol_relaxed_seahaven_towers",
		BOARD_ID_PYSOL_RELAXED_SEAHAVEN_TOWERS,
	},
	{
		"pysol_seahaven",
		BOARD_ID_PYSOL_SEAHAVEN_TOWERS,
	},
	{
		"pysol_seahaven_towers",
		BOARD_ID_PYSOL_SEAHAVEN_TOWERS,
	},
	{
		"pysol_streets_and_alleys",
		BOARD_ID_PYSOL_STREETS_AND_ALLEYS,
	},
	{
		"pysol_thumb_and_pouch",
		BOARD_ID_PYSOL_KLONDIKE,
	},
	{
		"pysol_vegas_klondike",
		BOARD_ID_PYSOL_KLONDIKE,
	},
	{
		"relaxed_freecell",
		BOARD_ID_RELAXED_FREECELL,
	},
	{
		"relaxed_seahaven_towers",
		BOARD_ID_RELAXED_SEAHAVEN_TOWERS,
	},
	{
		"relaxed_seahaven",
		BOARD_ID_RELAXED_SEAHAVEN_TOWERS,
	},
	{
		"seahaven_towers",
		BOARD_ID_SEAHAVEN_TOWERS,
	},
	{
		"simple_simon",
		BOARD_ID_SIMPLE_SIMON,
	},
	{
		"small_harp",
		BOARD_ID_SMALL_HARP,
	},
	{
		"streets_and_alleys",
		BOARD_ID_STREETS_AND_ALLEYS,
	},
	{
		"thumb_and_pouch",
		BOARD_ID_KLONDIKE,
	},
	{
		"vegas_klondike",
		BOARD_ID_KLONDIKE,
	},
	{
		"yukon",
		BOARD_ID_YUKON,
	},
	{
		"whitehead",
		BOARD_ID_WHITEHEAD,
	}

};

///\brief Walk the game names to find the game requested
///
///\param Name is the name of the game to be found
///\return BoardID of the game
BoardID GetBoardID(const char* Name)
{
	int NumberOfElements = sizeof(AllBoardNames)/sizeof(BoardName);

	for(int a=0;a<NumberOfElements;a++)
		if (!strcmp(Name, AllBoardNames[a].m_Name))
			return AllBoardNames[a].m_BoardID;

	return BOARD_ID_NONE;
}

AFreecellGameBoard* CreateAFreecellGameBoard(char* GameName, bool Display10AsT)
{
	AFreecellGameBoard* board = NULL;
	switch(GetBoardID(GameName))
	{
	case BOARD_ID_CRUEL:
	case BOARD_ID_GOOD_MEASURE:
		return NULL;
	case BOARD_ID_BAKERS_DOZEN:
		board = new PysolBakersDozenGameBoard(Display10AsT);
		break;
	case BOARD_ID_BELEAGUERED_CASTLE:
		board = new AisleRiotBeleagueredCastleGameBoard(Display10AsT);
		break;
	case BOARD_ID_CITADEL:
		board = new PysolCitadelGameBoard(Display10AsT);
		break;
	case BOARD_ID_DIE_SCHLANGE:
		board = new PysolDieSchlangeGameBoard(Display10AsT);
		break;
	case BOARD_ID_DER_KATZENSCHWANZ:
		board = new PysolDerKatzenschwantzGameBoard(Display10AsT);
		break;
	case BOARD_ID_EIGHT_OFF:
		board = new AisleRiotEightOffGameBoard(Display10AsT);
		break;
	case BOARD_ID_FAN:
		board = new PysolFanGameBoard(Display10AsT);
		break;
	case BOARD_ID_FORECELL:
		board = new PysolForecellGameBoard(Display10AsT);
		break;
	case BOARD_ID_FREECELL:
	case BOARD_ID_BAKERS_GAME:
	case BOARD_ID_KINGS_ONLY_BAKERS_GAME:
	case BOARD_ID_RELAXED_FREECELL:
	case BOARD_ID_STREETS_AND_ALLEYS:
		board = new AisleRiotFreecellGameBoard(Display10AsT);
		break;
	case BOARD_ID_GNOME_FREECELL:
		board = new GnomeFreecellGameBoard(Display10AsT);
		break;
	case BOARD_ID_GYPSY:
		board = new PysolGypsyGameBoard(Display10AsT);
		break;
	case BOARD_ID_KLONDIKE:
		board = new AisleRiotKlondikeGameBoard(Display10AsT);
		break;
	case BOARD_ID_MICROSOFT_FREECELL:
		board = new MicrosoftFreecellGameBoard(Display10AsT);
		break;
	case BOARD_ID_PYSOL_BELEAGUERED_CASTLE:
		board = new PysolBeleagueredCastleGameBoard(Display10AsT);
		break;
	case BOARD_ID_PYSOL_EIGHT_OFF:
		board = new PysolEightOffGameBoard(Display10AsT);
		break;
	case BOARD_ID_PYSOL_FREECELL:
	case BOARD_ID_PYSOL_BAKERS_GAME:
	case BOARD_ID_PYSOL_KINGS_ONLY_BAKERS_GAME:
	case BOARD_ID_PYSOL_RELAXED_FREECELL:
	case BOARD_ID_PYSOL_STREETS_AND_ALLEYS:
		board = new PysolFreecellGameBoard(Display10AsT);
		break;
	case BOARD_ID_PYSOL_KLONDIKE:
		board = new PysolKlondikeGameBoard(Display10AsT);
		break;
	case BOARD_ID_PYSOL_RELAXED_SEAHAVEN_TOWERS:
	case BOARD_ID_PYSOL_SEAHAVEN_TOWERS:
		board = new PysolSeahavenTowersGameBoard(Display10AsT);
		break;
	case BOARD_ID_SEAHAVEN_TOWERS:
	case BOARD_ID_RELAXED_SEAHAVEN_TOWERS:
		board = new AisleRiotSeahavenTowersGameBoard(Display10AsT);
		break;
	case BOARD_ID_SIMPLE_SIMON:
		board = new PysolSimpleSimonGameBoard(Display10AsT);
		break;
	case BOARD_ID_SMALL_HARP:
		board = new PysolSmallHarpGameBoard(Display10AsT);
		break;
	case BOARD_ID_WHITEHEAD:
		board = new PysolWhiteheadGameBoard(Display10AsT);
		break;
	case BOARD_ID_YUKON:
		board = new PysolYukonGameBoard(Display10AsT);
		break;
	case BOARD_ID_NONE:
	default:
		return NULL;
	}

	if ( board )
		board->Initilize();

	return board;
}

AFreecellGameBoard::AFreecellGameBoard( bool Display10AsT)
{
	m_Deck = NULL;
	m_Display10AsT = Display10AsT;
}

AFreecellGameBoard::AFreecellGameBoard()
{
	m_Deck = NULL;
	m_Display10AsT = false;
}


AFreecellGameBoard::~AFreecellGameBoard()
{
	if (m_Deck != NULL)
		delete [] m_Deck;
}

void AFreecellGameBoard::Initilize()
{
	//create a deck
	m_Deck = new char[GetNumberOfCardsInDeck()];

	for (int i=0;i<GetNumberOfCardsInDeck();i++)
		m_Deck[i] = MakeCard((i%13)+1, i/13);
}

int AFreecellGameBoard::GetNumberOfCardsInDeck()
{
	return 52;
}


char* AFreecellGameBoard::CardToString(char* String, char Card, bool AppendWhiteSpace, bool FlipCondition)
{
	if (FlipCondition)
		strcpy(String, "<");
	else
		String[0] = NULL;

	switch(GetValue(Card))
	{
	case 1:
		strcat(String, "A");
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		String[FlipCondition ? 1 : 0] = GetValue(Card) + '0';
		String[FlipCondition ? 2 : 1] = NULL;
		break;
	case 10:
		if (m_Display10AsT)
			strcat(String, "T");
		else
			strcat(String, "10");
		break;
	case 11:
		strcat(String, "J");
		break;
	case 12:
		strcat(String, "Q");
		break;
	case 13:
		strcat(String, "K");
		break;
	default:
		strcat(String, "***");
	}

	switch (GetSuit(Card))
	{
	case 0:
		strcat(String, "C");
		break;
	case 1:
		strcat(String, "D");
		break;
	case 2:
		strcat(String, "H");
		break;
	case 3:
		strcat(String, "S");
		break;
	default:
		strcat(String, "$$$");
	}

	if (FlipCondition)
		strcat(String, ">");

	if (AppendWhiteSpace)
		strcat(String, " ");

	return String;
}
