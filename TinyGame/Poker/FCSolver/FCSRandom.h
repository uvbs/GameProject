#ifndef FCS_RANDOM_H
#define FCS_RANDOM_H

////////////////////////////////////////////////
///\file FCSRandom.h
///\brief This file contains the FCSRandom class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

///Generates a random number
class FCSRandom
{
public:
	///Constructor
	FCSRandom(long Seed);

	///Destructor
	~FCSRandom();

	///\brief Get a random number
	///
	///\return A random number from the generator
	int GetRandomNumber();

	///\brief Give the random number generator a new seed
	///
	///\param Seed for the random number generator
	void ReSeed(long Seed);

protected:
	///\brief Get a 15 bit random number
	///
	///\return A 15 bit random number
	int Rand15();

	///Seed of the random number generator
	long m_Seed;
};

#endif