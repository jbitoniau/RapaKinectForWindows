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
#include "RKFWCaptureSettings.h"

#include <sstream>

namespace RKFW
{

CaptureSettings::CaptureSettings()
	: mImageContent(ImageContent_Color),
	  mImageFormat(),
	  mFrameRate(0),
	  mFocalDistanceInPixels(0)
{
}
CaptureSettings::CaptureSettings( ImageContentEnum imageContent, const ImageFormat& imageFormat, unsigned int frameRate, float focalDistanceInPixels )
	: mImageContent( imageContent ),
	  mImageFormat(imageFormat),
	  mFrameRate(frameRate),
	  mFocalDistanceInPixels(focalDistanceInPixels)
{
}

bool CaptureSettings::operator==( const CaptureSettings& other ) const
{
	return	mImageContent==other.mImageContent &&
			mImageFormat==other.mImageFormat && 
			mFrameRate==other.mFrameRate &&
			mFocalDistanceInPixels==other.mFocalDistanceInPixels;
}

std::string CaptureSettings::toString() const
{
	std::stringstream stream;
	if ( mImageContent==ImageContent_Color )
		stream << "Color";
	else if ( mImageContent==ImageContent_Infrared )
		stream << "Infrared";
	else if ( mImageContent==ImageContent_Depth )
		stream << "Depth";
	stream << ", " << mImageFormat.toString() << ", " << mFrameRate << " frames/s " << mFocalDistanceInPixels << " px focal distance";
	return stream.str();
}

}