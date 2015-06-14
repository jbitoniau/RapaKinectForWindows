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