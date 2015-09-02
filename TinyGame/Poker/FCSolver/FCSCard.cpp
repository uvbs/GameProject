////////////////////////////////////////////////
///\file FCSCard.cpp
///\brief This file contains the FCSCard class implementation
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>
#include "FCSCard.h"
#include "FCDebugState.h"
#include "FCCompactState.h"
#include "FCIndirectState.h"

///String representation of cards with a 10 string
char CardMap10[14][4] = { " ", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
///String representation of cards with a T string
char CardMapT[14][4] = { " ", "A", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K" };

///Convert a letter to uppercase (toupper is too general)
#define MakeUpperCase(c) ( (((c)>='a') && ((c)<='z')) ?  ((c)+'A'-'a') : (c))

FCSCard* FCSCard::Create()
{
	switch(GlobalStateType)
	{
	case FC_DEBUG_STATE:
	case FC_TALON_DEBUG_STATE:
		return new FCSDebugCard();
	case FC_COMPACT_STATE:
	case FC_TALON_COMPACT_STATE:
		return new FCSCompactCard();
	case FC_INDIRECT_STATE:
	case FC_TALON_INDIRECT_STATE:
		return new FCSIndirectCard();
	default:
		return NULL;
	}
}

int FCSCard::Compare(FCSCard* Card)
{
	//compare card number
	if (GetCardNumber() > Card->GetCardNumber())
		return 1;
	else if (GetCardNumber() < Card->GetCardNumber())
		return -1;

	//compare suits
	if (GetSuit() > Card->GetSuit())
		return 1;
	else if (GetSuit() < Card->GetSuit())
		return -1;

	return 0;
}


void FCSCard::User2Perl(const char *String)
{
	SetFlipped(User2PerlFlippedStatus(String));
	SetCardNumber(User2PerlCardNumber(String));
	SetSuit(User2PerlSuit(String));
}
	
char* FCSCard::Perl2User(char* String, int Display10AsT, bool DisplayDebug)
{
	bool CardNumberIsNull;
	char* ReturnString = String;

	if (DisplayDebug && GetFlipped())
	{
		*String = '<';
		String++;
	}

	Perl2UserCardNumber(GetCardNumber(), String, &CardNumberIsNull, Display10AsT, GetFlipped(), DisplayDebug);

	// Notice that if card_num_is_null is found to be true
	// it will affect the output of the suit too.
	Perl2UserSuit(GetSuit(), String + strlen(String), &CardNumberIsNull, GetFlipped(), DisplayDebug);

	if (DisplayDebug && GetFlipped())
		strcat(String, ">");

	return ReturnString;
}

char* FCSCard::Perl2UserCardNumber(int Number, char* String, bool *CardNumberIsNull,
								int Display10AsT, int Flipped, bool DisplayDebug)
{
	char (*CardMap) [4] = CardMap10;
	if (Display10AsT)
		CardMap = CardMapT;

	if ((DisplayDebug) && Flipped)
	{
		strncpy(String, "*", 2);
		*CardNumberIsNull = false;
	}
	else if ((!DisplayDebug) && Flipped)
	{
		strncpy(String, " ", 2);
		*CardNumberIsNull = false;
	}
	else if (!Flipped)
	{
		if ((Number >= 0) && (Number <= 13))
		{
			strncpy(String, CardMap[Number], strlen(CardMap[Number])+1);
			*CardNumberIsNull = (Number == 0);
		}
		else
		{
			strncpy(String, CardMap[0], strlen(CardMap[0])+1);
			*CardNumberIsNull = true;
		}
	}

	return String;
}

char* FCSCard::Perl2UserSuit(int Suit, char* String, bool *CardNumberIsNull, int Flipped, bool DisplayDebug)
{
	if ((!DisplayDebug) && Flipped)
	{
		strncpy(String, "*", 2);
	}
	if (!Flipped)
	{
		switch(Suit)
		{
		case 0:
			if (*CardNumberIsNull)
			{
				if (DisplayDebug)
					strncpy(String, "*", 2);
				else
					strncpy(String, " ", 2);
			}
			else
				strncpy(String, "H", 2);
			break;
		case 1:
			strncpy(String, "C", 2);
			break;
		case 2:
			strncpy(String, "D", 2);
			break;
		case 3:
			strncpy(String, "S", 2);
			break;
		default:
			strncpy(String, " ", 2);
		}
	}

	return String;
}

int FCSCard::User2PerlCardNumber(const char* String)
{
	char Rest;

	while (1)
	{
		Rest = MakeUpperCase(*String);
		switch(Rest)
		{
		case '\0':
		case ' ':
		case '\t':
			return 0;
		case 'A':
			return 1;
		case 'J':
			return 11;
		case 'Q':
			return 12;
		case 'K':
			return 13;
		case '1':
			return (*(String+1) == '0')?10:1;
		case '0':
		case 'T':
			return 10;
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return Rest - '0';
		default:
			String++;
		}
	}
}


int FCSCard::User2PerlSuit(const char* String)
{
	char c = MakeUpperCase(*String);
	while (
			(c != 'H') &&
			(c != 'S') &&
			(c != 'D') &&
			(c != 'C') &&
			(c != ' ') &&
			(c != '\0'))
	{
		String++;
		c = MakeUpperCase(*String);
	}

	switch (c)
	{
	case 'H':
		return 0;
	case 'C':
		return 1;
	case 'D':
		return 2;
	case 'S':
		return 3;
	default:
		return 0;
	}
}

int FCSCard::User2PerlFlippedStatus(const char * String)
{
	while (*String != '\0')
	{
		if ((*String != ' ') && (*String != '\t'))
		{
			return (*String == '<');
		}
		String++;
	}
	return 0;
}