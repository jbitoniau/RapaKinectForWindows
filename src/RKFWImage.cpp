#include "RKFWImage.h"

#include <stdio.h>
#include <cstring>
#include <assert.h>

namespace RKFW
{

// Construct an empty image with zero size. The image instance obtained can still  
// be filled with data later using the assignment operator (which can modify its format).
Image::Image()
	: mFormat(),
	  mBuffer()
{
}

// Construct a blank image of a specific format. The internal image data is allocated and zero-filled
Image::Image( const ImageFormat& imageFormat )
	: mFormat( imageFormat), 
	  mBuffer( imageFormat.getDataSizeInBytes() )
{
}

// Construct an image from another one. The source image data is copied during the process
Image::Image( const Image& other )
	: mFormat( other.getFormat() ), 
	  mBuffer( other.getBuffer() )
{
}

}
