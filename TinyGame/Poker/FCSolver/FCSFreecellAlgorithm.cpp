////////////////////////////////////////////////
///\file FCSFreecellAlgorithm.cpp
///\brief This file contains the implementation of the FCSFreecellAlgorithm class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include "FCSFreecellAlgorithm.h"

void FCSFreecellAlgorithm::DeleteDerived(FCSDerivedStatesList* Derive)
{
	if (Derive->m_States != NULL)
		delete [] Derive->m_States;
}