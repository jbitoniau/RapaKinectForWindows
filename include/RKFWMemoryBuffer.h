#pragma once

namespace RKFW
{

/*
	MemoryBuffer

	Represents a block of data in memory. 
*/
class MemoryBuffer
{
public:
	MemoryBuffer();
	MemoryBuffer( unsigned int sizeInBytes );
	MemoryBuffer( const MemoryBuffer& other );
	~MemoryBuffer();	

	unsigned int			getSizeInBytes() const	{ return mSizeInBytes; }
	const unsigned char*	getBytes() const		{ return mBytes; }
	unsigned char*			getBytes()				{ return mBytes; }
	
	void					fill( char value );
	bool					copyFrom( const MemoryBuffer& other );

private:
	MemoryBuffer& operator=( const MemoryBuffer& other );	// Not implemented on purpose

	unsigned char*			mBytes;
	unsigned int			mSizeInBytes;
};

}
