#ifndef FCS_INDIRECT_STATE_COMPARE_ALGORITHM_H
#define FCS_INDIRECT_STATE_COMPARE_ALGORITHM_H

////////////////////////////////////////////////
///\file FCSIndirectStateCompareAlgorithm.h
///\brief This file contains the compare classes specific to indirect states
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "CompareAlgorithms.h"

///\brief Compare two indirect state stacks of cards
///
///This duplicates the code in FCSIndirectState::StackCompare, but it puts it in the 
///desired compare template format<br>
///Data = FCSIndirectCard, ConfigData = void
template <class Data, class ConfigData>
class FCSIndirectCardCompareAlgorithm : public ACompareNodesAlgorithm<Data, ConfigData>
{
public:
	///\brief Compare function
	///\param Node1 is the first item to compare
	///\param Node2 is the second item to compare
	///\param TConfigData is any configurate data needed for the compare
	///\return Returns -1 if Node1 < Node2.  Returns 1 if Node1 > Node2.  Returns 0 if Node1 = Node2
	virtual int Compare(const Data* Node1, const Data* Node2, const ConfigData* TConfigData);
};

template <class Data, class ConfigData>
int FCSIndirectCardCompareAlgorithm<Data, ConfigData>::Compare(const Data* Node1, const Data* Node2, const ConfigData* TConfigData)
{
	FCSIndirectCard* Card1 = (FCSIndirectCard*)Node1;
	FCSIndirectCard* Card2 = (FCSIndirectCard*)Node2;
	FCSIndirectCard *Temp1, *Temp2;

	int MinimumLength = min(Card1->m_Card, Card2->m_Card),
		ReturnValue;

	for(int a=0;a<MinimumLength;a++)
	{
		Temp1 = &Card1[a+1] /*+ a + 1*/;
		Temp2 = &Card2[a+1] /*+ a + 1*/;

		if ((ReturnValue = Temp1->Compare(Temp2)) != 0)
			return ReturnValue;
	}
	/*
	 * The reason I do the stack length comparisons after the card-by-card
	 * comparison is to maintain correspondence with 
	 * StackSortCompare, and with the one card comparison
	 * of the other state representation mechanisms.
	 * */
	return Card1->Compare(Card2);
}

///Compares two FCSIndirectStateWithLocations objects
///
///This was needed for the storage algorithms of indirect states
///Data = FCSIndirectStateWithLocations<FCSStateWithLocations>*, ConfigData = void
template <class Data>
class FCSIndirectStatesCompareAlgorithm : public ACompareNodesAlgorithm<Data, void>
{
public:

	///\brief Compare function
	///\param Node1 is the first item to compare
	///\param Node2 is the second item to compare
	///\param TConfigData is any configurate data needed for the compare
	///\return Returns -1 if Node1 < Node2.  Returns 1 if Node1 > Node2.  Returns 0 if Node1 = Node2
	virtual int Compare(const Data* Node1, const Data* Node2, const void* TConfigData);
};

template <class Data>
int FCSIndirectStatesCompareAlgorithm<Data>::Compare(const Data* Node1, const Data* Node2, const void* TConfigData)
{
	if (((*Node1) == NULL) && ((*Node2) == NULL))
		return 0;

	if ((*Node2) == NULL)
		return 1;

	if ((*Node1) == NULL)
		return -1;

	return (*Node1)->Compare(*Node2);
}

#endif