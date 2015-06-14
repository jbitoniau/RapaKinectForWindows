#pragma once

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX
#include <windows.h>
#include <FaceTrackLib.h>

#include "RKFWFaceTracker.h"
#include "RKFWCOMObjectSharedPtr.h"

namespace RKFW
{

class FaceTrackerInternals
{
public:
	FaceTrackerInternals();
	virtual ~FaceTrackerInternals();

	bool								isStarted() const { return mIsStarted; }
	bool								start( ImageFormat colorImageFormat, float colorImageFocalLengthInPixels, 
											   ImageFormat depthImageFormat, float depthImageFocalLengthInPixels );
	bool								update( const Image& colorImage, const Image& depthImage );
	const FaceTrackerResult&			getResult() const { return mResult; }
	void								stop();

	const FaceTracker::Vertices&		getFaceModelNeutralVertices() const		{ return mFaceModelNeutralVertices; }
	const FaceTracker::Triangles&		getFaceModelNeutralTriangles() const	{ return mFaceModelNeutralTriangles; }
	const FaceTracker::Vertices&		getFaceModelFittedVertices() const		{ return mFaceModelFittedVertices; }
	
private:
	static bool							getIFTImageFormat( ImageFormat::Encoding encoding, FTIMAGEFORMAT& iftImageFormat );
	static void							copyImageToIFTImage( const Image& image, IFTImage& iftImage );
	static bool							getFaceModelNeutralGeometry( IFTFaceTracker* faceTracker, FaceTracker::Vertices& vertices, FaceTracker::Triangles& triangles );
	static bool							getFaceModelFittedVertices( IFTFaceTracker* faceTracker, IFTResult* faceTrackerResult, FaceTracker::Vertices& vertices );

	bool								mIsStarted;
	COMObjectSharedPtr<IFTImage>		mColorImage;
	COMObjectSharedPtr<IFTImage>		mDepthImage;
	COMObjectSharedPtr<IFTFaceTracker>	mFaceTracker; 
	COMObjectSharedPtr<IFTResult>		mFaceTrackerResult;

	FaceTrackerResult					mResult;
	FaceTracker::Vertices				mFaceModelNeutralVertices;
	FaceTracker::Triangles				mFaceModelNeutralTriangles;
	FaceTracker::Vertices				mFaceModelFittedVertices;

	// This needs to be large enough to contain all the shape units of the model. The one used by Kinect SDK has 11 shape units
	static const UINT					maxShapeUnitCount = 15;	
};

}