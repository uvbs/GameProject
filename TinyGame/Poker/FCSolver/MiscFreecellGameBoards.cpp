////////////////////////////////////////////////
///\file MiscFreecellGameBoards.cpp
///\brief This file contains the inplementatino of classes that generate miscellaneous freecell-type games
///\author Michael Mann
///\version 1.0
///\date September 2002
////////////////////////////////////////////////

#include <string.h>
#include "MiscFreecellGameBoards.h"

///\brief Create a random number in the same fashion that miccrosoft does
///
///\param Seed is the seed of the random number
///\return A random number
int MicrosoftRandom(long* Seed)
{
    *Seed = ((*Seed) * 214013 + 2531011);
    return ((*Seed) >> 16) & 0x7fff; 
}

MicrosoftFreecellGameBoard::MicrosoftFreecellGameBoard(bool Display10AsT) 
	: AFreecellGameBoard(Display10AsT)
{
}

MicrosoftFreecellGameBoard::~MicrosoftFreecellGameBoard()
{
}

void MicrosoftFreecellGameBoard::Shuffle(int Seed)
{
	int i, j,
		pos = 0,
		wLeft = GetNumberOfCardsInDeck();
	char* TempDeck = new char[GetNumberOfCardsInDeck()];

	//put the cards in "microsoft representation"
	for(i=0;i<13;i++)
	{
		for(j=0;j<4;j++)
		{
			TempDeck[pos] = m_Deck[i+j*13];
			pos++;
		}
	}

	for (i = 0; i < GetNumberOfCardsInDeck(); i++)
	{
		j = MicrosoftRandom((long*)&Seed) % wLeft;
		m_Deck[i] = TempDeck[j];
		TempDeck[j] = TempDeck[--wLeft];
	}

	delete [] TempDeck;
}

void MicrosoftFreecellGameBoard::Deal(char* GameBoard)
{
	char Output[9][30];
	char CardString[5];
	int i;

	for (i=0;i<GetNumberOfCardsInDeck();i++)
		Output[(i%8)+1][i/8] = m_Deck[i];

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

int MicrosoftFreecellGameBoard::GetGameBoardSize()
{
	return 160;
}


GnomeFreecellGameBoard::GnomeFreecellGameBoard(bool Display10AsT) : MicrosoftFreecellGameBoard(Display10AsT)
{
}

GnomeFreecellGameBoard::~GnomeFreecellGameBoard()
{
}

void GnomeFreecellGameBoard::Shuffle(int Seed)
{
	char* TempDeck = new char[GetNumberOfCardsInDeck()];
	int Index, i, j, pos = 0;
	int j_pos[4] = {13,26,0,-39};

	for (i = 0; i < GetNumberOfCardsInDeck(); i++)
	{
		Index = MicrosoftRandom((long*)&Seed) % (i+1);
		for (j = i; j > Index; j--)
			m_Deck[j] = m_Deck[j-1];
		m_Deck[Index] = 51 - i;
	}

	for (i = 0; i < GetNumberOfCardsInDeck(); i++)
	{
		Index = MicrosoftRandom((long*)&Seed) % (i+1);
		for (j = i; j > Index; j--)
			TempDeck[j] = TempDeck[j-1];
		TempDeck[Index] = m_Deck[51-i];
	}

	//need to format cards from "gnome representation" to work with CardToString()
	for (i = 0; i < GetNumberOfCardsInDeck(); i++)
		m_Deck[i] = MakeCard((TempDeck[51-i]%13)+1, (TempDeck[51-i])/13);

	delete [] TempDeck;
}

AisleRiotGameBoard::AisleRiotGameBoard(bool Display10AsT) : AFreecellGameBoard(Display10AsT)
{
}

AisleRiotGameBoard::AisleRiotGameBoard() : AFreecellGameBoard()
{
}

AisleRiotGameBoard::~AisleRiotGameBoard()
{
}

void AisleRiotGameBoard::Shuffle(int Seed)
{
	int i, j;
	char TempCard;

	for(i=0 ; i<GetNumberOfCardsInDeck() ; i++)
	{
		j = i + (MicrosoftRandom((long*)&Seed)%(GetNumberOfCardsInDeck()-i));
		TempCard = m_Deck[j];
		m_Deck[j] = m_Deck[i];
		m_Deck[i] = TempCard;
	}
}

AisleRiotFreecellGameBoard::AisleRiotFreecellGameBoard(bool Display10AsT) : AisleRiotGameBoard(Display10AsT)
{
}

AisleRiotFreecellGameBoard::~AisleRiotFreecellGameBoard()
{
}

void AisleRiotFreecellGameBoard::Deal(char* GameBoard)
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

int AisleRiotFreecellGameBoard::GetGameBoardSize()
{
	return 160;
}

AisleRiotKlondikeGameBoard::AisleRiotKlondikeGameBoard(bool Display10AsT) : AisleRiotGameBoard(Display10AsT)
{
}

AisleRiotKlondikeGameBoard::~AisleRiotKlondikeGameBoard()
{
}

void AisleRiotKlondikeGameBoard::Deal(char* GameBoard)
{
	char Output[8][30];
	char CardString[5];
	int CardNumber = 0,
		i, j;

	for(i = 0 ; i < 7 ; i++)
		Output[i][0] = '\0';

	CardNumber = 0;
	for(i = 0 ; i < 7 ; i++)
	{
		for(j = i ; j < 7 ; j++)
		{
			strcat(Output[j], CardToString(CardString, m_Deck[CardNumber], (j != i), (j != i)));
			CardNumber++;                            
		}
	}

	strcpy(GameBoard, "Talon: ");
	for(; CardNumber < GetNumberOfCardsInDeck(); CardNumber++)
		strcat(GameBoard, CardToString(CardString, m_Deck[CardNumber], (CardNumber != GetNumberOfCardsInDeck()-1)));
	strcat(GameBoard, "\n");

	for (i = 0; i < 7; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int AisleRiotKlondikeGameBoard::GetGameBoardSize()
{
	return 209;
}


AisleRiotEightOffGameBoard::AisleRiotEightOffGameBoard(bool Display10AsT) : AisleRiotGameBoard(Display10AsT)
{
}

AisleRiotEightOffGameBoard::~AisleRiotEightOffGameBoard()
{
}

void AisleRiotEightOffGameBoard::Deal(char* GameBoard)
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

int AisleRiotEightOffGameBoard::GetGameBoardSize()
{
	return 164;
}

AisleRiotSeahavenTowersGameBoard::AisleRiotSeahavenTowersGameBoard(bool Display10AsT) : AisleRiotGameBoard(Display10AsT)
{
}

AisleRiotSeahavenTowersGameBoard::~AisleRiotSeahavenTowersGameBoard()
{
}

void AisleRiotSeahavenTowersGameBoard::Deal(char* GameBoard)
{
	char Output[10][30];
	char CardString[5], Freecells[30];
	int i;

	for( i = 0 ; i < 10 ; i++)
		Output[i][0] = '\0';

	for( i = 0 ; i < 50 ; i++)
		strcat(Output[i % 10], CardToString(CardString, m_Deck[i], (i<50-10)));

	strcpy(Freecells, "FC: - ");

	for ( ; i < GetNumberOfCardsInDeck() ; i++)
		strcat(Freecells, CardToString(CardString, m_Deck[i], (i != GetNumberOfCardsInDeck()-1)));

	strcat(Freecells, "\n");
	strcpy(GameBoard, Freecells);

	for( i = 0 ; i < 10 ; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

int AisleRiotSeahavenTowersGameBoard::GetGameBoardSize()
{
	return 166;
}

AisleRiotBeleagueredCastleGameBoard::AisleRiotBeleagueredCastleGameBoard(bool Display10AsT)
{
	m_Deck = new char[GetNumberOfCardsInDeck()];
	m_Display10AsT = Display10AsT;

	//make a beleagured castle deck
	int CardNumber = 0;
	for(int Suit=0;Suit<4;Suit++)
	{
		for(int Value = 2; Value<=13; Value++)
		{
			m_Deck[CardNumber] = MakeCard(Value, Suit);
			CardNumber++;
		}
	}

}

AisleRiotBeleagueredCastleGameBoard::~AisleRiotBeleagueredCastleGameBoard()
{
}

///Make the deal more like PySol?
#define IndexProgToPysol(i) ((((i)%2) == 1)*4 + ((i/2)%4))

void AisleRiotBeleagueredCastleGameBoard::Deal(char* GameBoard)
{
	char Output[8][100];
	char CardString[5];
	int i;

	for (i = 0; i < 8; i++)
		Output[i][0] = '\0';

	for (i = 0; i < GetNumberOfCardsInDeck(); i++) 
		strcat(Output[IndexProgToPysol(i % 8)], CardToString(CardString, m_Deck[i], (i<GetNumberOfCardsInDeck()-8)));

	strcpy(GameBoard, "Foundations: H-A C-A D-A S-A\n");

	for (i = 0; i < 8; i++)
	{
		strcat(Output[i], "\n");
		strcat(GameBoard, Output[i]);
	}
}

#undef IndexProgToPysol

int AisleRiotBeleagueredCastleGameBoard::GetGameBoardSize()
{
	return 177;
}

int AisleRiotBeleagueredCastleGameBoard::GetNumberOfCardsInDeck()
{
	return 48;
}
