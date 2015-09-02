#ifndef MMANN_FCINTERNAL_HASH_H
#define MMANN_FCINTERNAL_HASH_H

////////////////////////////////////////////////
///\file FCInternalHash.h
///\brief This file contains the classes associated with and including FCInternalHash
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <assert.h>
#include "AGenericHash.h"

///\brief Templated item class for the FCInternalHash hash table
///
///This class is based off of the Shlomi Fish's (shlomif@vipe.technion.ac.il)
///implementation of hash functionality written in C
template <class Key>
class FCHashItem
{
public:

	///Constructor
	FCHashItem(Key* key, int value, FCHashItem<Key>* Next = NULL);

	///Pointer to data
	Key* m_Key;

	///HashValue of data
	int m_Value;

	///Next item in the chain
	FCHashItem<Key>* m_Next;
};

template <class Key>
FCHashItem<Key>::FCHashItem(Key* key, int value, FCHashItem<Key>* Next)
{
	m_Key = key;
	m_Value = value;
	m_Next = Next;
}

///\brief An templated hash class
///
///This class is based off of the Shlomi Fish's (shlomif@vipe.technion.ac.il)
///implementation of hash functionality written in C
template <class Key, class ConfigData>
class FCInternalHash : public AGenericHash<Key, ConfigData>
{

public:
	///Constructor
	FCInternalHash(int SizeWanted, ACompareNodesAlgorithm<Key, ConfigData>* CompareAlgorithm,
				   AHashAlgorithm<Key>* Hash,
				   DeleteHashDataEnum DeleteHashData = NO_DELETE_HASH_ITEM);

	///Destructor
	virtual ~FCInternalHash();

	///\brief Insert a value.  Don't add if already there
	///
	///\param key is the value being inserted
	///\param OptimizeForCaching is a flag for caching optimization
	///\return A pointer to the value inserted/found
	Key* Insert(Key* key, bool OptimizeForCaching);

	///\brief Delete all the items in the hash table
	virtual void DeleteItems();

protected:
	///\brief Increases the size of the hash table, allowing for smaller chains, and faster lookup.
	///
	///\param Hash is a hash table being increased
	void Rehash(FCInternalHash<Key, ConfigData>* Hash);

	///\brief Copy a FCInternalHash object into this FCInternalHash object
	///
	///\param Hash is the FCInternalHash object to be copied
	virtual void Copy(AGenericHash<Key, ConfigData>* Hash);

private:
	///List of Entries in the hash table
	FCHashItem<Key>**	m_Entries;
};

template <class Key, class ConfigData>
FCInternalHash<Key, ConfigData>::FCInternalHash(int SizeWanted, ACompareNodesAlgorithm<Key, ConfigData>* CompareAlgorithm,
													  AHashAlgorithm<Key>* Hash,
													  DeleteHashDataEnum DeleteHashData) : AGenericHash<Key, ConfigData>(SizeWanted, CompareAlgorithm, Hash, DeleteHashData)
{
	// Find a prime number that is greater than the initial wanted size
	m_Size = FindClosestPrime(SizeWanted);

	// Allocate a table of size entries
	m_Entries = new FCHashItem<Key>*[m_Size];

	// Initialize all the cells of the hash table to NULL, which indicate
	// that the cork of the linked list is right at the start
	memset(m_Entries, 0, sizeof(FCHashItem<Key>*)*m_Size);
}

template <class Key, class ConfigData>
FCInternalHash<Key, ConfigData>::~FCInternalHash()
{
}

template <class Key, class ConfigData>
void FCInternalHash<Key, ConfigData>::DeleteItems()
{
	FCHashItem<Key> *Item, *NextItem;

	for(int i=0;i<m_Size;i++)
	{
		Item = m_Entries[i];
		while (Item != NULL)
		{
			NextItem = Item->m_Next;
			switch(m_DeleteHashData)
			{
			case NO_DELETE_HASH_ITEM:
				break;
			case DELETE_HASH_ITEM:
				delete Item->m_Key;
				break;
			case DELETE_HASH_ITEM_ARRAY:
				delete [] Item->m_Key;
				break;
			default:
				//This shouldn't happen
				assert(false);
			}
			delete Item;
			Item = NextItem;
		}
	}

	delete [] m_Entries;
}


template <class Key, class ConfigData>
Key* FCInternalHash<Key, ConfigData>::Insert(Key* key, bool OptimizeForCaching)
{	
	FCHashItem<Key> *Item, *LastItem;
	int Value = m_Hash->Hash(key);

	// Get the index of the appropriate chain in the hash table
	FCHashItem<Key> **List = &m_Entries[Value % m_Size];

	// If first_item is non-existent
	if ((*List) == NULL)
	{
		// Allocate a first item with that key
		Item = (*List) = new FCHashItem<Key>(key, Value);
		goto RehashCheck;
	}

	// Initialize item to the chain's first_item
	Item = (*List);
	LastItem = NULL;

	while (Item != NULL)
	{
		// We first compare the hash values, because it is faster than
		// comparing the entire data structure.

//		printf("Item->m_Value: %d\tValue: %d\tCompareValue: %d\n", Item->m_Value, Value, m_Compare->Compare(Item->m_Key, key, NULL));
		if ( (Item->m_Value == Value) && (m_Compare->Compare(Item->m_Key, key, NULL) == 0) )
		{
			if (OptimizeForCaching)
			{
				// Place the item in the beginning of the chain.
				// If last_item == NULL it is already the first item so leave
				// it alone
				if (LastItem != NULL)
				{
					LastItem->m_Next = Item->m_Next;
					Item->m_Next = (*List);
					(*List) = Item;
				}
			}
			
			return Item->m_Key;
		}

		// Cache the item before the current in last_item
		LastItem = Item;
		// Move to the next item
		Item = Item->m_Next;
	}
		
	if (OptimizeForCaching)
	{
		// Put the new element at the beginning of the list
		Item = new FCHashItem<Key>(key, Value, (*List));
		(*List) = Item;
	}
	else
	{
		// Put the new element at the end of the list
		Item = LastItem->m_Next = new FCHashItem<Key>(key, Value);
	}

RehashCheck:
	m_NumberOfElements++;

	if (m_NumberOfElements > ((m_Size*3)>>2))
		Rehash(this);

	return NULL;
}

template <class Key, class ConfigData>
void FCInternalHash<Key, ConfigData>::Rehash(FCInternalHash<Key, ConfigData>* Hash)
{
	FCInternalHash<Key, ConfigData>* NewHash;
	FCHashItem<Key> *Item, *NextItem;
	int Place;

	// Allocate a new hash with hash_init()
	NewHash = new FCInternalHash<Key, ConfigData>(Hash->m_Size*2, Hash->m_Compare, Hash->m_Hash, m_DeleteHashData);
	NewHash->m_NumberOfElements = Hash->m_NumberOfElements;
   
	// Copy the items to the new hash and deallocate the old ones in the same time
	for(int i=0;i<Hash->m_Size;i++)
	{
		Item = Hash->m_Entries[i];
		// traverse the chain item by item
		while(Item != NULL)
		{
			// The place in the new hash table
			Place = Item->m_Value % NewHash->m_Size;

			// Store the next item in the linked list in a safe place,
			// so we can retrieve it after the assignment
			NextItem = Item->m_Next;

			// It is placed in front of the first element in the chain,
			// so it should link to it
			Item->m_Next = NewHash->m_Entries[Place];

			// Make it the first item in its chain
			NewHash->m_Entries[Place] = Item;

			// Move to the next item this one.
			Item = NextItem;
		}
	}

	// Free the entries of the old hash
	delete [] Hash->m_Entries;

	// Copy the new hash to the old one
	Hash->Copy(NewHash);

	delete NewHash;
}

template <class Key, class ConfigData>
void FCInternalHash<Key, ConfigData>::Copy(AGenericHash<Key, ConfigData>* Hash)
{
	//copy the parent data
	AGenericHash<Key, ConfigData>::Copy(Hash);

	m_Entries = ((FCInternalHash<Key, ConfigData>*)Hash)->m_Entries;
}

#endif