#ifndef MMANN_AVLREDBLACKNODE_H
#define MMANN_AVLREDBLACKNODE_H

////////////////////////////////////////////////
///\file AVLRedBlackNode.h
///\brief This file contains the AVLRedBlackNode class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <assert.h>
#include "AGenericTree.h"

///The two possible colors of a red-black tree (duh!)
enum RedBlackNodeColor {BLACK, RED};

///\brief Templated node class for a templated AVL red-black tree
///
///This class is based off of Ben Pfaff's <pfaffben@pilot.msu.edu>
///AVL implementation of a red-black tree written in C
template <class Data>
class AVLRedBlackNode
{
public:
	///Constructor
	AVLRedBlackNode(DeleteTreeDataEnum DeleteTreeData);

	///Destructor
	virtual ~AVLRedBlackNode();

	///Pointer to the left node
	AVLRedBlackNode<Data>* m_Left;

	///Pointer to the right node
	AVLRedBlackNode<Data>* m_Right;

	///Color of the node
	RedBlackNodeColor m_Color;

	///Pointer to the data in the node
	Data* m_Data;

	///Flag determining whether or not to delete the data when the node is deleted
	DeleteTreeDataEnum m_DeleteTreeData;
};


template <class Data>
AVLRedBlackNode<Data>::AVLRedBlackNode(DeleteTreeDataEnum DeleteTreeData)
{
	m_Left = m_Right = NULL;
	m_Color = BLACK;
	m_Data = NULL;
	m_DeleteTreeData = DeleteTreeData;
}

template <class Data>
AVLRedBlackNode<Data>::~AVLRedBlackNode()
{
	switch(m_DeleteTreeData)
	{
	case NO_DELETE_TREE_ITEM:
		break;
	case DELETE_TREE_ITEM:
		delete m_Data;
		break;
	case DELETE_TREE_ITEM_ARRAY:
		delete [] m_Data;
		break;
	default:
		//This shouldn't happen
		assert(false);
	}
}

#endif