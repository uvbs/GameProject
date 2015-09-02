#ifndef TESTAVLREDBLACKTREE_H
#define TESTAVLREDBLACKTREE_H

////////////////////////////////////////////////
///\file TestAVLRedBlackTree.h
///\brief This file contains functions to help test the AVLRedBlackTree class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "AVLRedBlackNode.h"

///\brief Shuffle the array of data
void Shuffle (int *Array, int n);


///\brief Examine NODE in a red-black tree.
///
/// *COUNT is increased by the number of nodes in the tree, including the current one.  
/// Returns the number of black nodes (including this node) in a path from this node to any leaf.
int RecurseTree (AVLRedBlackNode<int> *Node, int *Count, int GreaterEqual, int LessEqual, bool* Done);

#endif