////////////////////////////////////////////////
///\file FCSStackStorage.cpp
///\brief This file contains the implementation of the stack storage classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSStackStorage.h"
#include "FCIndirectState.h"
#include "AVLTree.h"
#include "GLIBTree.h"
#include "RedBlackTree.h"
#include "AVLRedBlackTree.h"
#include "SearchAlgorithms.h"
#include "FCHelpingAlgorithms.h"
#include "HashAlgorithms.h"

AFCSGenericStackStorage::~AFCSGenericStackStorage()
{
}

FCTreeStackStorage::FCTreeStackStorage()
{
	m_Tree = NULL;
}

FCTreeStackStorage::~FCTreeStackStorage()
{
	if (m_Tree != NULL)
	{
		m_Tree->DeleteTree();
		delete m_Tree;
	}
}

FCSIndirectCard* FCTreeStackStorage::Insert(FCSIndirectCard* Card)
{
	bool DummyIsInsert;
	return m_Tree->Search(Card, &DummyIsInsert);
}

FCAVLTreeStackStorage::FCAVLTreeStackStorage(ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare)
{
	m_Tree = new AVLTree<FCSIndirectCard, void>(Compare, DELETE_TREE_ITEM_ARRAY);
}

FCAVLTreeStackStorage::~FCAVLTreeStackStorage()
{
}

FCAVLRedBlackTreeStackStorage::FCAVLRedBlackTreeStackStorage(ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare)
{
	m_Tree = new AVLRedBlackTree<FCSIndirectCard, void>(Compare, DELETE_TREE_ITEM_ARRAY);
}

FCAVLRedBlackTreeStackStorage::~FCAVLRedBlackTreeStackStorage()
{
}

FCRedBlackTreeStackStorage::FCRedBlackTreeStackStorage(ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare)
{
	m_Tree = new RedBlackTree<FCSIndirectCard, void>(Compare, DELETE_TREE_ITEM_ARRAY);
}

FCRedBlackTreeStackStorage::~FCRedBlackTreeStackStorage()
{
}

FCGLIBTreeStackStorage::FCGLIBTreeStackStorage(ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare)
{
	m_Tree = new GLIBTree<FCSIndirectCard, void>(Compare, DELETE_TREE_ITEM_ARRAY);
}

FCGLIBTreeStackStorage::~FCGLIBTreeStackStorage()
{
}

FCHashStackStorage::FCHashStackStorage()
{
	m_Hash = NULL;
}

FCHashStackStorage::~FCHashStackStorage()
{
	if (m_Hash != NULL)
	{
		m_Hash->DeleteItems();
		delete m_Hash;
	}
}

FCSIndirectCard* FCHashStackStorage::Insert(FCSIndirectCard* Card)
{
	return m_Hash->Insert(Card, true);
}

FCGLIBHashStackStorage::FCGLIBHashStackStorage(int SizeWanted, ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare, 
									 AHashAlgorithm<FCSIndirectCard>* Hash)
{
	m_Hash = new GLIBHash<FCSIndirectCard, void>(SizeWanted, Compare, Hash, DELETE_HASH_ITEM_ARRAY);
}

FCGLIBHashStackStorage::~FCGLIBHashStackStorage()
{
}

FCInternalHashStackStorage::FCInternalHashStackStorage(int SizeWanted, ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare, 
											 AHashAlgorithm<FCSIndirectCard>* Hash)
{
	m_Hash = new FCInternalHash<FCSIndirectCard, void>(SizeWanted, Compare, Hash, DELETE_HASH_ITEM_ARRAY);
}

FCInternalHashStackStorage::~FCInternalHashStackStorage()
{
}

