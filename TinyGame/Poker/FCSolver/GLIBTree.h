#ifndef MMANN_GLIBTREE_H
#define MMANN_GLIBTREE_H

////////////////////////////////////////////////
///\file GLIBTree.h
///\brief This file contains the GLIBTree class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <iostream>
#include "RedBlackTree.h"
#include "GLIBNode.h"

///\brief A templated balanced tree class
///
///This class is based off of the GLIB implementation of a 
///balanced tree written in C
template <class Data, class ConfigData>
class GLIBTree : public AGenericTree<Data, ConfigData>
{
public:
	///Constructor
	GLIBTree(ACompareNodesAlgorithm<Data, ConfigData>* Compare, DeleteTreeDataEnum DeleteTreeData = NO_DELETE_TREE_ITEM,
					ConfigData* pConfigData = NULL);

	///Destructor
	virtual ~GLIBTree();

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

	///\brief Delete a node from the tree
	///
	///\param NodeData is the data of the node to be deleted
	///\return Data of the deleted node.  NULL if node isn't found.
	Data* Delete(Data* NodeData);

	///\brief Insert a node into the tree
	///
	///\param NodeData is the value to be inserted
	void Insert(Data* NodeData);

	///\brief Get the height of the tree
	///
	///\return Returns the height of the tree
	int GetHeight();

	///\brief Get the number of nodes in the tree
	///
	///\return Returns the number of nodes in the tree
	int GetNumberOfNodes();

private:
	///Root node of the tree
	GLIBTreeNode<Data, ConfigData> *m_Root;
};

template <class Data, class ConfigData>
GLIBTree<Data, ConfigData>::GLIBTree(ACompareNodesAlgorithm<Data, ConfigData>* Compare, DeleteTreeDataEnum DeleteTreeData,
					ConfigData* pConfigData) : AGenericTree<Data, ConfigData>(Compare, DeleteTreeData, pConfigData)
{
	m_Root = NULL;
}

template <class Data, class ConfigData>
GLIBTree<Data, ConfigData>::~GLIBTree()
{
	if (m_Root != NULL)
		GLIBTreeNode<Data, ConfigData>::DeleteAllGLIBTreeNodes(m_Root, m_DeleteTreeData);
}

//Insert
template <class Data, class ConfigData>
void GLIBTree<Data, ConfigData>::Insert(Data* NodeData)
{
	bool Inserted = false;

	m_Root = GLIBTreeNode<Data, ConfigData>::Insert(m_Root, m_CompareNodes, NodeData, &Inserted);
}

//Remove
template <class Data, class ConfigData>
Data* GLIBTree<Data, ConfigData>::Delete(Data* NodeData)
{
	m_Root = GLIBTreeNode<Data, ConfigData>::Remove(m_Root, m_CompareNodes, NodeData);
	return NodeData;
}

//Find
template <class Data, class ConfigData>
Data* GLIBTree<Data, ConfigData>::Find(Data* NodeData)
{
	return GLIBTreeNode<Data, ConfigData>::Lookup(m_Root, m_CompareNodes, NodeData);
}

template <class Data, class ConfigData>
Data* GLIBTree<Data, ConfigData>::Search(Data* NodeData, bool* NodeInserted)
{
	*NodeInserted = false;
	if (m_Root != NULL)
	{
		Data *Temp = GLIBTreeNode<Data, ConfigData>::Search(m_Root, m_CompareNodes, NodeData);
		if (Temp == NULL)
		{
			*NodeInserted = true;
			Insert(NodeData);
		}

		return Temp;
	}

	*NodeInserted = true;
	Insert(NodeData);
	return NULL;
}

template <class Data, class ConfigData>
int GLIBTree<Data, ConfigData>::GetHeight()
{
	if (m_Root != NULL)
		return GLIBTreeNode<Data, ConfigData>::GetHeight(m_Root);
	else
		return 0;
}

template <class Data, class ConfigData>
int GLIBTree<Data, ConfigData>::GetNumberOfNodes()
{
	if (m_Root != NULL)
		return GLIBTreeNode<Data, ConfigData>::GetNodeCount(m_Root);
	else
		return 0;
}

#endif