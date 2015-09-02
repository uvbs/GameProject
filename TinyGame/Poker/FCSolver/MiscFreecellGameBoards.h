#ifndef MISC_FREECELL_GAME_BOARDS_H
#define MISC_FREECELL_GAME_BOARDS_H

////////////////////////////////////////////////
///\file MiscFreecellGameBoards.h
///\brief This file contains classes that generate miscellaneous freecell-type games
///\author Michael Mann
///\version 1.0
///\date September 2002
////////////////////////////////////////////////

#include "AFreecellGameBoard.h"

///\brief Create a Microsoft freecell game
class MicrosoftFreecellGameBoard : public AFreecellGameBoard
{
public:
	///Constructor
	MicrosoftFreecellGameBoard(bool Display10AsT);
	///Destructor
	virtual ~MicrosoftFreecellGameBoard();

	///\brief Shuffles the deck of cards
	///
	///This is the same shuffling algorithm used to create Microsoft freecell games
	///\param Seed is the random number seed used in the shuffle
	virtual void Shuffle(int Seed);

	///\brief Deal a Microsoft freecell game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated freecell board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a Gnome freecell game
///
///Because the dealing is the same, this class is derived from MicrosoftFreecellGameBoard.
///The shuffle function has been modified to reflect Gnome game shuffling
class GnomeFreecellGameBoard : public MicrosoftFreecellGameBoard
{
public:
	///Constructor
	GnomeFreecellGameBoard(bool Display10AsT);
	///Destructor
	virtual ~GnomeFreecellGameBoard();

	///\brief Shuffles the deck of cards
	///
	///This is the same shuffling algorithm used to create Gnome freecell games
	///\param Seed is the random number seed used in the shuffle
	virtual void Shuffle(int Seed);
};

///\brief Create AisleRiot freecell-like games
///
///This class holds the shuffling algorithm for all AisleRiot games
class AisleRiotGameBoard : public AFreecellGameBoard
{
public:
	///Destructor
	virtual ~AisleRiotGameBoard();

/***************************
When I figure out what the algorithm for the UNIX srandom/random, I'll write
the appropriate functions for the shuffle algorithm
***************************/
	///\brief Shuffles the deck of cards
	///
	///This is the same shuffling algorithm used to create AisleRiot freecell-like games
	///\param Seed is the random number seed used in the shuffle
	virtual void Shuffle(int Seed);

	///\brief The size of a string needed to store the generated board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize() = 0;

protected:
	///Constructor
	AisleRiotGameBoard(bool Display10AsT);
	///Constructor
	AisleRiotGameBoard();
};

///\brief Create a AisleRiot freecell game
class AisleRiotFreecellGameBoard : public AisleRiotGameBoard
{
public:
	///Constructor
	AisleRiotFreecellGameBoard(bool Display10AsT);
	///Constructor
	virtual ~AisleRiotFreecellGameBoard();

	///\brief Deal a AisleRiot freecell game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated freecell board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a AisleRiot klondike game
class AisleRiotKlondikeGameBoard : public AisleRiotGameBoard
{
public:
	///Constructor
	AisleRiotKlondikeGameBoard(bool Display10AsT);
	///Destructor
	~AisleRiotKlondikeGameBoard();

	///\brief Deal a AisleRiot klondike game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated klondike board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a AisleRiot Eight Off game
class AisleRiotEightOffGameBoard : public AisleRiotGameBoard
{
public:
	///Constructor
	AisleRiotEightOffGameBoard(bool Display10AsT);
	///Destructor
	virtual ~AisleRiotEightOffGameBoard();

	///\brief Deal a AisleRiot eight off game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated eight off board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a AisleRiot Seahaven Towers game
class AisleRiotSeahavenTowersGameBoard : public AisleRiotGameBoard
{
public:
	///Constructor
	AisleRiotSeahavenTowersGameBoard(bool Display10AsT);
	///Destructor
	virtual ~AisleRiotSeahavenTowersGameBoard();

	///\brief Deal a AisleRiot seahaven towers game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated seahaven towers board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();
};

///\brief Create a AisleRiot Beleaguered Castle game
class AisleRiotBeleagueredCastleGameBoard : public AisleRiotGameBoard
{
public:
	///Constructor
	AisleRiotBeleagueredCastleGameBoard(bool Display10AsT);
	///Destructor
	virtual ~AisleRiotBeleagueredCastleGameBoard();

	/***************************
	When I figure out what the algorithm for the UNIX srandom/random, I'll write
	the appropriate functions for the shuffle algorithm
	***************************/
	///\brief Deal a AisleRiot beleaguered castle game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard);

	///\brief The size of a string needed to store the generated beleaguered castle board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize();

protected:
	///\brief Number of cards used in a beleaguered castle game (48)
	///
	///\return The number of cards used in a beleaguered castle game
	virtual int GetNumberOfCardsInDeck();
};


#endif