#ifndef MMANN_FCS_STATE_STORAGE
#define MMANN_FCS_STATE_STORAGE

////////////////////////////////////////////////
///\file FCSStateStorage.h
///\brief This file contains the state storage classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "CompareAlgorithms.h"
#include "HashAlgorithms.h"
#include "AGenericTree.h"
#include "FCInternalHash.h"
#include "GLIBHash.h"
#include "FCIndirectState.h"
#include "FCSIndirectStateWithLocations.h"
#include "FCSIndirectStateCompareAlgorithm.h"
#include "FCState.h"

///The sort margin size for the previous states array.
#define PREV_STATES_SORT_MARGIN		32
///The amount prev_states grow by each time it each resized.
#define PREV_STATES_GROW_BY			128
///How big to make the hash tables
#define HASH_TABLE_SIZE				2048
///How big to make the talon cache
#define TALON_CACHE_SIZE			512

///\brief Abstract, generic state storage class
class AFCSGenericStateStorage
{
public:
	///Destructor
	virtual ~AFCSGenericStateStorage();

	///\brief Insert a state into storage
	///
	///\param ExistingState is the found or recently inserted state
	///\param NewState is the state to try to insert
	///\return Return true if state was inserted, else false
	virtual bool CheckAndInsert(FCSStateWithLocations** ExistingState, FCSStateWithLocations* NewState) = 0;
};

///\brief Generic tree state storage class
class FCTreeStateStorage : public AFCSGenericStateStorage
{
public:
	///Constructor
	FCTreeStateStorage();

	///Destructor
	virtual ~FCTreeStateStorage();

	///\brief Insert a state into storage
	///
	///\param ExistingState is the found or recently inserted state
	///\param NewState is the state to try to insert
	///\return Return true if state was inserted, else false
	virtual bool CheckAndInsert(FCSStateWithLocations** ExistingState, FCSStateWithLocations* NewState);

protected:
	///Tree to store states
	AGenericTree<FCSStateWithLocations, void>* m_Tree;
};

///\brief AVL tree state storage class
class FCAVLTreeStateStorage : public FCTreeStateStorage
{
public:
	///Constructor
	FCAVLTreeStateStorage(ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare);

	///Destructor
	virtual ~FCAVLTreeStateStorage();
};

///\brief AVL Red-Black tree state storage class
class FCAVLRedBlackTreeStateStorage : public FCTreeStateStorage
{
public:
	///Constructor
	FCAVLRedBlackTreeStateStorage(ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare);

	///Destructor
	virtual ~FCAVLRedBlackTreeStateStorage();
};

///\brief Red-Black tree state storage class
class FCRedBlackTreeStateStorage : public FCTreeStateStorage
{
public:
	///Constructor
	FCRedBlackTreeStateStorage(ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare);

	///Destructor
	virtual ~FCRedBlackTreeStateStorage();
};

///\brief Glib tree state storage class
class FCGLIBTreeStateStorage : public FCTreeStateStorage
{
public:
	///Constructor
	FCGLIBTreeStateStorage(ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare);

	///Destructor
	virtual ~FCGLIBTreeStateStorage();
};

///\brief Generic hash state storage class
class FCHashStateStorage : public AFCSGenericStateStorage
{
public:
	///Constructor
	FCHashStateStorage();

	///Destructor
	virtual ~FCHashStateStorage();

	///\brief Insert a state into storage
	///
	///\param ExistingState is the found or recently inserted state
	///\param NewState is the state to try to insert
	///\return Return true if state was inserted, else false
	virtual bool CheckAndInsert(FCSStateWithLocations** ExistingState, FCSStateWithLocations* NewState);

protected:
	///Hash to store states
	AGenericHash<FCSStateWithLocations, void>* m_Hash;
};

///\brief GLIB hash state storage class
class FCGLIBHashStateStorage : public FCHashStateStorage
{
public:

	///Constructor
	FCGLIBHashStateStorage(int SizeWanted, ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare, 
						AHashAlgorithm<FCSStateWithLocations>* Hash);

	///Destructor
	virtual ~FCGLIBHashStateStorage();
};

///\brief Internal state storage class
class FCInternalHashStateStorage : public FCHashStateStorage
{
public:
	///Constructor
	FCInternalHashStateStorage(int SizeWanted, ACompareNodesAlgorithm<FCSStateWithLocations, void>* CompareAlgorithm, 
							AHashAlgorithm<FCSStateWithLocations>* Hash);
	
	///Destructor
	virtual ~FCInternalHashStateStorage();
};

///\brief Indirect stack storage class
class FCIndirectStateStorage : public AFCSGenericStateStorage
{
public:
	///Constructor
	FCIndirectStateStorage();

	///Destructor
	virtual ~FCIndirectStateStorage();

	///\brief Insert a state into storage
	///
	///\param ExistingState is the found or recently inserted state
	///\param NewState is the state to try to insert
	///\return Return true if state was inserted, else false
	virtual bool CheckAndInsert(FCSStateWithLocations** ExistingState, FCSStateWithLocations* NewState);

protected:
	///The sort-margin
	FCSIndirectStateWithLocations<FCSStateWithLocations>* m_IndirectPreviousStatesMargin[PREV_STATES_SORT_MARGIN];

	/// The number of states in the sort margin
	int m_NumberOfPreviousStatesMargin;    

	///The sorted cached states
	FCSIndirectStateWithLocations<FCSStateWithLocations>** m_IndirectPreviousStates;
	///Number of sorted cached states
	int m_NumberOfIndirectPreviousStates;
	///Upper limit of sorted cached states
	int m_MaxNumberOfIndirectPreviousStates;

	///Compares two FCSIndirectStateWithLocations objects
	FCSIndirectStatesCompareAlgorithm<FCSIndirectStateWithLocations<FCSStateWithLocations>*> m_Compare;
};

/****************************
Add talon class that is similar to above
*****************************/

#endif