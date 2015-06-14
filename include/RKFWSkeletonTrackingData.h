#pragma once

#include <string>

namespace RKFW
{

/*
	SkeletonTrackingData

	Represents the data provided by the Sensor when Skeleton Tracking is enabled.
	For now this class is pretty much empty. It needs to be completed to contain
	all the skeleton information provided by the Kinect SDK
*/
class SkeletonTrackingData
{
public:
	SkeletonTrackingData();
	SkeletonTrackingData( float timestampInSec, unsigned int numSkeletonsTracked );

	std::string toString( const std::string& separator=", " ) const;

	float			getTimestampInSec() const			{ return mTimestampInSec; }
	unsigned int	getNumSkeletonsTracked() const		{ return mNumSkeletonsTracked; } 

private:
	float			mTimestampInSec;
	unsigned int	mNumSkeletonsTracked;
};

}