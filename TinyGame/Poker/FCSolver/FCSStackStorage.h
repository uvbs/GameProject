#ifndef MMANN_FCS_STACK_STORAGE
#define MMANN_FCS_STACK_STORAGE

////////////////////////////////////////////////
///\file FCSStackStorage.h
///\brief This file contains the stack storage classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "CompareAlgorithms.h"
#include "HashAlgorithms.h"
#include "AGenericTree.h"
#include "FCInternalHash.h"
#include "GLIBHash.h"

class FCSIndirectCard;

///\brief Abstract, generic stack storage class
class AFCSGenericStackStorage
{
public:
	///Destructor
	virtual ~AFCSGenericStackStorage();

	///\brief Insert a FCSIndirectCard
	///
	///\param Card to insert
	///\return Returns Card if successful, NULL otherwise
	virtual FCSIndirectCard* Insert(FCSIndirectCard* Card) = 0;
};

///\brief Generic tree stack storage class
class FCTreeStackStorage : public AFCSGenericStackStorage
{
public:
	///Constructor
	FCTreeStackStorage();

	///Destructor
	virtual ~FCTreeStackStorage();

	///\brief Insert a FCSIndirectCard
	///
	///\param Card to insert
	///\return Returns Card if successful, NULL otherwise
	virtual FCSIndirectCard* Insert(FCSIndirectCard* card);
	
protected:

	///Tree to store indirect states
	AGenericTree<FCSIndirectCard, void>* m_Tree;
};

///\brief AVL tree stack storage class
class FCAVLTreeStackStorage : public FCTreeStackStorage
{
public:
	///Constructor
	FCAVLTreeStackStorage(ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare);

	///Destructor
	virtual ~FCAVLTreeStackStorage();
};

///\brief AVL Red-Black tree stack storage class
class FCAVLRedBlackTreeStackStorage : public FCTreeStackStorage
{
public:
	///Constructor
	FCAVLRedBlackTreeStackStorage(ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare);

	///Destructor
	virtual ~FCAVLRedBlackTreeStackStorage();
};

///\brief Red-Black tree state and stack storage class
class FCRedBlackTreeStackStorage : public FCTreeStackStorage
{
public:
	///Constructor
	FCRedBlackTreeStackStorage(ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare);

	///Destructor
	virtual ~FCRedBlackTreeStackStorage();
};

///\brief Glib tree stack storage class
class FCGLIBTreeStackStorage : public FCTreeStackStorage
{
public:
	///Constructor
	FCGLIBTreeStackStorage(ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare);

	///Destructor
	virtual ~FCGLIBTreeStackStorage();
};

///\brief Generic hash stack storage class
class FCHashStackStorage : public AFCSGenericStackStorage
{
public:
	///Constructor
	FCHashStackStorage();

	///Destructor
	virtual ~FCHashStackStorage();

	///\brief Insert a FCSIndirectCard
	///
	///\param Card to insert
	///\return Returns Card if successful, NULL otherwise
	virtual FCSIndirectCard* Insert(FCSIndirectCard* card);


protected:
	///Hash to store indirect states
	AGenericHash<FCSIndirectCard, void>* m_Hash;
};

///\brief GLIB hash stack storage class
class FCGLIBHashStackStorage : public FCHashStackStorage
{
public:

	///Constructor
	FCGLIBHashStackStorage(int SizeWanted, ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare,
							AHashAlgorithm<FCSIndirectCard>* Hash);

	///Destructor
	virtual ~FCGLIBHashStackStorage();
};

///\brief Internal stack storage class
class FCInternalHashStackStorage : public FCHashStackStorage
{
public:
	///Constructor
	FCInternalHashStackStorage(int SizeWanted, ACompareNodesAlgorithm<FCSIndirectCard, void>* Compare, 
							AHashAlgorithm<FCSIndirectCard>* Hash);
	
	///Destructor
	virtual ~FCInternalHashStackStorage();
};

#endif