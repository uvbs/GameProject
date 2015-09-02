#ifndef MMANN_AGENERICTREE_H
#define MMANN_AGENERICTREE_H

////////////////////////////////////////////////
///\file AGenericTree.h
///\brief This file contains the AGenericTree class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "CompareAlgorithms.h"

///How a tree data item should be deleted
enum DeleteTreeDataEnum {NO_DELETE_TREE_ITEM, DELETE_TREE_ITEM, DELETE_TREE_ITEM_ARRAY};

///\brief An abstract, generic templated tree class
template <class Data, class ConfigData>
class AGenericTree
{
public:

	///Destructor
	virtual ~AGenericTree();

	///\brief Find a data item.  If not found, insert it
	///
	///\param NodeData is the value being searched for.
	///\param NodeInserted is a flag to determine if the data was inserted
	///\return A pointer to the data found. NULL if data is inserted
	virtual Data* Search(Data* NodeData, bool* NodeInserted) = 0;

	///\brief Find a data item.  If not found, return NULL
	///
	///\param NodeData is the value being searched for.
	///\return A pointer to the data found.  NULL if data isn't found.
	virtual Data* Find(Data* NodeData) = 0;

	///\brief Some trees can't destory themselves with a destructor, 
	/// use this function instead
	virtual void DeleteTree() {}

protected:
	///Constructor
	AGenericTree(ACompareNodesAlgorithm<Data, ConfigData>* CompareNodes, DeleteTreeDataEnum DeleteTreeData = NO_DELETE_TREE_ITEM,
			 ConfigData* pConfigData = NULL);

	///The compare algorithm to compare nodes in the tree
	ACompareNodesAlgorithm<Data, ConfigData>* m_CompareNodes;

	///Configuration data (if necessary) for the compare algorithms
	ConfigData* m_ConfigData;

	///Determines how to delete the data when the hash table is deleted
	DeleteTreeDataEnum m_DeleteTreeData;
};

template <class Data, class ConfigData>
AGenericTree<Data, ConfigData>::AGenericTree(ACompareNodesAlgorithm<Data, ConfigData>* CompareNodes,
					 DeleteTreeDataEnum DeleteTreeData, ConfigData* pConfigData)
{
	m_CompareNodes = CompareNodes;
	m_ConfigData = pConfigData;
	m_DeleteTreeData = DeleteTreeData;
}


template <class Data, class ConfigData>
AGenericTree<Data, ConfigData>::~AGenericTree()
{
}


#endif
