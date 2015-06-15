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

//#define WIN32_LEAN_AND_MEAN 
//#define NOMINMAX
#include <windows.h>
#if _MSC_VER <= 1500
#define static_assert(e, msg)			// Define static_assert for old version of VisualStudio so compilation of NuiApi works
#endif
#include <NuiApi.h>

#include <assert.h>
#include <stdio.h>
#include <sstream>

#include "RKFWVector3.h"
#include "RKFWCOMObjectSharedPtr.h"
#include "RKFWSensor.h"				// Mainly for the enums it defines
#include "RKFWCapturedImage.h"

namespace RKFW
{

class SensorInternals
{
public:
	SensorInternals( Sensor* parentSensor, unsigned int index );
	SensorInternals( Sensor* parentSensor, const std::wstring instanceId );
	virtual ~SensorInternals();

	bool								isValid() const							{ return NULL!=mSensor.get(); }
	Sensor::StatusEnum					getStatus() const;

	void								update();
	
	std::wstring						getConnectionId() const;

	static int							getMinElevationAngleInDeg()				{ return NUI_CAMERA_ELEVATION_MINIMUM; }	
	static int							getMaxElevationAngleInDeg()				{ return NUI_CAMERA_ELEVATION_MAXIMUM; }
	int									getElevationAngleInDeg() const;
	bool								setElevationAngleInDeg( int angleInDegrees );

	Vector3f							getAccelerationInGs() const;

	bool								initialize( Sensor::SubsystemEnum flags );
	void								shutdown();

	const ColorCameraSettings&			getColorCameraSettings() const						{ return mColorCameraSettings; }
	bool								setColorCameraSettings( const ColorCameraSettings& settings );
	bool								resetColorCameraSettings();

	bool								startColorCapture( Sensor::ColorCaptureSettingsEnum settingsEnum );
	const CapturedImage*				getColorCapturedImage() const						{ return mColorCapturedImage; }
	Sensor::ColorCaptureSettingsEnum	getStartedColorCaptureSettings() const				{ return mStartedColorCaptureSettingsEnum; }

	bool								startDepthCapture( Sensor::DepthCaptureSettingsEnum settingsEnum, bool includePlayerIndexInDepth, bool nearMode );
	const CapturedImage*				getDepthCapturedImage() const						{ return mDepthCapturedImage; }
	Sensor::DepthCaptureSettingsEnum	getStartedDepthCaptureSettings() const				{ return mStartedDepthCaptureSettingsEnum; }

	bool								startSkeletonTracking( bool seatedMode );
	bool								stopSkeletonTracking();
	const SkeletonTrackingData*			getSkeletonTrackingData() const						{ return mSkeletonTrackingData; }

	void								addListener( Sensor::Listener* listener );
	bool								removeListener( Sensor::Listener* listener );
		
protected:
	static DWORD	initializationFlagsToNuiInitializeFlags( Sensor::SubsystemEnum flags );
	static bool		getStreamImage( INuiSensor* sensor, HANDLE streamHandle, CapturedImage& capturedImage );

	static void		colorCaptureSettingsEnumToNuiImageResolutionAndNuiImageType( Sensor::ColorCaptureSettingsEnum settings, NUI_IMAGE_RESOLUTION& imageResolution, NUI_IMAGE_TYPE& imageType );
	static void		depthCaptureSettingsEnumToNuiImageResolutionAndNuiImageType( Sensor::DepthCaptureSettingsEnum settings, bool includePlayerIndexInDepth, NUI_IMAGE_RESOLUTION& imageResolution, NUI_IMAGE_TYPE& imageType );

	static bool		getColorCameraSettings( INuiSensor* sensor, ColorCameraSettings& cameraSettings );
	static bool		setColorCameraSettings( INuiSensor* sensor, const ColorCameraSettings& newSettings, const ColorCameraSettings& currentSettings );

	void			updateCachedColorCameraSettings();
	void			resetData();

private:
	Sensor*									mParentSensor;
	COMObjectSharedPtr<INuiSensor>			mSensor;
	
	ColorCameraSettings						mColorCameraSettings;

	Sensor::ColorCaptureSettingsEnum		mStartedColorCaptureSettingsEnum;
	HANDLE									mColorStreamHandle;
	CapturedImage*							mColorCapturedImage;
	
	Sensor::DepthCaptureSettingsEnum		mStartedDepthCaptureSettingsEnum;
	HANDLE									mDepthStreamHandle;
	CapturedImage*							mDepthCapturedImage;
	
	SkeletonTrackingData*					mSkeletonTrackingData;
	
	typedef	std::vector<Sensor::Listener*>	Listeners; 
	Listeners								mListeners;
};

}