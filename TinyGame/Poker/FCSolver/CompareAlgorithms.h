#ifndef MMANN_COMPARE_ALGORITHMS_H
#define MMANN_COMPARE_ALGORITHMS_H

////////////////////////////////////////////////
///\file CompareAlgorithms.h
///\brief This file contains the comparing algorithm classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <string.h>

#ifdef min
#undef min
#endif
///find the minimum of two values
#define min(a,b) ((a)<(b)?(a):(b))

///\brief Abstract, templated compare algorithm class
template <class Data, class ConfigData>
class ACompareNodesAlgorithm
{
public:
	///\brief Compare function
	///\param Node1 is the first item to compare
	///\param Node2 is the second item to compare
	///\param TConfigData is any configurate data needed for the compare
	///\return Returns -1 if Node1 < Node2.  Returns 1 if Node1 > Node2.  Returns 0 if Node1 = Node2
	virtual int Compare(const Data* Node1, const Data* Node2, const ConfigData* TConfigData) = 0;
};

///\brief templated compare algorithm class
///
///This compare algorithm compares the data through < and >
template <class Data, class ConfigData>
class SimpleCompareNodesAlgorithm : public ACompareNodesAlgorithm<Data, ConfigData>
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
int SimpleCompareNodesAlgorithm<Data, ConfigData>::Compare(const Data* Node1, const Data* Node2, const ConfigData* TConfigData)
{
	if (*Node1 < *Node2)
		return -1;
	if (*Node1 > *Node2)
		return 1;

	return 0;
}

///\brief templated compare algorithm class
///
///This compare algorithm compares the data through a compare function
///This compare algorithm assumes the class being compares has a Compare() function
template <class Data, class ConfigData>
class CompareFunctionCompareNodesAlgorithm : public ACompareNodesAlgorithm<Data, ConfigData>
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
int CompareFunctionCompareNodesAlgorithm<Data, ConfigData>::Compare(const Data* Node1, const Data* Node2, const ConfigData* TConfigData)
{
	return ((Data*)Node1)->Compare(Node2);
}

#endif