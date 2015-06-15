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

#include "RKFWImageFormat.h"

namespace RKFW
{

/*
	CaptureSettings

	Represents the parameters/settings provided to the Kinect when asked to start
	a color or depth capture.
	These parameters tell the Kinect camera what image format (width, height and encoding)
	to produce, at what framerate, and for the case of the color camera whether it should return 
	a classic color image, or an infrared one.
	Another information present in the settings is the focal distance of the camera
	when it shoots at the desired width/height (Notes: not entirely sure it should be in there,
	I've added it for the FaceTracker which requires this information).
*/
class CaptureSettings
{
public:
	enum ImageContentEnum
	{
		ImageContent_Color,		// Meaning *visible* color (as opposed to infrared)
		ImageContent_Infrared,
		ImageContent_Depth
	};

	CaptureSettings();
	CaptureSettings( ImageContentEnum imageContent, const ImageFormat& imageFormat, unsigned int frameRate, float focalDistanceInPixels );
	bool operator==( const CaptureSettings& other ) const;

	ImageContentEnum	getImageContent() const				{ return mImageContent; }
	const ImageFormat&	getImageFormat() const				{ return mImageFormat; }
	unsigned int		getFrameRate() const				{ return mFrameRate; }
	float				getFocalDistanceInPixels() const	{ return mFocalDistanceInPixels; }
	std::string			toString() const;

private:	
	ImageContentEnum	mImageContent;
	ImageFormat			mImageFormat;
	unsigned int		mFrameRate;
	float				mFocalDistanceInPixels;
};

}