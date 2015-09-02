#ifndef MMANN_HASH_ALGORITHMS_H
#define MMANN_HASH_ALGORITHMS_H

////////////////////////////////////////////////
///\file HashAlgorithms.h
///\brief This file contains the hash algorithm classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "MD5Hash.h"

///\brief Abstract, templated hash algorithm class
template <class Key>
class AHashAlgorithm
{
public:
	///\brief Hash function
	///\param key is the value to be hashed
	///\return Returns hash value
	virtual int Hash(const Key* key) = 0;
};

///\brief templated hash algorithm class
///
///This hash algorithm mods the value with the size of the hash table
template <class Key>
class SimpleHashAlgorithm : public AHashAlgorithm<Key>
{
public:
	///Constructor
	SimpleHashAlgorithm(Key Capacity);

	///\brief Hash function
	///\param key is the value to be hashed
	///\return Returns hash value
	virtual int Hash(const Key* key);

private:
	///Capacity of the hash table
	Key m_Capacity;
};

template <class Key>
SimpleHashAlgorithm<Key>::SimpleHashAlgorithm(Key Capacity)
{
	m_Capacity = Capacity;
}

template <class Key>
int SimpleHashAlgorithm<Key>::Hash(const Key* key)
{
	return key%m_Capacity;
}

///\brief templated hash algorithm class
///
///This hash algorithm uses the MD5 hash algorithm
template <class Key>
class MD5HashAlgorithm : public AHashAlgorithm<Key>
{
public:
	///\brief Hash function
	///\param key is the value to be hashed
	///\return Returns hash value
	virtual int Hash(const Key* key);

protected:
	///MD5Hash functionality used for the hash
	MD5Hash  m_MD5Hash;	
};

template <class Key>
int MD5HashAlgorithm<Key>::Hash(const Key* key)
{
	unsigned char HashValue[MD5_HASHBYTES];
	int RealHashValue;

	m_MD5Hash.Init();
	m_MD5Hash.Update((unsigned char*)key, sizeof(Key));
	m_MD5Hash.Final(HashValue);

	RealHashValue = (*(int*)HashValue);
	if (RealHashValue < 0)
	{
		// This is a bit mask that nullifies the sign bit of the
		// number so it will always be positive
		RealHashValue &= (~(1<<((sizeof(RealHashValue)<<3)-1)));
	}

	return RealHashValue;
}

///\brief templated hash algorithm class
///
///This hash algorithm was based from a function ripped from Perl source code
template <class Key>
class PerlHashAlgorithm : public AHashAlgorithm<Key>
{
public:
	///\brief Hash function
	///\param key is the value to be hashed
	///\return Returns hash value
	virtual int Hash(const Key* key);
};

template <class Key>
int PerlHashAlgorithm<Key>::Hash(const Key* key)
{
	int HashValue = 0;

	// Calculate the hash value for the stack
	// This hash function was ripped from the Perl source code. 
	const char * Ptr = (char*)key;
	const char * End = Ptr+key[0]+1;

	while (Ptr < End)
		HashValue += (HashValue << 5) + *(Ptr++);

	HashValue += (HashValue >> 5);

	return HashValue;
}

///\brief Abstract, templated algorithm class to perform functions on hash table data
template <class Key, class ConfigData>
class AProcessHashAlgorithm
{
public:
	///\brief Function to be performed on hash table data
	///\param key is the value in the table that the function applies to
	///\param config is any addition configuration data needed for the function
	///\return Returns success/fail
	virtual int ProcessHash(Key* key, ConfigData* config) = 0;
};

///\brief Templated algorithm class that performs a function on hash table data
///
///Right now, this is just a place holder to accompany the GLIB Hash class I wrote.
///It does nothing.
template <class Key, class ConfigData>
class RemoveHashAlgorithm : public AProcessHashAlgorithm<Key, ConfigData>
{
public:
	///\brief Function to be performed on hash table data
	///\param key is the value in the table that the function applies to
	///\param config is any addition configuration data needed for the function
	///\return Returns success/fail
	virtual int ProcessHash(Key* key, ConfigData* config) {return 0;}
};

#endif