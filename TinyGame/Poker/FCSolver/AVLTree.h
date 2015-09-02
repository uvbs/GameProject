#ifndef MMANN_AVLTREE_H
#define MMANN_AVLTREE_H

////////////////////////////////////////////////
///\file AVLTree.h
///\brief This file contains the AVLTree class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <assert.h>
#include "AVLNode.h"

///\brief A templated balanced tree class
///
///This class is based off of Daniel Nagy's
///AVL implementation of a balanced tree written in C
template <class Data, class ConfigData>
class AVLTree : public AGenericTree<Data, ConfigData>
{
public:
	///Constructor
	AVLTree(ACompareNodesAlgorithm<Data, ConfigData>* Compare, DeleteTreeDataEnum DeleteTreeData = NO_DELETE_TREE_ITEM,
					ConfigData* pConfigData = NULL);

	///Destructor
	virtual ~AVLTree();

	///\brief Find a data item.  If not found, insert it
	///
	///\param NodeData is the value being searched for.
	///\param NodeInserted is a flag to determine if the data was inserted
	///\return A pointer to the data found.  NULL if data is inserted
	virtual Data* Search(Data* NodeData, bool* NodeInserted);

	///\brief Find a data item.  If not found, return NULL
	///
	///\param NodeData is the value being searched for.
	///\return A pointer to the data found.  NULL if data isn't found.
	virtual Data* Find(Data* NodeData);

	///\brief This tree can't destory itself with a destructor, 
	/// use this function instead
	virtual void DeleteTree();

	///\brief Delete a node from the tree
	///
	///\param NodeData is the data of the node to be deleted.
	///\return Data of the deleted node.  NULL if node isn't found.
	Data* Delete(Data* NodeData);

	///\brief Delete the root node of the tree
	///
	///\return Data of the deleted node.  NULL if there aren't any nodes.
	Data* DeleteRoot();
	
protected:

	///\brief Recursively search for the correct place for a node and insert it
	///
	///\param Tree is the AVLTree to be searched
	///\param NodeData is the data of the node to be inserted
	///\param HasTreeGrown determines whether the tree needs to be rebalanced
	///\return Pointer to the data added
	Data* Insert(AVLTree<Data, ConfigData>* Tree, Data* NodeData, bool &HasTreeGrown);

	///\brief Recursively search for data on an AVLTree
	///
	///\param Tree is the AVLTree being searched
	///\param NodeData is the data being searched for
	///\return Pointer to the data found
	///\overload 
	Data* Find(AVLTree<Data, ConfigData>* Tree, Data* NodeData);

	///\brief Recursively search for a node and delete it
	///
	///\param Tree is the AVLTree to be searched
	///\param NodeData is the data of the node to be removed (but not deleted).
	///\param HasTreeShrunk determines whether the tree needs to be rebalanced
	///\return Removed node.  User is responsible to delete it.  NULL if node isn't found.
	AVLNode<Data>* Remove(AVLTree<Data, ConfigData>* Tree, Data* NodeData, bool &HasTreeShrunk);

	///\brief Remove the root node of an AVL tree
	///
	///\param Tree is the AVLTree to be searched
	///\param HasTreeShrunk determines whether the tree needs to be rebalanced
	///\return Removed node.  User is responsible to delete it.  NULL if no root node.
	AVLNode<Data>* RemoveRoot(AVLTree<Data, ConfigData>* Tree, bool &HasTreeShrunk);

	///\brief Swing node to the left, NO BALANCE MAINTAINANCE!!!
	///
	///\param Root is the node that the tree will rotate around
	void SwingLeft(AVLNode<Data>** Root);

	///\brief Swing node to the right, NO BALANCE MAINTAINANCE!!!
	///
	///\param Root is the node that the tree will rotate around
	void SwingRight(AVLNode<Data>** Root);

	///\brief Clean up after swings.  This is what does the balance maintainance.
	///
	///\param Root is the node that will be the base of the balance
	void BalanceAfterNastySwing(AVLNode<Data>* Root);

	///Root node of the tree
	AVLNode<Data>* m_Root;
};

template <class Data, class ConfigData>
AVLTree<Data, ConfigData>::AVLTree(ACompareNodesAlgorithm<Data, ConfigData>* Compare, DeleteTreeDataEnum DeleteTreeData,
					ConfigData* pConfigData) : AGenericTree<Data, ConfigData>(Compare, DeleteTreeData, pConfigData)
{
	m_Root = NULL;
}

template <class Data, class ConfigData>
AVLTree<Data, ConfigData>::~AVLTree()
{
}

template <class Data, class ConfigData>
void AVLTree<Data, ConfigData>::DeleteTree()
{
	if (m_Root != NULL)
		DeleteAllAVLNodes(m_Root, m_DeleteTreeData);
}

template <class Data, class ConfigData>
Data* AVLTree<Data, ConfigData>::Delete(Data* NodeData)
{
bool LocalHasTreeShrunk;
AVLNode<Data>* TempNode;
Data* TempData;

	TempNode = Remove(this, NodeData, LocalHasTreeShrunk);
	TempData = TempNode->m_Data;
	delete TempNode;
	return TempData;
}

template <class Data, class ConfigData>
AVLNode<Data>* AVLTree<Data, ConfigData>::Remove(AVLTree<Data, ConfigData>* Tree, Data* NodeData, bool &HasTreeShrunk)
{
	AVLNode<Data>* TempNode;

	int CompareValue = Tree->m_CompareNodes->Compare(Tree->m_Root->m_Data, NodeData, Tree->m_ConfigData);
	if (CompareValue > 0)
	{
		//remove from left subtree
		AVLTree<Data, ConfigData> LeftSubtree(Tree->m_CompareNodes, Tree->m_DeleteTreeData, Tree->m_ConfigData);
		if (LeftSubtree.m_Root = Tree->m_Root->m_Left)
		{
			TempNode = Remove(&LeftSubtree, NodeData, HasTreeShrunk);
			Tree->m_Root->m_Left = LeftSubtree.m_Root;
			if (HasTreeShrunk)
			{
				switch(Tree->m_Root->m_Balance++)
				{
				case -1:
					HasTreeShrunk = true;
					return TempNode;
				case 0:
					HasTreeShrunk = false;
					return TempNode;
				}
				switch(Tree->m_Root->m_Right->m_Balance)
				{
				case 0:
					SwingLeft(&(Tree->m_Root));
					Tree->m_Root->m_Balance = -1;
					Tree->m_Root->m_Left->m_Balance = 1;
					HasTreeShrunk = false;
					return TempNode;
				case 1:
					SwingLeft(&(Tree->m_Root));
					Tree->m_Root->m_Balance = 0;
					Tree->m_Root->m_Left->m_Balance = 0;
					HasTreeShrunk = true;
					return TempNode;
				}
				
				SwingRight(&(Tree->m_Root->m_Right));
				SwingLeft(&(Tree->m_Root));
				BalanceAfterNastySwing(Tree->m_Root);
				HasTreeShrunk = true;
				return TempNode;
			}
		}
	}
	else if (CompareValue == 0)
	{
		return RemoveRoot(Tree, HasTreeShrunk);
	}
	else if (CompareValue < 0)
	{
		//remove the right subtree
		AVLTree<Data, ConfigData> RightSubtree(Tree->m_CompareNodes, Tree->m_DeleteTreeData, Tree->m_ConfigData);
		if (RightSubtree.m_Root = Tree->m_Root->m_Right)
		{
			TempNode = Remove(&RightSubtree, NodeData, HasTreeShrunk);
			Tree->m_Root->m_Right = RightSubtree.m_Root;
			if (HasTreeShrunk)
			{
				switch(Tree->m_Root->m_Balance--)
				{
				case 1:
					HasTreeShrunk = true;
					return TempNode;
				case 0:
					HasTreeShrunk = false;
					return TempNode;
				}
				switch(Tree->m_Root->m_Left->m_Balance)
				{
				case 0:
					SwingRight(&(Tree->m_Root));
					Tree->m_Root->m_Balance = 1;
					Tree->m_Root->m_Right->m_Balance = -1;
					HasTreeShrunk = false;
					return TempNode;
				case -1:
					SwingRight(&(Tree->m_Root));
					Tree->m_Root->m_Balance = 0;
					Tree->m_Root->m_Right->m_Balance = 0;
					HasTreeShrunk = true;
					return TempNode;
				}

				SwingLeft(&(Tree->m_Root->m_Left));
				SwingRight(&(Tree->m_Root));
				BalanceAfterNastySwing(Tree->m_Root);
				HasTreeShrunk = true;
				return TempNode;
			}
		}
	}

	HasTreeShrunk = false;
	return TempNode;
}

template <class Data, class ConfigData>
Data* AVLTree<Data, ConfigData>::DeleteRoot()
{
bool LocalHasTreeShrunk;
AVLNode<Data>* TempNode;
Data* TempData;

	TempNode = RemoveRoot(this, LocalHasTreeShrunk);
	TempData = TempNode->m_Data;
	delete TempNode;
	return TempData;
}

template <class Data, class ConfigData>
AVLNode<Data>* AVLTree<Data, ConfigData>::RemoveRoot(AVLTree<Data, ConfigData>* Tree, bool &HasTreeShrunk)
{
	if (Tree->m_Root == NULL)
		return NULL;

	AVLNode<Data>* Node = Tree->m_Root;
	AVLNode<Data>* TempNode;

	if (Tree->m_Root->m_Left == NULL)
	{
		if (Tree->m_Root->m_Right == NULL)
		{
			Tree->m_Root = NULL;
			HasTreeShrunk = true;
			return Node;
		}
		Tree->m_Root = Tree->m_Root->m_Right;
		HasTreeShrunk = true;
		return Node;
	}
	if (Tree->m_Root->m_Right == NULL)
	{
		Tree->m_Root = Tree->m_Root->m_Left;
		HasTreeShrunk = true;
		return Node;
	}

	TempNode = Node;
	if (Tree->m_Root->m_Balance < 0)
	{
		//remove from the left subtree
		Node = Tree->m_Root->m_Left;
		while (Node->m_Right)
			Node = Node->m_Right;
	}
	else
	{
		//remove from the right subtree
		Node = Tree->m_Root->m_Right;
		while (Node->m_Left)
			Node = Node->m_Left;
	}

	Remove(Tree, Node->m_Data, HasTreeShrunk);
	Node->m_Left = Tree->m_Root->m_Left;
	Node->m_Right = Tree->m_Root->m_Right;
	Node->m_Balance = Tree->m_Root->m_Balance;
	Tree->m_Root = Node;
	if (Node->m_Balance == 0)
	{
		return TempNode;
	}

	HasTreeShrunk = false;
	return TempNode;
}

template <class Data, class ConfigData>
void AVLTree<Data, ConfigData>::SwingLeft(AVLNode<Data>** Root)
{
	AVLNode<Data>* a = *Root;
	AVLNode<Data>* b = a->m_Right;
	*Root = b;
	a->m_Right = b->m_Left;
	b->m_Left = a;
}

template <class Data, class ConfigData>
void AVLTree<Data, ConfigData>::SwingRight(AVLNode<Data>** Root)
{
	AVLNode<Data>* a = *Root;
	AVLNode<Data>* b = a->m_Left;
	*Root = b;
	a->m_Left = b->m_Right;
	b->m_Right = a;
}

template <class Data, class ConfigData>
void AVLTree<Data, ConfigData>::BalanceAfterNastySwing(AVLNode<Data>* Root)
{
	switch(Root->m_Balance)
	{
	case -1:
		Root->m_Left->m_Balance = 0;
		Root->m_Right->m_Balance = 1;
		break;
	case 1:
		Root->m_Left->m_Balance = -1;
		Root->m_Right->m_Balance = 0;
		break;
	case 0:
		Root->m_Left->m_Balance = 0;
		Root->m_Right->m_Balance = 0;
		break;
	}

	Root->m_Balance = 0;
}

template <class Data, class ConfigData>
Data* AVLTree<Data, ConfigData>::Find(Data* NodeData)
{
	return Find(this, NodeData);
}

template <class Data, class ConfigData>
Data* AVLTree<Data, ConfigData>::Find(AVLTree<Data, ConfigData>* Tree, Data* NodeData)
{
	if (Tree->m_Root == NULL)
		return NULL;

	int CompareValue = Tree->m_CompareNodes->Compare(Tree->m_Root->m_Data, NodeData, Tree->m_ConfigData);
	if (CompareValue > 0)
	{
		AVLTree<Data, ConfigData> LeftSubtree(Tree->m_CompareNodes, Tree->m_DeleteTreeData, Tree->m_ConfigData);
		LeftSubtree.m_Root = Tree->m_Root->m_Left;
		return Find(&LeftSubtree, NodeData);
	}
	else if (CompareValue == 0)
	{
		return Tree->m_Root->m_Data;
	}
	else if (CompareValue < 0)
	{
		AVLTree<Data, ConfigData> RightSubtree(Tree->m_CompareNodes, Tree->m_DeleteTreeData, Tree->m_ConfigData);
		RightSubtree.m_Root = Tree->m_Root->m_Right;
		return Find(&RightSubtree, NodeData);
	}

	//this shouldn't happen!!!!
	assert(false);
	return NULL;
}

template <class Data, class ConfigData>
Data* AVLTree<Data, ConfigData>::Search(Data* NodeData, bool* NodeInserted)
{
bool LocalHasTreeGrown;

	*NodeInserted = false;
	Data* FindNode = Find(NodeData);
	if (FindNode == NULL)
	{
		*NodeInserted = true;
		Insert(this, NodeData, LocalHasTreeGrown);
	}

	return FindNode;
}

template <class Data, class ConfigData>
Data* AVLTree<Data, ConfigData>::Insert(AVLTree<Data, ConfigData>* Tree, Data* NodeData, bool &HasTreeGrown)
{
	//insert into an empty tree
	if (Tree->m_Root == NULL)
	{
		Tree->m_Root = new AVLNode<Data>(NodeData);
		HasTreeGrown = true;
		return NodeData;
	}

	if (Tree->m_CompareNodes->Compare(Tree->m_Root->m_Data, NodeData, NULL) > 0)
	{
		//insert into the left subtree
		if (Tree->m_Root->m_Left != NULL)
		{
			AVLTree<Data, ConfigData> LeftSubtree(m_CompareNodes);
			LeftSubtree.m_Root = Tree->m_Root->m_Left;
			Insert(&LeftSubtree, NodeData, HasTreeGrown);
			if (HasTreeGrown)
			{
				switch(Tree->m_Root->m_Balance--)
				{
				case 1:
					HasTreeGrown = false;
					return NodeData;
				case 0:
					HasTreeGrown = true;
					return NodeData;
				}

				if (Tree->m_Root->m_Left->m_Balance < 0)
				{
					SwingRight(&(Tree->m_Root));
					Tree->m_Root->m_Balance = 0;
					Tree->m_Root->m_Right->m_Balance = 0;
				}
				else
				{
					SwingLeft(&(Tree->m_Root->m_Left));
					SwingRight(&(Tree->m_Root));
					BalanceAfterNastySwing(Tree->m_Root);
				}
			}
			else
			{
				Tree->m_Root->m_Left = LeftSubtree.m_Root;
			}

			HasTreeGrown = false;
			return NodeData;
		}
		else
		{
			Tree->m_Root->m_Left = new AVLNode<Data>(NodeData);
			if (Tree->m_Root->m_Balance--)
			{
				HasTreeGrown = false;
				return NodeData;
			}

			HasTreeGrown = true;
			return NodeData;
		}
	}
	else
	{
		//insert into the right subtree
		if (Tree->m_Root->m_Right != NULL)
		{
			AVLTree<Data, ConfigData> RightSubtree(m_CompareNodes);
			RightSubtree.m_Root = Tree->m_Root->m_Right;
			Insert(&RightSubtree, NodeData, HasTreeGrown);
			if (HasTreeGrown)
			{
				switch(Tree->m_Root->m_Balance++)
				{
				case -1:
					HasTreeGrown = false;
					return NodeData;
				case 0:
					HasTreeGrown = true;
					return NodeData;
				}

				if (Tree->m_Root->m_Right->m_Balance > 0)
				{
					SwingLeft(&(Tree->m_Root));
					Tree->m_Root->m_Balance = 0;
					Tree->m_Root->m_Left->m_Balance = 0;
				}
				else
				{
					SwingRight(&(Tree->m_Root->m_Right));
					SwingLeft(&(Tree->m_Root));
					BalanceAfterNastySwing(Tree->m_Root);
				}
			}
			else
			{
				Tree->m_Root->m_Right = RightSubtree.m_Root;
			}

			HasTreeGrown = false;
			return NodeData;
		}
		else
		{
			Tree->m_Root->m_Right = new AVLNode<Data>(NodeData);
			if (Tree->m_Root->m_Balance++)
			{
				HasTreeGrown = false;
				return NodeData;
			}

			HasTreeGrown = true;
			return NodeData;
		}
	}
}

#endif