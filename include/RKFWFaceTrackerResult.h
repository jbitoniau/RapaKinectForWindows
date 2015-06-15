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

#include <string>
#include "RKFWVector3.h"

namespace RKFW
{

/*
	FaceTrackerResult

	Represents a result of the FaceTracker work
*/
class FaceTrackerResult
{
public:
	FaceTrackerResult();

	std::string toString( const std::string& separator=", " ) const;

	// Indicate wether tracking is happening. If not, the previous scale, rotation and translation are preserved
	// For the communication with Unity, We use a union for bool values so they are stored on an int. 
	// Warning: this is only true on a 32bits build!
	union	
	{	
		bool	mIsTracking;
		int		mIsTrackingInt;
	};	

	// The scale of the face. A scale of 1 corresponds to the base size of the face 3D model
	float		mFaceScale;

	// The orientation of the face expressed as Euler angles in degrees supposedly in the following 
	// sequence: around X (pitch?), Y (yaw) and Z (roll)... 
	// See http://msdn.microsoft.com/en-us/library/microsoft.kinect.facetracking.iftresult.get3dpose.aspx
	Vector3f	mFaceRotation;			
	
	// The position of the face in meters relative to the camera
	Vector3f	mFaceTranslation;	
};

}