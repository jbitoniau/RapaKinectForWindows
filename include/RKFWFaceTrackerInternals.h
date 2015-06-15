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