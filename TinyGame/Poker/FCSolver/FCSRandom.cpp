////////////////////////////////////////////////
///\file FCSRandom.cpp
///\brief This file contains the implementation of FCSRandom class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSRandom.h"

FCSRandom::FCSRandom(long Seed)
{
	m_Seed = Seed;
}

FCSRandom::~FCSRandom()
{
}

// This function constructs a larger integral number of out of two 15-bit ones.
int FCSRandom::GetRandomNumber()
{
	int one = Rand15(), 
		two = Rand15();

	return (one | (two << 15));
}

void FCSRandom::ReSeed(long Seed)
{
	m_Seed = Seed;
}

int FCSRandom::Rand15()
{
	m_Seed = m_Seed * 214013 + 2531011;
	return (m_Seed >> 16) & 0x7fff;
}