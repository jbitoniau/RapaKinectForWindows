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
	CapturedImage

	Represents the product of the color or depth camera when a capture occurs.
	These cameras don't simply return an image. They also provide additional information
	such as the image timestamp and sequence number.
*/
class CapturedImage
{
public:
	CapturedImage( ImageFormat imageFormat );

	const Image&	getImage() const									{ return mImage; }
	unsigned int	getSequenceNumber() const							{ return mSequenceNumber; }
	float			getTimestampInSec()	const							{ return mTimestampInSec; }

	Image&			getImage()											{ return mImage; }
	void			setSequenceNumber( unsigned int	sequenceNumber )	{ mSequenceNumber = sequenceNumber; }
	void			setTimestampInSec( float timestamp )				{ mTimestampInSec = timestamp; }

private:
	Image			mImage;
	unsigned int	mSequenceNumber;
	float			mTimestampInSec;
};

}