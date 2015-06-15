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

#include "RKFWVector3.h"
#include "RKFWImage.h"
#include "RKFWFaceTrackerResult.h"
#include <vector>

namespace RKFW
{

class FaceTrackerInternals;

/*
	FaceTracker
	
	An object that can detect and track a single human face in 3D on provided color and depth images.

	The FaceTrackerResult obtained indicates whether a face is being tracked or not and if it is
	gives its position, rotation and scale.

	Additionally, when a face is tracked the FaceTracker can provided the geometry of a fitted 3D face
	corresponding to the user's. If you want to display the 3D face on top of the user's in the 
	color image, you have to apply the FaceTrackerResult transformation to the fitted face geometry.

	Reference: http://msdn.microsoft.com/en-us/library/jj130970.aspx
*/
class FaceTracker
{
public:
	typedef std::vector<Vector3f> Vertices;
	typedef std::vector<unsigned int> Triangles;

	FaceTracker();
	virtual ~FaceTracker();

	bool						isStarted() const;
	bool						start( ImageFormat colorImageFormat, float colorImageFocalLengthInPixels, 
									   ImageFormat depthImageFormat, float depthImageFocalLengthInPixels );
	bool						update( const Image& colorImage, const Image& depthImage );
	const FaceTrackerResult&	getResult() const;
	void						stop();
	
	// If not started, the following methods return empty vectors
	const Vertices&				getFaceModelNeutralVertices() const;
	const Triangles&			getFaceModelNeutralTriangles() const;

	// If not started, or tracking is lost, the method returns an empty vector
	const Vertices&				getFaceModelFittedVertices() const;

private:
	FaceTrackerInternals*		mInternals;
};

}