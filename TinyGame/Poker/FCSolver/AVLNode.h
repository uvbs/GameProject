#ifndef MMANN_AVLNODE_H
#define MMANN_AVLNODE_H

////////////////////////////////////////////////
///\file AVLNode.h
///\brief This file contains the AVLNode class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

///\brief Templated node class for a templated AVL tree
template <class Data>
class AVLNode
{
public:
	///Constructor
	AVLNode(Data* data);

	///Destructor - doesn't delete left and right nodes
	virtual ~AVLNode();

	///Pointer to the left node
	AVLNode<Data>* m_Left;

	///Pointer to the right node
	AVLNode<Data>* m_Right;

	///Pointer to the data in the node
	Data* m_Data;

	///Balance of the node - used to rebranch tree
	signed char m_Balance;
};

template <class Data>
AVLNode<Data>::AVLNode(Data* data)
{
	m_Left = m_Right = NULL;
	m_Balance = 0;
	m_Data = data;
}

template<class Data>
AVLNode<Data>::~AVLNode()
{
}

///Walks through and deletes the node and its children
///\param Node is node to be deleted
///\param How to delete the data in the node
template <class Data>
void DeleteAllAVLNodes(AVLNode<Data>* Node, DeleteTreeDataEnum DeleteTreeData)
{
	if (Node->m_Left != NULL)
		DeleteAllAVLNodes(Node->m_Left, DeleteTreeData);
	if (Node->m_Right != NULL)
		DeleteAllAVLNodes(Node->m_Right, DeleteTreeData);

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