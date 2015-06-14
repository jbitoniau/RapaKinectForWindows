#include "RKFWFaceTrackerResult.h"

#include <sstream>

/*
	Notes:
	- Maybe the method to get the fitted face geometry should be in this class rather 
	  than in the FaceTracker
*/
namespace RKFW
{

FaceTrackerResult::FaceTrackerResult()
	: mIsTrackingInt(0), // false
	  mFaceScale(1.f),
	  mFaceRotation(),
	  mFaceTranslation()
{
}

std::string FaceTrackerResult::toString( const std::string& separator ) const
{
	std::stringstream stream;
	stream.precision(2);
	stream << std::fixed;
	stream << "Is tracking: " << mIsTracking << separator;
	stream << "Face scale: " << mFaceScale << separator;
	stream << "Face rotation: " << mFaceRotation.x() << " " << mFaceRotation.y() << " " << mFaceRotation.z() << separator;
	stream << "Face translation: " << mFaceTranslation.x() << " " << mFaceTranslation.y() << " " << mFaceTranslation.z();
	return stream.str();
}

}