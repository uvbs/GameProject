////////////////////////////////////////////////
///\file FCIndirectState.cpp
///\brief This file contains the implementation of classes dealing with Freecell Solver Indirect States
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include "FCIndirectState.h"
#include "FCHelpingAlgorithms.h"

FCSIndirectCard::FCSIndirectCard()
{
	EmptyCard();
}

FCSIndirectCard::~FCSIndirectCard()
{
}

void FCSIndirectCard::Copy(FCSCard* Card)
{
	m_Card = ((FCSIndirectCard*)Card)->m_Card;
}

void FCSIndirectCard::EmptyCard()
{
	m_Card = 0;
}

bool FCSIndirectCard::IsEmptyCard()
{
	return (m_Card == 0);
}

char FCSIndirectCard::GetCardNumber()
{
	return (m_Card & 0x0F);
}

char FCSIndirectCard::GetSuit()
{
	return ((m_Card >> 4) & 0x03);
}

char FCSIndirectCard::GetFlipped()
{
	return (m_Card >> 6);
}

void FCSIndirectCard::SetSuit(char Suit)
{
	m_Card = ((m_Card&0x4F)|(Suit<<4));
}

void FCSIndirectCard::SetCardNumber(char CardNumber)
{
	m_Card = ((m_Card&0xF0)|CardNumber);
}

void FCSIndirectCard::SetFlipped(char Flipped)
{
	m_Card = ((m_Card&0x3F)|(Flipped<<6));
}

int FCSIndirectCard::GetClassSize()
{
	return sizeof(FCSIndirectCard);
}

FCSIndirectState::FCSIndirectState()
{
	for(char a=0;a<MAX_NUM_STACKS;a++)
	{
		m_Stacks[a] = NULL;
	}
	memset(m_Foundations, 0, MAX_NUM_DECKS*4);
}

FCSIndirectState::~FCSIndirectState()
{
}

int FCSIndirectState::StackCompare(int StackPosition1, int StackPosition2)
{
	FCSIndirectCard* Card1 = m_Stacks[StackPosition1];
	FCSIndirectCard* Card2 = m_Stacks[StackPosition2];

	int MinimumLength = min(Card1->m_Card, Card2->m_Card),
		ReturnValue;

	for(int a=0;a<MinimumLength;a++)
	{
		Card1 = &m_Stacks[StackPosition1][a+1];
		Card2 = &m_Stacks[StackPosition2][a+1];

		ReturnValue = Card1->Compare(Card2);
		if (ReturnValue != 0)
			return ReturnValue;
	}
	/*
	 * The reason I do the stack length comparisons after the card-by-card
	 * comparison is to maintain correspondence with 
	 * StackSortCompare, and with the one card comparison
	 * of the other state representation mechanisms.
	 * */

	Card1 = m_Stacks[StackPosition1];
	Card2 = m_Stacks[StackPosition2];

	return Card1->Compare(Card2);
}

int FCSIndirectState::GetClassSize()
{
	return sizeof(FCSIndirectState);
}

int FCSIndirectState::StackSortCompare(int StackPosition1, int StackPosition2)
{
	FCSIndirectCard* Card1 = &m_Stacks[StackPosition1][1];
	FCSIndirectCard* Card2 = &m_Stacks[StackPosition2][1];

	return Card1->Compare(Card2);	
}

int FCSIndirectState::Compare(FCSIndirectState* State)
{
	int CompareValue, a;

	for (a=0;a<MAX_NUM_STACKS;a++)
	{
		if (m_Stacks[a] == State->m_Stacks[a])
			continue;

		if ((m_Stacks[a] != NULL) && (State->m_Stacks[a] == NULL))
			return 1;

		if ((m_Stacks[a] == NULL) && (State->m_Stacks[a] != NULL))
			return -1;

		//compare length of stack
		if (m_Stacks[a][0].m_Card > State->m_Stacks[a][0].m_Card)
			return 1;

		if (m_Stacks[a][0].m_Card < State->m_Stacks[a][0].m_Card)
			return -1;

		//compare all the cards
		for (int i = 1; i<=m_Stacks[a][0].m_Card;i++)
			if ( (CompareValue = m_Stacks[a][i].Compare(&State->m_Stacks[a][i])) != 0)
				return CompareValue;
	}

	for (a=0;a<MAX_NUM_FREECELLS;a++)
	{
		if ( (CompareValue = m_Freecells[a].Compare(&(State->m_Freecells[a]))) != 0)
			return CompareValue;
	}

	return memcmp(m_Foundations, State->m_Foundations, MAX_NUM_DECKS*4);
}

void FCSIndirectState::Copy(FCSIndirectState* State)
{
	for (char a = 0;a<MAX_NUM_FREECELLS;a++)
		m_Freecells[a].Copy(&State->m_Freecells[a]);

	memcpy(m_Foundations, State->m_Foundations, MAX_NUM_DECKS*4);
}


