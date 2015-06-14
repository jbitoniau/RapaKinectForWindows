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