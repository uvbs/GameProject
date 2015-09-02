////////////////////////////////////////////////
///\file FCSDebugMove.cpp
///\brief This file contains the implementation of classes dealing with debug move objects
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSDebugMove.h"
#include "FCHelpingAlgorithms.h"

FCSDebugMove::FCSDebugMove() : FCSMove()
{
	m_Foundation = 0;
	m_NumberOfCardsInSequence = 0;
	m_NumberOfCardsFlipped = 0;
	m_SourceFreecell = 0;
	m_DestinationFreecell = 0;
	m_SourceStack = 0;
	m_DestinationStack = 0;
	m_Type = FCS_MOVE_TYPE_NULL;
}

FCSDebugMove::~FCSDebugMove()
{
}

void FCSDebugMove::Copy(FCSMove* Move)
{
	m_Foundation = Move->GetFoundation();
	m_NumberOfCardsInSequence = Move->GetNumberOfCardsInSequence();
	m_NumberOfCardsFlipped = Move->GetNumberOfCardsFlipped();
	m_SourceFreecell = Move->GetSourceFreecell();
	m_DestinationFreecell = Move->GetDestFreecell();
	m_SourceStack = Move->GetSourceStack();
	m_DestinationStack = Move->GetDestStack();
	m_Type = Move->GetType();
}

unsigned char FCSDebugMove::GetSourceStack()
{
	return m_SourceStack;
}

unsigned char FCSDebugMove::GetSourceFreecell()
{
	return m_SourceFreecell;
}

unsigned char FCSDebugMove::GetDestStack()
{
	return m_DestinationStack;
}

unsigned char FCSDebugMove::GetDestFreecell()
{
	return m_DestinationFreecell;
}

unsigned char FCSDebugMove::GetFoundation()
{
	return m_Foundation;
}

FCSMoveTypes FCSDebugMove::GetType()
{
	return m_Type;
}

unsigned char FCSDebugMove::GetNumberOfCardsInSequence()
{
	return m_NumberOfCardsInSequence;
}

unsigned char FCSDebugMove::GetNumberOfCardsFlipped()
{
	return m_NumberOfCardsFlipped;
}

void FCSDebugMove::SetSourceStack(unsigned char Value)
{
	m_SourceStack = Value;
}

void FCSDebugMove::SetSourceFreecell(unsigned char Value)
{
	m_SourceFreecell = Value;
}

void FCSDebugMove::SetDestStack(unsigned char Value)
{
	m_DestinationStack = Value;
}

void FCSDebugMove::SetDestFreecell(unsigned char Value)
{
	m_DestinationFreecell = Value;
}

void FCSDebugMove::SetFoundation(unsigned char Value)
{
	m_Foundation = Value;
}

void FCSDebugMove::SetType(FCSMoveTypes Value)
{
	m_Type = Value;
}

void FCSDebugMove::SetNumberOfCardsInSequence(unsigned char Value)
{
	m_NumberOfCardsInSequence = Value;
}

void FCSDebugMove::SetNumberOfCardsFlipped(unsigned char Value)
{
	m_NumberOfCardsFlipped = Value;
}

FCSDebugMoveStack::FCSDebugMoveStack() : FCSMoveStack()
{
	m_Moves = new FCSDebugMove[m_MaxNumberOfMoves];
}

FCSDebugMoveStack::~FCSDebugMoveStack()
{
	delete [] m_Moves;
}

FCSMoveStack* FCSDebugMoveStack::Copy()
{
	FCSDebugMoveStack* ReturnStack = new FCSDebugMoveStack();

	ReturnStack->m_MaxNumberOfMoves = m_MaxNumberOfMoves;
	ReturnStack->m_NumberOfMoves = m_NumberOfMoves;
	for(int a=0;a<m_MaxNumberOfMoves;a++)
		ReturnStack->m_Moves[a].Copy(&(m_Moves[a]));

	return ReturnStack;
}

void FCSDebugMoveStack::Push(FCSMove* Move)
{
	// If all the moves inside the stack are taken then resize the move vector
	if (m_NumberOfMoves == m_MaxNumberOfMoves)
	{
		m_MaxNumberOfMoves += max((m_MaxNumberOfMoves >> 3), FCS_MOVE_STACK_GROW_BY);
		Realloc<FCSDebugMove>(&m_Moves, m_NumberOfMoves, m_MaxNumberOfMoves);
	}

	m_Moves[m_NumberOfMoves++].Copy(Move);
}

int FCSDebugMoveStack::Pop(FCSMove** Move)
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