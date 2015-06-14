#pragma once

#include "RKFWImage.h"

namespace RKFW
{

/*
	ImageConverter	

	An object that converts the image it is given (via the update() method) into 
	a target ImageFormat specified at construction.
	
	Only some encoding conversions are supported. The size of the provided images must 
	be the one specified in the target format given at construction.

	The class exposes static methods to perform specific encoding conversions by hand.
*/
class ImageConverter
{
public:
	ImageConverter( const ImageFormat& outputImageFormat );
	virtual ~ImageConverter();

	bool			update( const Image& sourceImage );
	const Image&	getImage() const			{ return *mImage; }
	Image&			getImage()					{ return *mImage; }

	static bool		convertUYVYImageToBGRX32Image( const Image& uyvyImage, Image& bgrx32Image );
	static bool		convertGrayscale16ImageToBGRX32Image( const Image& grayscale16Image, Image& bgrx32Image );
	static bool		convertBayerImageToBGRX32Image( const Image& bayerImage, Image& bgrx32Image );
	static bool		convertImage( const Image& source, Image& destinationImage );

private:
	Image*			mImage;
};

}