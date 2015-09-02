#ifndef FC_HELPING_ALGORITHMS_H
#define FC_HELPING_ALGORITHMS_H

////////////////////////////////////////////////
///\file FCHelpingAlgorithms.h
///\brief This file contains algorithms that don't have a real place, but are needed
/// for the freecell solver
///
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>

///\brief The templated equalivalent of realloc() with type safety
template <class Datatype>
void Realloc(Datatype** Array, int OldSize, int NewSize)
{
	if (NewSize <= OldSize)
		return;

	Datatype* Temp = (*Array);
	(*Array) = new Datatype[NewSize];

	memcpy((*Array), Temp, OldSize*sizeof(Datatype));
	delete [] Temp;
}

#endif
