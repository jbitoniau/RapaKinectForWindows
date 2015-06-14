#pragma once

#include "RKFWImageFormat.h"
#include "RKFWMemoryBuffer.h"

namespace RKFW
{

/*
	Image
*/
class Image
{
public:
	Image();
	Image( const ImageFormat& imageFormat );
	Image( const Image& other );

	const ImageFormat&				getFormat() const		{ return mFormat; }
	
	MemoryBuffer&					getBuffer()				{ return mBuffer; }
	const MemoryBuffer&				getBuffer() const		{ return mBuffer; }
		
private:
	ImageFormat						mFormat;
	MemoryBuffer					mBuffer;	
};

}