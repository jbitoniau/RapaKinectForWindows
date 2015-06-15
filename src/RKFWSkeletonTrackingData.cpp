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