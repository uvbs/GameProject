#ifndef FCS_STATE_WITH_LOCATIONS_MATRIX_H
#define FCS_STATE_WITH_LOCATIONS_MATRIX_H

////////////////////////////////////////////////
///\file FCSStateWithLocationsMatrix.h
///\brief This file contains classes to deal with 2-dimensional FCSStateWithLocation arrays
///\author Michael Mann
///\version 1.0
///\date November 2002
////////////////////////////////////////////////

///\brief Abstract class to deal with child classes of FCSStateWithLocation
///
///This was created because casting multidimensional arrays doesn't work and is
///very bad coding practice.  This is the abstract base class for creating and accessing
///a 2-dimensional FCSStateWithLocation child class.
class AFCSStateWithLocationsMatrix
{
public:
	///Destructor
	virtual ~AFCSStateWithLocationsMatrix() {}

	///\brief Get a FCSStateWithLocations object from a 2-dimensional array
	///
	///\param Dim1 is the x position in the array
	///\param Dim2 is the y position in the array
	///\return A Pointer to the FCSStateWithLocations object in the array
	virtual FCSStateWithLocations* Get(int Dim1, int Dim2) = 0;

	///\brief Set an array of FCSStateWithLocations objects into a 2-dimensional array
	///
	///\param Dim1 is the first dimension in the class array
	///\param SetPtr a pointer to an FCSStateWithLocations object array
	virtual void Set(int Dim1, FCSStateWithLocations* SetPtr) = 0;

	///\brief Create a single instance of a FCSStateWithLocations child object in the array
	///
	///\param Dim1 is the dimension to create the object in
	virtual void Create(int Dim1) = 0;

	///\brief Create an array FCSStateWithLocations child objects in the array
	///
	///\param Dim1 is the dimension to create the objects in
	virtual void CreateArray(int Dim1, int Size) = 0;

	///\brief Delete a single instance of a FCSStateWithLocations child object in the array
	///
	///\param Dim1 is the dimension to delete the object
	virtual void Delete(int Dim1) = 0;

	///\brief Delete an array FCSStateWithLocations child objects in the array
	///
	///\param Dim1 is the dimension to delete the objects
	virtual void DeleteArray(int Dim1) = 0;

	///\brief Delete the m_MovesToParent member of an FCSStateWithLocations object in 
	///the array
	///
	///This is needed because of the dynamic allocation of different FCSStateWithLocations objects
	///\param Dim1 is the x position in the array
	///\param Dim2 is the y position in the array
	virtual void DeleteStateWithLocationsParent(int Dim1, int Dim2) = 0;

	///\brief Realloc one of the arrays in the 2-dimensional array
	///
	///\param Dim1 is the dimension that is being realloced
	///\param OldSize is the previous number of objects in the array
	///\param NewSize is the desired number of objects in the array
	virtual void ReallocSingle(int Dim1, int OldSize, int NewSize) = 0;

	///\brief Realloc the 2-dimensional array by reallocing the number of 
	///ConcreteClass pointers in the array
	///
	///\param OldSize is the previous number of pointers
	///\param NewSize is the desired number of pointers
	virtual void ReallocArray(int OldSize, int NewSize) = 0;
};

///\brief Concrete class to deal with child classes of FCSStateWithLocation.  ConcreteClass
///is the child class the 2-dimensional array is created in.
///
///This was created because casting multidimensional arrays doesn't work and is
///very bad coding practice.  This is the base class for creating and accessing
///a 2-dimensional FCSStateWithLocation child classes.
template <class ConcreteClass>
class FCSStateWithLocationsMatrix : public AFCSStateWithLocationsMatrix
{
public:
	///\brief Constructor
	///
	///\param Size is the number of ConcreteClass pointers in the array
	FCSStateWithLocationsMatrix(int Size);

	///Destructor
	virtual ~FCSStateWithLocationsMatrix();

	///\brief Get a FCSStateWithLocations child object from a 2-dimensional array
	///
	///\param Dim1 is the first dimension
	///\param Dim2 is the second dimension
	///\return A Pointer to the FCSStateWithLocations child object in the array
	virtual FCSStateWithLocations* Get(int Dim1, int Dim2);

	///\brief Set an array of FCSStateWithLocations child objects into a 2-dimensional array
	///
	///\param Dim1 is the first dimension in the class array
	///\param SetPtr a pointer to an FCSStateWithLocations child object array
	virtual void Set(int Dim1, FCSStateWithLocations* SetPtr);

	///\brief Create a single instance of a FCSStateWithLocations child object in the array
	///
	///\param Dim1 is the dimension to create the object in
	virtual void Create(int Dim1);

	///\brief Create an array FCSStateWithLocations child objects in the array
	///
	///\param Dim1 is the dimension to create the object in
	virtual void CreateArray(int Dim1, int Size);

	///\brief Delete a single instance of a FCSStateWithLocations child object in the array
	///
	///\param Dim1 is the dimension to delete the object
	virtual void Delete(int Dim1);

	///\brief Delete an array FCSStateWithLocations child objects in the array
	///
	///\param Dim1 is the dimension to delete the objects
	virtual void DeleteArray(int Dim1);

	///\brief Delete the m_MovesToParent member of an FCSStateWithLocations object in 
	///the array
	///
	///This is needed because of the dynamic allocation of different FCSStateWithLocations objects
	///\param Dim1 is the x position in the array
	///\param Dim2 is the y position in the array
	virtual void DeleteStateWithLocationsParent(int Dim1, int Dim2);

	///\brief Realloc one of the arrays in the 2-dimensional array
	///
	///\param Dim1 is the dimension that is being realloced
	///\param OldSize is the previous number of objects in the array
	///\param NewSize is the desired number of objects in the array	
	virtual void ReallocSingle(int Dim1, int OldSize, int NewSize);

	///\brief Realloc the 2-dimensional array by reallocing the number of 
	///ConcreteClass pointers in the array
	///
	///\param OldSize is the previous number of pointers
	///\param NewSize is the desired number of pointers
	virtual void ReallocArray(int OldSize, int NewSize);

protected:

	///2-dimensional array of FCSStateWithLocation child objects
	ConcreteClass** m_Array;
};

template <class ConcreteClass>
FCSStateWithLocationsMatrix<ConcreteClass>::FCSStateWithLocationsMatrix(int Size)
{
	m_Array = new ConcreteClass*[Size];
}

template <class ConcreteClass>
FCSStateWithLocationsMatrix<ConcreteClass>::~FCSStateWithLocationsMatrix()
{
	delete [] m_Array;
}

template <class ConcreteClass>
FCSStateWithLocations* FCSStateWithLocationsMatrix<ConcreteClass>::Get(int Dim1, int Dim2)
{
	return &m_Array[Dim1][Dim2];
}

template <class ConcreteClass>
void FCSStateWithLocationsMatrix<ConcreteClass>::Set(int Dim1, FCSStateWithLocations* SetPtr)
{
	m_Array[Dim1] = (ConcreteClass*)SetPtr;
}


template <class ConcreteClass>
void FCSStateWithLocationsMatrix<ConcreteClass>::Create(int Dim1)
{
	m_Array[Dim1] = new ConcreteClass();
}


template <class ConcreteClass>
void FCSStateWithLocationsMatrix<ConcreteClass>::CreateArray(int Dim1, int Size)
{
	m_Array[Dim1] = new ConcreteClass[Size];
}

template <class ConcreteClass>
void FCSStateWithLocationsMatrix<ConcreteClass>::Delete(int Dim1)
{
	delete m_Array[Dim1];
}

template <class ConcreteClass>
void FCSStateWithLocationsMatrix<ConcreteClass>::DeleteArray(int Dim1)
{
	delete [] m_Array[Dim1];
}

template <class ConcreteClass>
void FCSStateWithLocationsMatrix<ConcreteClass>::DeleteStateWithLocationsParent(int Dim1, int Dim2)
{
	if (m_Array[Dim1][Dim2].m_MovesToParent != NULL)
		delete [] m_Array[Dim1][Dim2].m_MovesToParent;
}

template <class ConcreteClass>
void FCSStateWithLocationsMatrix<ConcreteClass>::ReallocSingle(int Dim1, int OldSize, int NewSize)
{
	Realloc<ConcreteClass>(&m_Array[Dim1], OldSize, NewSize);
}

template <class ConcreteClass>
void FCSStateWithLocationsMatrix<ConcreteClass>::ReallocArray(int OldSize, int NewSize)
{
	Realloc<ConcreteClass*>(&m_Array, OldSize, NewSize);
}

#endif