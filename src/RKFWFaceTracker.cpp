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
#include "RKFWFaceTracker.h"

#include "RKFWFaceTrackerInternals.h"

namespace RKFW
{

/*
	Notes:
	- The FaceTracker tracks the first face it finds in the whole image. It should be possible have it track a face
	  in a certain area and have some kind of MultiFaceTracker class above distribute areas to various FaceTrackers.
	  A system like this would enable the tracking of multiple faces. 
	  See http://msdn.microsoft.com/en-us/library/microsoft.kinect.facetracking.iftfacetracker.detectfaces.aspx
*/
FaceTracker::FaceTracker()
	: mInternals(NULL)
{
	mInternals = new FaceTrackerInternals();
}

FaceTracker::~FaceTracker()
{
	delete mInternals;
	mInternals = NULL;
}

bool FaceTracker::isStarted() const
{
	return mInternals->isStarted();
}

bool FaceTracker::start( ImageFormat colorImageFormat, float colorImageFocalLengthInPixels,
						 ImageFormat depthImageFormat, float depthImageFocalLengthInPixels )
{
	return mInternals->start( colorImageFormat, colorImageFocalLengthInPixels, depthImageFormat, depthImageFocalLengthInPixels );
}

bool FaceTracker::update( const Image& colorImage, const Image& depthImage )
{
	return mInternals->update( colorImage, depthImage );
}

void FaceTracker::stop()
{
	mInternals->stop();
}

const FaceTrackerResult& FaceTracker::getResult() const
{
	return mInternals->getResult();
}

const FaceTracker::Vertices& FaceTracker::getFaceModelNeutralVertices() const
{
	return mInternals->getFaceModelNeutralVertices();
}

const FaceTracker::Triangles& FaceTracker::getFaceModelNeutralTriangles() const
{
	return mInternals->getFaceModelNeutralTriangles();
}

const FaceTracker::Vertices& FaceTracker::getFaceModelFittedVertices() const
{
	return mInternals->getFaceModelFittedVertices();
}

}
