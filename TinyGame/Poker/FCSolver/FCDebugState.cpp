////////////////////////////////////////////////
///\file FCDebugState.cpp
///\brief This file contains the implementation of classes dealing with Freecell Solver Debug States
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>
#include "FCDebugState.h"
#include "FCHelpingAlgorithms.h"


FCSDebugCard::FCSDebugCard()
{
	EmptyCard();
}

FCSDebugCard::~FCSDebugCard()
{
}

void FCSDebugCard::Copy(FCSCard* Card)
{
	m_CardNumber = ((FCSDebugCard*)Card)->m_CardNumber;
	m_Flags = ((FCSDebugCard*)Card)->m_Flags;
	m_Suit = ((FCSDebugCard*)Card)->m_Suit;
}

void FCSDebugCard::EmptyCard()
{
	m_CardNumber = 0;
	m_Suit = 0;
	m_Flags = 0;
}

bool FCSDebugCard::IsEmptyCard()
{
	if ((m_CardNumber == 0) && (m_Suit == 0) && (m_Flags == 0))
		return true;

	return false;
}

char FCSDebugCard::GetCardNumber()
{
	return m_CardNumber;
}

char FCSDebugCard::GetSuit()
{
	return m_Suit;
}

char FCSDebugCard::GetFlipped()
{
	return m_Flags;
}

void FCSDebugCard::SetSuit(char Suit)
{
	m_Suit = Suit;
}

void FCSDebugCard::SetCardNumber(char CardNumber)
{
	m_CardNumber = CardNumber;
}

void FCSDebugCard::SetFlipped(char Flipped)
{
	m_Flags = Flipped;
}

int FCSDebugCard::GetClassSize()
{
	return sizeof(FCSDebugCard);
}

FCSDebugStack::FCSDebugStack()
{
	m_NumberOfCards = 0;
}

int FCSDebugStack::Compare(FCSDebugStack* Stack)
{
	int CompareValue;

	if (m_NumberOfCards > Stack->m_NumberOfCards)
		return 1;
	else if (m_NumberOfCards < Stack->m_NumberOfCards)
		return -1;

	for (int a=0;a<MAX_NUM_CARDS_IN_A_STACK;a++)
	{
		if ( (CompareValue = m_Cards[a].Compare(&(Stack->m_Cards[a]))) != 0)
			return CompareValue;
	}

	return 0;
}

FCSDebugState::FCSDebugState()
{
	memset(m_Foundations, 0, MAX_NUM_DECKS*4);
}

int FCSDebugState::GetClassSize()
{
	return sizeof(FCSDebugState);
}

int FCSDebugState::StackCompare(int StackPosition1, int StackPosition2)
{
	FCSDebugCard Card1 = m_Stacks[StackPosition1].m_Cards[0];
	FCSDebugCard Card2 = m_Stacks[StackPosition2].m_Cards[0];

	return Card1.Compare(&Card2);
}

int FCSDebugState::Compare(FCSDebugState* State)
{
	int CompareValue, a;
	for (a = 0;a<MAX_NUM_STACKS;a++)
		if ( (CompareValue = m_Stacks[a].Compare(&(State->m_Stacks[a]))) != 0)
			return CompareValue;

	for (a = 0;a<MAX_NUM_FREECELLS;a++)
		if ( (CompareValue = m_Freecells[a].Compare(&(State->m_Freecells[a]))) != 0)
			return CompareValue;

	return memcmp(m_Foundations, State->m_Foundations, MAX_NUM_DECKS*4);
}

