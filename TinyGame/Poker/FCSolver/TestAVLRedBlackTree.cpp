////////////////////////////////////////////////
///\file TestAVLRedBlackTree.cpp
///\brief This file contains the implementation of functions used to help test the AVLRedBlackTree class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include "TestAVLRedBlackTree.h"

void Shuffle (int *Array, int n)
{  
	for (int i = 0; i < n; i++)
	{
		int j = i + rand () % (n - i);
		int t = Array[j];
		Array[j] = Array[i];
		Array[i] = t;
	}
}

int RecurseTree (AVLRedBlackNode<int> *Node, int *Count, int GreaterEqual, int LessEqual, bool* Done)
{
	if (Node) 
	{

		const int Data = *(Node->m_Data);
		int NodeLeft = 1;
		int NodeRight = 1;

		(*Count)++;

		if (!(Data >= GreaterEqual) || !(Data <= LessEqual))
		{
			printf (" Node %d is out of order in the tree.\n", Data);
			*Done = true;
		}

		if (Node->m_Left)
			NodeLeft = RecurseTree(Node->m_Left, Count, GreaterEqual, Data - 1, Done);
		if (Node->m_Right)
			NodeRight = RecurseTree(Node->m_Right, Count, Data + 1, LessEqual, Done);
		  
		if (Node->m_Color != RED && Node->m_Color != BLACK)
		{
			printf (" Node %d is neither red nor black (%d).\n", Data, Node->m_Color);
			*Done = true;
		}

		if ((Node->m_Color == RED) && (Node->m_Left) && (Node->m_Left->m_Color == RED))
		{
			printf (" Red node %d has red left child %d\n", Data, *(Node->m_Left->m_Data));
			*Done = true;
		}
  
		if ((Node->m_Color == RED) && (Node->m_Right) && (Node->m_Right->m_Color == RED))
		{
			printf (" Red node %d has red right child %d\n", Data, *(Node->m_Right->m_Data));
			*Done = true;
		}
  
		if (NodeLeft != NodeRight)
		{
			printf (" Node %d has two different black-heights: left bh=%d, " "right bh=%d\n", Data, NodeLeft, NodeRight);
			*Done = true;
		}

		return (Node->m_Color == BLACK) + NodeLeft;
	}

	return 1;
}