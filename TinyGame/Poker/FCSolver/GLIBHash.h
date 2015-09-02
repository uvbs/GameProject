#ifndef MMANN_GLIBHASH_H
#define MMANN_GLIBHASH_H

////////////////////////////////////////////////
///\file GLIBHash.h
///\brief This file contains the classes associated with and including GLIBHash
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////


#include "CompareAlgorithms.h"
#include "HashAlgorithms.h"

///Minimum size for a GLIB hash table
#define GLIB_HASH_TABLE_MIN_SIZE		11
///Maximum size for a GLIB hash table
#define GLIB_HASH_TABLE_MAX_SIZE		13845163

///\brief Templated item class for the GLIB hash table
///
///This class is based off of the GLIB implementation of a hash table written in C
template <class Key>
class GLIBHashNode
{
public:
	///Constructor
	GLIBHashNode(Key* key, int Value, DeleteHashDataEnum DeleteHashData);

	///Destructor
	virtual ~GLIBHashNode();

	///Pointer to data
	Key*  m_Key;

	///HashValue of data
	int m_Value;

	///Next item in the chain
	GLIBHashNode<Key>* m_Next;

	///Determines how to delete the data when the hash node is deleted
	DeleteHashDataEnum m_DeleteHashData;
};

template <class Key>
GLIBHashNode<Key>::GLIBHashNode(Key* key, int Value, DeleteHashDataEnum DeleteHashData)
{
	m_Key = key;
	m_Value = Value;
	m_Next = NULL;
	m_DeleteHashData = DeleteHashData;
}

template <class Key>
GLIBHashNode<Key>::~GLIBHashNode()
{
	switch(m_DeleteHashData)
	{
	case NO_DELETE_HASH_ITEM:
		break;
	case DELETE_HASH_ITEM:
		delete m_Key;
		break;
	case DELETE_HASH_ITEM_ARRAY:
		delete [] m_Key;
		break;
	default:
		//This shouldn't happen
		assert(false);
	}

	if (m_Next != NULL)
		delete m_Next;
}

///\brief An templated hash class
///
///This class is based off of the GLIB implementation of a hash table written in C
template <class Key, class ConfigData>
class GLIBHash : public AGenericHash<Key, ConfigData>
{
public:
	///Constructor
	GLIBHash(int SizeWanted, ACompareNodesAlgorithm<Key, ConfigData>* Compare, 
				AHashAlgorithm<Key>* Hash, DeleteHashDataEnum DeleteHashData = NO_DELETE_HASH_ITEM);

	///Destructor
	virtual ~GLIBHash();

	///\brief Insert a value.  Don't add if already there
	///
	///\param key is the value being inserted
	///\param OptimizeForCaching is a flag for caching optimization
	///\return A pointer to the value inserted/found
	virtual Key* Insert(Key* key, bool OptimizeForCaching);

	///\brief Delete a node from the table
	///
	///\param key is the hash value of the data.
	void Remove(Key* key);

	///\brief Find a data item and return its hash value
	///
	///\param key is the data being searched for.
	///\return Hash value of the data.
	int Lookup(Key* key);

	///\brief Find a data item and get all the information about it
	///
	///\param LookupKey is the key being searched for
	///\param OriginalKey is the key found and returned
	///\param Value is the hash value returned
	///\return Whether or not the key was found
	bool LookupExtended(Key* LookupKey, Key** OriginalKey, int** Value);

	///\brief Prevent the hash table from resizing
	void Freeze();

	///\brief Allow the hash table to resize
	void Thaw();

	///\brief Resize the hash table
	void Resize();

	///\brief Perform an action on all the entries in the hash table
	///
	///\param ProcessFunc is the action to be performed
	///\param UserData is any extra information needed to perform the action
	void ForEach(AProcessHashAlgorithm<Key, ConfigData>* ProcessFunc, ConfigData* UserData);

	///\brief Perform an action on all the entries in the hash table before removing them
	///
	///\param ProcessFunc is the action to be performed
	///\param UserData is any extra information needed to perform the action
	unsigned int ForEachRemove(RemoveHashAlgorithm<Key, ConfigData>* RemoveFunc, ConfigData* configData);
	
	///\brief Number of Entries in the hash table
	int GetSize();

protected:
	///\brief Insert a value.  Don't add if already there
	///
	///\param key is the data being inserted
	///\param Value is the hash value of the data
	void Insert(Key* key, int Value);

	///\brief Find a data item and return the hash entry
	///
	///\param key is the data being searched for.
	///\return Hash entry.
	GLIBHashNode<Key>** LookupNode(Key* key);

private:
	///Determines whether or not to grow the hash table 
	unsigned int  m_Frozen;

	///Nodes in the hash table
	GLIBHashNode<Key> **m_Nodes;
};

template <class Key, class ConfigData>
GLIBHash<Key, ConfigData>::GLIBHash(int SizeWanted, ACompareNodesAlgorithm<Key, ConfigData>* Compare, 
									AHashAlgorithm<Key>* Hash, DeleteHashDataEnum DeleteHashData) : AGenericHash<Key, ConfigData>(SizeWanted, Compare, Hash, DeleteHashData)
{
	m_Frozen = 0;
	m_Nodes = new GLIBHashNode<Key>*[m_Size];
	for (int i = 0; i < m_Size; i++)
		m_Nodes[i] = NULL;
}

template <class Key, class ConfigData>
GLIBHash<Key, ConfigData>::~GLIBHash()
{
	for (int i = 0;i<m_Size;i++)
		delete m_Nodes[i];

	delete [] m_Nodes;
}

template <class Key, class ConfigData>
Key* GLIBHash<Key, ConfigData>::Insert(Key* key, bool OptimizeForCaching)
{
	GLIBHashNode<Key> **Node = LookupNode(key);

	if (*Node != NULL)
		return (*Node)->m_Key;

	int HashValue = m_Hash->Hash(key);
	Insert(key, HashValue);
	return NULL;
}

template <class Key,class ConfigData>
void GLIBHash<Key, ConfigData>::Insert(Key* key, int Value)
{
	GLIBHashNode<Key> **Node = LookupNode(key);
  
	if (*Node != NULL)
		(*Node)->m_Value = Value;

	*Node = new GLIBHashNode<Key>(key, Value, m_DeleteHashData);
	m_NumberOfElements++;
	if (!m_Frozen)
		Resize();
}

template <class Key, class ConfigData>
void GLIBHash<Key, ConfigData>::Remove(Key* key)
{
	GLIBHashNode<Key> **Node = LookupNode(key),
							*DestroyNode;

	if (*Node)
	{
		DestroyNode = *Node;
		(*Node) = DestroyNode->m_Next;
		delete DestroyNode;
		m_NumberOfElements--;
  
		if (!m_Frozen)
			Resize();
	}
}

template <class Key, class ConfigData>
int GLIBHash<Key, ConfigData>::Lookup(Key* key)
{
	GLIBHashNode<Key> *Node = LookupNode(key);
  
	return Node ? Node->m_Value : 0;
}

template <class Key, class ConfigData>
bool GLIBHash<Key, ConfigData>::LookupExtended(Key* LookupKey, Key** OriginalKey, int** Value)
{
	GLIBHashNode<Key> *Node = LookupNode(LookupKey);
  
	if (Node)
	{
		if (OriginalKey)
			*OriginalKey = Node->m_Key;
		if (Value)
			*Value = Node->m_Value;
		return true;
	}
	else
		return false;
}

template <class Key, class ConfigData>
void GLIBHash<Key, ConfigData>::Freeze()
{
	m_Frozen++;
}

template <class Key, class ConfigData>
void GLIBHash<Key, ConfigData>::Thaw()
{
	if (m_Frozen)
		if (!(--m_Frozen))
			Resize();
}

template <class Key, class ConfigData>
void GLIBHash<Key, ConfigData>::Resize()
{
	GLIBHashNode<Key> **NewNodes;
	GLIBHashNode<Key> *Node, *Next;
	float NodesPerList = (float)m_NumberOfElements/(float)m_Size;
	unsigned int HashValue;
	int NewSize = FindClosestPrime(m_NumberOfElements);
  
	if ((NodesPerList > 0.3 || m_Size <= GLIB_HASH_TABLE_MIN_SIZE) &&
		(NodesPerList < 3.0 || m_Size >= GLIB_HASH_TABLE_MAX_SIZE))
		return;

	if (NewSize == m_Size)
		return;
  
	NewNodes = new GLIBHashNode<Key>*[NewSize];

	for (int a = 0;a<NewSize;a++)
		NewNodes[a] = NULL;

	for (int i = 0; i < m_Size; i++)
		for (Node = m_Nodes[i]; Node; Node = Next)
		{
			Next = Node->m_Next;
			HashValue = m_Hash->Hash(Node->m_Key) % NewSize;
			Node->m_Next = NewNodes[HashValue];
			NewNodes[HashValue] = Node;
		}
  
	delete [] m_Nodes;
	m_Nodes = NewNodes;
	m_Size = NewSize;
}

template <class Key, class ConfigData>
void GLIBHash<Key, ConfigData>::ForEach(AProcessHashAlgorithm<Key, ConfigData>* ProcessFunc, ConfigData* UserData)
{
	GLIBHashNode<Key> *Node;
  
	for (int i = 0; i < m_Size; i++)
		for (Node = m_Nodes[i]; Node; Node = Node->m_Next)
			ProcessFunc->ProcessHash(Node->m_Key, Node->m_Value, UserData);
}

template <class Key, class ConfigData>
unsigned int GLIBHash<Key, ConfigData>::ForEachRemove(RemoveHashAlgorithm<Key, ConfigData>* RemoveFunc, ConfigData* configData)
{
	GLIBHashNode<Key> *Node, *Previous;
	unsigned int Deleted = 0;

	for (int i = 0; i < m_Size; i++)
	{
	Restart:
		Previous = NULL;
  
		for (Node = m_Nodes[i]; Node; Previous = Node, Node = Node->m_Next)
		{
			if (RemoveFunc->ProcessHash(Node->m_Key, Node->m_Value, configData))
			{
				Deleted += 1;
				m_NumberOfElements -= 1;

				if (Previous)
				{
					Previous->m_Next = Node->m_Next;
					delete Node;
					Node = Previous;
				}
				else
				{
					m_Nodes[i] = Node->m_Next;
					delete Node;
					goto Restart;
				}
			}
		}
	}

	if (!m_Frozen)
		Resize();

	return Deleted;
}

template <class Key, class ConfigData>
int GLIBHash<Key, ConfigData>::GetSize()
{
	return m_NumberOfElements;
}

template <class Key, class ConfigData>
GLIBHashNode<Key>** GLIBHash<Key, ConfigData>::LookupNode(Key* key)
{
	GLIBHashNode<Key> **Node;
  
	Node = &m_Nodes[m_Hash->Hash(key) % m_Size];
  
	/* Hash table lookup needs to be fast.
	 *  We therefore remove the extra conditional of testing
	 *  whether to call the key_compare_func or not from
	 *  the inner loop.
	 */
	if (m_Compare)
		while (*Node && (m_Compare->Compare((*Node)->m_Key, key, NULL) != 0) )
			Node = &(*Node)->m_Next;
	else
		while (*Node && (*Node)->m_Key != key)
			Node = &(*Node)->m_Next;

	return Node;
}

#endif