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
#include "RKFWSensorInternals.h"

#include <algorithm>

namespace RKFW
{

SensorInternals::SensorInternals( Sensor* parentSensor, unsigned int index )
	: mParentSensor(parentSensor),
	  mSensor(NULL),
	  mColorCameraSettings(),
	  mStartedColorCaptureSettingsEnum(Sensor::ColorCaptureSettingsEnumCount),
	  mColorStreamHandle(0),
	  mColorCapturedImage(NULL),
	  mStartedDepthCaptureSettingsEnum(Sensor::DepthCaptureSettingsEnumCount),
	  mDepthStreamHandle(0),
	  mDepthCapturedImage(NULL),
	  mSkeletonTrackingData(NULL),
	  mListeners()
{
	INuiSensor* sensor = NULL;
	HRESULT hr = NuiCreateSensorByIndex( index, &sensor );
	if ( SUCCEEDED(hr) )
		mSensor = COMObjectSharedPtr<INuiSensor>( sensor );

	updateCachedColorCameraSettings();
}

SensorInternals::SensorInternals( Sensor* parentSensor, const std::wstring instanceId )
	: mParentSensor(parentSensor),
	  mSensor(NULL),
	  mColorCameraSettings(),
	  mStartedColorCaptureSettingsEnum(Sensor::ColorCaptureSettingsEnumCount),
	  mColorStreamHandle(0),
	  mColorCapturedImage(NULL),
	  mStartedDepthCaptureSettingsEnum(Sensor::DepthCaptureSettingsEnumCount),
	  mDepthStreamHandle(0),
	  mDepthCapturedImage(NULL),
	  mSkeletonTrackingData(NULL),
	  mListeners()
{
	INuiSensor* sensor = NULL;
	HRESULT hr = NuiCreateSensorById( instanceId.c_str(), &sensor );
	if ( SUCCEEDED(hr) )
		mSensor = COMObjectSharedPtr<INuiSensor>( sensor );

	updateCachedColorCameraSettings();
}

SensorInternals::~SensorInternals()
{
	delete mColorCapturedImage;
	mColorCapturedImage = NULL;

	delete mDepthCapturedImage;
	mDepthCapturedImage = NULL;
}

Sensor::StatusEnum SensorInternals::getStatus() const
{
	if ( !isValid() )
		return Sensor::Status_Unknown;
	
	HRESULT hr = mSensor->NuiStatus();
	switch ( hr ) 
	{
		case S_OK :							return Sensor::Status_OK;
		case S_NUI_INITIALIZING	:			return Sensor::Status_Initializing;
		case E_NUI_NOTCONNECTED :			return Sensor::Status_NotConnected;
		case E_NUI_NOTGENUINE :				return Sensor::Status_NotGenuine;
		case E_NUI_NOTSUPPORTED :			return Sensor::Status_NotSupported;
		case E_NUI_INSUFFICIENTBANDWIDTH :	return Sensor::Status_InsufficientBandwidth;
		case E_NUI_NOTPOWERED :				return Sensor::Status_NotPowered;
		case E_NUI_NOTREADY :				return Sensor::Status_NotReady;
	}
	assert(false);
	return Sensor::Status_Unknown;
}

bool SensorInternals::getStreamImage( INuiSensor* sensor, HANDLE streamHandle, CapturedImage& capturedImage )
{
	// Get the image frame
	NUI_IMAGE_FRAME imageFrame;
	HRESULT hr = sensor->NuiImageStreamGetNextFrame( streamHandle, 0, &imageFrame );
	if ( FAILED(hr) )
	{
		// This can mean there's no image ready yet
		return false;
	}

	// Get the frame texture
	INuiFrameTexture* frameTexture = imageFrame.pFrameTexture;
	assert( frameTexture );
	if ( !frameTexture )
	{
		sensor->NuiImageStreamReleaseFrame( streamHandle, &imageFrame );	
		return false;
	}
    
	// Get a locked rectangle on the texture
	NUI_LOCKED_RECT lockedRect;
    hr = frameTexture->LockRect(0, &lockedRect, NULL, 0);
	assert( SUCCEEDED(hr) );
	if ( FAILED(hr) )
	{
		hr = sensor->NuiImageStreamReleaseFrame( streamHandle, &imageFrame );
		assert( SUCCEEDED(hr) );
		return false;
	}

	// Check that the parameters of the area conform to those of the image
	int numBytes = lockedRect.size;
	//int pitchInBytes = lockedRect.Pitch;
	const byte* bytes = lockedRect.pBits;
	if ( static_cast<int>( capturedImage.getImage().getBuffer().getSizeInBytes() )!=numBytes )
	{
		hr = frameTexture->UnlockRect(0);
		assert( SUCCEEDED(hr) );
		hr = sensor->NuiImageStreamReleaseFrame( streamHandle, &imageFrame );
		assert( SUCCEEDED(hr) );
		return false;
	}
	
	// Does the copy 
	assert( numBytes==static_cast<int>( capturedImage.getImage().getBuffer().getSizeInBytes() ) );
	memcpy( capturedImage.getImage().getBuffer().getBytes(), bytes, numBytes );

	// Fill in the sequence number
	capturedImage.setSequenceNumber( imageFrame.dwFrameNumber );
	
	// Fill in the timestamp
	float timestamp = static_cast<float>(imageFrame.liTimeStamp.QuadPart)/1000.f;
	capturedImage.setTimestampInSec( timestamp );

	// Release resources
	hr = frameTexture->UnlockRect(0);
	assert( SUCCEEDED(hr) );
	hr = sensor->NuiImageStreamReleaseFrame( streamHandle, &imageFrame );
	assert( SUCCEEDED(hr) );

	return true;
}

void SensorInternals::update()
{
	if ( !isValid() )
		return;

	if ( mColorStreamHandle )
	{
		unsigned int sequenceNumber = mColorCapturedImage->getSequenceNumber();
		getStreamImage( mSensor.get(), mColorStreamHandle, *mColorCapturedImage );
		if ( mColorCapturedImage->getSequenceNumber()!=sequenceNumber )
		{
			// Notify 
			for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
				(*itr)->onCapturedColorImage( mParentSensor );
		}
	}

	if ( mDepthStreamHandle )
	{
		unsigned int sequenceNumber = mDepthCapturedImage->getSequenceNumber();
		getStreamImage( mSensor.get(), mDepthStreamHandle, *mDepthCapturedImage );
		if ( mDepthCapturedImage->getSequenceNumber()!=sequenceNumber )
		{
			// Notify 
			for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
				(*itr)->onCapturedDepthImage( mParentSensor );
		}
	}

	if ( mSkeletonTrackingData )
	{
		// http://msdn.microsoft.com/en-us/library/jj131025.aspx
		NUI_SKELETON_FRAME skeletonFrame = {0};
		if ( SUCCEEDED( mSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)))
        {
			unsigned int numSkeletonsTracked = 0;

			for ( unsigned int i=0; i<NUI_SKELETON_COUNT; ++i )
			{
				const NUI_SKELETON_DATA& skeletonData = skeletonFrame.SkeletonData[i];
				if ( skeletonData.dwTrackingID != 0 )
					numSkeletonsTracked++;
			}

			float timestamp = static_cast<float>(skeletonFrame.liTimeStamp.QuadPart)/1000.f;
			
			*mSkeletonTrackingData = SkeletonTrackingData( timestamp, numSkeletonsTracked );

			// Notify 
			for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
				(*itr)->onCapturedSkeletonTrackingData( mParentSensor );
       }
	}
}

std::wstring SensorInternals::getConnectionId() const
{
	std::wstring result;
	if ( !isValid() )
		return result;

	result = mSensor->NuiDeviceConnectionId();
	return result;
}

int SensorInternals::getElevationAngleInDeg() const
{
	if ( !isValid() )
		return 0;

	LONG angleInDegrees = 0;
	HRESULT hr = mSensor->NuiCameraElevationGetAngle( &angleInDegrees );
	if ( FAILED(hr) )
		return 0;
	return static_cast<int>( angleInDegrees );
} 

bool SensorInternals::setElevationAngleInDeg( int angleInDegrees )
{
	if ( !isValid() )
		return false;

	HRESULT hr = mSensor->NuiCameraElevationSetAngle( angleInDegrees );
	if ( FAILED(hr) )
		return false;
	return true;
}

Vector3f SensorInternals::getAccelerationInGs() const
{
	Vector3f result;
	if ( !isValid() )
		return result;

	// Calling NuiAccelerometerGetCurrentReading() on the INuiSensor object
	// when the sensor has just been disconnected can cause the application to hang. 
	// To avoid this, we check the status
	if ( getStatus()!=Sensor::Status_OK )
		return result;

	::Vector4 acc = { 0.f, 0.f, 0.f, 0.f };
	HRESULT hr = mSensor->NuiAccelerometerGetCurrentReading( &acc );
	if ( FAILED(hr) )
		return result;
	result.x() = acc.x;
	result.y() = acc.y;
	result.z() = acc.z;
	return result;
}
 
DWORD SensorInternals::initializationFlagsToNuiInitializeFlags( Sensor::SubsystemEnum flags/*, Sensor::DepthImageContent depthImageContent*/ )
{
	DWORD ret = 0;

	if ( flags & Sensor::Subsystem_ColorCamera )
		ret |= NUI_INITIALIZE_FLAG_USES_COLOR;

	if ( flags & Sensor::Subsystem_DepthCamera  )
		ret |= NUI_INITIALIZE_FLAG_USES_DEPTH;

	if ( flags & Sensor::Subsystem_SkeletonTracking )
	{
		ret |= NUI_INITIALIZE_FLAG_USES_SKELETON ;			
		if ( flags & Sensor::Subsystem_DepthCamera  )
			ret |= NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX;
	}

	return ret;
}

bool SensorInternals::initialize( Sensor::SubsystemEnum flags )
{
	if ( !isValid() )
		return false;

	DWORD nuiFlags = initializationFlagsToNuiInitializeFlags( flags );

	// Notes: There are a few things to know about NuiInitialize 
	// - if NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX is set, the skeleton tracking subsystem will 
	//   be quietly activated internally
	// - NUI_INITIALIZE_FLAG_USES_DEPTH and NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX define 
	//   strictly the types of image that can be requested when starting the depth capture. 
	// - No flag set will cause NuiInitialize to fail 
	// - If the Kinect Sensor is already in use by another application, NuiInitialize will fail
	// - Multiple calls to NuiInitialize() are possible. Each call stops any capture in progress
	HRESULT hr = mSensor->NuiInitialize( nuiFlags );
	bool ret = SUCCEEDED(hr);
	if ( ret )
	{
		// Clear members holding data 
		resetData();
		
		// Just in case synchronise the cached value (in practise they should be already OK)
		updateCachedColorCameraSettings();

		// Notify 
		for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
			(*itr)->onInitialize( mParentSensor );
	}
	return ret;
}

void SensorInternals::shutdown()
{	
	if ( !isValid() )
		return;

	mSensor->NuiShutdown();

	// Clear members holding data 
	resetData();

	// Just in case synchronise the cached value (in practise they should be already OK)
	updateCachedColorCameraSettings();

	// Notify 
	for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
		(*itr)->onShutdown( mParentSensor );
}

bool SensorInternals::setColorCameraSettings( const ColorCameraSettings& settings )
{
	if ( !isValid() )
		return false;

	// Make the changes needed 
	bool changed = setColorCameraSettings( mSensor.get(), settings, mColorCameraSettings );
	
	if ( changed )
	{
		// Update the cached values
		mColorCameraSettings = settings;

		// Notify 
		for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
			(*itr)->onColorCameraSettingsChanged( mParentSensor );
	}
	return true;
}

void SensorInternals::updateCachedColorCameraSettings()
{
	// Get the values from the hardware
	ColorCameraSettings currentSettings;
	getColorCameraSettings( mSensor.get(), currentSettings );

	// If the values 
	bool changed = (currentSettings!=mColorCameraSettings);
	
	if ( changed )
	{
		// The current values become the cached ones
		mColorCameraSettings = currentSettings;

		// Notify client of changed
		for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
			(*itr)->onColorCameraSettingsChanged( mParentSensor );	
	}
}	

void SensorInternals::colorCaptureSettingsEnumToNuiImageResolutionAndNuiImageType( Sensor::ColorCaptureSettingsEnum settings, NUI_IMAGE_RESOLUTION& imageResolution, NUI_IMAGE_TYPE& imageType )
{
	imageResolution = NUI_IMAGE_RESOLUTION_80x60;
	imageType = NUI_IMAGE_TYPE_COLOR;
	switch ( settings )
	{
		case Sensor::ColorCaptureSettings_Infrared_Grayscale16_640x480_30FPS: 
			imageResolution = NUI_IMAGE_RESOLUTION_640x480;
			imageType = NUI_IMAGE_TYPE_COLOR_INFRARED;
			break;	
		case Sensor::ColorCaptureSettings_Color_RawBayer_1280x960_12FPS: 
			imageResolution = NUI_IMAGE_RESOLUTION_1280x960;
			imageType = NUI_IMAGE_TYPE_COLOR_RAW_BAYER;
			break;	
		case Sensor::ColorCaptureSettings_Color_RawBayer_640x480_30FPS: 
			imageResolution = NUI_IMAGE_RESOLUTION_640x480;
			imageType = NUI_IMAGE_TYPE_COLOR_RAW_BAYER;
			break;	
		case Sensor::ColorCaptureSettings_Color_UYVY_640x480_15FPS: 
			imageResolution = NUI_IMAGE_RESOLUTION_640x480;
			imageType = NUI_IMAGE_TYPE_COLOR_RAW_YUV;
			break;	
		case Sensor::ColorCaptureSettings_Color_BGRX32_1280x960_12FPS: 
			imageResolution = NUI_IMAGE_RESOLUTION_1280x960;
			imageType = NUI_IMAGE_TYPE_COLOR;
			break;	
		case Sensor::ColorCaptureSettings_Color_BGRX32_640x480_30FPS: 
			imageResolution = NUI_IMAGE_RESOLUTION_640x480;
			imageType = NUI_IMAGE_TYPE_COLOR;
			break;	
		case Sensor::ColorCaptureSettings_Color_BGRX32_640x480_15FPS: 
			imageResolution = NUI_IMAGE_RESOLUTION_640x480;
			imageType = NUI_IMAGE_TYPE_COLOR_YUV;
			break;	
		default:
			{ assert(false); }
	}
}

void SensorInternals::depthCaptureSettingsEnumToNuiImageResolutionAndNuiImageType( Sensor::DepthCaptureSettingsEnum settings, bool includePlayerIndexInDepth, NUI_IMAGE_RESOLUTION& imageResolution, NUI_IMAGE_TYPE& imageType )
{
	imageResolution = NUI_IMAGE_RESOLUTION_80x60;
	switch ( settings )
	{
		case Sensor::DepthCaptureSettings_Grayscale16_320x240_30FPS:
			imageResolution = NUI_IMAGE_RESOLUTION_320x240;
			break;		

		case Sensor::DepthCaptureSettings_Grayscale16_640x480_30FPS:
			imageResolution = NUI_IMAGE_RESOLUTION_640x480;
			imageType = NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX;
			break;	

		case Sensor::DepthCaptureSettings_Grayscale16_80x60_30FPS:
			imageResolution = NUI_IMAGE_RESOLUTION_80x60;
			break;	
		
		default:
			{ assert(false); }
	}
	if ( includePlayerIndexInDepth )
		imageType = NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX;
	else 
		imageType = NUI_IMAGE_TYPE_DEPTH;
}

void SensorInternals::resetData()
{
	mStartedColorCaptureSettingsEnum = Sensor::ColorCaptureSettingsEnumCount;
	mColorStreamHandle = NULL;
	delete mColorCapturedImage;
	mColorCapturedImage = NULL;

	mStartedDepthCaptureSettingsEnum = Sensor::DepthCaptureSettingsEnumCount;
	mDepthStreamHandle = NULL;
	delete mDepthCapturedImage;
	mDepthCapturedImage = NULL;

	delete mSkeletonTrackingData;
	mSkeletonTrackingData = NULL;
}

bool SensorInternals::startColorCapture( Sensor::ColorCaptureSettingsEnum settingsEnum )
{
	if ( !isValid() )
		return false;

	// Prepare NUI parameters 
	NUI_IMAGE_RESOLUTION nuiImageResolution;
	NUI_IMAGE_TYPE nuiImageType;
	colorCaptureSettingsEnumToNuiImageResolutionAndNuiImageType( settingsEnum, nuiImageResolution, nuiImageType );
	const DWORD numFramesToBuffer = 2;
	HANDLE nextFrameEvent = 0;
	HANDLE streamHandle;
	HRESULT hr = mSensor->NuiImageStreamOpen( nuiImageType, nuiImageResolution, 0, numFramesToBuffer, nextFrameEvent, &streamHandle );
	if ( FAILED(hr) )
		return false;
	mColorStreamHandle = streamHandle;

	// Store enum value that was used to start capture
	mStartedColorCaptureSettingsEnum = settingsEnum;

	// Delete existing CapturedImage
	delete mColorCapturedImage;
	mColorCapturedImage = NULL;

	// Prepare new one that will receive the captured data
	const CaptureSettings& captureSettings = Sensor::getColorCaptureSettings( settingsEnum );
	mColorCapturedImage = new CapturedImage( captureSettings.getImageFormat() );
	
	// Notify 
	for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
		(*itr)->onColorCaptureStarted( mParentSensor );

	return true;
}

bool SensorInternals::startDepthCapture( Sensor::DepthCaptureSettingsEnum settingsEnum, bool includePlayerIndexInDepth, bool nearMode )
{
	if ( !isValid() )
		return false;

	// Prepare NUI parameters 
	NUI_IMAGE_RESOLUTION nuiImageResolution;
	NUI_IMAGE_TYPE nuiImageType;
	depthCaptureSettingsEnumToNuiImageResolutionAndNuiImageType( settingsEnum, includePlayerIndexInDepth, nuiImageResolution, nuiImageType );
	const DWORD numFramesToBuffer = 2;
	HANDLE nextFrameEvent = 0;
	HANDLE streamHandle;
	DWORD flags = 0;
	if ( nearMode )
		flags = NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE;

	HRESULT hr = mSensor->NuiImageStreamOpen( nuiImageType, nuiImageResolution, flags, numFramesToBuffer, nextFrameEvent, &streamHandle );
	if ( FAILED(hr) )
		return false;
	mDepthStreamHandle = streamHandle;

	// Store enum value that was used to start capture
	mStartedDepthCaptureSettingsEnum = settingsEnum;

	// Delete existing CapturedImage
	delete mDepthCapturedImage;
	mDepthCapturedImage = NULL;

	// Prepare new one that will receive the captured data
	const CaptureSettings& captureSettings = Sensor::getDepthCaptureSettings( settingsEnum );
	mDepthCapturedImage = new CapturedImage( captureSettings.getImageFormat() );

	// Notify 
	for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
		(*itr)->onDepthCaptureStarted( mParentSensor );

	return true;
}

bool SensorInternals::startSkeletonTracking( bool seatedMode )
{
	if ( !isValid() )
		return false;

	DWORD flags = 0;
	if ( seatedMode ) 
		flags |= NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT;
	HRESULT hr = mSensor->NuiSkeletonTrackingEnable( NULL, flags );
	bool ret = SUCCEEDED(hr);
	if ( ret )
	{
		delete mSkeletonTrackingData;				
		mSkeletonTrackingData = new SkeletonTrackingData();

		// Notify 
		for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
			(*itr)->onSkeletonTrackingStarted( mParentSensor );
	}
	return ret;
}

bool SensorInternals::stopSkeletonTracking()
{
	// Notes: 
	// Not sure what happens with this function, but it doesn't seem to disable skeleton tracking at all. 
	// The player index data is still present in the depth buffer and getting the Skeleton still works...
	if ( !isValid() )
		return false;

	HRESULT hr = mSensor->NuiSkeletonTrackingDisable();
	bool ret = SUCCEEDED(hr);
	if ( ret )
	{
		delete mSkeletonTrackingData;			
		mSkeletonTrackingData = NULL;
	}
	return ret;
}

// This method reads the color camera settings from the Kinect hardware.
// Getting all the settings is relatively fast (doing it once a frame in Unity wasn't noticeable)
bool SensorInternals::getColorCameraSettings( INuiSensor* sensor, ColorCameraSettings& cameraSettings ) 
{
	if ( !sensor )
		return false;

	HRESULT hr = NULL;
	INuiColorCameraSettings* sensorCameraSettingsRaw = NULL;
	hr = sensor->NuiGetColorCameraSettings( &sensorCameraSettingsRaw );
	COMObjectSharedPtr<INuiColorCameraSettings> sensorCameraSettings( sensorCameraSettingsRaw );
	if ( FAILED(hr) )
		return false; // The color camera settings are only accessible on the Kinect For Windows hardware
	
	BOOL boolValue;
	hr = sensorCameraSettings->GetAutoExposure( &boolValue );
	assert( SUCCEEDED(hr) );
	boolValue ? cameraSettings.mAutoExposure = true : cameraSettings.mAutoExposure = false;
	
	hr = sensorCameraSettings->GetAutoWhiteBalance( &boolValue );
	assert( SUCCEEDED(hr) );
	boolValue ? cameraSettings.mAutoWhiteBalance = true : cameraSettings.mAutoWhiteBalance = false;
	
	NUI_BACKLIGHT_COMPENSATION_MODE backlightCompensationMode;
	hr = sensorCameraSettings->GetBacklightCompensationMode( &backlightCompensationMode ); 
	cameraSettings.mBacklightCompensationMode = static_cast<ColorCameraSettings::BacklightCompensationModeEnum>(backlightCompensationMode);
	assert( SUCCEEDED(hr) );
	
	hr = sensorCameraSettings->GetBrightness( &cameraSettings.mBrightness );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinBrightness( &cameraSettings.mMinBrightness );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxBrightness( &cameraSettings.mMaxBrightness );
	assert( SUCCEEDED(hr) );

	hr = sensorCameraSettings->GetContrast( &cameraSettings.mContrast );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinContrast( &cameraSettings.mMinContrast );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxContrast( &cameraSettings.mMaxContrast );
	assert( SUCCEEDED(hr) );
	
	hr = sensorCameraSettings->GetExposureTime( &cameraSettings.mExposureTime );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinExposureTime( &cameraSettings.mMinExposureTime );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxExposureTime( &cameraSettings.mMaxExposureTime );
	assert( SUCCEEDED(hr) );

	hr = sensorCameraSettings->GetFrameInterval( &cameraSettings.mFrameInterval );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinFrameInterval( &cameraSettings.mMinFrameInterval );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxFrameInterval( &cameraSettings.mMaxFrameInterval );
	assert( SUCCEEDED(hr) );

	hr = sensorCameraSettings->GetGain( &cameraSettings.mGain );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinGain( &cameraSettings.mMinGain );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxGain( &cameraSettings.mMaxGain );
	assert( SUCCEEDED(hr) );

	hr = sensorCameraSettings->GetGamma( &cameraSettings.mGamma );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinGamma( &cameraSettings.mMinGamma );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxGamma( &cameraSettings.mMaxGamma );
	assert( SUCCEEDED(hr) );

	hr = sensorCameraSettings->GetHue( &cameraSettings.mHue );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinHue( &cameraSettings.mMinHue );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxHue( &cameraSettings.mMaxHue );
	assert( SUCCEEDED(hr) );

	hr = sensorCameraSettings->GetSaturation( &cameraSettings.mSaturation );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinSaturation( &cameraSettings.mMinSaturation );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxSaturation( &cameraSettings.mMaxSaturation );
	assert( SUCCEEDED(hr) );

	hr = sensorCameraSettings->GetSharpness( &cameraSettings.mSharpness );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMinSharpness( &cameraSettings.mMinSharpness );
	assert( SUCCEEDED(hr) );
	hr = sensorCameraSettings->GetMaxSharpness( &cameraSettings.mMaxSharpness );
	assert( SUCCEEDED(hr) );

	LONG longValue;
	hr = sensorCameraSettings->GetWhiteBalance( &longValue );
	assert( SUCCEEDED(hr) );
	cameraSettings.mWhiteBalance = static_cast<int>(longValue);
	hr = sensorCameraSettings->GetMinWhiteBalance( &longValue );
	assert( SUCCEEDED(hr) );
	cameraSettings.mMinWhiteBalance = static_cast<int>(longValue);
	hr = sensorCameraSettings->GetMaxWhiteBalance( &longValue );
	assert( SUCCEEDED(hr) );
	cameraSettings.mMaxWhiteBalance = static_cast<int>(longValue);

	NUI_POWER_LINE_FREQUENCY  powerLineFrequency;
	hr = sensorCameraSettings->GetPowerLineFrequency ( &powerLineFrequency ); 
	cameraSettings.mPowerLineFrequency = static_cast<ColorCameraSettings::PowerLineFrequencyEnum>(powerLineFrequency);
	assert( SUCCEEDED(hr) );
	
	return true;
}

// This method sets the color camera settings to the Kinect hardware.
// The various color-camera set functions of the Kinect API are extremely slow
// This is why this method takes two input settings. The first one contains the new values, 
// the second contains the previously cached values.
// Only the values that are detected as different between the two are set to the hardware
// The method returns true if a change happened
bool SensorInternals::setColorCameraSettings( INuiSensor* sensor, const ColorCameraSettings& newSettings, const ColorCameraSettings& currentSettings ) 
{
	if ( !sensor )
		return false;
	
	// Check that we've got at least something to do
	if ( newSettings==currentSettings )
		return false;

	HRESULT hr = NULL;
	INuiColorCameraSettings* sensorCameraSettingsRaw = NULL;
	hr = sensor->NuiGetColorCameraSettings( &sensorCameraSettingsRaw );
	COMObjectSharedPtr<INuiColorCameraSettings> sensorCameraSettings( sensorCameraSettingsRaw );
	if ( FAILED(hr) )
		return false;			// The color camera settings are only accessible on the Kinect For Windows hardware
	
	bool changed = false;

	if ( newSettings.mAutoExposure != currentSettings.mAutoExposure )
	{
		hr = sensorCameraSettings->SetAutoExposure( newSettings.mAutoExposure );
		assert( SUCCEEDED(hr) );			
		changed = true;
	}

	if ( newSettings.mAutoWhiteBalance != currentSettings.mAutoWhiteBalance )
	{
		hr = sensorCameraSettings->SetAutoWhiteBalance( newSettings.mAutoWhiteBalance );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mBacklightCompensationMode != currentSettings.mBacklightCompensationMode )
	{
		hr = sensorCameraSettings->SetBacklightCompensationMode( static_cast<NUI_BACKLIGHT_COMPENSATION_MODE>(newSettings.mBacklightCompensationMode) );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mBrightness != currentSettings.mBrightness )
	{
		hr = sensorCameraSettings->SetBrightness( newSettings.mBrightness );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mContrast != currentSettings.mContrast )
	{
		hr = sensorCameraSettings->SetContrast( newSettings.mContrast );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mExposureTime != currentSettings.mExposureTime )
	{
		hr = sensorCameraSettings->SetExposureTime( newSettings.mExposureTime );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mFrameInterval != currentSettings.mFrameInterval )
	{
		hr = sensorCameraSettings->SetFrameInterval( newSettings.mFrameInterval );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mGain != currentSettings.mGain )
	{
		hr = sensorCameraSettings->SetGain( newSettings.mGain );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mGamma != currentSettings.mGamma )
	{
		hr = sensorCameraSettings->SetGamma( newSettings.mGamma );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
		
	if ( newSettings.mHue != currentSettings.mHue )
	{
		hr = sensorCameraSettings->SetHue( newSettings.mHue );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mSaturation != currentSettings.mSaturation )
	{
		hr = sensorCameraSettings->SetSaturation( newSettings.mSaturation );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mSharpness != currentSettings.mSharpness )
	{
		hr = sensorCameraSettings->SetSharpness( newSettings.mSharpness );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mWhiteBalance != currentSettings.mWhiteBalance )
	{
		hr = sensorCameraSettings->SetWhiteBalance( newSettings.mWhiteBalance );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	
	if ( newSettings.mPowerLineFrequency != currentSettings.mPowerLineFrequency )
	{
		hr = sensorCameraSettings->SetPowerLineFrequency( static_cast<NUI_POWER_LINE_FREQUENCY>(newSettings.mPowerLineFrequency) );
		assert( SUCCEEDED(hr) );	
		changed = true;
	}
	return changed;
}

bool SensorInternals::resetColorCameraSettings()
{
	if ( !isValid() )
		return false;

	INuiColorCameraSettings* sensorCameraSettingsRaw = NULL;
	HRESULT hr = mSensor->NuiGetColorCameraSettings( &sensorCameraSettingsRaw );
	COMObjectSharedPtr<INuiColorCameraSettings> sensorCameraSettings( sensorCameraSettingsRaw );
	if ( FAILED(hr) )
		return false;				// The color camera settings are only accessible on the Kinect For Windows hardware

	// Reset the hardware
	hr = sensorCameraSettings->ResetCameraSettingsToDefault();
	assert( SUCCEEDED(hr) );

	// Update cached color camera settings
	updateCachedColorCameraSettings();
	
	return true;
}

void SensorInternals::addListener( Sensor::Listener* listener )
{
	assert(listener);
	mListeners.push_back(listener);
}

bool SensorInternals::removeListener( Sensor::Listener* listener )
{
	Listeners::iterator itr = std::find( mListeners.begin(), mListeners.end(), listener );
	if ( itr==mListeners.end() )
		return false;
	mListeners.erase( itr );
	return true;
}
}