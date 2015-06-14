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