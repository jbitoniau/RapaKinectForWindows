#include "RKFWCapturedImage.h"

namespace RKFW
{

CapturedImage::CapturedImage( ImageFormat imageFormat )
	: mImage(imageFormat),
	  mSequenceNumber(0),
	  mTimestampInSec(0)
{
}

}
