#ifndef FCS_CARD_H
#define FCS_CARD_H

////////////////////////////////////////////////
///\file FCSCard.h
///\brief This file contains the FCSCard class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCEnums.h"

///\brief This class represents card behavior
///
///This is an abstract class.  It provides functionality to turn cards into a string
///representation and vice versa.  It also provides pure virtual functions for the 
///behavior of a card
class FCSCard
{
public:
	///\brief Create a card
	///
	///\return Pointer to new card
	static FCSCard* Create();

	///\brief This function converts an entire card from its string representations
	///(e.g: "AH", "KS", "8D") to a FCSCard datatype
	///
	///\param String is the string to be converted
	void User2Perl(const char *String);
	
	///\brief Convert a card to its string representation.
	///
	///\param String will be the new string representation
	///\param Display10AsT whether 10 should be printed as T or not
	///\param DisplayDebug determines whether or not to display debug information
	///\return A pointer to the string
	char* Perl2User(char* String, int Display10AsT, bool DisplayDebug);

	///\biref Converts a card number from its internal representation to a string.
	///
	///\param Number is the card number
	///\param String is the string to output to.
	///\param CardNumberIsNull a pointer to a bool that indicates whether the 
	///card number is out of range or equal to zero
	///\param Display10AsT is whether 10 should be printed as T or not
	///\param Flipped is whether the card is flipped or not
	///\param DisplayDebug determines whether or not to display debug information
	///\return A pointer to the string
	static char* Perl2UserCardNumber(int Number, char* String, bool *CardNumberIsNull, 
								int Display10AsT, int Flipped, bool DisplayDebug);

	///\brief Converts a suit to its user representation.
	///
	///\param Suit is the card suit
	///\param String is the string to output to.
	///\param CardNumberIsNull a pointer to a bool that indicates whether the 
	///card number is out of range or equal to zero
	///\param Flipped is whether the card is flipped or not
	///\param DisplayDebug determines whether or not to display debug information
	///\return A pointer to the string
	char* Perl2UserSuit(int Suit, char* String, bool *CardNumberIsNull, int Flipped, 
						bool DisplayDebug);

	///\brief Converts a card number from a string (e.g: "A", "K", "9") to an integer
	///
	///\param String is the string to be converted
	///\return Card number as an integer
	static int User2PerlCardNumber(const char* String);

	///\brief Converts a string containing a suit letter (e.g: "H", "S","D", "C") 
	/// into its suit ID.  The suit letter may be after the start of the string
	///
	///\param String is the string to be converted
	///\return Suit as an integer
	static int User2PerlSuit(const char* String);

	///\brief Copy the value of a card into a card
	///
	///\param Card is the card to be copied
	virtual void Copy(FCSCard* Card) = 0;

	///\brief Compare a card to another
	///
	///\param Card to be compared to
	///\return -1 if this card < Card, 1 if card > Card, 0 if card = Card
	int Compare(FCSCard* Card);

	///\brief Make card have no value
	virtual void EmptyCard() = 0;

	///\brief Determines whether or not card is empty
	virtual bool IsEmptyCard() = 0;

	///\brief Retrieves card number
	///
	///\return Card Number of card
	virtual char GetCardNumber() = 0;

	///\brief Retrieves suit
	///
	///\return Suit of card
	virtual char GetSuit() = 0;

	///\brief Retrieves whether or not card is flipped (visible)
	///
	///\return Visibility of card
	virtual char GetFlipped() = 0;

	///\brief Set suit of card
	///
	///\param New suit of card
	virtual void SetSuit(char Suit) = 0;

	///\brief Set card number of card
	///
	///\param New card number of card
	virtual void SetCardNumber(char CardNumber) = 0;

	///\brief Set flippedness (visibility) of card
	///
	///\param New visibility of card
	virtual void SetFlipped(char Flipped) = 0;

	///\brief Get the size of FCSCard child class
	///
	///\return Size of state class
	virtual int GetClassSize() = 0;

protected:

	///\brief Converts flipped status of a card from a string to an integer
	///
	///\param String is the string to be converted
	///\return Flipped status as an integer
	int User2PerlFlippedStatus(const char * String);
};

#endif