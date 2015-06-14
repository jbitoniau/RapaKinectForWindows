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