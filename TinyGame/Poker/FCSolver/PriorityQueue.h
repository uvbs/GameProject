#ifndef MMANN_PRIORITY_QUEUE_H
#define MMANN_PRIORITY_QUEUE_H

////////////////////////////////////////////////
///\file PriorityQueue.h
///\brief This file contains the classes dealing with a priority queue
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCHelpingAlgorithms.h"

///\brief A priority queue element
template <class Data>
class PriorityQueueElement
{
public:
	///Item in the queue
	Data* m_Item;

	///Rating on the queue
	int m_Rating;
};

///A templated priority queue
template <class Data>
class PriorityQueue
{
public:
	///Constructor
	PriorityQueue(int MaxElements, int MaxRating, int GrowBy, bool IsAscending);

	///Destructor
	~PriorityQueue();

	///\brief Push an item onto the queue
	///
	///\param Item to be put on the queue
	///\param Rating of the item
	void Push(Data* Item, int Rating);
	
	///\brief Pop the next item off the queue
	///
	///\return The item off the queue
	Data* Pop();

	///\brief Determines if queue is empty
	///
	///\return Whether or not queue is empty
	bool IsEmpty();

private:
	///Max size of queue
	int m_MaxSize;

	///Current size of queue
	int m_CurrentSize;

	///pointer to elements in queue
	PriorityQueueElement<Data>* m_Elements;	

	///biggest element possible
	int m_MaxRating;						

	///how big to grow queue if filled
	int m_GrowBy;							

	///sort heap with maximum scoring elements first
	bool m_IsAscendingHeap;					
};


/* given an index to any element in a binary tree stored in a linear array with the root at 1 and 
   a "sentinel" value at 0 these macros are useful in making the code clearer */

/// The parent is always given by index/2
#define PQ_PARENT_INDEX(i) ((i)>>1)
/// The first enry in the queue
#define PQ_FIRST_ENTRY (1)

///Left children are at index *2
#define PQ_LEFT_CHILD_INDEX(i) ((i)<<1)
///Right children are at (index * 2) +1
#define PQ_RIGHT_CHILD_INDEX(i) (((i)<<)+1)


template <class Data>
PriorityQueue<Data>::PriorityQueue(int MaxElements, int MaxRating, int GrowBy, bool IsAscending)
{
	m_IsAscendingHeap = IsAscending;
	m_MaxSize = MaxElements;
	m_CurrentSize = 0;
	m_Elements = new PriorityQueueElement<Data>[m_MaxSize+1];
	m_GrowBy = GrowBy;
	m_MaxRating = MaxRating;
}

template <class Data>
PriorityQueue<Data>::~PriorityQueue()
{
	delete [] m_Elements;
}

template <class Data>
void PriorityQueue<Data>::Push(Data* Item, int Rating)
{   
	int i;

	if (m_CurrentSize == m_MaxSize )
	{
		int NewSize = m_MaxSize + m_GrowBy;

		Realloc<PriorityQueueElement<Data> >(&m_Elements, m_CurrentSize+1, NewSize+1);

		m_MaxSize = NewSize;
	}


	// set i to the first unused element and increment CurrentSize
	i = (m_CurrentSize += 1);

	/* while the parent of the space we're putting the new node into is worse than
	   our new node, swap the space with the worse node. We keep doing that until we
	   get to a worse node or until we get to the top

	   note that we also can sort so that the minimum elements bubble up so we need to loops
	   with the comparison operator flipped... */

	if(m_IsAscendingHeap)
	{
		while( ( i==PQ_FIRST_ENTRY ?
				 (m_MaxRating) /* return biggest possible rating if first element */
				 :
				 (m_Elements[ PQ_PARENT_INDEX(i) ].m_Rating )
			   ) 
			   < Rating 
			 )
		{
			m_Elements[ i ] = m_Elements[ PQ_PARENT_INDEX(i) ];

			i = PQ_PARENT_INDEX(i);
		}
	}
	else
	{
		while( ( i==PQ_FIRST_ENTRY ?
				 (m_MaxRating) /* return biggest possible rating if first element */
				 :
				 (m_Elements[ PQ_PARENT_INDEX(i) ].m_Rating )
			   ) 
			   > Rating
			 )
		{
			m_Elements[ i ] = m_Elements[ PQ_PARENT_INDEX(i) ];

			i = PQ_PARENT_INDEX(i);
		}
	}

	// then add the element at the space we created.
	m_Elements[i].m_Item = Item;
	m_Elements[i].m_Rating = Rating;
}

template <class Data>
Data* PriorityQueue<Data>::Pop()
{
	if (IsEmpty())
	{
		return NULL;
	}

	int i;
	int child;

	PriorityQueueElement<Data> pMaxElement = m_Elements[PQ_FIRST_ENTRY];
	//pointer to last element in tree
	PriorityQueueElement<Data> pLastElement = m_Elements[ m_CurrentSize-- ];

	if(m_IsAscendingHeap)
	{

		/* code to pop an element from an ascending (top to bottom) pqueue */

		/*  UNTESTED */

		for( i=PQ_FIRST_ENTRY; PQ_LEFT_CHILD_INDEX(i) <= m_CurrentSize; i=child )
		{
			/* set child to the smaller of the two children... */

			child = PQ_LEFT_CHILD_INDEX(i);

			if( (child != m_CurrentSize) &&
				(m_Elements[child + 1].m_Rating > m_Elements[child].m_Rating) )
			{
				child ++;
			}

			if( pLastElement.m_Rating < m_Elements[ child ].m_Rating )
			{
				m_Elements[ i ] = m_Elements[ child ];
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		/* code to pop an element from a descending (top to bottom) pqueue */

		for( i=PQ_FIRST_ENTRY; PQ_LEFT_CHILD_INDEX(i) <= m_CurrentSize; i=child )
		{
			/* set child to the larger of the two children... */

			child = PQ_LEFT_CHILD_INDEX(i);

			if( (child != m_CurrentSize) &&
				(m_Elements[child + 1].m_Rating < m_Elements[child].m_Rating) )
			{
				child ++;
			}

			if(pLastElement.m_Rating > m_Elements[ child ].m_Rating )
			{
				m_Elements[ i ] = m_Elements[ child ];
			}
			else
			{
				break;
			}
		}
	}

	m_Elements[i] = pLastElement;

	return pMaxElement.m_Item;
}

template <class Data>
bool PriorityQueue<Data>::IsEmpty()
{
	if( m_CurrentSize == 0 )
	{
		return true;
	}

	return false;
}

#endif
