////////////////////////////////////////////////
///\file FCSStateStorage.cpp
///\brief This file contains the implementation of the state storage classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include "FCSStateStorage.h"
#include "AVLTree.h"
#include "GLIBTree.h"
#include "RedBlackTree.h"
#include "AVLRedBlackTree.h"
#include "SearchAlgorithms.h"
#include "FCHelpingAlgorithms.h"
#include "HashAlgorithms.h"
#include "FCState.h"
#include "FCSIndirectStateWithLocations.h"

AFCSGenericStateStorage::~AFCSGenericStateStorage()
{
}

FCTreeStateStorage::FCTreeStateStorage()
{
	m_Tree = NULL;
}

FCTreeStateStorage::~FCTreeStateStorage()
{
	if (m_Tree != NULL)
	{
		m_Tree->DeleteTree();
		delete m_Tree;
	}
}

bool FCTreeStateStorage::CheckAndInsert(FCSStateWithLocations** ExistingState, FCSStateWithLocations* NewState)
{
	bool IsInsert;
	(*ExistingState) = m_Tree->Search(NewState, &IsInsert);
	return IsInsert;
}

FCAVLTreeStateStorage::FCAVLTreeStateStorage(ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare)
{
	m_Tree = new AVLTree<FCSStateWithLocations, void>(Compare);
}

FCAVLTreeStateStorage::~FCAVLTreeStateStorage()
{
}

FCAVLRedBlackTreeStateStorage::FCAVLRedBlackTreeStateStorage(ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare)
{
	m_Tree = new AVLRedBlackTree<FCSStateWithLocations, void>(Compare);
}

FCAVLRedBlackTreeStateStorage::~FCAVLRedBlackTreeStateStorage()
{
}

FCRedBlackTreeStateStorage::FCRedBlackTreeStateStorage(ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare)
{
	m_Tree = new RedBlackTree<FCSStateWithLocations, void>(Compare);
}

FCRedBlackTreeStateStorage::~FCRedBlackTreeStateStorage()
{
}

FCGLIBTreeStateStorage::FCGLIBTreeStateStorage(ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare)
{
	m_Tree = new GLIBTree<FCSStateWithLocations, void>(Compare);
}

FCGLIBTreeStateStorage::~FCGLIBTreeStateStorage()
{
}

FCHashStateStorage::FCHashStateStorage()
{
	m_Hash = NULL;
}

FCHashStateStorage::~FCHashStateStorage()
{
	if (m_Hash != NULL)
	{
		m_Hash->DeleteItems();
		delete m_Hash;
	}
}

bool FCHashStateStorage::CheckAndInsert(FCSStateWithLocations** ExistingState, FCSStateWithLocations* NewState)
{
	*ExistingState = m_Hash->Insert(NewState, true);

	if (*ExistingState == NULL)
	{
		// The new state was found.  Already inserted
		return true;
	}

	return false;
}

FCGLIBHashStateStorage::FCGLIBHashStateStorage(int SizeWanted, ACompareNodesAlgorithm<FCSStateWithLocations, void>* Compare, 
									 AHashAlgorithm<FCSStateWithLocations>* Hash)
{
	m_Hash = new GLIBHash<FCSStateWithLocations, void>(SizeWanted, Compare, Hash);
}

FCGLIBHashStateStorage::~FCGLIBHashStateStorage()
{
}

FCInternalHashStateStorage::FCInternalHashStateStorage(int SizeWanted, ACompareNodesAlgorithm<FCSStateWithLocations, void>* CompareAlgorithm, 
											 AHashAlgorithm<FCSStateWithLocations>* Hash)
{
	m_Hash = new FCInternalHash<FCSStateWithLocations, void>(SizeWanted, CompareAlgorithm, Hash);
}

FCInternalHashStateStorage::~FCInternalHashStateStorage()
{
}

FCIndirectStateStorage::FCIndirectStateStorage()
{
	m_NumberOfPreviousStatesMargin = 0;
	m_NumberOfIndirectPreviousStates = 0;
	m_MaxNumberOfIndirectPreviousStates = PREV_STATES_GROW_BY;
	m_IndirectPreviousStates = new FCSIndirectStateWithLocations<FCSStateWithLocations>*[m_MaxNumberOfIndirectPreviousStates];
	for (int a = 0;a<m_MaxNumberOfIndirectPreviousStates;a++)
		m_IndirectPreviousStates[a] = NULL;
}

FCIndirectStateStorage::~FCIndirectStateStorage()
{
	delete [] m_IndirectPreviousStates;
}

/*
bool FCIndirectStateStorage::CheckAndInsert(FCSStateWithLocations** ExistingState, FCSStateWithLocations* NewState)
{
	FCSIndirectStateWithLocations<FCSStateWithLocations>** PosPtr;
	bool Found;

	//Try to see if the state is found in m_IndirectPreviousStates
	if ((PosPtr = BSearch<FCSIndirectStateWithLocations<FCSStateWithLocations>*>((FCSIndirectStateWithLocations<FCSStateWithLocations>**)&NewState, 
														m_IndirectPreviousStates,
														m_NumberOfIndirectPreviousStates, 
														&m_Compare)) == NULL)
	{
		//It isn't in the PreviousStates, but maybe it's in the sort margin
		PosPtr = FreecellSolverBSearch<FCSIndirectStateWithLocations<FCSStateWithLocations>*>((FCSIndirectStateWithLocations<FCSStateWithLocations>**)&NewState, 
																m_IndirectPreviousStatesMargin,
																m_NumberOfPreviousStatesMargin, &m_Compare,
																&Found);

		if (Found)
		{
			*ExistingState = *PosPtr;
			return false;
		}

		//Insert the state into its corresponding place in the sort margin
		memmove((PosPtr+1), PosPtr, sizeof(FCSIndirectStateWithLocations<FCSStateWithLocations>*) * 
					(m_NumberOfPreviousStatesMargin - (PosPtr - m_IndirectPreviousStatesMargin)));
		*PosPtr = (FCSIndirectStateWithLocations<FCSStateWithLocations>*)NewState;

		m_NumberOfPreviousStatesMargin++;

		if (m_NumberOfPreviousStatesMargin >= PREV_STATES_SORT_MARGIN)
		{
			//The sort margin is full, let's combine it with the main array
			if (m_NumberOfIndirectPreviousStates + m_NumberOfPreviousStatesMargin > 
				m_MaxNumberOfIndirectPreviousStates)
			{
				while (m_NumberOfIndirectPreviousStates + m_NumberOfPreviousStatesMargin > 
						m_MaxNumberOfIndirectPreviousStates)
				{
					m_MaxNumberOfIndirectPreviousStates += PREV_STATES_GROW_BY;
				}

				Realloc<FCSIndirectStateWithLocations<FCSStateWithLocations>*>(&m_IndirectPreviousStates,
									m_NumberOfIndirectPreviousStates+1, m_MaxNumberOfIndirectPreviousStates);
			}

			SFOMergeLargeAndSmallSortedArrays<FCSIndirectStateWithLocations<FCSStateWithLocations>*>(
					m_IndirectPreviousStates, m_NumberOfIndirectPreviousStates,
					m_IndirectPreviousStatesMargin, m_NumberOfPreviousStatesMargin,
					&m_Compare);

			m_NumberOfIndirectPreviousStates += m_NumberOfPreviousStatesMargin;
			m_NumberOfPreviousStatesMargin = 0;
		}

		return true;
	}

	*ExistingState = *PosPtr;
	return false;
}
*/


bool FCIndirectStateStorage::CheckAndInsert(FCSStateWithLocations** ExistingState, FCSStateWithLocations* NewState)
{
	FCSIndirectStateWithLocations<FCSStateWithLocations>** PosPtr;
	bool Found;

	//Try to see if the state is found in m_IndirectPreviousStates
	PosPtr = FreecellSolverBSearch<FCSIndirectStateWithLocations<FCSStateWithLocations>*>((FCSIndirectStateWithLocations<FCSStateWithLocations>**)&NewState,
														m_IndirectPreviousStates,
														m_NumberOfIndirectPreviousStates, 
														&m_Compare, &Found);

	if (!Found)
	{
		//It isn't in the PreviousStates, but maybe it's in the sort margin
		PosPtr = FreecellSolverBSearch<FCSIndirectStateWithLocations<FCSStateWithLocations>*>((FCSIndirectStateWithLocations<FCSStateWithLocations>**)&NewState, 
																m_IndirectPreviousStatesMargin,
																m_NumberOfPreviousStatesMargin, &m_Compare,
																&Found);

		if (Found)
		{
			*ExistingState = *PosPtr;
			return false;
		}

		//Insert the state into its corresponding place in the sort margin
		memmove((PosPtr+1), PosPtr, sizeof(FCSIndirectStateWithLocations<FCSStateWithLocations>*) * 
					(m_NumberOfPreviousStatesMargin - (PosPtr - m_IndirectPreviousStatesMargin)));

		*PosPtr = (FCSIndirectStateWithLocations<FCSStateWithLocations>*)NewState;

		m_NumberOfPreviousStatesMargin++;

		if (m_NumberOfPreviousStatesMargin >= PREV_STATES_SORT_MARGIN)
		{
			//The sort margin is full, let's combine it with the main array
			if (m_NumberOfIndirectPreviousStates + m_NumberOfPreviousStatesMargin > 
				m_MaxNumberOfIndirectPreviousStates)
			{
				while (m_NumberOfIndirectPreviousStates + m_NumberOfPreviousStatesMargin > 
						m_MaxNumberOfIndirectPreviousStates)
				{
					m_MaxNumberOfIndirectPreviousStates += PREV_STATES_GROW_BY;
				}

				Realloc<FCSIndirectStateWithLocations<FCSStateWithLocations>*>(&m_IndirectPreviousStates,
									m_NumberOfIndirectPreviousStates, m_MaxNumberOfIndirectPreviousStates);
			}

			SFOMergeLargeAndSmallSortedArrays<FCSIndirectStateWithLocations<FCSStateWithLocations>*>(
					m_IndirectPreviousStates, m_NumberOfIndirectPreviousStates,
					m_IndirectPreviousStatesMargin, m_NumberOfPreviousStatesMargin,
					&m_Compare);

			m_NumberOfIndirectPreviousStates += m_NumberOfPreviousStatesMargin;
			m_NumberOfPreviousStatesMargin = 0;
		}

		return true;
	}

	*ExistingState = *PosPtr;
	return false;
}

