#ifndef MMANN_SEARCH_ALGORITHMS_H
#define MMANN_SEARCH_ALGORITHMS_H

////////////////////////////////////////////////
///\file SearchAlgorithms.h
///\brief This file contains templated search functions
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "CompareAlgorithms.h"

///\brief A templated standard binary search function
///
///\param Key is the value being searched for
///\param Base is the array being searching
///\param Size is the size of the array
///\param Compare is the compare algorithm to compare values in the array
///\return Returns the found value.  Return NULL if not found
template <class Datatype>
Datatype* BSearch(Datatype* Key, Datatype* Base, int Size, ACompareNodesAlgorithm<Datatype, void>* Compare)
{
	if (Size == 0)
		return NULL;

	int Middle = Size/2;
	int CompareValue = Compare->Compare(Key, &Base[Middle], NULL);
	if (CompareValue == 0)
	{
		return &Base[Middle];
	}
	else if (CompareValue < 0)
	{
		return BSearch<Datatype>(Key, Base, Middle, Compare);
	}
	else if (CompareValue > 0)
	{
		Datatype* NewBase = &(Base[Middle+1]);
		return BSearch<Datatype>(Key, NewBase, (Size-1)/2, Compare);
	}

	return NULL;
}

///\brief An improved binary search function
///
///This is based off of the implementation of Shlomi Fish's (shlomif@vipe.technion.ac.il)
///binary search algorithm written in C
///\param Key is the value being searched for
///\param Array is the array being searching
///\param Length is the size of the array
///\param Compare is the compare algorithm to compare values in the array
///\param Found determines whether or not the item was found
///\return Returns item if found.  If not found, the function returns the place in which
///it should be placed
template <class Datatype>
Datatype* FreecellSolverBSearch(Datatype* Key, Datatype* Array, int Length, 
								ACompareNodesAlgorithm<Datatype, void>* Compare, bool* Found)
{
	int Low = 0,
		High = Length - 1,
		Mid,
		Result;

	while (Low <= High)
	{
		Mid = ((Low+High)>>1);
		Result = Compare->Compare(Key, &Array[Mid], NULL);

		if (Result < 0)
		{
			High = Mid-1;
		}
		else if (Result > 0)
		{
			Low = Mid+1;
		}
		else
		{
			*Found = true;
			return (&Array[Mid]);
		}
	}

	*Found = false;
	return (&Array[High+1]);
}

///\brief Merges a large sorted array with a small sorted array. 
///
///The arrays could be of any length whatsoever, but it works faster if the 
///first is significantly bigger than the second.  This function assumes that 
///big_array is allocated with enough space to hold the extra elements.
///The arrays should be distinct or else there would be unexpected results.
///\param BigArray is the larger of the two arrays
///\param BigArraySize is the size of the larger array
///\param SmallArray is the smaller of the two arrays
///\param SmallArraySize is the size of the smaller array
///\param Compare is the compare algorithm to compare values in the array
template <class Datatype>
void SFOMergeLargeAndSmallSortedArrays(Datatype* BigArray, int BigArraySize, 
									   Datatype* SmallArray, int SmallArraySize,
									   ACompareNodesAlgorithm<Datatype, void>* Compare)
{
	int NumberOfBigItemsMoved = 0,
		Position,
		StartOffset,
		EndOffset;
	Datatype* PositionPtr;
	bool Found;

	for (int ItemToMove = SmallArraySize-1 ; ItemToMove >= 0; ItemToMove--)
	{
		PositionPtr = FreecellSolverBSearch<Datatype>(&SmallArray[ItemToMove],
									BigArray, BigArraySize-NumberOfBigItemsMoved,
									Compare, &Found);

		Position = PositionPtr-BigArray;
		//This expression was simplified from the original
		EndOffset = BigArraySize - NumberOfBigItemsMoved + ItemToMove + 1;
		StartOffset = EndOffset + Position - (BigArraySize - NumberOfBigItemsMoved);

		memmove(&BigArray[StartOffset], &BigArray[Position], (EndOffset - StartOffset)*sizeof(Datatype));
		memcpy(&BigArray[StartOffset-1], &SmallArray[ItemToMove], sizeof(Datatype));
		NumberOfBigItemsMoved += (EndOffset - StartOffset);
	}
}

#endif