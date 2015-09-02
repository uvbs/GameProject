////////////////////////////////////////////////
///\file FCState.cpp
///\brief This file contains the implementation of the base freecell state classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "FCState.h"
#include "FCTalonState.h"
#include "FCDebugState.h"
#include "FCCompactState.h"
#include "FCIndirectState.h"
#include "FCSDebugStateWithLocations.h"
#include "FCSCompactStateWithLocations.h"
#include "FCSIndirectStateWithLocations.h"
#include "FCSTalonStateWithLocations.h"

///The state type currently being used by the solver
FCStateType GlobalStateType = FC_DEBUG_STATE;

#ifdef min
#undef min
#endif
///The minimum of two values
#define min(a,b) ((a)<(b)?(a):(b))

///String freecell prefixes
const char * const FreecellsPrefixes[] = { "FC:", "Freecells:", "Freecell:", ""};
///String foundation prefixes
const char * const FoundationsPrefixes[] = { "Decks:", "Deck:", "Founds:", "Foundations:", "Foundation:", "Found:", ""};

#ifdef WIN32
#define strncasecmp(a,b,c) (strnicmp((a),(b),(c)))
#endif

FCSState::FCSState()
{
}

FCSState::~FCSState()
{
}

FCSStateWithLocations::FCSStateWithLocations()
{
char a;

	m_Depth = 0;
	m_Visited = 0;
	m_VisitIterations = 0;
	m_Parent = NULL;
	m_MovesToParent = NULL;

	for(a=0;a<MAX_NUM_STACKS;a++)
	{
		m_StackLocations[a] = a;
	}
	for(a=0;a<MAX_NUM_FREECELLS;a++)
	{
		m_FreecellLocations[a] = a;
	}
}

FCSStateWithLocations::~FCSStateWithLocations()
{
}

void FCSStateWithLocations::Copy(FCSStateWithLocations* State)
{
	memcpy(m_StackLocations, State->m_StackLocations, MAX_NUM_STACKS);
	m_Parent = State->m_Parent;
	m_MovesToParent = State->m_MovesToParent;
	memcpy(m_FreecellLocations, State->m_FreecellLocations, MAX_NUM_FREECELLS);
	m_Depth = State->m_Depth;
	m_Visited = State->m_Visited;
	m_VisitIterations = State->m_VisitIterations;
}

FCSStateWithLocations* FCSStateWithLocations::CreateInitialState(const char *String, 
											int NumberOfFreecells, int NumberOfStacks, 
											int NumberOfDecks)
{
	FCSStateWithLocations* ReturnState;

	int DecksIndex[4], c, i;
	const char * Str;
	FCSCard* Card = FCSCard::Create();
	bool FirstLine = true,
		 PrefixFound;

	const char * const * Prefixes;

	ReturnState = CreateStateWithLocations();
	ReturnState->Initialize(NumberOfStacks);
	
	Str = String;

	for(int s=0;s<NumberOfStacks;s++)
	{
		// Move to the next stack
		if (!FirstLine)
		{
			while((*Str) != '\n')
			{
				Str++;
			}
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

int FCSStateWithLocations::CheckStateValidity(int NumberOfFreecells, int NumberOfStacks, int NumberOfDecks, 
											  FCSCard** MisplacedCard, FCSTalonType TalonType)
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

void FCSStateWithLocations::StateAsString(char* String, FCSDebugDisplayInfo* DebugInfo)
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
		for(a=0;a<((DebugInfo->m_NumberOfFreecells/4)+(DebugInfo->m_NumberOfFreecells%4 !=0));a++)
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

void FCSStateWithLocations::CleanState()
{
}

void FCSStateWithLocations::CacheStacks(AFCSGenericStackStorage* Storage, int StackNumber)
{
}

void FCSStateWithLocations::Initialize(int NumberOfStacks)
{
}

int GetFCSStateWithLocationsClassSize()
{
	switch(GlobalStateType)
	{
	case FC_DEBUG_STATE:
		return sizeof(FCSDebugStateWithLocations<FCSStateWithLocations>);
	case FC_COMPACT_STATE:
		return sizeof(FCSCompactStateWithLocations<FCSStateWithLocations>);
	case FC_INDIRECT_STATE:
		return sizeof(FCSIndirectStateWithLocations<FCSStateWithLocations>);
	case FC_TALON_DEBUG_STATE:
		return sizeof(FCSDebugTalonStateWithLocations);
	case FC_TALON_COMPACT_STATE:
		return sizeof(FCSCompactTalonStateWithLocations);
	case FC_TALON_INDIRECT_STATE:
		return sizeof(FCSIndirectTalonStateWithLocations);
	default:
		return 0;
	}
}

FCSStateWithLocations* CreateStateWithLocations()
{
	switch(GlobalStateType)
	{
	case FC_DEBUG_STATE:
		return new FCSDebugStateWithLocations<FCSStateWithLocations>();
	case FC_COMPACT_STATE:
		return new FCSCompactStateWithLocations<FCSStateWithLocations>();
	case FC_INDIRECT_STATE:
		return new FCSIndirectStateWithLocations<FCSStateWithLocations>();
	case FC_TALON_DEBUG_STATE:
		return new FCSDebugTalonStateWithLocations();
	case FC_TALON_COMPACT_STATE:
		return new FCSCompactTalonStateWithLocations();
	case FC_TALON_INDIRECT_STATE:
		return new FCSIndirectTalonStateWithLocations();
	default:
		return NULL;
	}
}

AFCSStateWithLocationsMatrix* CreateStateWithLocationsMatrix(int Size)
{
	switch(GlobalStateType)
	{
	case FC_DEBUG_STATE:
		return new FCSStateWithLocationsMatrix<FCSDebugStateWithLocations<FCSStateWithLocations> >(Size);
	case FC_COMPACT_STATE:
		return new FCSStateWithLocationsMatrix<FCSCompactStateWithLocations<FCSStateWithLocations> >(Size);
	case FC_INDIRECT_STATE:
		return new FCSStateWithLocationsMatrix<FCSIndirectStateWithLocations<FCSStateWithLocations> >(Size);
	case FC_TALON_DEBUG_STATE:
		return new FCSStateWithLocationsMatrix<FCSDebugTalonStateWithLocations>(Size);
	case FC_TALON_COMPACT_STATE:
		return new FCSStateWithLocationsMatrix<FCSCompactTalonStateWithLocations>(Size);
	case FC_TALON_INDIRECT_STATE:
		return new FCSStateWithLocationsMatrix<FCSIndirectTalonStateWithLocations>(Size);
	default:
		return NULL;
	}
}

void ReallocStateWithLocationsArray(FCSStateWithLocations*** Array, int OldSize, int NewSize)
{
	switch (GlobalStateType)
	{
	case FC_DEBUG_STATE:
		Realloc<FCSDebugStateWithLocations<FCSStateWithLocations>*>((FCSDebugStateWithLocations<FCSStateWithLocations>***)Array, OldSize, NewSize);
		break;
	case FC_COMPACT_STATE:
		Realloc<FCSCompactStateWithLocations<FCSStateWithLocations>*>((FCSCompactStateWithLocations<FCSStateWithLocations>***)Array, OldSize, NewSize);
		break;
	case FC_INDIRECT_STATE:
		Realloc<FCSIndirectStateWithLocations<FCSStateWithLocations>*>((FCSIndirectStateWithLocations<FCSStateWithLocations>***)Array, OldSize, NewSize);
		break;
	case FC_TALON_DEBUG_STATE:
		Realloc<FCSDebugTalonStateWithLocations*>((FCSDebugTalonStateWithLocations***)Array, OldSize, NewSize);
		break;
	case FC_TALON_COMPACT_STATE:
		Realloc<FCSCompactTalonStateWithLocations*>((FCSCompactTalonStateWithLocations***)Array, OldSize, NewSize);
		break;
	case FC_TALON_INDIRECT_STATE:
		Realloc<FCSIndirectTalonStateWithLocations*>((FCSIndirectTalonStateWithLocations***)Array, OldSize, NewSize);
		break;
	default:
		//this shouldn't happen
		assert(false);
	}
}


FCSStateWithLocations* CreateInitialState(const char *String, int NumberOfFreecells, 
										  int NumberOfStacks, int NumberOfDecks, FCSTalonType TalonType)
{
	if (TalonType == FCS_TALON_NONE)
		return FCSStateWithLocations::CreateInitialState(String, NumberOfFreecells, 
														 NumberOfStacks, NumberOfDecks);

	return FCSTalonStateWithLocations::CreateInitialState(String, NumberOfFreecells, 
														  NumberOfStacks, NumberOfDecks, TalonType);
}

int MD5StateWithLocationsHashAlgorithm::Hash(const FCSStateWithLocations* Key)
{
	unsigned char HashValue[MD5_HASHBYTES];
	int RealHashValue;

	m_MD5Hash.Init();
	m_MD5Hash.Update((unsigned char*)(((FCSStateWithLocations*)Key)->GetState()), ((FCSStateWithLocations*)Key)->GetState()->GetClassSize());
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
