////////////////////////////////////////////////
///\file FCSCompactMove.cpp
///\brief This file contains the implementation of classes dealing with compact move objects
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSCompactMove.h"
#include "FCHelpingAlgorithms.h"

FCSCompactMove::FCSCompactMove() : FCSMove()
{
	memset(m_Data, 0, sizeof(char)*4);
}

FCSCompactMove::~FCSCompactMove()
{
}

void FCSCompactMove::Copy(FCSMove* Move)
{
	memcpy(m_Data, ((FCSCompactMove*)Move)->m_Data, sizeof(((FCSCompactMove*)Move)->m_Data));
}

unsigned char FCSCompactMove::GetSourceStack()
{
	return m_Data[FCS_MOVE_SRC];
}

unsigned char FCSCompactMove::GetSourceFreecell()
{
	return m_Data[FCS_MOVE_SRC];
}

unsigned char FCSCompactMove::GetDestStack()
{
	return m_Data[FCS_MOVE_DEST];
}

unsigned char FCSCompactMove::GetDestFreecell()
{
	return m_Data[FCS_MOVE_DEST];
}

unsigned char FCSCompactMove::GetFoundation()
{
	return m_Data[FCS_MOVE_DEST];
}

FCSMoveTypes FCSCompactMove::GetType()
{
	return (FCSMoveTypes)m_Data[FCS_MOVE_TYPE];
}

unsigned char FCSCompactMove::GetNumberOfCardsInSequence()
{
	return m_Data[FCS_MOVE_NUM_CARDS_IN_SEQ];
}

unsigned char FCSCompactMove::GetNumberOfCardsFlipped()
{
	return m_Data[FCS_MOVE_NUM_CARDS_FLIPPED];
}

void FCSCompactMove::SetSourceStack(unsigned char Value)
{
	m_Data[FCS_MOVE_SRC] = Value;
}

void FCSCompactMove::SetSourceFreecell(unsigned char Value)
{
	m_Data[FCS_MOVE_SRC] = Value;
}

void FCSCompactMove::SetDestStack(unsigned char Value)
{
	m_Data[FCS_MOVE_DEST] = Value;
}

void FCSCompactMove::SetDestFreecell(unsigned char Value)
{
	m_Data[FCS_MOVE_DEST] = Value;
}

void FCSCompactMove::SetFoundation(unsigned char Value)
{
	m_Data[FCS_MOVE_DEST] = Value;
}

void FCSCompactMove::SetType(FCSMoveTypes Value)
{
	m_Data[FCS_MOVE_TYPE] = Value;
}

void FCSCompactMove::SetNumberOfCardsInSequence(unsigned char Value)
{
	m_Data[FCS_MOVE_NUM_CARDS_IN_SEQ] = Value;
}

void FCSCompactMove::SetNumberOfCardsFlipped(unsigned char Value)
{
	m_Data[FCS_MOVE_NUM_CARDS_FLIPPED] = Value;
}


FCSCompactMoveStack::FCSCompactMoveStack() : FCSMoveStack()
{
	m_Moves = new FCSCompactMove[m_MaxNumberOfMoves];
}

FCSCompactMoveStack::~FCSCompactMoveStack()
{
	delete [] m_Moves;
}

FCSMoveStack* FCSCompactMoveStack::Copy()
{
	FCSCompactMoveStack* ReturnStack = new FCSCompactMoveStack();

	ReturnStack->m_MaxNumberOfMoves = m_MaxNumberOfMoves;
	ReturnStack->m_NumberOfMoves = m_NumberOfMoves;
	for (int a =0;a<m_MaxNumberOfMoves;a++)
		ReturnStack->m_Moves[a].Copy(&(m_Moves[a]));

	return ReturnStack;
}

void FCSCompactMoveStack::Push(FCSMove* Move)
{
	// If all the moves inside the stack are taken then resize the move vector
	if (m_NumberOfMoves == m_MaxNumberOfMoves)
	{
		m_MaxNumberOfMoves += max((m_MaxNumberOfMoves >> 3), FCS_MOVE_STACK_GROW_BY);
		Realloc<FCSCompactMove>(&m_Moves, m_NumberOfMoves, m_MaxNumberOfMoves);
	}
	
	m_Moves[m_NumberOfMoves++].Copy(Move);
}

int FCSCompactMoveStack::Pop(FCSMove** Move)
{
	if (m_NumberOfMoves > 0)
	{
		(*Move)->Copy(&(m_Moves[--m_NumberOfMoves]));
		return 0;
	}
	else
	{
		return 1;
	}
}