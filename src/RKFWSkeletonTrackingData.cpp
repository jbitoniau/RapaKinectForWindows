#include "RKFWSkeletonTrackingData.h"

#include <sstream>

namespace RKFW
{

SkeletonTrackingData::SkeletonTrackingData()
	: mTimestampInSec(0), 
	  mNumSkeletonsTracked(0)
{
}

SkeletonTrackingData::SkeletonTrackingData( float timestampInSec, unsigned int numSkeletonsTracked )
	: mTimestampInSec(timestampInSec), 
	  mNumSkeletonsTracked(numSkeletonsTracked)
{
}

std::string SkeletonTrackingData::toString( const std::string& separator ) const
{
	std::stringstream stream;
	stream.precision(2);
	stream << std::fixed;
	stream << "Number of skeletons tracked: " << getNumSkeletonsTracked() << separator;
	stream << "Timestamp (in sec): " << getTimestampInSec();
	return stream.str();
}

}