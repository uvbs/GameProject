////////////////////////////////////////////////
///\file PysolGameBoards.cpp
///\brief This file contains the implementation of the classes to create Pysol game boards
///\author Michael Mann
///\version 1.0
///\date September 2002
////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include "PysolGameBoards.h"

double PysolRandom64(integer64* Seed)
{
	(*Seed) = ((*Seed)*INT64_CONSTANT(6364136223846793005) + INT64_CONSTANT(1)) & INT64_CONSTANT(0xffffffffffffffff);
    return (((*Seed) >> 21) & INT64_CONSTANT(0x7fffffff)) / 2147483648.0;
}

int PysolRandom64Int(integer64* Seed, int LowerBound, int UpperBound)
{
    return (LowerBound + (int)(PysolRandom64(Seed) * (UpperBound+1-LowerBound)));
}

APysolFreecellGameBoard::APysolFreecellGameBoard(bool Display10AsT) : AFreecellGameBoard(Display10AsT)
{
}

APysolFreecellGameBoard::APysolFreecellGameBoard() : AFreecellGameBoard()
{
}

APysolFreecellGameBoard::~APysolFreecellGameBoard()
{
}

void APysolFreecellGameBoard::Shuffle(int Seed)
{
	int j, n = GetNumberOfCardsInDeck()-1;
	char* TempDeck = new char[GetNumberOfCardsInDeck()];
	char TempCard;


	//create a deck for temp deck
	for (int i=0;i<GetNumberOfCardsInDeck();i++)
		TempDeck[i] = MakeCard((i%13)+1, i/13);

	//type casting int* to integer64* doesn't work, so I'm making the int into a integer64
	integer64 Seed64 = Seed;

	while (n > 0)
	{
		j = PysolRandom64Int(&Seed64, 0, n);
		TempCard = TempDeck[n];
		TempDeck[n] = TempDeck[j];
		TempDeck[j] = TempCard;
		n--;
	}

	for(j=0;j<GetNumberOfCardsInDeck();j++)
		m_Deck[j] = TempDeck[GetNumberOfCardsInDeck()-j-1];

	delete [] TempDeck;
}

char* APysolFreecellGameBoard::CardToString(char* String, char Card, bool AppendWhiteSpace, bool FlipCondition)
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
		strcat(String, "S");
		break;
	case 2:
		strcat(String, "H");
		break;
	case 3:
		strcat(String, "D");
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


PysolFreecellGameBoard::PysolFreecellGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolFreecellGameBoard::~PysolFreecellGameBoard()
{
}

void PysolFreecellGameBoard::Deal(char* GameBoard)
{
	char Output[8][30];
	char CardString[5];
	int i;

	for (i = 0; i < 8; i++)
		Output[i][0] = '\0';

	for (i = 0; i < GetNumberOfCardsInDeck(); i++) 
		strcat(Output[i%8], CardToString(CardString, m_Deck[i], (i<GetNumberOfCardsInDeck()-8)));

	for (i = 0; i < 8; i++) 
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolFreecellGameBoard::GetGameBoardSize()
{
	return 160;
}

PysolDerKatzenschwantzGameBoard::PysolDerKatzenschwantzGameBoard(bool Display10AsT)
{
	m_Deck = new char[GetNumberOfCardsInDeck()];
	m_Display10AsT = Display10AsT;

	int CardNumber = 0;
	for (int i=0;i<2;i++)
	{
		for (int Suit = 0; Suit<4; Suit++)
		{
			for (int Value = 1; Value<=13;Value++)
			{
				m_Deck[CardNumber] = MakeCard(Value, Suit);
				CardNumber++;
			}
		}
	}
}

PysolDerKatzenschwantzGameBoard::~PysolDerKatzenschwantzGameBoard()
{
}

void PysolDerKatzenschwantzGameBoard::Deal(char* GameBoard)
{
	GameBoard[0] = NULL;
	char CardString[5];

	for (int i = 0; i < GetNumberOfCardsInDeck(); i++) 
	{
		if (GetValue(m_Deck[i]) == 13)
			strcat(GameBoard, "\n");

		strcat(GameBoard, CardToString(CardString, m_Deck[i], true));	
	}
}

int PysolDerKatzenschwantzGameBoard::GetGameBoardSize()
{
	return 328;
}

int PysolDerKatzenschwantzGameBoard::GetNumberOfCardsInDeck()
{
	return 104;
}

PysolDieSchlangeGameBoard::PysolDieSchlangeGameBoard(bool Display10AsT) : PysolDerKatzenschwantzGameBoard(Display10AsT)
{
}

PysolDieSchlangeGameBoard::~PysolDieSchlangeGameBoard()
{
}

void PysolDieSchlangeGameBoard::Deal(char* GameBoard)
{
	char CardString[5];

	strcpy(GameBoard, "Foundations: H-A S-A D-A C-A H-A S-A D-A C-A\n");
	for (int i = 0; i < GetNumberOfCardsInDeck(); i++) 
	{
		if (GetValue(m_Deck[i]) == 13)
			strcat(GameBoard, "\n");
		
		if (GetValue(m_Deck[i]) != 1)
			strcat(GameBoard, CardToString(CardString, m_Deck[i], true));
	}
}

int PysolDieSchlangeGameBoard::GetGameBoardSize()
{
	return 349;
}

PysolForecellGameBoard::PysolForecellGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolForecellGameBoard::~PysolForecellGameBoard()
{
}

void PysolForecellGameBoard::Deal(char* GameBoard)
{
	char Output[8][30];
	char CardString[5], Freecells[30];
	int i;

	/* Freecell */
	for (i = 0; i < 8; i++) 
		Output[i][0] = '\0';

	for (i = 0; i < 48; i++) 
		strcat(Output[i % 8], CardToString(CardString, m_Deck[i], (i<48-8)));

	strcpy(Freecells, "FC: ");
	for ( ; i < GetNumberOfCardsInDeck() ; i++)
		strcat(Freecells, CardToString(CardString, m_Deck[i], (i != GetNumberOfCardsInDeck()-1)));

	strcat(Freecells, "\n");
	strcpy(GameBoard, Freecells);

	for (i = 0; i < 8; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolForecellGameBoard::GetGameBoardSize()
{
	return 164;
}

PysolEightOffGameBoard::PysolEightOffGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolEightOffGameBoard::~PysolEightOffGameBoard()
{
}

void PysolEightOffGameBoard::Deal(char* GameBoard)
{
	char Output[8][30];
	char CardString[5], Freecells[30];
	int i;

	/* Freecell */
	for (i = 0; i < 8; i++) 
		Output[i][0] = '\0';

	for (i = 0; i < 48; i++) 
		strcat(Output[i % 8], CardToString(CardString, m_Deck[i], (i<48-8)));

	strcpy(Freecells, "FC: ");
	for ( ; i < GetNumberOfCardsInDeck() ; i++)
	{
		strcat(Freecells, CardToString(CardString, m_Deck[i], true));
		strcat(Freecells, "- ");
	}

	strcat(Freecells, "\n");
	strcpy(GameBoard, Freecells);

	for (i = 0; i < 8; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolEightOffGameBoard::GetGameBoardSize()
{
	return 173;
}

PysolSeahavenTowersGameBoard::PysolSeahavenTowersGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolSeahavenTowersGameBoard::~PysolSeahavenTowersGameBoard()
{
}

void PysolSeahavenTowersGameBoard::Deal(char* GameBoard)
{
	char Output[10][30];
	char CardString[5], Freecells[30];
	int i;

	for( i = 0 ; i < 10 ; i++)
		Output[i][0] = '\0';

	for( i = 0 ; i < 50 ; i++)
		strcat(Output[i % 10], CardToString(CardString, m_Deck[i], (i<50-10)));

	strcpy(Freecells, "FC: - ");

	for ( ; i < GetNumberOfCardsInDeck(); i++)
		strcat(Freecells, CardToString(CardString, m_Deck[i], (i != GetNumberOfCardsInDeck()-1)));

	strcat(Freecells, "\n");
	strcpy(GameBoard, Freecells);

	for( i = 0 ; i < 10 ; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolSeahavenTowersGameBoard::GetGameBoardSize()
{
	return 166;
}

PysolBakersDozenGameBoard::PysolBakersDozenGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolBakersDozenGameBoard::~PysolBakersDozenGameBoard()
{
}

void PysolBakersDozenGameBoard::Deal(char* GameBoard)
{
	int KingsCount, i;
	char TempCard, 
		 Output[13][30], 
		 CardString[5];

	for (i = 0; i < 13; i++)
	{
		KingsCount = 0;

		for (int j = 0; j<4; j++)
		{
			if (GetValue(m_Deck[j*13+i]) == 13)
			{
				//swap cards
				TempCard = m_Deck[KingsCount*13+i];
				m_Deck[KingsCount*13+i] = m_Deck[j*13+i];
				m_Deck[j*13+i] = TempCard;
				KingsCount++;
			}
		}
	}

	for (i=0;i<13;i++)
		Output[i][0] = NULL;
	
	for (i = GetNumberOfCardsInDeck()-1; i >=0 ;i--)
		strcat(Output[i%13], CardToString(CardString, m_Deck[i], (i>12)));

	for (i=12;i>=0;i--)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolBakersDozenGameBoard::GetGameBoardSize()
{
	return 160;
}

PysolGypsyGameBoard::PysolGypsyGameBoard(bool Display10AsT) : PysolDerKatzenschwantzGameBoard(Display10AsT)
{
}

PysolGypsyGameBoard::~PysolGypsyGameBoard()
{
}

void PysolGypsyGameBoard::Deal(char* GameBoard)
{
	int i;
	char Output[8][30], 
		 CardString[6];

	for (i = 0; i < 8; i++)
		Output[i][0] = '\0';

	for (i = 0; i<24; i++)
		strcat(Output[i%8], CardToString(CardString, m_Deck[i], true, i<16));

	strcpy(GameBoard, "Talon: ");
	for (;i<8*13;i++)
		strcat(GameBoard, CardToString(CardString, m_Deck[i], true));
	
	strcat(GameBoard, "\n");
	for (i=0;i<8;i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolGypsyGameBoard::GetGameBoardSize()
{
	return 368;
}

PysolKlondikeGameBoard::PysolKlondikeGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolKlondikeGameBoard::~PysolKlondikeGameBoard()
{
}

void PysolKlondikeGameBoard::Deal(char* GameBoard)
{
	int CardNumber = 0,
		i, j;
	char Output[7][35], 
		 CardString[6];

	for (i = 0; i < 7; i++)
		Output[i][0] = '\0';

	for (i=1;i<7;i++)
	{
		for (j = 0 ; j < 7-i;j++)
		{
			strcat(Output[j], CardToString(CardString, m_Deck[CardNumber], true, true));
			CardNumber++;
		}
	}
	
	for (i = 0;i<7;i++)
	{
		strcat(Output[i], CardToString(CardString, m_Deck[CardNumber], false));
		CardNumber++;
	}

	strcpy(GameBoard, "Talon: ");
	while (CardNumber < 52)
	{
		strcat(GameBoard, CardToString(CardString, m_Deck[CardNumber], true));
		CardNumber++;
	}

	//reverse output
	strcat(GameBoard, "\n");
	for (i=6;i>=0;i--)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolKlondikeGameBoard::GetGameBoardSize()
{
	return 210;
}

PysolWhiteheadGameBoard::PysolWhiteheadGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolWhiteheadGameBoard::~PysolWhiteheadGameBoard()
{
}

void PysolWhiteheadGameBoard::Deal(char* GameBoard)
{
	int CardNumber = 0,
		i, j;
	char Output[7][35], 
		 CardString[6];

	for (i = 0; i < 7; i++)
		Output[i][0] = '\0';

	for (i=1;i<7;i++)
	{
		for (j = 0 ; j < 7-i;j++)
		{
			strcat(Output[j], CardToString(CardString, m_Deck[CardNumber], true));
			CardNumber++;
		}
	}
	
	for (i = 0;i<7;i++)
	{
		strcat(Output[i], CardToString(CardString, m_Deck[CardNumber], false));
		CardNumber++;
	}

	strcpy(GameBoard, "Talon: ");
	while (CardNumber < 52)
	{
		strcat(GameBoard, CardToString(CardString, m_Deck[CardNumber], true));
		CardNumber++;
	}

	//reverse output
	strcat(GameBoard, "\n");
	for (i=6;i>=0;i--)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolWhiteheadGameBoard::GetGameBoardSize()
{
	return 168;
}

PysolSmallHarpGameBoard::PysolSmallHarpGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolSmallHarpGameBoard::~PysolSmallHarpGameBoard()
{
}

void PysolSmallHarpGameBoard::Deal(char* GameBoard)
{
	int CardNumber = 0,
		i, j;
	char Output[7][35], 
		 CardString[6];

	for (i = 0; i < 7; i++)
		Output[i][0] = '\0';

	for (i=1;i<7;i++)
	{
		for (j = 0 ; j < 7-i;j++)
		{
			strcat(Output[j], CardToString(CardString, m_Deck[CardNumber], true, true));
			CardNumber++;
		}
	}
	
	for (i = 0;i<7;i++)
	{
		strcat(Output[i], CardToString(CardString, m_Deck[CardNumber], false));
		CardNumber++;
	}

	strcpy(GameBoard, "Talon: ");
	while (CardNumber < 52)
	{
		strcat(GameBoard, CardToString(CardString, m_Deck[CardNumber], true));
		CardNumber++;
	}

	strcat(GameBoard, "\n");
	for (i=0;i<7;i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolSmallHarpGameBoard::GetGameBoardSize()
{
	return 210;
}

PysolSimpleSimonGameBoard::PysolSimpleSimonGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolSimpleSimonGameBoard::~PysolSimpleSimonGameBoard()
{
}

void PysolSimpleSimonGameBoard::Deal(char* GameBoard)
{
	int CardNumber = 0,
		i, j;
	char Output[10][30], 
		 CardString[6];

	for (i = 0; i < 10; i++)
		Output[i][0] = '\0';

	for (i = 9; i > 2;i--)
	{
		for (j = 0; j < i;j++)
		{
			strcat(Output[j], CardToString(CardString, m_Deck[CardNumber], true));
			CardNumber++;
		}
	}

	for (i = 0; i < 10;i++)
	{
		strcat(Output[i], CardToString(CardString, m_Deck[CardNumber], false));
		CardNumber++;
	}

	for (i=0;i<10;i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}

}

int PysolSimpleSimonGameBoard::GetGameBoardSize()
{
	return 160;
}

PysolYukonGameBoard::PysolYukonGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolYukonGameBoard::~PysolYukonGameBoard()
{
}

void PysolYukonGameBoard::Deal(char* GameBoard)
{
	int CardNumber = 0,
		i, j;
	char Output[7][50], 
		 CardString[6];

	for (i = 0; i < 7; i++)
		Output[i][0] = '\0';

	for (i = 1; i < 7; i++)
	{
		for (j = i; j < 7;j++)
		{
			strcat(Output[j], CardToString(CardString, m_Deck[CardNumber], true, true));
			CardNumber++;
		}
	}

	for (i = 0; i < 4;i++)
	{
		for (j = 1; j < 7; j++)
		{
			strcat(Output[j], CardToString(CardString, m_Deck[CardNumber], true, false));
			CardNumber++;
		}
	}

	for (i = 0; i < 7;i++)
	{
		strcat(Output[i], CardToString(CardString, m_Deck[CardNumber], true, false));
		CardNumber++;
	}

	for (i=0;i<7;i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
	
}

int PysolYukonGameBoard::GetGameBoardSize()
{
	return 209;
}

PysolBeleagueredCastleGameBoard::PysolBeleagueredCastleGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolBeleagueredCastleGameBoard::~PysolBeleagueredCastleGameBoard()
{
}

void PysolBeleagueredCastleGameBoard::Deal(char* GameBoard)
{
	char Output[8][30];
	char CardString[5];
	int i, j;

	for (i = 0; i < 8; i++)
		Output[i][0] = '\0';

	//take the aces out
	for (i = 0; i < GetNumberOfCardsInDeck(); i++)
	{
		if (GetValue(m_Deck[i]) == 1)
		{
			for (j = i; j < GetNumberOfCardsInDeck()-1; j++)
			{
				m_Deck[j] = m_Deck[j+1];
			}
			i--;
		}
	}

	//just to make sure that the final 4 "cards" aren't used;
	memset(&m_Deck[48], 0, 4);

	for (i = 0; i < 48; i++) 
		strcat(Output[i % 8], CardToString(CardString, m_Deck[i], (i<GetNumberOfCardsInDeck()-8)));

	//make the foundation string
	strcpy(GameBoard, "Foundations: H-A C-A D-A S-A\n");

	for (i = 0; i < 8; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolBeleagueredCastleGameBoard::GetGameBoardSize()
{
	return 181;
}

PysolCitadelGameBoard::PysolCitadelGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolCitadelGameBoard::~PysolCitadelGameBoard()
{
}

void PysolCitadelGameBoard::Deal(char* GameBoard)
{
	char Output[8][30], CardString[5], Foundations[4];
	char FoundationSuit[4] = {2, 0 ,3, 1};
	int Length, i, j;

	for (i = 0; i < 8; i++)
		Output[i][0] = '\0';

	//take the aces out
	for (i = 0; i < GetNumberOfCardsInDeck(); i++)
	{
		if (GetValue(m_Deck[i]) == 1)
		{
			for (j = i; j < GetNumberOfCardsInDeck()-1; j++)
			{
				m_Deck[j] = m_Deck[j+1];
			}
			i--;
		}
	}

	//just to make sure that the final 4 "cards" aren't used;
	memset(&m_Deck[48], 0, 4);

	//put the aces in the foundation
	memset(Foundations, 1, 4);

	for (i = 0; i < 48; i++)
	{
		if (GetValue(m_Deck[i]) == Foundations[GetSuit(m_Deck[i])]+1)
		{
			Foundations[GetSuit(m_Deck[i])]++;
		}
		else
		{
			strcat(Output[i % 8], CardToString(CardString, m_Deck[i], (i<GetNumberOfCardsInDeck()-8)));
		}
	}

	//Add the foundation cards
	strcpy(GameBoard, "Foundations: ");
	for (i = 0; i<sizeof(FoundationSuit);i++)
	{
		switch(FoundationSuit[i])
		{
		case 0:
			strcat(GameBoard, "C-");
			break;
		case 1:
			strcat(GameBoard, "S-");
			break;
		case 2:
			strcat(GameBoard, "H-");
			break;
		case 3:
			strcat(GameBoard, "D-");
			break;
		}

		switch(Foundations[FoundationSuit[i]])
		{
		case 1:
			strcat(GameBoard, "A");
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			Length = strlen(GameBoard);
			GameBoard[Length] = Foundations[FoundationSuit[i]] + '0';
			GameBoard[Length+1] = NULL;
			break;
		case 10:
			if (m_Display10AsT)
				strcat(GameBoard, "T");
			else
				strcat(GameBoard, "10");
			break;
		case 11:
			strcat(GameBoard, "J");
			break;
		case 12:
			strcat(GameBoard, "Q");
			break;
		case 13:
			strcat(GameBoard, "K");
			break;
		default:
			strcat(GameBoard, "***");
		}

		if (FoundationSuit[i] != 1)
			strcat(GameBoard, " ");
	}

	strcat(GameBoard, "\n");
	for (i = 0; i < 8; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int PysolCitadelGameBoard::GetGameBoardSize()
{
	return 181;
}

PysolFanGameBoard::PysolFanGameBoard(bool Display10AsT) : APysolFreecellGameBoard(Display10AsT)
{
}

PysolFanGameBoard::~PysolFanGameBoard()
{
}

void PysolFanGameBoard::Deal(char* GameBoard)
{
	char Output[18][15];
	char CardString[5];
	int i;

	for (i = 0; i < 18; i++)
		Output[i][0] = '\0';

	for (i = 0; i<GetNumberOfCardsInDeck()-1;i++)
		strcat(Output[i%17], CardToString(CardString, m_Deck[i], i < 2*17));

	strcpy(Output[17], CardToString(CardString, m_Deck[GetNumberOfCardsInDeck()-1], false));

	GameBoard[0] = NULL;
	for (i = 0; i < 18; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}

}

int PysolFanGameBoard::GetGameBoardSize()
{
	return 160;
}
