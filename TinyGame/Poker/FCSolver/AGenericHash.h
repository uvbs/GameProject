#ifndef MMANN_AGENERICHASH_H
#define MMANN_AGENERICHASH_H

////////////////////////////////////////////////
///\file AGenericHash.h
///\brief This file contains the AGenericHash class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

///list of 101 primes to use for hash tables
static const int HashPrimesList[] = {
	257,
	293,
	331,
	367,
	409,
	461,
	521,
	587,
	653,
	739,
	827,
	929,
	1049,
	1181,
	1321,
	1487,
	1693,
	1879,
	2113,
	2377,
	2677,
	3011,
	3389,
	3821,
	4283,
	4813,
	5417,
	6091,
	6857,
	7717,
	8677,
	9767,
	10973,
	12343,
	13901,
	15629,
	17573,
	19777,
	22247,
	25031,
	28151,
	31663,
	35671,
	40087,
	45083,
	50723,
	57059,
	64187,
	72211,
	81239,
	91393,
	102829,
	115663,
	130121,
	146389,
	164683,
	185291,
	208433,
	234499,
	263803,
	296767,
	333857,
	375593,
	422537,
	475367,
	534799,
	601631,
	676829,
	761429,
	856627,
	963689,
	1084133,
	1219649,
	1372109,
	1543631,
	1736599,
	1953659,
	2197847,
	2472577,
	2781677,
	3129383,
	3520519,
	3960587,
	4455667,
	5012627,
	5639191,
	6344087,
	7137101,
	8029243,
	9032887,
	10161997,
	11432249,
	12861281,
	14468933,
	16277561,
	18312263,
	20601271,
	23176429,
	26073497,
	29332687,
	32999269,
	37124167,
	41764741,
	46985261,
	52858427,
	59465723,
	66898963,
	75261311,
	84668971,
	95252603,
	107159153,
	120554059,
	135623317,
	152576233,
	171648317,
	193104269,
	217242323,
	244397591,
	274947317,
	309315703,
	347980163,
	391477727,
	440412409,
	495463937,
	557396929,
	627071569,
	705455519,
	793637437,
	892842107,
	1004447359,
	1130003291,
	1271253703,
	1430160409,
	1608930451,
	1810046779,
	2036302607,
	-1
};

///Number of primes in the primes list array
#define NUM_PRIMES sizeof(HashPrimesList)-1

///\brief Find the closest prime to a particular number
///
///\param Number to find closest prime
///\return Closest prime
inline int FindClosestPrime(int Number)
{
	int i;

	for(i=0;i<NUM_PRIMES;i++)
		if (HashPrimesList[i] > Number)
			break;

	return HashPrimesList[i];
}

///How a hash data item should be deleted
enum DeleteHashDataEnum {NO_DELETE_HASH_ITEM, DELETE_HASH_ITEM, DELETE_HASH_ITEM_ARRAY};

///\brief An abstract, generic templated hash class
template <class Key, class ConfigData>
class AGenericHash
{
public:
	///Destructor
	virtual ~AGenericHash();

	///\brief Insert a value.  Don't add if already there
	///
	///\param key is the value being inserted
	///\param OptimizeForCaching is a flag for caching optimization
	///\return A pointer to the value inserted/found
	virtual Key* Insert(Key* key, bool OptimizeForCaching) = 0;

	///\brief Delete all the items in the hash table
	virtual void DeleteItems();

protected:
	///Constructor
	AGenericHash(int SizeWanted, ACompareNodesAlgorithm<Key, ConfigData>* Compare,
				 AHashAlgorithm<Key>* Hash,
				 DeleteHashDataEnum DeleteHashData = NO_DELETE_HASH_ITEM);

	///\brief Copy a AGenericHash object into this AGenericHash object
	///
	///\param Hash is the AGenericHash object to be copied
	virtual void Copy(AGenericHash<Key, ConfigData>* Hash);

	///Size of the hash table
	int m_Size;

	///Number of elements in hash table
	int m_NumberOfElements;

	///The compare algorithm to compare values in the hash table
	ACompareNodesAlgorithm<Key, ConfigData>* m_Compare;

	///Hash algorithm for the hash table
	AHashAlgorithm<Key>* m_Hash;

	///Determines how to delete the data when the hash table is deleted
	DeleteHashDataEnum m_DeleteHashData;

};

template <class Key,class ConfigData>
AGenericHash<Key, ConfigData>::AGenericHash(int SizeWanted, 
											ACompareNodesAlgorithm<Key, ConfigData>* Compare,
											AHashAlgorithm<Key>* Hash,
											DeleteHashDataEnum DeleteHashData)
{
	m_Size = SizeWanted;
	m_NumberOfElements = 0;
	m_Compare = Compare;
	m_Hash = Hash;
	m_DeleteHashData = DeleteHashData;
}

template <class Key, class ConfigData>
AGenericHash<Key, ConfigData>::~AGenericHash()
{
}

template <class Key, class ConfigData>
void AGenericHash<Key, ConfigData>::DeleteItems()
{
}


template <class Key, class ConfigData>
void AGenericHash<Key, ConfigData>::Copy(AGenericHash<Key, ConfigData>* Hash)
{
	m_Size = Hash->m_Size;
	m_NumberOfElements = Hash->m_NumberOfElements;
	m_Compare = Hash->m_Compare;
	m_Hash = Hash->m_Hash;
	m_DeleteHashData = Hash->m_DeleteHashData;
}

#endif