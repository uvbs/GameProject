#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "FCTalonState.h"
#include "FCSTalonStateWithLocations.h"

////////////////////////////////////////////////
///\file FCTalonState.cpp
///\brief This file contains the implementation of the base freecell state classes that have talons
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

/****************************
Need to figure out a way to link this with the same globals in FCState.cpp
*****************************/
///String freecell prefixes
const char * const FreecellsPrefixes[] = { "FC:", "Freecells:", "Freecell:", ""};
///String foundation prefixes
const char * const FoundationsPrefixes[] = { "Decks:", "Deck:", "Founds:", "Foundations:", "Foundation:", "Found:", ""};

///String talon prefixes
const char * const TalonPrefixes[] = { "Talon:", "Queue:" , ""};
///String numboer of redeal prefixes
const char * const NumberOfRedealsPrefixes[] = { "Num-Redeals:", "Redeals-Num:", "Redeals-Number:", ""};

#ifdef WIN32
#define strncasecmp(a,b,c) (strnicmp((a),(b),(c)))
#endif

FCSTalonStateData::FCSTalonStateData()
{
	memset(m_TalonParams, 0, sizeof(m_TalonParams));
}

FCSTalonStateData::~FCSTalonStateData()
{
}

void FCSTalonStateData::Copy(FCSTalonStateData* Data)
{
	memcpy(m_TalonParams, Data->m_TalonParams, 3);
	m_KlondikeTalonLength = Data->m_KlondikeTalonLength;
}

int FCSTalonStateData::Compare(const FCSTalonStateData* TalonData)
{
	if (m_KlondikeTalonLength < TalonData->m_KlondikeTalonLength)
		return -1;

	if (m_KlondikeTalonLength > TalonData->m_KlondikeTalonLength)
		return 1;

	return memcmp(m_TalonParams, TalonData->m_TalonParams, FC_TALON_NUMBER_OF_TALON_PARAMETERS);
}

void FCSTalonStateData::SetKlondikeNumberOfRedealsLeft(char RedealsLeft)
{
	m_TalonParams[FC_TALON_PARAM_KLONDIKE_NUMBER_OF_REDEALS_LEFT] = RedealsLeft;
}

char FCSTalonStateData::GetKlondikeNumberOfRedealsLeft()
{
	return m_TalonParams[FC_TALON_PARAM_KLONDIKE_NUMBER_OF_REDEALS_LEFT];
}

void FCSTalonStateData::SetTalonPosition(char Position)
{
	m_TalonParams[FC_TALON_PARAM_POSITION] = Position;
}

char FCSTalonStateData::GetTalonPosition()
{
	return m_TalonParams[FC_TALON_PARAM_POSITION];
}

void FCSTalonStateData::SetTalonLength(char Length)
{
	m_TalonParams[FC_TALON_PARAM_LENGTH] = Length;
}

char FCSTalonStateData::GetTalonLength()
{
	return m_TalonParams[FC_TALON_PARAM_LENGTH];
}

void FCSTalonStateData::SetKlondikeTalonLength(char Length)
{
	m_KlondikeTalonLength = Length;
}

char FCSTalonStateData::GetKlondikeTalonLength()
{
	return m_KlondikeTalonLength;
}

void FCSTalonStateData::SetKlondikeTalonStackPosition(char Position)
{
	m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION] = Position;
}

char FCSTalonStateData::GetKlondikeTalonStackPosition()
{
	return m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION];
}

void FCSTalonStateData::SetKlondikeTalonQueuePosition(char Position)
{
	m_TalonParams[FC_TALON_PARAM_KLONDIKE_QUEUE_POSITION] = Position;
}

char FCSTalonStateData::GetKlondikeTalonQueuePosition()
{
	return m_TalonParams[FC_TALON_PARAM_KLONDIKE_QUEUE_POSITION];
}

void FCSTalonStateData::KlondikeTalonRedealBare()
{
	m_TalonParams[FC_TALON_PARAM_KLONDIKE_STACK_POSITION] = -1;
	m_TalonParams[FC_TALON_PARAM_KLONDIKE_QUEUE_POSITION] = 0;
}


int MD5TalonStateDataHashAlgorithm::Hash(const FCSTalonStateData* key)
{
	unsigned char HashValue[MD5_HASHBYTES];
	int RealHashValue;

	//hash the talon cards
	m_MD5Hash.Init();
	m_MD5Hash.Update((unsigned char*)((FCSTalonStateData*)key)->GetTalon(0), ((FCSTalonStateData*)key)->GetTalon(0)->GetClassSize() * ((FCSTalonStateData*)key)->GetKlondikeTalonLength());
	m_MD5Hash.Final(HashValue);
	RealHashValue = (*(int*)HashValue);

	if (RealHashValue < 0)
	{
		// This is a bit mask that nullifies the sign bit of the
		// number so it will always be positive
		RealHashValue &= (~(1<<((sizeof(RealHashValue)<<3)-1)));
	}

	return RealHashValue;
}


FCSTalonStateWithLocations::FCSTalonStateWithLocations() : FCSStateWithLocations()
{
}

FCSTalonStateWithLocations::~FCSTalonStateWithLocations()
{
}

FCSStateWithLocations* FCSTalonStateWithLocations::CreateInitialState(const char *String, int NumberOfFreecells, 
													int NumberOfStacks, int NumberOfDecks, FCSTalonType TalonType)
{
	FCSTalonStateWithLocations* ReturnState;

	int DecksIndex[4], c, i;
	const char * Str;
	FCSCard* Card = FCSCard::Create();
	bool FirstLine = true,
		 PrefixFound;

	const char * const * Prefixes;

	ReturnState = (FCSTalonStateWithLocations*)CreateStateWithLocations();
	ReturnState->Initialize(NumberOfStacks);

	Str = String;

	if (TalonType == FCS_TALON_KLONDIKE)
		ReturnState->GetTalonData()->SetKlondikeNumberOfRedealsLeft(-1);

	for(int s=0;s<NumberOfStacks;s++)
	{
		// Move to the next stack
		if (!FirstLine)
		{
			while((*Str) != '\n')
				Str++;

			Str++;
		}
		FirstLine = false;

		Prefixes = FreecellsPrefixes;
		PrefixFound = false;
		for(i=0;Prefixes[i][0] != '\0'; i++)
		{
			if (!strncasecmp(Str, Prefixes[i], strlen(Prefixes[i])))
			{
				PrefixFound = true;
				Str += strlen(Prefixes[i]);
				break;
			}
		}

		if (PrefixFound)
		{
			for(c=0;c<NumberOfFreecells;c++)
			{
				ReturnState->EmptyFreecell(c);
			}
			for(c=0;c<NumberOfFreecells;c++)
			{
				if (c!=0)
				{
					while(
							((*Str) != ' ') && 
							((*Str) != '\t') && 
							((*Str) != '\n') && 
							((*Str) != '\r')
						 )
					{
						Str++;
					}
					if ((*Str == '\n') || (*Str == '\r'))
					{
						break;
					}
					Str++;
				}

				while ((*Str == ' ') || (*Str == '\t'))
				{
					Str++;
				}
				if ((*Str == '\r') || (*Str == '\n'))
					break;

				if ((*Str == '*') || (*Str == '-'))
				{
					Card->EmptyCard();
				}
				else
				{
					Card->User2Perl(Str);
				}

				ReturnState->PutCardInFreecell(c, Card);
			}

			while ((*Str != '\n') && (*Str != '\0'))
			{
				Str++;
			}
			s--;
			continue;
		}

		Prefixes = FoundationsPrefixes;
		PrefixFound = false;
		for(i=0;Prefixes[i][0] != '\0'; i++)
		{
			if (!strncasecmp(Str, Prefixes[i], strlen(Prefixes[i])))
			{
				PrefixFound = true;
				Str += strlen(Prefixes[i]);
				break;
			}
		}

		if (PrefixFound)
		{
			int d;

			for(d=0;d<NumberOfDecks*4;d++)
			{
				ReturnState->SetFoundation(d, 0);
			}

			for(d=0;d<4;d++)
			{
				DecksIndex[d] = 0;
			}
			while (1)
			{
				while((*Str == ' ') || (*Str == '\t'))
					Str++;
				if ((*Str == '\n') || (*Str == '\r'))
					break;
				d = FCSCard::User2PerlSuit(Str);
				Str++;
				while (*Str == '-')
					Str++;
				c = FCSCard::User2PerlCardNumber(Str);
				while (
						(*Str != ' ') && 
						(*Str != '\t') && 
						(*Str != '\n') && 
						(*Str != '\r')
					  )
				{
					Str++;
				}

				ReturnState->SetFoundation(DecksIndex[d]*4+d, c);
				DecksIndex[d]++;
				if (DecksIndex[d] >= NumberOfDecks)
				{
					DecksIndex[d] = 0;
				}
			}
			s--;
			continue;
		}

		Prefixes = TalonPrefixes;
		PrefixFound = false;
		for(i=0;Prefixes[i][0] != '\0'; i++)
		{
			if (!strncasecmp(Str, Prefixes[i], strlen(Prefixes[i])))
			{
				PrefixFound = true;
				Str += strlen(Prefixes[i]);
				break;
			}
		}

		if (PrefixFound)
		{
			// Input the Talon
			int TalonSize;

			TalonSize = MAX_NUM_DECKS*52+16;
			ReturnState->CreateTalons(TalonSize);
			ReturnState->GetTalonData()->SetTalonPosition(0);

			for(c=0 ; c < TalonSize ; c++)
			{
				// Move to the next card
				if (c!=0)
				{
					while(
						((*Str) != ' ') && 
						((*Str) != '\t') && 
						((*Str) != '\n') && 
						((*Str) != '\r')
					)
					{
						Str++;
					}
					if ((*Str == '\n') || (*Str == '\r'))
						break;
				}

				while ((*Str == ' ') || (*Str == '\t'))
					Str++;

				if ((*Str == '\n') || (*Str == '\r'))
					break;

				Card->User2Perl(Str);
				ReturnState->GetTalon(c)->Copy(Card);
			}
			ReturnState->GetTalonData()->SetTalonLength(c);

			if (TalonType == FCS_TALON_KLONDIKE)
			{
				ReturnState->GetTalonData()->SetKlondikeTalonLength(ReturnState->GetTalonData()->GetTalonLength());
				ReturnState->GetTalonData()->SetKlondikeTalonStackPosition(-1);
				ReturnState->GetTalonData()->SetKlondikeTalonQueuePosition(0);
			}

			s--;
			continue;
		}

		Prefixes = NumberOfRedealsPrefixes;
		PrefixFound = false;
		for(i=0;Prefixes[i][0] != '\0'; i++)
		{
			if (!strncasecmp(Str, Prefixes[i], strlen(Prefixes[i])))
			{
				PrefixFound = true;
				Str += strlen(Prefixes[i]);
				break;
			}
		}

		if (PrefixFound)
		{
			while ((*Str < '0') && (*Str > '9') && (*Str != '\n'))
				Str++;

			if (*Str != '\n')
			{
				int NumberOfRedeals = atoi(Str);
				if (TalonType == FCS_TALON_KLONDIKE)
				{
					ReturnState->GetTalonData()->SetKlondikeNumberOfRedealsLeft( (NumberOfRedeals < 0) ? (-1) : ((NumberOfRedeals > 127) ? 127 : NumberOfRedeals) );
				}
			}
			s--;
			continue;
		}

		for(c=0 ; c < MAX_NUM_CARDS_IN_A_STACK ; c++)
		{
			// Move to the next card
			if (c!=0)
			{
				while(
					((*Str) != ' ') && 
					((*Str) != '\t') && 
					((*Str) != '\n') && 
					((*Str) != '\r')
				)
				{
					Str++;
				}
				if ((*Str == '\n') || (*Str == '\r'))
				{
					break;
				}
			}

			while ((*Str == ' ') || (*Str == '\t'))
			{
				Str++;
			}
			if ((*Str == '\n') || (*Str == '\r'))
			{
				break;
			}
			Card->User2Perl(Str);

			ReturnState->PushCardIntoStack(s, Card);
		} 
	}

	//this is needed becuase the "card type" is dymanic and can't be just a local variable
	delete Card;

	return ReturnState;
}

int FCSTalonStateWithLocations::CheckStateValidity(int NumberOfFreecells, int NumberOfStacks, 
												   int NumberOfDecks, FCSCard** MisplacedCard, FCSTalonType TalonType)
{
	int Cards[4][14];
	int c, d;

	// Initialize all cards to 0
	for(d=0;d<4;d++)
		for(c=1;c<=13;c++)
			Cards[d][c] = 0;

	// Mark the cards in the decks
	for(d=0;d<NumberOfDecks*4;d++)
		for (c=1; c<=GetFoundation(d); c++)
			Cards[d%4][c]++;

	// Mark the cards in the freecells
	for(int f=0;f<NumberOfFreecells;f++)
		if (GetFreecellCardNumber(f) != 0)
			Cards[GetFreecellCardSuit(f)][GetFreecellCardNumber(f)]++;

	// Mark the cards in the stacks
	for(int s=0;s<NumberOfStacks;s++)
	{
		for (c=0; c<GetStackLength(s); c++)
		{
			if (GetStackCardNumber(s, c) == 0)
			{
				(*MisplacedCard) = FCSCard::Create();
				(*MisplacedCard)->EmptyCard();
				return 3;
			}
			Cards[GetStackCardSuit(s, c)][GetStackCardNumber(s, c)]++;
		}
	}

	// Mark the cards in the (gypsy) talon
	if ((TalonType == FCS_TALON_GYPSY) || (TalonType == FCS_TALON_KLONDIKE))
	{
		for(c = ((TalonType == FCS_TALON_GYPSY) ? GetTalonData()->GetTalonPosition() : 0) ; 
			c < ((TalonType == FCS_TALON_GYPSY) ? GetTalonData()->GetTalonLength() : GetTalonData()->GetKlondikeTalonLength()); 
			c++)
		{
			if (!GetTalon(c)->IsEmptyCard())
				Cards[GetTalon(c)->GetSuit()][GetTalon(c)->GetCardNumber()]++;
		}
	}

	// Now check if there are extra or missing cards
	for(d=0;d<4;d++)
	{
		for(c=1;c<=13;c++)
		{
			if (Cards[d][c] != NumberOfDecks)
			{
				(*MisplacedCard) = FCSCard::Create();
				(*MisplacedCard)->SetSuit(d);
				(*MisplacedCard)->SetCardNumber(c);
				return (Cards[d][c] < NumberOfDecks) ? 1 : 2;
			}
		}
	}

	return 0;
}

void FCSTalonStateWithLocations::StateAsString(char* String, FCSDebugDisplayInfo* DebugInfo)
{
	char Freecells[10], Decks[MAX_NUM_DECKS*4][10], StackCards[10];
	bool CardNumberIsNull;
	int MaxNumberOfCards, NumberOfCards, Length, b, s;

	FCSCard* TempCard;

	char Str2[128], Str3[128], *PtrStr2, *PtrStr3, *Str, a;

	int StackLocations[MAX_NUM_STACKS];
	int FreecellLocations[MAX_NUM_FREECELLS];

	if (DebugInfo->m_DisplayDebugOptions & DEBUG_CANONIZED_ORDER_OUTPUT)
	{
		for(a=0;a<DebugInfo->m_NumberOfStacks;a++)
			StackLocations[a] = a;

		for(a=0;a<DebugInfo->m_NumberOfFreecells;a++)
			FreecellLocations[a] = a;
	}
	else
	{
		for(a=0;a<DebugInfo->m_NumberOfStacks;a++)
			StackLocations[m_StackLocations[a]] = a;

		for(a=0;a<DebugInfo->m_NumberOfFreecells;a++)
			FreecellLocations[m_FreecellLocations[a]] = a;
	}

	for(a=0;a<DebugInfo->m_NumberOfDecks*4;a++)
	{
		FCSCard::Perl2UserCardNumber(GetFoundation(a), Decks[a], &CardNumberIsNull,
									(DebugInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T), 0, DebugInfo->m_DisplayDebug);
		if (Decks[a][0] == ' ')
			Decks[a][0] = '0';
	}

	Str = String;

	if (!(DebugInfo->m_DisplayDebugOptions & DEBUG_IS_OUTPUT_PARSEABLE))
	{
		for(a=0;a<((DebugInfo->m_NumberOfFreecells/4)+(DebugInfo->m_NumberOfFreecells%4 != 0));a++)
		{
			PtrStr2 = Str2;
			PtrStr3 = Str3;
			for(b=0;b<min(DebugInfo->m_NumberOfFreecells-a*4, 4);b++)
			{
				TempCard = GetFreecellCard(m_FreecellLocations[a*4+b]);
				PtrStr2 += sprintf(PtrStr2, "%3s ", 
					TempCard->Perl2User(Freecells, (DebugInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T),
										DebugInfo->m_DisplayDebug)
					);
				PtrStr3 += sprintf(PtrStr3, "--- ");
			}
			if (a < DebugInfo->m_NumberOfDecks)
			{
				Str += sprintf(Str, "%-16s        H-%1s C-%1s D-%1s S-%1s\n",
					Str2,
					Decks[a*4],
					Decks[a*4+1],
					Decks[a*4+2],
					Decks[a*4+3]);                    
			}
			else
			{
				Str += sprintf(Str, "%s\n", Str2);
			}
			Str += sprintf(Str, "%s\n", Str3);
		}
		for(;a<DebugInfo->m_NumberOfDecks;a++)
		{
			Str += sprintf(Str, "%-16s        H-%1s C-%1s D-%1s S-%1s\n",
					"",
					Decks[a*4],
					Decks[a*4+1],
					Decks[a*4+2],
					Decks[a*4+3]);    
		}
		*(Str++) = '\n';
		*(Str++) = '\n';
		*Str = '\0';

		for(s=0;s<DebugInfo->m_NumberOfStacks;s++)
		{
			Str += sprintf(Str, " -- ");
		}
		*(Str++) = '\n';
		*Str = '\0';

		MaxNumberOfCards = 0;
		for(s=0;s<DebugInfo->m_NumberOfStacks;s++)
			if (GetStackLength(StackLocations[s]) > MaxNumberOfCards)
				MaxNumberOfCards = GetStackLength(StackLocations[s]);

		for(NumberOfCards=0;NumberOfCards<MaxNumberOfCards;NumberOfCards++)
		{
			for(s = 0; s<DebugInfo->m_NumberOfStacks; s++)
			{
				if (NumberOfCards >= GetStackLength(StackLocations[s]))
				{
					strcpy(Str, "    ");
					Str += strlen(Str);
				}
				else
				{
					TempCard = GetStackCard(StackLocations[s], NumberOfCards);
					Str += sprintf(Str,	"%3s ", 
						TempCard->Perl2User(StackCards, (DebugInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T), 
											DebugInfo->m_DisplayDebug));                   
				}
			}
			strcpy(Str, "\n");
			Str += strlen(Str);
		}

		//output the talons
		if (DebugInfo->m_TalonType == FCS_TALON_KLONDIKE)
		{
			strcpy(Str, "\n");
			Str += strlen(Str);

			for (a = 0; a < GetTalonData()->GetKlondikeTalonLength(); a++)
			{
				TempCard = GetTalon(a);
				Str += sprintf(Str, "%3s ", TempCard->Perl2User(StackCards, (DebugInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T), 
											DebugInfo->m_DisplayDebug));
			}

			strcpy(Str, "\n");
			Str += strlen(Str);
		}
	}
	else
	{

		Str += sprintf(Str, "Foundations: ");
		for(a=0;a<DebugInfo->m_NumberOfDecks;a++)
		{
			Str += sprintf(Str,
				"H-%s C-%s D-%s S-%s ",
				Decks[a*4],
				Decks[a*4+1],
				Decks[a*4+2],
				Decks[a*4+3]
				);
		}
		Str += sprintf(Str, "\nFreecells: ");
    
		for(a=0;a<DebugInfo->m_NumberOfFreecells;a++)
		{
			TempCard = GetFreecellCard(FreecellLocations[a]);
			Str += sprintf(Str,	"%3s", 
				TempCard->Perl2User(Freecells, (DebugInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T), 
									DebugInfo->m_DisplayDebug)			
				);
			if (a < DebugInfo->m_NumberOfFreecells-1)
			{
				*(Str++) = ' ';
				*Str = '\0';
			}
		}

		*(Str++) = '\n';
		*Str = '\0';

		//output the talons
		if (DebugInfo->m_TalonType == FCS_TALON_KLONDIKE)
		{
			Str += sprintf(Str, "Talons: ");
			for (a = 0; a < GetTalonData()->GetKlondikeTalonLength(); a++)
			{
				TempCard = GetTalon(a);
				Str += sprintf(Str, "%3s ", TempCard->Perl2User(StackCards, (DebugInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T), 
											DebugInfo->m_DisplayDebug));
			}
		}

		*(Str++) = '\n';
		*Str = '\0';

		for(s=0;s<DebugInfo->m_NumberOfStacks;s++)
		{
			strcpy(Str, ": ");
			Str += strlen(Str);

			Length = GetStackLength(StackLocations[s]);
			for(NumberOfCards=0;NumberOfCards<Length;NumberOfCards++)
			{
				TempCard = GetStackCard(StackLocations[s], NumberOfCards);
				TempCard->Perl2User(StackCards, (DebugInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T),
									DebugInfo->m_DisplayDebug);

				strcpy(Str, StackCards);
				Str += strlen(Str);
				if (NumberOfCards < Length-1)
				{
					strcpy(Str, " ");
					Str += strlen(Str);
				}
			}
			strcpy(Str,"\n");
			Str += strlen(Str);
		}
	}
}

FCSCard* FCSTalonStateWithLocations::GetTalon(int Position)
{
	return GetTalonData()->GetTalon(Position);
}

FCSTalonStateData* CreateTalonStateData()
{
	switch (GlobalStateType)
	{
	case FC_TALON_DEBUG_STATE:
		return new FCSDebugTalonStateData();
	case FC_TALON_COMPACT_STATE:
		return new FCSCompactTalonStateData();
	case FC_TALON_INDIRECT_STATE:
		return new FCSIndirectTalonStateData();
	default:
		return NULL;
	}

	return NULL;
}
