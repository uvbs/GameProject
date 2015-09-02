////////////////////////////////////////////////
///\file FCCompactState.cpp
///\brief This file contains the implementation of classes dealing with Freecell Solver Compact States
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>
#include "FCCompactState.h"
#include "FCHelpingAlgorithms.h"

FCSCompactCard::FCSCompactCard()
{
	EmptyCard();
}

FCSCompactCard::~FCSCompactCard()
{
}

void FCSCompactCard::Copy(FCSCard* Card)
{
	m_Card = ((FCSCompactCard*)Card)->m_Card;
}

void FCSCompactCard::EmptyCard()
{
	m_Card = 0;
}

bool FCSCompactCard::IsEmptyCard()
{
	return (m_Card == 0);
}

char FCSCompactCard::GetCardNumber()
{
	return (m_Card & 0x0F);
}

char FCSCompactCard::GetSuit()
{
	return ((m_Card >> 4) & 0x03);
}

char FCSCompactCard::GetFlipped()
{
	return (m_Card >> 6);
}

void FCSCompactCard::SetSuit(char Suit)
{
	m_Card = ((m_Card&0x4F)|(Suit<<4));

}

void FCSCompactCard::SetCardNumber(char CardNumber)
{
	m_Card = ((m_Card&0xF0)|CardNumber);
}

void FCSCompactCard::SetFlipped(char Flipped)
{
	m_Card = ((m_Card&0x3F)|(Flipped<<6));
}

int FCSCompactCard::GetClassSize()
{
	return sizeof(FCSCompactCard);
}


int FCSCompactState::GetClassSize()
{
	return sizeof(FCSCompactState);
}

int FCSCompactState::StackCompare(int StackPosition1, int StackPosition2)
{
	FCSCompactCard Card1 = m_Data[StackPosition1+1];
	FCSCompactCard Card2 = m_Data[StackPosition2+1];

	return Card1.Compare(&Card2);
}

int FCSCompactState::Compare(FCSCompactState* State)
{
	int CompareValue;
	for (int a = 0;a<MAX_NUM_STACKS*(MAX_NUM_CARDS_IN_A_STACK+1)+MAX_NUM_FREECELLS+4*MAX_NUM_DECKS;a++)
		if ( (CompareValue = m_Data[a].Compare(&(State->m_Data[a]))) != 0)
			return CompareValue;

	return 0;
}

