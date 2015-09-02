#ifndef PYSOL_GAME_BOARDS_H
#define PYSOL_GAME_BOARDS_H

////////////////////////////////////////////////
///\file PysolGameBoards.h
///\brief This file contains the classes to create Pysol game boards
///\author Michael Mann
///\version 1.0
///\date September 2002
////////////////////////////////////////////////

#include "AFreecellGameBoard.h"

///\brief Generates a 64-bit random number
///
///This random function was derived from the python-based solitare game, PySol
///\param Seed is the seed for the random number generator
///\return Random number generated
double PysolRandom64(integer64* Seed);

///\brief Generates a 64-bit random number in int form
///
///This random function was derived from the python-based solitare game, PySol
///\param Seed is the seed for the random number generator
///\param LowerBound is the lower bound of the random number
///\param UpperBound is the upper bound of the random number
///\return Random number generated as an int
int PysolRandom64Int(integer64* Seed, int LowerBound, int UpperBound);

///\brief Create PySol freecell-like games
///
///This class holds the shuffling algorithm for all PySol games
class APysolFreecellGameBoard : public AFreecellGameBoard
{
public:
	///Destructor
	virtual ~APysolFreecellGameBoard();

	///\brief The size of a string needed to store the generated board output
	///
	///\return Size of string needed
	virtual void Shuffle(int Seed);

	///\brief The size of a string needed to store the generated board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize() = 0;

protected:
	///Constructor
	APysolFreecellGameBoard(bool Display10AsT);
	///Constructor
	APysolFreecellGameBoard();

	///\brief Converts a char Card into a string
	///
	///This differents from the parent class in that the suit values are different
	///\param String is the string created from the card
	///\param Card is the card to be converted
	///\param AppendWhiteSpace determines whether or not to add a space in the string after the card
	///\param FlipCondition is whether or not the card is visible on a deal
	///\return A pointer to the string
	virtual char* CardToString(char* String, char Card, bool AppendWhiteSpace, bool FlipCondition = false);
};

///\brief Create a PySol freecell game
class PysolFreecellGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolFreecellGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolFreecellGameBoard();

	///\brief Deal a PySol freecell game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated freecell board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Der Katzenschwantz game
class PysolDerKatzenschwantzGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolDerKatzenschwantzGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolDerKatzenschwantzGameBoard();

	///\brief Deal a PySol Der Katzenschwantz game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated der katzenschwantz board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();

	///\brief Number of cards used in a der katzenschwantz game (104 - 2 decks)
	///
	///\return The number of cards used in a der katzenschwantz game
	virtual int GetNumberOfCardsInDeck();
};

///\brief Create a PySol Die Schlange game
///
///This class is derived from der katzenschwantz because it contains 2 decks
class PysolDieSchlangeGameBoard : public PysolDerKatzenschwantzGameBoard
{
public:
	///Constructor
	PysolDieSchlangeGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolDieSchlangeGameBoard();

	///\brief Deal a PySol Die Schlange game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated die schlange board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Forecell game
class PysolForecellGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolForecellGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolForecellGameBoard();

	///\brief Deal a PySol forecell game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated forecell board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Eight Off game
class PysolEightOffGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolEightOffGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolEightOffGameBoard();

	///\brief Deal a PySol eight off game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated eight off board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Seahaven Towers game
class PysolSeahavenTowersGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolSeahavenTowersGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolSeahavenTowersGameBoard();

	///\brief Deal a PySol seahaven towers game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated seahaven towers board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Baker's Dozen game
class PysolBakersDozenGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolBakersDozenGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolBakersDozenGameBoard();

	///\brief Deal a PySol baker's dozen game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated baker's dozen board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Gypsy game
class PysolGypsyGameBoard : public PysolDerKatzenschwantzGameBoard
{
public:
	///Constructor
	PysolGypsyGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolGypsyGameBoard();

	///\brief Deal a PySol gypsy game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated gypsy board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Klondike game
class PysolKlondikeGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolKlondikeGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolKlondikeGameBoard();

	///\brief Deal a PySol klondike game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated klondike board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();

};

///\brief Create a PySol Whitehead game
class PysolWhiteheadGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolWhiteheadGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolWhiteheadGameBoard();

	///\brief Deal a PySol whitehead game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated whitehead board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Small Harp game
class PysolSmallHarpGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolSmallHarpGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolSmallHarpGameBoard();

	///\brief Deal a PySol small harp game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated small harp board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Simple Simon game
class PysolSimpleSimonGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolSimpleSimonGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolSimpleSimonGameBoard();

	///\brief Deal a PySol simple simon game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated simple simon board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Yukon game
class PysolYukonGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolYukonGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolYukonGameBoard();

	///\brief Deal a PySol yukon game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated yukon board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Beleaguered Castle game
class PysolBeleagueredCastleGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolBeleagueredCastleGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolBeleagueredCastleGameBoard();

	///\brief Deal a PySol beleaguered castle game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated beleaguered castle board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Citadel game
class PysolCitadelGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolCitadelGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolCitadelGameBoard();

	///\brief Deal a PySol citadel game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated citadel board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a PySol Fan game
class PysolFanGameBoard : public APysolFreecellGameBoard
{
public:
	///Constructor
	PysolFanGameBoard(bool Display10AsT);
	///Destructor
	virtual ~PysolFanGameBoard();

	///\brief Deal a PySol fan game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated fan board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

#endif