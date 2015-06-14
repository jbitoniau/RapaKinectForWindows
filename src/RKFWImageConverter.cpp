#include "RKFWImageConverter.h"

#include <assert.h>

namespace RKFW
{

ImageConverter::ImageConverter( const ImageFormat& outputImageFormat )
	: mImage(NULL)	
{
	mImage = new Image( outputImageFormat );
}

ImageConverter::~ImageConverter()
{
	delete mImage;
	mImage = NULL;
}

bool ImageConverter::update( const Image& sourceImage )
{
	if ( sourceImage.getFormat()==mImage->getFormat() )
		return mImage->getBuffer().copyFrom( sourceImage.getBuffer() );
	return convertImage( sourceImage, *mImage );
}

#define CLIP_INT_TO_UCHAR(value) ( (value)<0 ? 0 : ( (value)>255 ? 255 : static_cast<unsigned char>(value) ) ) 

bool ImageConverter::convertUYVYImageToBGRX32Image( const Image& uyvyImage, Image& bgrx32Image )
{
	// Pre-checks
	if ( uyvyImage.getFormat().getEncoding()!=ImageFormat::UYVY )
		return false;
	if ( bgrx32Image.getFormat().getEncoding()!=ImageFormat::BGRX32 )
		return false;

	unsigned int width = uyvyImage.getFormat().getWidth();
	unsigned int height = uyvyImage.getFormat().getHeight();
	if ( bgrx32Image.getFormat().getWidth()!=width || bgrx32Image.getFormat().getHeight()!=height )
		 return false;

	// General information about YUV color space can be found here:
	// http://en.wikipedia.org/wiki/YUV 
	// or here:
	// http://www.fourcc.org/yuv.php

	// The following conversion code comes from here:
	// http://stackoverflow.com/questions/4491649/how-to-convert-yuy2-to-a-bitmap-in-c
	// http://msdn.microsoft.com/en-us/library/aa904813(VS.80).aspx#yuvformats_2
	const unsigned char* sourceBytes = uyvyImage.getBuffer().getBytes();
	unsigned char* destBytes = bgrx32Image.getBuffer().getBytes();
	for ( unsigned int y=0; y<height; ++y )
	{
		for ( unsigned int i=0; i<width/2; ++i )
		{
			int u0 = sourceBytes[0];
			int y0 = sourceBytes[1];
			int v0 = sourceBytes[2];
			int y1 = sourceBytes[3];
		
			sourceBytes += 4;	
			
			int c = y0 - 16;
			int d = u0 - 128;
			int e = v0 - 128;
			destBytes[0] = CLIP_INT_TO_UCHAR(( 298 * c + 516 * d           + 128) >> 8);		// Blue
			destBytes[1] = CLIP_INT_TO_UCHAR(( 298 * c - 100 * d - 208 * e + 128) >> 8);		// Green
			destBytes[2] = CLIP_INT_TO_UCHAR(( 298 * c           + 409 * e + 128) >> 8);		// Red
			destBytes[3] = 255;		
			
			c = y1 - 16;
			destBytes[4] = CLIP_INT_TO_UCHAR(( 298 * c + 516 * d           + 128) >> 8);		// Blue
			destBytes[5] = CLIP_INT_TO_UCHAR(( 298 * c - 100 * d - 208 * e + 128) >> 8);		// Green
			destBytes[6] = CLIP_INT_TO_UCHAR(( 298 * c           + 409 * e + 128) >> 8);		// Red
			destBytes[7] = 255;
			destBytes += 8;
		}
	}
	return true;	
}

bool ImageConverter::convertGrayscale16ImageToBGRX32Image( const Image& grayscale16Image, Image& bgrx32Image )
{
	// Pre-checks
	if ( grayscale16Image.getFormat().getEncoding()!=ImageFormat::Grayscale16 )
		return false;
	if ( bgrx32Image.getFormat().getEncoding()!=ImageFormat::BGRX32 )
		return false;

	unsigned int width = grayscale16Image.getFormat().getWidth();
	unsigned int height = grayscale16Image.getFormat().getHeight();
	if ( bgrx32Image.getFormat().getWidth()!=width || bgrx32Image.getFormat().getHeight()!=height )
		 return false;

	unsigned int sourceNumBytes = grayscale16Image.getBuffer().getSizeInBytes();
	
	const unsigned short int* source = reinterpret_cast<const unsigned short int*>( grayscale16Image.getBuffer().getBytes() );
	unsigned char* dest = bgrx32Image.getBuffer().getBytes();
	
	for ( unsigned int i=0; i<sourceNumBytes/2; ++i )
	{
		unsigned short int gray = *source;
		source++;

		unsigned char rescaledGray = static_cast<unsigned char>( gray / 256 ); 
		dest[0] = rescaledGray; 
		dest[1] = rescaledGray; 
		dest[2] = rescaledGray; 
		dest[3] = 255; 
		dest+=4;
	}
	return true;
}

/*
	This method does a raw conversion of a bayer image into BGRX-32 image. 
	The result is pretty ugly but shows how bayer works.
	Proper conversions or "demosaicing" exist. 
	See:
		http://elynxsdk.free.fr/API/html/convert_bayer.html
		http://en.wikipedia.org/wiki/Demosaicing
		http://www.cambridgeincolour.com/tutorials/camera-sensors.htm
		http://elynxlab.free.fr/userGuide/res/raw/Color%20Filter%20Array%20Interpolation.pdf
*/
bool ImageConverter::convertBayerImageToBGRX32Image( const Image& bayerImage, Image& bgrx32Image )
{
	// Pre-checks
	if ( bayerImage.getFormat().getEncoding()!=ImageFormat::Bayer )
		return false;
	if ( bgrx32Image.getFormat().getEncoding()!=ImageFormat::BGRX32 )
		return false;

	unsigned int width = bayerImage.getFormat().getWidth();
	unsigned int height = bayerImage.getFormat().getHeight();
	if ( bgrx32Image.getFormat().getWidth()!=width || bgrx32Image.getFormat().getHeight()!=height )
		 return false;
	
	const unsigned char* source = bayerImage.getBuffer().getBytes();
	unsigned char* dest = bgrx32Image.getBuffer().getBytes();
	for ( unsigned int i=0; i<height/2; ++i )
	{
		for ( unsigned int j=0; j<width/2; ++j )
		{
			unsigned char g = source[0];
			unsigned char r = source[1];
			source+=2;

			dest[0] = 0;
			dest[1] = g;
			dest[2] = 0;
			dest[3] = 255;
			
			dest[4] = 0;
			dest[5] = 0;
			dest[6] = r;
			dest[7] = 255;
			dest+=8;
		}
		for ( unsigned int j=0; j<width/2; ++j )
		{
			unsigned char b = source[0];
			unsigned char g = source[1];
			source+=2;

			dest[0] = b;
			dest[1] = 0;
			dest[2] = 0;
			dest[3] = 255;
			
			dest[4] = 0;
			dest[5] = g;
			dest[6] = 0;
			dest[7] = 255;
			dest+=8;
		}		
	}
	return true;
}

bool ImageConverter::convertImage( const Image& sourceImage, Image& destinationImage )
{
	if ( sourceImage.getFormat()==destinationImage.getFormat() )
		return false;
	ImageFormat::Encoding sourceEncoding = sourceImage.getFormat().getEncoding();
	ImageFormat::Encoding destinationEncoding = destinationImage.getFormat().getEncoding();
	if ( sourceEncoding==ImageFormat::UYVY && destinationEncoding==ImageFormat::BGRX32 )
		return convertUYVYImageToBGRX32Image( sourceImage, destinationImage );
	else if ( sourceEncoding==ImageFormat::Grayscale16 && destinationEncoding==ImageFormat::BGRX32 )
		return convertGrayscale16ImageToBGRX32Image( sourceImage, destinationImage );
	else if ( sourceEncoding==ImageFormat::Bayer && destinationEncoding==ImageFormat::BGRX32 )
		return convertBayerImageToBGRX32Image( sourceImage, destinationImage );
	return false;
}

}

