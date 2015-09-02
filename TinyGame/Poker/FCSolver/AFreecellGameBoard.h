#ifndef AFREECELL_GAME_BOARD_H
#define AFREECELL_GAME_BOARD_H

////////////////////////////////////////////////
///\file AFreecellGameBoard.h
///\brief This file contains the AFreecellGameBoard class and all the game supporting data  
///\author Michael Mann
///\version 1.0
///\date September 2002
////////////////////////////////////////////////


#ifdef _MSC_VER
typedef __int64 integer64;
///needed for 64-bit conversion
#define INT64_CONSTANT(c) (c##i64)
#elif defined(__GNUC__)
typedef __int64_t integer64;
///needed for 64-bit conversion
#define INT64_CONSTANT(c) (c##ll)
#endif

///Maximum string length of a game
#define GAME_NAME_LENGTH			32

///This is all the games that are currently supported
enum BoardID 
{
	BOARD_ID_NONE = -1,
	BOARD_ID_BAKERS_DOZEN,
	BOARD_ID_BAKERS_GAME,
	BOARD_ID_BELEAGUERED_CASTLE,
	BOARD_ID_CITADEL,
	BOARD_ID_CRUEL,
	BOARD_ID_DER_KATZENSCHWANZ,
	BOARD_ID_DIE_SCHLANGE, 
	BOARD_ID_EIGHT_OFF,
	BOARD_ID_FAN,
	BOARD_ID_FORECELL,
	BOARD_ID_FREECELL,
	BOARD_ID_GNOME_FREECELL,
	BOARD_ID_GOOD_MEASURE,
	BOARD_ID_GYPSY,
	BOARD_ID_KINGS_ONLY_BAKERS_GAME,
	BOARD_ID_KLONDIKE,
	BOARD_ID_MICROSOFT_FREECELL,
	BOARD_ID_PYSOL_BAKERS_GAME,
	BOARD_ID_PYSOL_EIGHT_OFF,
	BOARD_ID_PYSOL_FREECELL,
	BOARD_ID_PYSOL_KINGS_ONLY_BAKERS_GAME,
	BOARD_ID_PYSOL_RELAXED_FREECELL,
	BOARD_ID_PYSOL_RELAXED_SEAHAVEN_TOWERS,
	BOARD_ID_PYSOL_SEAHAVEN_TOWERS,
	BOARD_ID_PYSOL_BELEAGUERED_CASTLE,
	BOARD_ID_PYSOL_KLONDIKE,
	BOARD_ID_PYSOL_STREETS_AND_ALLEYS,
	BOARD_ID_RELAXED_FREECELL,
	BOARD_ID_RELAXED_SEAHAVEN_TOWERS,
	BOARD_ID_SEAHAVEN_TOWERS,
	BOARD_ID_SIMPLE_SIMON,
	BOARD_ID_SMALL_HARP,
	BOARD_ID_STREETS_AND_ALLEYS,
	BOARD_ID_WHITEHEAD,
	BOARD_ID_YUKON
};

///\brief Structure to link a game name string with its enum value
typedef struct BoardName
{
	///Name of the game
	char m_Name[GAME_NAME_LENGTH];

	///Preset ID for the game
	BoardID m_BoardID;

} BoardName;

/*
 * Card:
 * Bits 0-3 - Card Number
 * Bits 4-5 - Suit
 * Bits 6   - Flipped
 */

///Convert card values into a char
#define MakeCard(value, suit) (((suit&0x03)<<4)+(value))
///Get the suit of a card
#define GetSuit(card) ((card >> 4) & 0x03)
///Get the face value of a card
#define GetValue(card) (card&0x0f)

///\brief The abstract, parent class for all game board generation
class AFreecellGameBoard
{
public:
	///Destructor
	virtual ~AFreecellGameBoard();

	void Initilize();

	///\brief Shuffles the deck of cards
	///
	///\param Seed is the random number seed used in the shuffle
	virtual void Shuffle(int Seed) = 0;

	///\brief Deal the cards of a game
	///
	///\param GameBoard is the string the output is put into
	virtual void Deal(char* GameBoard) = 0;
	
	///\brief The size of a string needed to store the generated board output
	///
	///\return Size of string needed
	virtual int GetGameBoardSize() = 0;

protected:
	///Constructor
	AFreecellGameBoard( bool Display10AsT );
	///Constructor
	AFreecellGameBoard();

	///\brief Number of cards used in a game
	///
	///\return The number of cards used in a game
	virtual int GetNumberOfCardsInDeck();

	///\brief Converts a char Card into a string
	///
	///\param String is the string created from the card
	///\param Card is the card to be converted
	///\param AppendWhiteSpace determines whether or not to add a space in the string after the card
	///\param FlipCondition is whether or not the card is visible on a deal
	///\return A pointer to the string
	virtual char* CardToString(char* String, char Card, bool AppendWhiteSpace, bool FlipCondition = false);

	///The deck of cards for the game
	char* m_Deck;
	///Display "10" as a "T" 
	bool m_Display10AsT;
};

///\brief Create a game board generator object
///
///\param GameName is the string name of the game to be created
///\param Display10AsT determines whether or not to display a 10 as "10" or "T"
///\return A pointer to the newly created object
AFreecellGameBoard* CreateAFreecellGameBoard(char* GameName, bool Display10AsT);


#endif