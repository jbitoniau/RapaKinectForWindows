#include "RKFWMemoryBuffer.h"

#include <stddef.h>		// For NULL
#include <memory.h>

namespace RKFW
{

MemoryBuffer::MemoryBuffer()
	: mBytes(NULL),
	  mSizeInBytes(0)
{
}

MemoryBuffer::MemoryBuffer( unsigned int sizeInBytes )
	: mBytes(NULL),
	  mSizeInBytes(sizeInBytes)
{
	mBytes = new unsigned char[mSizeInBytes];
	fill(0);
}

MemoryBuffer::MemoryBuffer( const MemoryBuffer& other )
	: mBytes(NULL),
	  mSizeInBytes( other.getSizeInBytes() )
{
	mBytes = new unsigned char[mSizeInBytes];
	memcpy( mBytes, other.getBytes(), other.getSizeInBytes() );
}

MemoryBuffer::~MemoryBuffer()
{
	delete[] mBytes;
	mBytes = NULL;
	mSizeInBytes = 0;
}

void MemoryBuffer::fill( char value )
{
	memset( mBytes, value, getSizeInBytes() );
}

bool MemoryBuffer::copyFrom( const MemoryBuffer& other )
{
	if ( other.getSizeInBytes()!=getSizeInBytes() )
		return false;
	memcpy( mBytes, other.getBytes(), getSizeInBytes() );
	return true;
}

}