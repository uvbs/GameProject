#ifndef MMANN_GLIBNODE_H
#define MMANN_GLIBNODE_H

////////////////////////////////////////////////
///\file GLIBNode.h
///\brief This file contains the GLIBNode class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

///\brief Templated node class for a templated GLIB balanced tree
///
///This class is based off of the GLIB implementation of a 
///balanced tree written in C
template <class Data, class ConfigData>
class GLIBTreeNode
{
public:

	///Constructor
	GLIBTreeNode(Data* data);

	///Destructor
	virtual ~GLIBTreeNode();

	///\brief Delete Node and all its children
	///
	///\param Node is the node to be deleted (with its children)
	static void DeleteAllGLIBTreeNodes(GLIBTreeNode<Data, ConfigData>* Node, DeleteTreeDataEnum DeleteTreeData);

	///\brief Recursively search for the correct place for a node and insert it
	///
	///\param Node is starting node to be searched
	///\param Compare is the algorithm used to compare nodes
	///\param NodeData is the data of the node to be inserted
	///\param Inserted is a flag to determine if the data was inserted
	///\return A pointer to the node inserted or found
	static GLIBTreeNode<Data, ConfigData>* Insert(GLIBTreeNode<Data, ConfigData>* Node, ACompareNodesAlgorithm<Data, ConfigData>* Compare,
									 Data* NodeData, bool* Inserted);

	///\brief Delete a node from the tree
	///
	///\param Node is the node to start searching from
	///\param Compare is the algorithm used to compare nodes
	///\param NodeData is the value of the node to be deleted (Data is not deleted!).
	///\return New root of the tree
	static GLIBTreeNode<Data, ConfigData>* Remove(GLIBTreeNode<Data, ConfigData>* Node, ACompareNodesAlgorithm<Data, ConfigData>* Compare,
									 Data* NodeData);

	///\brief Recursive search for data.  If not found, return NULL
	///
	///\param Node is the node to start searching from
	///\param Compare is the algorithm used to compare nodes
	///\param NodeData is the value being searched for.
	///\return A pointer to the value found.  NULL if data isn't found.
	static Data* Lookup(GLIBTreeNode<Data, ConfigData>* Node, ACompareNodesAlgorithm<Data, ConfigData>* Compare,
									 Data* NodeData);

	///\brief Loop search for data.  If not found, return NULL
	///
	///\param Node is the node to start searching from
	///\param Compare is the algorithm used to compare nodes
	///\param NodeData is the value being searched for.
	///\return A pointer to the data found.  NULL if data isn't found.
	static Data* Search(GLIBTreeNode<Data, ConfigData>* Node, ACompareNodesAlgorithm<Data, ConfigData>* Compare, Data* NodeData);

	///\brief Get height (depth) of the tree
	///
	///\param Node is the node to start counting depth
	///\return Height of the tree.
	static int GetHeight(GLIBTreeNode<Data, ConfigData>* Node);

	///\brief Get the number of nodes in a tree
	///
	///\param Node is the node to start counting from
	///\return Number of nodes in tree.
	static int GetNodeCount(GLIBTreeNode<Data, ConfigData>* Node);

protected:

	///\brief Balance the node tree
	///
	///\param Node is the node that will be the base of the balance
	///\return Returns the newly balanced node
	static GLIBTreeNode<Data, ConfigData>* Balance(GLIBTreeNode<Data, ConfigData>* Node);

	///\brief Remove the leftmost node
	///
	///\param Node is the node to start from
	///\param Leftmost is the node that is removed
	///\return Pointer to a newly balanced tree
	static GLIBTreeNode<Data, ConfigData>* RemoveLeftmost(GLIBTreeNode<Data, ConfigData>* Node, GLIBTreeNode<Data, ConfigData>** Leftmost);

	///\brief Balance the left side of the tree
	///
	///\param Node is the node that will be the base of the balance
	///\param OldBalance is the balance of the starting node
	///\return Pointer to a newly balanced tree
	static GLIBTreeNode<Data, ConfigData>* RestoreLeftBalance(GLIBTreeNode<Data, ConfigData>* Node, int OldBalance);

	///\brief Balance the right side of the tree
	///
	///\param Node is the node that will be the base of the balance
	///\param OldBalance is the balance of the starting node
	///\return Pointer to a newly balanced tree
	static GLIBTreeNode<Data, ConfigData>* RestoreRightBalance(GLIBTreeNode<Data, ConfigData>* Node, int OldBalance);

	///\brief Rotate node one node to the left
	///
	///\param Node is the node to be moved
	///\return Node that replaced the moved node
	static GLIBTreeNode<Data, ConfigData>* RotateLeft(GLIBTreeNode<Data, ConfigData>* Node);

	///\brief Rotate node one node to the right
	///
	///\param Node is the node to be moved
	///\return Node that replaced the moved node
	static GLIBTreeNode<Data, ConfigData>* RotateRight(GLIBTreeNode<Data, ConfigData>* Node);

	///Pointer to the left node
	GLIBTreeNode<Data, ConfigData>* m_Left;

	///Pointer to the right node
	GLIBTreeNode<Data, ConfigData>* m_Right;

	///Pointer to the data in the node
	Data* m_Data;

	///Balance of the node - used to rebranch tree
	signed char m_Balance;

};

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>::GLIBTreeNode(Data* data)
{
	m_Left = m_Right = NULL;
	m_Balance = 0;
	m_Data = data;
}
	
template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>::~GLIBTreeNode()
{
}

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>* GLIBTreeNode<Data, ConfigData>::Insert(GLIBTreeNode<Data, ConfigData>* Node, 
													ACompareNodesAlgorithm<Data, ConfigData>* Compare,
													Data* NodeData, bool* Inserted)
{
	int OldBalance;
	int CompareValue;

	if (Node == NULL)
	{
		*Inserted = true;
		return new GLIBTreeNode<Data, ConfigData>(NodeData);
	}

	CompareValue = (*Compare).Compare(Node->m_Data, NodeData, NULL);
	if (CompareValue == 0)
	{
		*Inserted = false;
		Node->m_Data = NodeData;
		return Node;
	}

	if (CompareValue < 0)
	{
		if (Node->m_Left != NULL)
		{
			OldBalance = Node->m_Left->m_Balance;
			Node->m_Left = Insert(Node->m_Left, Compare, NodeData, Inserted);
			if ((OldBalance != Node->m_Left->m_Balance) && Node->m_Left->m_Balance)
				Node->m_Balance -= 1;
		}
		else
		{
			*Inserted = true;
			Node->m_Left = new GLIBTreeNode<Data, ConfigData>(NodeData);
			Node->m_Balance -= 1;
		}
	}
	else if (CompareValue > 0)
	{
		if (Node->m_Right != NULL)
		{
			OldBalance = Node->m_Right->m_Balance;
			Node->m_Right = Insert(Node->m_Right, Compare, NodeData, Inserted);
			if ((OldBalance != Node->m_Right->m_Balance) && Node->m_Right->m_Balance)
				Node->m_Balance += 1;
		}
		else
		{
			*Inserted = true;
			Node->m_Right = new GLIBTreeNode<Data, ConfigData>(NodeData);
			Node->m_Balance += 1;
		}
	}

	if ((*Inserted) && ((Node->m_Balance < -1) || (Node->m_Balance > 1)))
	{
		Node = Balance(Node);
	}

	return Node;
}

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>* GLIBTreeNode<Data, ConfigData>::Remove(GLIBTreeNode<Data, ConfigData>* Node, 
											 ACompareNodesAlgorithm<Data, ConfigData>* Compare,
											 Data* NodeData)
{
	if (Node == NULL)
		return NULL;

	GLIBTreeNode<Data, ConfigData>* NewRoot;
	int OldBalance;
	int CompareValue;

	CompareValue = (*Compare).Compare(Node->m_Data, NodeData, NULL);
	if (CompareValue == 0)
	{
		if (Node->m_Right == NULL)
		{
			NewRoot = Node->m_Left;
		}
		else
		{
			OldBalance = Node->m_Right->m_Balance;
			Node->m_Right = RemoveLeftmost(Node->m_Right, &NewRoot);
			NewRoot->m_Left = Node->m_Left;
			NewRoot->m_Right = Node->m_Right;
			NewRoot->m_Balance = Node->m_Balance;
			NewRoot = RestoreRightBalance(NewRoot, OldBalance);
		}

		delete Node;
		return NewRoot;
	}
	else if (CompareValue < 0)
	{
		if (Node->m_Left != NULL)
		{
			OldBalance = Node->m_Left->m_Balance;
			Node->m_Left = Remove(Node->m_Left, Compare, NodeData);
			Node = RestoreLeftBalance(Node, OldBalance);
		}
	}
	else if (CompareValue > 0)
	{
		if (Node->m_Right != NULL)
		{
			OldBalance = Node->m_Right->m_Balance;
			Node->m_Right = Remove(Node->m_Right, Compare, NodeData);
			Node = RestoreRightBalance(Node, OldBalance);
		}
	}

	return Node;
}

template <class Data, class ConfigData>
Data* GLIBTreeNode<Data, ConfigData>::Lookup(GLIBTreeNode<Data, ConfigData>* Node, 
													ACompareNodesAlgorithm<Data, ConfigData>* Compare,
													Data* NodeData)
{
	if (Node == NULL)
		return NULL;

	int CompareValue = (*Compare).Compare(Node->m_Data, NodeData, NULL);
	if (CompareValue == 0)
		return Node->m_Data;

	if (CompareValue < 0)
	{
		if (Node->m_Left != NULL)
			return Lookup((GLIBTreeNode<Data, ConfigData>*)Node->m_Left, Compare, NodeData);
	}
	else if (CompareValue > 0)
	{
		if (Node->m_Right != NULL)
			return Lookup((GLIBTreeNode<Data, ConfigData>*)Node->m_Right, Compare, NodeData);
	}

	return NULL;
}

template <class Data, class ConfigData>
Data* GLIBTreeNode<Data, ConfigData>::Search(GLIBTreeNode<Data, ConfigData>* Node, ACompareNodesAlgorithm<Data, ConfigData>* Compare, Data* NodeData)
{
	if (Node == NULL)
		return NULL;

	int Direction;
	
	do
	{
		Direction = (*Compare).Compare(Node->m_Data, NodeData, NULL);
		if (Direction == 0)
			return Node->m_Data;

		if (Direction < 0)
			Node = Node->m_Left;
		else if (Direction > 0)
			Node = Node->m_Right;
	}
	while ((Node != NULL) && (Direction != 0));

	return NULL;
}

template <class Data, class ConfigData>
int GLIBTreeNode<Data, ConfigData>::GetHeight(GLIBTreeNode<Data, ConfigData>* Node)
{
	if (Node != NULL)
	{
		int LeftHeight = 0, 
			RightHeight = 0;
		
		if (Node->m_Left != NULL)
			LeftHeight = GetHeight(Node->m_Left);

		if (Node->m_Right != NULL)
			RightHeight = GetHeight(Node->m_Right);

		return ((LeftHeight > RightHeight) ? LeftHeight : RightHeight) + 1;
	}

	return 0;
}

template <class Data, class ConfigData>
int GLIBTreeNode<Data, ConfigData>::GetNodeCount(GLIBTreeNode<Data, ConfigData>* Node)
{
	int Count = 1;

	if (Node->m_Left)
		Count += GetNodeCount(Node->m_Left);
	if (Node->m_Right)
		Count += GetNodeCount(Node->m_Right);

	return Count;
}

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>* GLIBTreeNode<Data, ConfigData>::Balance(GLIBTreeNode<Data, ConfigData>* Node)
{
	if (Node->m_Balance < -1)
	{
		if (Node->m_Left->m_Balance > 0)
			Node->m_Left = RotateLeft(Node->m_Left);
		Node = RotateRight(Node);
	}
	else if (Node->m_Balance > 1)
	{
		if (Node->m_Right->m_Balance < 0)
			Node->m_Right = RotateRight(Node->m_Right);
		Node = RotateLeft(Node);
	}

	return Node;
}

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>* GLIBTreeNode<Data, ConfigData>::RemoveLeftmost(GLIBTreeNode<Data, ConfigData>* Node, GLIBTreeNode<Data, ConfigData>** Leftmost)
{
	if (Node->m_Left == NULL)
	{
		*Leftmost = Node;
		return Node->m_Right;
	}

	int OldBalance = Node->m_Left->m_Balance;
	Node->m_Left = RemoveLeftmost(Node->m_Left, Leftmost);
	return RestoreLeftBalance(Node, OldBalance);
}

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>* GLIBTreeNode<Data, ConfigData>::RestoreLeftBalance(GLIBTreeNode<Data, ConfigData>* Node, int OldBalance)
{
	if (Node->m_Left == NULL)
		Node->m_Balance += 1;
	else if ((Node->m_Left->m_Balance != OldBalance) && (Node->m_Left->m_Balance == 0))
		Node->m_Balance += 1;

	if (Node->m_Balance > 1)
		return Balance(Node);

	return Node;
}

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>* GLIBTreeNode<Data, ConfigData>::RestoreRightBalance(GLIBTreeNode<Data, ConfigData>* Node, int OldBalance)
{
	if (Node->m_Right == NULL)
		Node->m_Balance -= 1;
	else if ((Node->m_Right->m_Balance != OldBalance) && (Node->m_Right->m_Balance == 0))
		Node->m_Balance -= 1;

	if (Node->m_Balance < -1)
		return Balance(Node);

	return Node;
}

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>* GLIBTreeNode<Data, ConfigData>::RotateLeft(GLIBTreeNode<Data, ConfigData>* Node)
{
	GLIBTreeNode<Data, ConfigData> *Left = (GLIBTreeNode<Data, ConfigData>*)Node->m_Left,
								   *Right = (GLIBTreeNode<Data, ConfigData>*)Node->m_Right;

	Node->m_Right = Right->m_Left;
	Right->m_Left = Node;

	int aBalance = Node->m_Balance,
		bBalance = Right->m_Balance;

	if (bBalance <= 0)
	{
		if (aBalance >= 1)
			Right->m_Balance = bBalance - 1;
		else
			Right->m_Balance = aBalance + bBalance - 2;
		Node->m_Balance = aBalance - 1;
	}
	else
	{
		if (aBalance <= bBalance)
			Right->m_Balance = aBalance - 2;
		else
			Right->m_Balance = bBalance - 1;
		Node->m_Balance = aBalance - bBalance - 1;
	}

	return Right;
}

template <class Data, class ConfigData>
GLIBTreeNode<Data, ConfigData>* GLIBTreeNode<Data, ConfigData>::RotateRight(GLIBTreeNode<Data, ConfigData>* Node)
{
	GLIBTreeNode<Data, ConfigData> *Left = (GLIBTreeNode<Data, ConfigData>*)Node->m_Left;
	Node->m_Left = Left->m_Right;
	Left->m_Right = Node;

	int aBalance = Node->m_Balance,
		bBalance = Left->m_Balance;

	if (bBalance <= 0)
	{
		if (bBalance > aBalance)
			Left->m_Balance = bBalance + 1;
		else
			Left->m_Balance = aBalance + 2;
		Node->m_Balance = aBalance - bBalance + 1;
	}
	else
	{
		if (aBalance <= -1)
			Left->m_Balance = bBalance + 1;
		else
			Left->m_Balance = aBalance + bBalance + 2;
		Node->m_Balance = aBalance + 1;
	}

	return Left;
}

template <class Data, class ConfigData>
void GLIBTreeNode<Data, ConfigData>::DeleteAllGLIBTreeNodes(GLIBTreeNode<Data, ConfigData>* Node, DeleteTreeDataEnum DeleteTreeData)
{
	if (Node->m_Left != NULL)
		DeleteAllGLIBTreeNodes(Node->m_Left, DeleteTreeData);
	if (Node->m_Right != NULL)
		DeleteAllGLIBTreeNodes(Node->m_Right, DeleteTreeData);

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