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
