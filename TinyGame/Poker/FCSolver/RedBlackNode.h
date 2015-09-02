#ifndef MMANN_REDBLACKNODE_H
#define MMANN_REDBLACKNODE_H

////////////////////////////////////////////////
///\file RedBlackNode.h
///\brief This file contains the RedBlackNode class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "AVLRedBlackNode.h"

///\brief Templated node class for a templated red-black tree
///
///This class is based off of Damian Ivereigh's
///implementation of a red-black tree written in C
template <class Data>
class RedBlackNode
{
public:

	///Constructor
	RedBlackNode(Data* NodeData);
	
	///Destructor
	~RedBlackNode();

	///Pointer to the left node
	RedBlackNode<Data>* m_Left;

	///Pointer to the right node
	RedBlackNode<Data>* m_Right;

	///Pointer to the upper node on the tree
	RedBlackNode<Data>* m_Up;

	///Color of the node
	RedBlackNodeColor m_Color;

	///Pointer to the data in the node
	Data* m_Data;
};

template <class Data>
RedBlackNode<Data>::RedBlackNode(Data* NodeData)
{
	m_Left = m_Right = m_Up = NULL;
	m_Color = BLACK;
	m_Data = NodeData;
}

template <class Data>
RedBlackNode<Data>::~RedBlackNode()
{
}

///Walks through and deletes the node and its children
///\param Node is node to be deleted
///\param How to delete the data in the node
template <class Data>
void DeleteAllRedBlackNodes(RedBlackNode<Data>* Node, DeleteTreeDataEnum DeleteTreeData)
{
	if (Node->m_Left != NULL)
		DeleteAllRedBlackNodes(Node->m_Left, DeleteTreeData);
	if (Node->m_Right != NULL)
		DeleteAllRedBlackNodes(Node->m_Right, DeleteTreeData);

	switch(DeleteTreeData)
	{
	case NO_DELETE_TREE_ITEM:
		break;
	case DELETE_TREE_ITEM:
		delete Node->m_Data;
		break;
	case DELETE_TREE_ITEM_ARRAY:
		delete [] Node->m_Data;
		break;
	default:
		//This shouldn't happen
		assert(false);
	}

	delete Node;
}


#endif