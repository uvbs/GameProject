#ifndef MMANN_AVLREDBLACKTREE_H
#define MMANN_AVLREDBLACKTREE_H

////////////////////////////////////////////////
///\file AVLRedBlackTree.h
///\brief This file contains the AVLRedBlackTree and TraverseNode classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "AVLRedBlackNode.h"
#include "CompareAlgorithms.h"
#include "TestAVLRedBlackTree.h"
#include "AGenericTree.h"
#include <limits.h>

///Maximum height (depth) of an AVL red-black tree
#define RB_MAX_HEIGHT 32

///Direction of node on the stack
enum StackDirection{SDLEFT = 0, SDRIGHT};

///Stores information on the traversal of nodes by the AVL red-black tree
template <class Data>
class TraverseNode
{
public:
	///Constructor
	TraverseNode();
	
	///Detemines if a TraverseNode is initialized
	bool m_Initialize;

	///Position of the Top of the Stack
	int m_TopOfStack;

	///Node currently being traversed
	const AVLRedBlackNode<Data> *m_Node;

	///Node Stack
	const AVLRedBlackNode<Data> *m_Stack[RB_MAX_HEIGHT];
};

///\brief A templated red-black tree class
///
///This class is based off of Ben Pfaff's <pfaffben@pilot.msu.edu>
///AVL implementation of a red-black tree written in C
///
///Basically a red/black balanced tree has the following properties:-* <BR>
///1) Every node is either red or black (color is RED or BLACK)<BR>
///2) A leaf (NULL pointer) is considered black<BR>
///3) If a node is red then its children are black<BR>
///4) Every path from a node to a leaf contains the same no of black nodes
///
///3) & 4) above guarantee that the longest path (alternating
///red and black nodes) is only twice as long as the shortest
///path (all black nodes). Thus the tree remains fairly balanced.
template <class Data, class ConfigData>
class AVLRedBlackTree : public AGenericTree<Data, ConfigData>
{
public:
	///Constructor
	AVLRedBlackTree(ACompareNodesAlgorithm<Data, ConfigData>* Compare, DeleteTreeDataEnum DeleteTreeData = NO_DELETE_TREE_ITEM,
					ConfigData* pConfigData = NULL);

	///Destructor
	virtual ~AVLRedBlackTree();

	///\brief Make a copy of the tree (creates new nodes)
	///
	///\return Pointer to the new tree
	AVLRedBlackTree<Data, ConfigData>* Copy();

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

	///\brief Walks the tree in inorder, looking for a data item.  If not found, return NULL.
	///
	///\param TraverseNode used to walk the tree
	///\return A pointer to the data found.  NULL if data isn't found.
	Data* Traverse(TraverseNode<Data>* TravNode);

	///\brief Search tree for a data item close to the value of Item, and return it.
	///
	/// Search tree for an item close to the value of Item, and return it.
	/// This function will return a null pointer only if tree is empty.
	///\param NodeData is the data being searched for.
	///\return A pointer to the data found.  This function will return a null pointer only if tree is empty.
	Data* FindClose(const Data* NodeData);

	///\brief Delete a node from the tree
	///
	///\param NodeData is the data of the node to be deleted.
	///\return Data of the deleted node.  NULL if node isn't found.
	Data* Delete(const Data* NodeData);

	///\brief Verify the validity of the tree
	///
	///The tree is verified by the number of nodes in the tree matching m_Count
	///\return Whether or not the tree finished counting.
	void Verify(bool* IsDone);

	///\brief Compares two AVLRedBlackTrees
	///
	///\param Tree Tree to compare with
	///\return True if equal, False if not equal
	bool Compare(AVLRedBlackTree<Data, ConfigData>* Tree);

	///\brief Compares two nodes and their children
	///
	///\param Node1 The first node
	///\param Node2 The second node
	///\return True if equal, False if not equal
	bool CompareNodes(AVLRedBlackNode<Data>* Node1, AVLRedBlackNode<Data>* Node2);


private:
	///Root node of the tree
	AVLRedBlackNode<Data>* m_Root;

	///Number of nodes in the tree
	int m_Count;
};

/**********************************************
AVLRedBlackTree
***********************************************/

template <class Data, class ConfigData>
AVLRedBlackTree<Data, ConfigData>::AVLRedBlackTree(ACompareNodesAlgorithm<Data, ConfigData>* Compare,  DeleteTreeDataEnum DeleteTreeData,
												  ConfigData* pConfigData) : AGenericTree<Data, ConfigData>(Compare, DeleteTreeData, pConfigData)
{
	m_Root = new AVLRedBlackNode<Data>(DeleteTreeData);
	m_Count = 0;
}

template <class Data, class ConfigData>
AVLRedBlackTree<Data, ConfigData>::~AVLRedBlackTree()
{
	//Uses Knuth's Algorithm 2.3.1T as modified in exercise 13
	//(postorder traversal)

	//T1
	AVLRedBlackNode<Data> *StackANodes[RB_MAX_HEIGHT];
	unsigned long StackABits = 0;
	int StackAHeight = 0;
	AVLRedBlackNode<Data> *CurrentNode = m_Root->m_Left;

	while(true)
	{
		//T2
		while (CurrentNode != NULL)
		{
			//T3
			StackABits &= ~(1ul << StackAHeight);
			StackANodes[StackAHeight++] = CurrentNode;
			CurrentNode = CurrentNode->m_Left;		//going left
		}

		//T4
		while(true)
		{
			if (!StackAHeight)
			{
				delete m_Root;
				return;
			}

			CurrentNode = StackANodes[--StackAHeight];
			if (!(StackABits & (1ul << StackAHeight)))
			{
				StackABits |= (1ul << StackAHeight++);
				CurrentNode = CurrentNode->m_Right; ///going Right;
				break;
			}

			delete CurrentNode;
		}
	}

	delete m_Root;
}

template <class Data, class ConfigData>
AVLRedBlackTree<Data, ConfigData>* AVLRedBlackTree<Data, ConfigData>::Copy()
{
	/* This is a combination of Knuth's Algorithm 2.3.1C (copying a
	 binary tree) and Algorithm 2.3.1T as modified by exercise 12
	(preorder traversal). */

	AVLRedBlackTree<Data, ConfigData>* NewTree;

	//PT1
	const AVLRedBlackNode<Data> *StackPANode[RB_MAX_HEIGHT];
	const AVLRedBlackNode<Data> **StackPAStackPointer = StackPANode;
	NewTree = new AVLRedBlackTree<Data, ConfigData>(m_CompareNodes, m_DeleteTreeData, m_ConfigData);
	NewTree->m_Count = m_Count;
	const AVLRedBlackNode<Data> *p = m_Root;

	//QT1
	AVLRedBlackNode<Data> *StackQANode[RB_MAX_HEIGHT];
	AVLRedBlackNode<Data> **StackQAStackPointer = StackQANode;
	AVLRedBlackNode<Data> *q = NewTree->m_Root;

	while(true)
	{
		//C4
		if (p->m_Left != NULL)
		{
			AVLRedBlackNode<Data> *r = new AVLRedBlackNode<Data>(m_DeleteTreeData);
			q->m_Left = r;
		}

		//C5 Find preorder successor of P and Q
		goto start;
		while(true)
		{
			//PT2
			while (p != NULL)
			{
				goto escape;
				start:
				//PT3
				*StackPAStackPointer++ = p;
				*StackQAStackPointer++ = q;
				p = p->m_Left;
				q = q->m_Left;
			}

			//PT4
			if (StackPAStackPointer == StackPANode)
			{
				assert(StackQAStackPointer == StackQANode);
				return NewTree;
			}

			p = *--StackPAStackPointer;
			q = *--StackQAStackPointer;

			//PT5
			p = p->m_Right;
			q = q->m_Right;
		}

		escape:
		//C2
		if (p->m_Right)
		{
			AVLRedBlackNode<Data> *r = new AVLRedBlackNode<Data>(m_DeleteTreeData);
			q->m_Right = r;
		}

		//C3
		q->m_Color = p->m_Color;
		q->m_Data = p->m_Data;
	}
}

template <class Data, class ConfigData>
bool AVLRedBlackTree<Data, ConfigData>::Compare(AVLRedBlackTree<Data, ConfigData>* Tree)
{
	if (Tree == NULL)
		return false;

	if ((Tree->m_CompareNodes != m_CompareNodes) || 
		(Tree->m_ConfigData != m_ConfigData))
		return false;

	return CompareNodes(m_Root, Tree->m_Root);
}

template <class Data, class ConfigData>
Data* AVLRedBlackTree<Data, ConfigData>::Traverse(TraverseNode<Data>* TravNode)
{
	//Uses Knuth's algorithm 2.3.1T (inorder traversal)
	if (!TravNode->m_Initialize)
	{
		TravNode->m_Initialize = true;
		TravNode->m_TopOfStack = 0;
		TravNode->m_Node = m_Root.m_Left;
	}
	else
	{
		//T5
		TravNode->m_Node = m_Root.m_Right;
	}

	while(true)
	{
		//T2
		while (TravNode->m_Node != NULL)
		{
			//T3
			TravNode->m_Stack[TravNode->m_TopOfStack++] = TravNode->m_Node;
			TravNode->m_Node = TravNode->m_Node->m_Left;
		}

		//T4
		if (TravNode->m_TopOfStack == 0)
		{
			TravNode->m_Initialize = false;
			return NULL;
		}

		TravNode->m_Node = TravNode->m_Stack[--TravNode->m_TopOfStack];

		//T5
		return TravNode->m_Node->m_Data;
	}
}

template <class Data, class ConfigData>
Data* AVLRedBlackTree<Data, ConfigData>::Search(Data* NodeData, bool* NodeInserted)
{
	/* Algorithm based on RB-Insert from section 14.3 of _Introduction
	 to Algorithms_, Cormen et al., MIT Press 1990, ISBN
	 0-262-03141-8. */

	*NodeInserted = false;
	AVLRedBlackNode<Data> *StackANodes[RB_MAX_HEIGHT];
	StackDirection StackADirections[RB_MAX_HEIGHT];
	int StackAPointer = 1;

	AVLRedBlackNode<Data> *t, *x, *y, *n;

	t = m_Root;
	x = t->m_Left;

	if (x == NULL)
	{
		m_Count++;
		assert(m_Count == 1);
		x = t->m_Left = new AVLRedBlackNode<Data>(m_DeleteTreeData);
		*NodeInserted = true;
		x->m_Data = NodeData;
		x->m_Color = BLACK;
		return NULL;
	}

	StackADirections[0] = SDLEFT;
	StackANodes[0] = m_Root;

	while(true)
	{
		int CompareValue = m_CompareNodes->Compare(NodeData, x->m_Data, m_ConfigData);
		if (CompareValue < 0)
		{
			StackANodes[StackAPointer] = x;
			StackADirections[StackAPointer++] = SDLEFT;
			y = x->m_Left;
			if (y == NULL)
			{
				n = x = x->m_Left = new AVLRedBlackNode<Data>(m_DeleteTreeData);
				*NodeInserted = true;
				break;
			}	
		}
		else if (CompareValue > 0)
		{
			StackANodes[StackAPointer] = x;
			StackADirections[StackAPointer++] = SDRIGHT;
			y = x->m_Right;
			if (y == NULL)
			{
				n = x = x->m_Right = new AVLRedBlackNode<Data>(m_DeleteTreeData);
				*NodeInserted = true;
				break;
			}
		}
		else
			return x->m_Data;

		x = y;
	}

	m_Count++;
	x->m_Data = NodeData;
	x->m_Left = x->m_Right = NULL;
	x->m_Color = RED;

	while(true)
	{
		if ((StackAPointer < 3) || (StackANodes[StackAPointer - 1]->m_Color != RED))
			break;

		if (StackADirections[StackAPointer - 2] == SDLEFT)
		{
			y = StackANodes[StackAPointer - 2]->m_Right;
			if (y != NULL && y->m_Color == RED)
			{
				//case 1
				StackANodes[StackAPointer - 1]->m_Color = y->m_Color = BLACK;
				StackANodes[StackAPointer - 2]->m_Color = RED;
				StackAPointer -= 2;
			}
			else
			{
				if (StackADirections[StackAPointer - 1] == SDRIGHT)
				{
					//case 2
					x = StackANodes[StackAPointer - 1];
					y = x->m_Right;
					x->m_Right = y->m_Left;
					y->m_Left = x;
					StackANodes[StackAPointer - 2]->m_Left = y;
				}
				else
					y = StackANodes[StackAPointer - 1];

				//case 3
				x = StackANodes[StackAPointer - 2];
				x->m_Color = RED;
				y->m_Color = BLACK;
				x->m_Left = y->m_Right;
				y->m_Right = x;
				StackADirections[StackAPointer - 3] ? 
					StackANodes[StackAPointer - 3]->m_Right = y :
					StackANodes[StackAPointer - 3]->m_Left = y;
				break;
			}
		}
		else
		{
			y = StackANodes[StackAPointer - 2]->m_Left;
			if (y != NULL && y->m_Color == RED)
			{
				//case 1
				StackANodes[StackAPointer - 1]->m_Color = y->m_Color = BLACK;
				StackANodes[StackAPointer - 2]->m_Color = RED;
				StackAPointer -= 2;
			}
			else
			{
				if (StackADirections[StackAPointer - 1] == SDLEFT)
				{
					//case 2
					x = StackANodes[StackAPointer - 1];
					y = x->m_Left;
					x->m_Left = y->m_Right;
					y->m_Right = x;
					StackANodes[StackAPointer - 2]->m_Right = y;
				}
				else
					y = StackANodes[StackAPointer - 1];

				//case 3
				x = StackANodes[StackAPointer - 2];
				x->m_Color = RED;
				y->m_Color = BLACK;

				x->m_Right = y->m_Left;
				y->m_Left = x;
				StackADirections[StackAPointer - 3] ? 
					StackANodes[StackAPointer - 3]->m_Right = y :
					StackANodes[StackAPointer - 3]->m_Left = y;
				break;
			}
		}
	}

	m_Root->m_Left->m_Color = BLACK;

	return NULL;
}

template <class Data, class ConfigData>
Data* AVLRedBlackTree<Data, ConfigData>::Find(Data* NodeData)
{
	const AVLRedBlackNode<Data> *CurrentNode;

	for (CurrentNode = m_Root->m_Left; CurrentNode != NULL;)
	{
		int CompareValue = m_CompareNodes->Compare(NodeData, CurrentNode->m_Data, m_ConfigData);
		if (CompareValue < 0)
			CurrentNode = CurrentNode->m_Left;
		else if (CompareValue > 0)
			CurrentNode = CurrentNode->m_Right;
		else
			return CurrentNode->m_Data;
	}

	return NULL;
}

template <class Data, class ConfigData>
Data* AVLRedBlackTree<Data, ConfigData>::FindClose(const Data* NodeData)
{
	const AVLRedBlackNode<Data> *CurrentNode;

	CurrentNode = m_Root.m_Left;
	if (CurrentNode == NULL)
		return NULL;

	while(true)
	{
		int CompareValue = m_CompareNodes->Compare(NodeData, CurrentNode->m_Data, m_ConfigData);
		StackDirection Direction;

		if (CompareValue < 0)
			Direction = SDLEFT;
		else if (CompareValue > 0)
			Direction = SDRIGHT;
		else
			return CurrentNode->m_Data;

		if ((Direction == SDRIGHT) && CurrentNode->m_Right)
			CurrentNode = CurrentNode->m_Right;
		else if ((Direction == SDLEFT) && CurrentNode->m_Left)
			CurrentNode = CurrentNode->m_Left;
		else
			return CurrentNode->m_Data;
	}
}

template <class Data, class ConfigData>
Data* AVLRedBlackTree<Data, ConfigData>::Delete(const Data* NodeData)
{
	/* Algorithm based on RB-Delete and RB-Delete-Fixup from section
	 14.4 of _Introduction to Algorithms_, Cormen et al., MIT Press
	 1990, ISBN 0-262-03141-8. */

	AVLRedBlackNode<Data> *StackPNodes[RB_MAX_HEIGHT];
	StackDirection StackPDirections[RB_MAX_HEIGHT];
	int StackPPointer = 1;

	AVLRedBlackNode<Data> *w, *x, *y, *z;

	StackPDirections[0] = SDLEFT;
	StackPNodes[0] = m_Root;
	z = m_Root->m_Left;
	while(true)
	{
		if (z == NULL)
			return NULL;

		int CompareValue = m_CompareNodes->Compare(NodeData, z->m_Data, m_ConfigData);
		if (CompareValue == 0)
			break;

		StackPNodes[StackPPointer] = z;
		if (CompareValue < 0)
		{
			z = z->m_Left;
			StackPDirections[StackPPointer] = SDLEFT;
		}
		else if (CompareValue > 0)
		{
			z = z->m_Right;
			StackPDirections[StackPPointer] = SDRIGHT;
		}
		
		StackPPointer++;
	}

	m_Count--;

	NodeData = z->m_Data;

	//RB-Delete: Line 1
	if (z->m_Left == NULL || z->m_Right == NULL)
	{
		//Line 2
		y = z;

		//Lines 4-6
		if (y->m_Left != NULL)
			x = y->m_Left;
		else
			x = y->m_Right;

		StackPDirections[StackPPointer - 1] ? 
			StackPNodes[StackPPointer - 1]->m_Right = x :
			StackPNodes[StackPPointer - 1]->m_Left = x; 
	}
	else
	{
		StackPNodes[StackPPointer] = z;
		StackPDirections[StackPPointer++] = SDRIGHT;

		//Line 3
		y = z->m_Right;
		while (y->m_Left)
		{
			StackPNodes[StackPPointer] = y;
			StackPDirections[StackPPointer++] = SDLEFT;
			y = y->m_Left;
		}

		//Lines 4-6
		x = y->m_Right;

		//Lines 13-15
		z->m_Data = y->m_Data;
		StackPDirections[StackPPointer - 1] ? 
			StackPNodes[StackPPointer - 1]->m_Right = x :
			StackPNodes[StackPPointer - 1]->m_Left = x; 
	}

	//Line 16
	if (y->m_Color == RED)
	{
		delete y;
		return (Data*)NodeData;
	}

	delete y;

	//Numbers below are line numbers from the RB-Delete-Fixup
	while (StackPPointer > 1 && (x == NULL || x->m_Color == BLACK))
	//1
	{
		if (StackPDirections[StackPPointer - 1] == SDLEFT)
		//2
		{
			w = StackPNodes[StackPPointer - 1]->m_Right;
			//3
			if (w->m_Color == RED)
			//4
			{
				//Case1
				w->m_Color = BLACK;
				//5
				StackPNodes[StackPPointer - 1]->m_Color = RED;
				//6
				StackPNodes[StackPPointer - 1]->m_Right = w->m_Left;
				//7
				w->m_Left = StackPNodes[StackPPointer - 1];
				StackPDirections[StackPPointer - 2] ? 
					StackPNodes[StackPPointer - 2]->m_Right = w :
					StackPNodes[StackPPointer - 2]->m_Left = w;

				StackPNodes[StackPPointer] = StackPNodes[StackPPointer - 1];
				StackPDirections[StackPPointer] = SDLEFT;
				StackPNodes[StackPPointer - 1] = w;
				StackPPointer++;
				w = StackPNodes[StackPPointer - 1]->m_Right;
				//8
			}

			if ((w->m_Left == NULL || w->m_Left->m_Color == BLACK) &&
				(w->m_Right == NULL || w->m_Right->m_Color == BLACK))
			//9
			{
				//Case2
				w->m_Color = RED;
				//10
				x = StackPNodes[StackPPointer - 1];
				//11
				StackPPointer--;
			}
			else
			{
				if (w->m_Right == NULL || w->m_Right->m_Color == BLACK)
				//12
				{
					//Case 3
					w->m_Left->m_Color = BLACK;
					//13
					w->m_Color = RED;
					//14
					y = w->m_Left;
					//15
					w->m_Left = y->m_Right;
					y->m_Right = w;
					w = StackPNodes[StackPPointer - 1]->m_Right = y;
					//16
				}

				//Case 4
				w->m_Color = StackPNodes[StackPPointer - 1]->m_Color;
				//17
				StackPNodes[StackPPointer - 1]->m_Color = BLACK;
				//18
				w->m_Right->m_Color = BLACK;
				//19
				StackPNodes[StackPPointer - 1]->m_Right = w->m_Left;
				//20
				w->m_Left = StackPNodes[StackPPointer - 1];
				StackPDirections[StackPPointer - 2] ? 
					StackPNodes[StackPPointer - 2]->m_Right = w :
					StackPNodes[StackPPointer - 2]->m_Left = w;
				x = m_Root->m_Left;
				//21
				break;
			}
		}
		else
		{
			w = StackPNodes[StackPPointer - 1]->m_Left;
			if (w->m_Color == RED)
			{
				//Case 1
				w->m_Color = BLACK;
				StackPNodes[StackPPointer - 1]->m_Color = RED;
				
				StackPNodes[StackPPointer - 1]->m_Left = w->m_Right;
				w->m_Right = StackPNodes[StackPPointer - 1];
				StackPDirections[StackPPointer - 2] ? 
					StackPNodes[StackPPointer - 2]->m_Right = w :
					StackPNodes[StackPPointer - 2]->m_Left = w;
				
				StackPNodes[StackPPointer] = StackPNodes[StackPPointer - 1];
				StackPDirections[StackPPointer] = SDRIGHT;
				StackPNodes[StackPPointer - 1] = w;
				StackPPointer++;

				w = StackPNodes[StackPPointer - 1]->m_Left;
			}

			if ((w->m_Left == NULL || w->m_Left->m_Color == BLACK) && 
				(w->m_Right == NULL || w->m_Right->m_Color == BLACK))
			{
				//Case 2
				w->m_Color = RED;
				x = StackPNodes[StackPPointer - 1];
				StackPPointer--;
			}
			else
			{
				if (w->m_Left == NULL || w->m_Left->m_Color == BLACK)
				{
					//Case 3
					w->m_Right->m_Color = BLACK;
					w->m_Color = RED;

					y = w->m_Right;
					w->m_Right = y->m_Left;
					y->m_Left = w;

					w = StackPNodes[StackPPointer - 1]->m_Left = y;
				}

				//Case 4
				w->m_Color = StackPNodes[StackPPointer - 1]->m_Color;
				StackPNodes[StackPPointer - 1]->m_Color = BLACK;
				w->m_Left->m_Color = BLACK;

				StackPNodes[StackPPointer - 1]->m_Left = w->m_Right;
				w->m_Right = StackPNodes[StackPPointer - 1];
				StackPDirections[StackPPointer - 2] ? 
					StackPNodes[StackPPointer - 2]->m_Right = w :
					StackPNodes[StackPPointer - 2]->m_Left = w;

				x = m_Root->m_Left;
				break;
			}
		}
	}

	if (x != NULL)
		x->m_Color = BLACK;
	//23

	return (Data*) NodeData;
}

template <class Data, class ConfigData>
bool AVLRedBlackTree<Data, ConfigData>::CompareNodes(AVLRedBlackNode<Data>* Node1, AVLRedBlackNode<Data>* Node2)
{
	if ((Node1 == NULL || Node2 == NULL))
	{
		if ((Node1 == NULL && Node2 == NULL))
			return true;

		return false;
	}

	if ((Node1->m_Data != Node2->m_Data) || (Node1->m_Color != Node2->m_Color) || 
		((Node1->m_Left != NULL) ^ (Node2->m_Left != NULL)) ||
		((Node1->m_Right != NULL) ^ (Node2->m_Right != NULL)))
	{
		printf("Nodes differ: %d Node2=%d Node1->m_Color=%d Node2->m_Color=%d a:",
			Node1->m_Data, Node2->m_Data, Node1->m_Color, Node2->m_Color);
		if (Node1->m_Left)
			printf("l");
		if (Node1->m_Right)
			printf("r");
		printf(" b:");
		if (Node2->m_Left)
			printf("l");
		if (Node2->m_Right)
			printf("r");
		printf("\n");
		return false;
	}

	bool Compare = true;
	if (Node1->m_Left != NULL)
		Compare = CompareNodes(Node1->m_Left, Node2->m_Left);
	if ((Compare) && (Node1->m_Right != NULL))
		Compare = CompareNodes(Node1->m_Right, Node2->m_Right);

	return Compare;
}	

template <class Data, class ConfigData>
void AVLRedBlackTree<Data, ConfigData>::Verify(bool* IsDone)
{
	int Count = 0;
	RecurseTree (m_Root.m_Left, &Count, INT_MIN, INT_MAX, Done);
	if (Count != m_Count)
	{
		cout << " Tree has " << Count << " nodes, but tree count is " 
			 << m_Count << "." << endl;
		*IsDone = true;
	}
}

/**********************************************
TraverseNode
***********************************************/

//constructor
template <class Data>
TraverseNode<Data>::TraverseNode()
{
	m_Initialize = false;
	m_TopOfStack = 0;
	m_Node = NULL;
}

#endif