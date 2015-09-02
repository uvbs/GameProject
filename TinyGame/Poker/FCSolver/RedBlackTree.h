#ifndef MMANN_CPP_REDBLACK_H
#define MMANN_CPP_REDBLACK_H

////////////////////////////////////////////////
///\file RedBlackTree.h
///\brief This file contains the RedBlackTree class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include "CompareAlgorithms.h"
#include "RedBlackNode.h"
#include "AGenericTree.h"

///Different modes that a RedBlackNode can looked up
enum RedBlackTreeLookup {RBNONE = -1, RBEQUAL = 0, RBGTEQ, RBLTEQ, RBLESS, RBGREAT, RBNEXT, RBPREV, RBFIRST, RBLAST};

///\brief A templated red-black tree class
///
///This class is based off of Damian Ivereigh's
///implementation of a red-black tree written in C<BR>
///The algorithm is the fairly standard red/black taken from 
///"Introduction to Algorithms" by Cormen, Leiserson & Rivest.
///
///Basically a red/black balanced tree has the following properties:-* <BR>
///1) Every node is either red or black (color is RED or BLACK)<BR>
///2) A leaf (RBNULL pointer) is considered black<BR>
///3) If a node is red then its children are black<BR>
///4) Every path from a node to a leaf contains the same no of black nodes
///
///3) & 4) above guarantee that the longest path (alternating
///red and black nodes) is only twice as long as the shortest
///path (all black nodes). Thus the tree remains fairly balanced.
template <class Data, class ConfigData>
class RedBlackTree : public AGenericTree<Data, ConfigData>
{
public:
	///Constructor
	RedBlackTree(ACompareNodesAlgorithm<Data, ConfigData>* Compare, DeleteTreeDataEnum DeleteTreeData = NO_DELETE_TREE_ITEM,
				 ConfigData* pConfigData = NULL);
	
	///Destructor
	~RedBlackTree();

	///\brief Find a data item.  If not found, insert it
	///
	///\param NodeData is the value being searched for.
	///\param NodeInserted is a flag to determine if the data was inserted
	///\return A pointer to the data found. NULL if data is inserted
	virtual Data* Search(Data* NodeData, bool* NodeInserted);

	///\brief Find a data item.  If not found, return NULL
	///
	///\param NodeData is the value being searched for.
	///\return A pointer to the data found.  NULL if data isn't found.
	virtual Data* Find(Data* NodeData);

	///\brief Search for a value according to a mode
	///
	///\param Mode to look for value
	///\param NodeData is the value being searched for
	Data* Lookup(RedBlackTreeLookup Mode, Data* NodeData);

	///\brief Delete a node from the tree
	///
	///\param NodeData is the data of the node to be deleted.
	///\return Data of the deleted node.  NULL if node isn't found.
	Data* Delete(Data* NodeData);

protected:

	///\brief Find a data item, conditionally insert it if not found
	///
	///\param Insert determines whether or not to insert an unfound node.
	///\param NodeData is the value being searched for.
	///\param NodeInserted is a flag to determine if the data was inserted
	///\return A pointer to the data found or inserted
	RedBlackNode<Data>* Traverse(bool Insert, Data* NodeData, bool* NodeInserted);

	///\brief Delete a node from the tree
	///
	///\param Node is the node to be deleted.
	///\param Root is the parent node to be modified after the deletion
	void DeleteNode(RedBlackNode<Data>** Root, RedBlackNode<Data>* Node);

	///\brief Rebalance the tree after a deletion
	///
	///\param Node is the node to be deleted.
	///\param Root is the parent node to be modified after the deletion
	void DeleteNodeFix(RedBlackNode<Data>** Root, RedBlackNode<Data>* Node);

	///\brief Rotate node one node to the left
	///
	///\param x is the node to be moved
	///\param Root is the parent node to be modified after the move
	void LeftRotate(RedBlackNode<Data>** Root, RedBlackNode<Data>* x);

	///\brief Rotate node one node to the right
	///
	///\param y is the node to be moved
	///\param Root is the parent node to be modified after the move
	void RightRotate(RedBlackNode<Data>** Root, RedBlackNode<Data>* y);
	///\brief Get the smallest value greater than x
	///
	///\param Node contains the value x
	///\return A pointer to the smallest value greater than x
	RedBlackNode<Data>* GetSuccessor(const RedBlackNode<Data>* Node);

	///\brief Get the largest value smaller than x
	///
	///\param Node contains the value x
	///\return A pointer to the largest key smaller than x
	RedBlackNode<Data>* GetPredecessor(const RedBlackNode<Data>* Node);

	///Root node of the tree
	RedBlackNode<Data>* m_Root;
};

template <class Data, class ConfigData>
RedBlackTree<Data, ConfigData>::RedBlackTree(ACompareNodesAlgorithm<Data, ConfigData>* Compare, DeleteTreeDataEnum DeleteTreeData,
				 ConfigData* pConfigData) : AGenericTree<Data, ConfigData>(Compare, DeleteTreeData, pConfigData)
{
	m_Root = NULL;
}

template <class Data, class ConfigData>
RedBlackTree<Data, ConfigData>::~RedBlackTree()
{
	if (m_Root != NULL)
		DeleteAllRedBlackNodes(m_Root, m_DeleteTreeData);
}

template <class Data, class ConfigData>
Data* RedBlackTree<Data, ConfigData>::Search(Data* NodeData, bool* NodeInserted)
{
	RedBlackNode<Data>* Node = Traverse(true, NodeData, NodeInserted);

	return ((*NodeInserted) ? NULL : Node->m_Data);
}

template <class Data, class ConfigData>
Data* RedBlackTree<Data, ConfigData>::Find(Data* NodeData)
{
	RedBlackNode<Data>* x;
	bool DummyIsInsert;
	
	//if we have an empty tree, return
	if (m_Root == NULL)
		return NULL;

	x = Traverse(false, NodeData, &DummyIsInsert);

	return (x == NULL ? NULL : x->m_Data);
}

template <class Data, class ConfigData>
Data* RedBlackTree<Data, ConfigData>::Lookup(RedBlackTreeLookup Mode, Data* NodeData)
{
	if (m_Root == NULL)
		return NULL;

	RedBlackNode<Data> *x, *y;
	int CompareValue;
	bool Found = false;

	y = NULL;
	x = m_Root;

	if (Mode == RBFIRST)
	{
		//keep going until you hit NULL
		while (x != NULL)
		{
			y = x;
			x = x->m_Left;
		}

		return y->m_Data;
	}
	else if (Mode == RBLAST)
	{
		//keep going until you hit NULL
		while (x != NULL)
		{
			y = x;
			x = x->m_Right;
		}

		return y->m_Data;
	}

	//walk x down the tree
	while ((x != NULL) && (!Found))
	{
		y=x;
		CompareValue = m_CompareNodes->Compare(NodeData, x->m_Data, m_ConfigData);
		if (CompareValue < 0)
			x = x->m_Left;
		else if (CompareValue > 0)
			x = x->m_Right;
		else
			Found = true;
	}

	if (Found && (Mode == RBEQUAL || Mode == RBGTEQ || Mode == RBLTEQ))
		return x->m_Data;

	if (!Found && (Mode == RBEQUAL || Mode == RBNEXT || Mode == RBPREV))
		return NULL;

	if ((Mode == RBGTEQ) || (!Found && Mode == RBGREAT))
	{
		if (CompareValue > 0)
			return GetSuccessor(y)->m_Data;
		else
			return y->m_Data;
	}

	if ((Mode == RBLTEQ) || (!Found && Mode == RBLESS))
	{
		if (CompareValue < 0)
			return GetPredecessor(y)->m_Data;
		else
			return y->m_Data;
	}

	if ((Mode == RBNEXT) || (Found && Mode == RBGREAT))
	{
		RedBlackNode<Data>* Dummy = GetSuccessor(x);
		if (Dummy != NULL)
			return Dummy->m_Data;
		return NULL;
	}
	if ((Mode == RBPREV) || (Found && Mode == RBLESS))
	{
		RedBlackNode<Data>* Dummy = GetPredecessor(x);
		if (Dummy != NULL)
			return Dummy->m_Data;
		return NULL;
	}

	//all cases should be covered
	assert(false);
	return NULL;
}

template <class Data, class ConfigData>
RedBlackNode<Data>* RedBlackTree<Data, ConfigData>::Traverse(bool Insert, Data* NodeData, bool* NodeInserted)
{
	*NodeInserted = false;
	RedBlackNode<Data> *x, *y, *z;
	int CompareValue;
	bool Found = false;

	y = NULL;
	x = m_Root;

	//walk x down the tree
	while ((x != NULL) && (!Found))
	{
		y = x;

		CompareValue = m_CompareNodes->Compare(NodeData, x->m_Data, m_ConfigData);

		if (CompareValue < 0)
			x = x->m_Left;
		else if (CompareValue > 0)
			x = x->m_Right;
		else
			Found = true;
	}

	if (Found || !Insert)
		return x;

	z = new RedBlackNode<Data>(NodeData);
	*NodeInserted = true;
	z->m_Up = y;
	
	if (y == NULL)
		m_Root = z;
	else
	{
		CompareValue = m_CompareNodes->Compare(z->m_Data, y->m_Data, m_ConfigData);
		if (CompareValue < 0)
			y->m_Left = z;
		else
			y->m_Right = z;
	}

	z->m_Left = NULL;
	z->m_Right = NULL;

	//color new node red
	z->m_Color = RED;

	// Having added a red node, we must now walk back up the tree balancing
	// it, by a series of rotations and changing of colours
	x=z;

	// While we are not at the top and our parent node is red
	// N.B. Since the root node is garanteed black, then we
	// are also going to stop if we are the child of the root

	while ((x != m_Root) && (x->m_Up->m_Color == RED))
	{
		//if our parent is on the left side of our grandparent
		if (x->m_Up == x->m_Up->m_Up->m_Left)
		{
			//get the right side of our grandparent
			y = x->m_Up->m_Up->m_Right;
			if ((y != NULL) && (y->m_Color == RED))
			{
				//make parent black
				x->m_Up->m_Color = BLACK;
				//make uncle black
				y->m_Color = BLACK;
				//make grandparent red
				x->m_Up->m_Up->m_Color = RED;
				//now consider grandparent
				x = x->m_Up->m_Up;
			}
			else
			{
				//if we are on the right side of parent
				if (x == x->m_Up->m_Right)
				{
					//move up parent
					x = x->m_Up;
					LeftRotate(&m_Root, x);
				}

				//make parent black
				x->m_Up->m_Color = BLACK;
				//make grandparent red
				x->m_Up->m_Up->m_Color = RED;
				//right rotate grandparent
				RightRotate(&m_Root, x->m_Up->m_Up);
			}
		}
		else
		{
			//everything here is same as above, just exchange left for right
			y = x->m_Up->m_Up->m_Left;
			if ((y != NULL) && (y->m_Color == RED))
			{
				//make parent black
				x->m_Up->m_Color = BLACK;
				//make uncle black
				y->m_Color = BLACK;
				//make grandparent red
				x->m_Up->m_Up->m_Color = RED;
				//now consider grandparent
				x = x->m_Up->m_Up;
			}
			else
			{
				//if we are on the right side of parent
				if (x == x->m_Up->m_Left)
				{
					//move up parent
					x = x->m_Up;
					RightRotate(&m_Root, x);
				}

				//make parent black
				x->m_Up->m_Color = BLACK;
				//make grandparent red
				x->m_Up->m_Up->m_Color = RED;
				//right rotate grandparent
				LeftRotate(&m_Root, x->m_Up->m_Up);
			}
		}
	}

	//set the root node black
	m_Root->m_Color = BLACK;

	return z;
}

template <class Data, class ConfigData>
Data* RedBlackTree<Data, ConfigData>::Delete(Data* NodeData)
{
	RedBlackNode<Data> *x;
	Data* y;
	bool DummyIsInsert;

	x = Traverse(false, NodeData, &DummyIsInsert);

	if (x == NULL)
		return NULL;

	y = x->m_Data;

	DeleteNode(&m_Root, x);

	return y;
}

template <class Data, class ConfigData>
void RedBlackTree<Data, ConfigData>::DeleteNode(RedBlackNode<Data>** Root, RedBlackNode<Data>* Node)
{
	RedBlackNode<Data> *x,
					  *y;

	if ((Node->m_Left == NULL) || (Node->m_Right == NULL))
		y = Node;
	else
		y = GetSuccessor(Node);

	if (y->m_Left != NULL)
		x = y->m_Left;
	else
		x = y->m_Right;

	if (x != NULL)
		x->m_Up = y->m_Up;

	if (y->m_Up == NULL)
	{
		*Root = x;
	}
	else
	{
		if (y == y->m_Up->m_Left)
			y->m_Up->m_Left = x;
		else
			y->m_Up->m_Right = x;
	}

	if (y != Node)
		Node->m_Data = y->m_Data;

	if (y->m_Color == BLACK)
		DeleteNodeFix(Root, x);

	delete y;
}

template <class Data, class ConfigData>
void RedBlackTree<Data, ConfigData>::DeleteNodeFix(RedBlackNode<Data>** Root, RedBlackNode<Data>* Node)
{
	RedBlackNode<Data> *w;

	while ((Node != *Root) && (Node != NULL) && (Node->m_Color == BLACK))
	{
		if (Node == Node->m_Up->m_Left)
		{
			w = Node->m_Up->m_Right;
			if (w->m_Color == RED)
			{
				w->m_Color = BLACK;
				Node->m_Up->m_Color = RED;
				LeftRotate(Root, Node->m_Up);
				w = Node->m_Up->m_Right;
			}

			if ((w->m_Left->m_Color == BLACK) && 
				(w->m_Right->m_Color == BLACK))
			{
				w->m_Color = RED;
				Node = Node->m_Up;
			}
			else
			{
				if (w->m_Right->m_Color == BLACK)
				{
					w->m_Left->m_Color = BLACK;
					w->m_Color = RED;
					RightRotate(Root, w);
					w = Node->m_Up->m_Right;
				}

				w->m_Color = Node->m_Up->m_Color;
				Node->m_Up->m_Color = BLACK;
				w->m_Right->m_Color = BLACK;
				LeftRotate(Root, Node->m_Up);
				Node = *Root;
			}

		}
		else
		{
			w = Node->m_Up->m_Left;
			if (w->m_Color == RED)
			{
				w->m_Color = BLACK;
				Node->m_Up->m_Color = RED;
				RightRotate(Root, Node->m_Up);
				w = Node->m_Up->m_Left;
			}

			if ((w->m_Right->m_Color == BLACK) && 
				(w->m_Left->m_Color == BLACK))
			{
				w->m_Color = RED;
				Node = Node->m_Up;
			}
			else
			{
				if (w->m_Left->m_Color == BLACK)
				{
					w->m_Right->m_Color = BLACK;
					w->m_Color = RED;
					LeftRotate(Root, w);
					w = Node->m_Up->m_Left;
				}

				w->m_Color = Node->m_Up->m_Color;
				Node->m_Up->m_Color = BLACK;
				w->m_Left->m_Color = BLACK;
				RightRotate(Root, Node->m_Up);
				Node = *Root;
			}
		}
	}

	if (Node != NULL)
		Node->m_Color = BLACK;
}

/*
** Rotate our tree thus:-
**
**             X        LeftRotate(Node)--->		     Y
**           /   \                                     /   \
**          A	  Y    <---RightRotate(Node)		  X    C
**              /   \                               /   \
**             B     C                             A     B
**
** N.B. This does not change the ordering.
**
** We assume that Node is not NULL
*/
template <class Data, class ConfigData>
void RedBlackTree<Data, ConfigData>::LeftRotate(RedBlackNode<Data>** Root, RedBlackNode<Data>* x)
{
	RedBlackNode<Data> *y;

	assert(x != NULL);
	assert(x->m_Right != NULL);

	y = x->m_Right;	//set Y

	//turn Y's left subtree into node's right subtree (move B)
	x->m_Right = y->m_Left;

	//if B is not NULL, set its parent to be X
	if (y->m_Left != NULL)
		y->m_Left->m_Up = x;

	// set Y's parent to be what X's parent was
	y->m_Up = x->m_Up;

	//if X was the root
	if (x->m_Up == NULL)
	{
		*Root = y;
	}
	else
	{
		//set X's parent left or right pointer to Y
		if (x == x->m_Up->m_Left)
			x->m_Up->m_Left = y;
		else
			x->m_Up->m_Right = y;
	}

	//put X on Y's left
	y->m_Left = x;
	
	//set X's parent to be Y
	x->m_Up = y;

}

template <class Data, class ConfigData>
void RedBlackTree<Data, ConfigData>::RightRotate(RedBlackNode<Data>** Root, RedBlackNode<Data>* y)
{
	RedBlackNode<Data> *x;

	assert(y != NULL);
	assert(y->m_Left != NULL);

	x = y->m_Left; //set x

	// Turn X's right subtree into Y's left subtree (move B) 
	y->m_Left = x->m_Right;

	/* If B is not null, set it's parent to be Y */
	if (x->m_Right != NULL)
		x->m_Right->m_Up = y;

	/* Set X's parent to be what Y's parent was */
	x->m_Up = y->m_Up;

	/* if Y was the root */
	if (y->m_Up == NULL)
	{
		*Root = x;
	}
	else
	{
		/* Set Y's parent's left or right pointer to be X */
		if (y == y->m_Up->m_Left)
		{
			y->m_Up->m_Left = x;
		}
		else
		{
			y->m_Up->m_Right = x;
		}
	}

	/* Put Y on X's right */
	x->m_Right = y;

	/* Set Y's parent to be X */
	y->m_Up = x;
}

template <class Data, class ConfigData>
RedBlackNode<Data>* RedBlackTree<Data, ConfigData>::GetSuccessor(const RedBlackNode<Data>* Node)
{
	RedBlackNode<Data> *y;

	if (Node->m_Right != NULL)
	{
		// If right is not NULL then go right one and then keep going left until 
		// we find a node with no left pointer.
		for (y = Node->m_Right; y->m_Left != NULL; y = y->m_Left);
	}
	else
	{
		// Go up the tree until we get to a node that is on the
		// left of its parent (or the root) and then return the parent
		y = Node->m_Up;
		while ((y != NULL) && (Node == y->m_Right))
		{
			Node = y;
			y = y->m_Up;
		}
	}

	return y;
}

template <class Data, class ConfigData>
RedBlackNode<Data>* RedBlackTree<Data, ConfigData>::GetPredecessor(const RedBlackNode<Data>* Node)
{
	RedBlackNode<Data> *y;

	if (Node->m_Left != NULL)
	{
		// If left is not NULL then go left one and 
		//then keep going right until we find a node with no right pointer.
		for (y = Node->m_Left; y->m_Right != NULL; y = y->m_Right);
	}
	else
	{
		/* Go up the tree until we get to a node that is on the
		** right of its parent (or the root) and then return the
		** parent.
		*/
		y = Node->m_Up;
		while ((y != NULL) && (Node == y->m_Left))
		{
			Node = y;
			y = y->m_Up;
		}
	}

	return y;
}

#endif
