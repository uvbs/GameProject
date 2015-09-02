#ifndef MMANN_MD5HASH_H
#define MMANN_MD5HASH_H

////////////////////////////////////////////////
///\file MD5Hash.h
///\brief This file contains the MD5Hash class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

///Number of hash bytes used in the hash algorithm
#define MD5_HASHBYTES 16

///\brief Hash function class
///
///This class is based off of Ron Rivest and Colin Plumb's
///implementation of a hash function written in C
class MD5Hash
{
public:
	///Constructor
	MD5Hash();

	///Destructor
	~MD5Hash();

	///\brief Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
	///initialization constants.
	void Init();

	///\brief Update context to reflect the concatenation of another buffer full
	///of bytes.
	///
	///\param Buffer is the value to be hashed
	///\param Length is the size of bytes of the value
	void Update(unsigned char const *Buffer, unsigned Length);

	///\brief Final wrapup - pad to 64-byte boundary with the bit pattern 
	///1 0* (64-bit count of bits processed, MSB-first)
	///
	///\param Digest is the hash value being manipulated
	void Final(unsigned char Digest[MD5_HASHBYTES]);

protected:

	///\brief The core of the MD5 algorithm, this alters an existing MD5 hash to
	///reflect the addition of 16 longwords of new data.  MD5Update blocks
	///the data and converts bytes into longwords for this routine.
	void Transform(unsigned __int32 buf[4], unsigned __int32 const in[16]);

private:
	///holds hash algorithm data
	unsigned __int32 m_Buffer[4];

	///holds hash algorithm data
	unsigned __int32 m_Bits[2];

	///holds hash algorithm data
	unsigned char m_In[64];
};

#endif