/*
   The MIT License (MIT) (http://opensource.org/licenses/MIT)
   
   Copyright (c) 2015 Jacques Menuet
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
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