#pragma once

#include "RKFWVector3.h"
#include "RKFWImage.h"
#include "RKFWColorCameraSettings.h"
#include "RKFWCaptureSettings.h"
#include "RKFWCapturedImage.h"
#include "RKFWSkeletonTrackingData.h"

#include <vector>

namespace RKFW
{

class SensorInternals;
	
/*
	Sensor

	Represents a physical Kinect device/sensor and provides access to its functionalities:
	- color camera
	- depth camera
	- skeleton tracking
*/
class Sensor
{
public:
	enum SubsystemEnum
	{
		Subsystem_ColorCamera				= 1 << 0,
		Subsystem_DepthCamera				= 1 << 1,
		Subsystem_SkeletonTracking			= 1 << 2		
		//Subsystem_Audio					= 1 << 3		
	};
	
	enum ColorCaptureSettingsEnum								
	{
		ColorCaptureSettings_Color_RawBayer_1280x960_12FPS,
		ColorCaptureSettings_Color_RawBayer_640x480_30FPS,
		ColorCaptureSettings_Color_UYVY_640x480_15FPS,
		ColorCaptureSettings_Color_BGRX32_1280x960_12FPS,
		ColorCaptureSettings_Color_BGRX32_640x480_30FPS,
		ColorCaptureSettings_Color_BGRX32_640x480_15FPS,			// This BGRX32 image comes from an UYVY image internally converted by the Kinect
		ColorCaptureSettings_Infrared_Grayscale16_640x480_30FPS,
		
		ColorCaptureSettingsEnumCount
	};

	enum DepthCaptureSettingsEnum								
	{
		DepthCaptureSettings_Grayscale16_640x480_30FPS,
		DepthCaptureSettings_Grayscale16_320x240_30FPS,
		DepthCaptureSettings_Grayscale16_80x60_30FPS,
				
		DepthCaptureSettingsEnumCount
	};

	enum StatusEnum
	{
		Status_OK,
		Status_Initializing,
		Status_NotConnected,
		Status_NotGenuine,
		Status_NotSupported,
		Status_InsufficientBandwidth,
		Status_NotPowered,
		Status_NotReady,
		Status_Unknown,

		StatusEnumCount
	};
	
	Sensor( unsigned int index );
	Sensor( const std::wstring instanceId );
	virtual ~Sensor();

	bool							isValid() const;
	StatusEnum						getStatus() const;
	const char*						getStatusName() const;
	static const char*				getStatusName( StatusEnum status );

	void							update();

	std::wstring					getConnectionId() const;

	static int						getMinElevationAngleInDeg();		
	static int						getMaxElevationAngleInDeg();
	int								getElevationAngleInDeg() const;
	bool							setElevationAngleInDeg( int angleInDegrees );
	
	Vector3f						getAccelerationInGs() const;

	bool							initialize( SubsystemEnum flags );
	void							shutdown();

	const ColorCameraSettings&		getColorCameraSettings() const;
	bool							setColorCameraSettings( const ColorCameraSettings& settings );
	bool							resetColorCameraSettings();

	static const CaptureSettings&	getColorCaptureSettings( ColorCaptureSettingsEnum settingsEnum );
	bool							startColorCapture( ColorCaptureSettingsEnum settingsEnum );
	ColorCaptureSettingsEnum		getStartedColorCaptureSettings() const;		// Returns the started ColorCaptureSettingsEnum, or ColorCaptureSettingsEnumCount if no color capture was started
	const CapturedImage*			getColorCapturedImage() const;				// Returns the CapturedImage for the color camera, or NULL if it wasn't started or if the first image has not yet been received
	
	static const CaptureSettings&	getDepthCaptureSettings( DepthCaptureSettingsEnum settingsEnum );	
	bool							startDepthCapture( DepthCaptureSettingsEnum settingsEnum, bool includePlayerIndexInDepth, bool nearMode ); // Will fail if includePlayerIndexInDepth is set without the Skeleton subsystem activated
	DepthCaptureSettingsEnum		getStartedDepthCaptureSettings() const;		// Same as for color camera
	const CapturedImage*			getDepthCapturedImage() const;				// Same as for color camera

	bool							startSkeletonTracking( bool seatedMode );
	bool							stopSkeletonTracking();
	const SkeletonTrackingData*		getSkeletonTrackingData() const;

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void onInitialize( Sensor* /*sensor*/ )			{}
		virtual void onShutdown( Sensor* /*sensor*/ )				{}
		
		virtual void onColorCameraSettingsChanged( Sensor* /*sensor*/ ) {}

		virtual void onColorCaptureStarted( Sensor* /*sensor*/ )	{}
		virtual void onCapturedColorImage( Sensor* /*sensor*/ )	{}

		virtual void onDepthCaptureStarted( Sensor* /*sensor*/ )	{}
		virtual void onCapturedDepthImage( Sensor* /*sensor*/ )	{}

		virtual void onSkeletonTrackingStarted( Sensor* /*sensor*/ )		{}
		virtual	void onCapturedSkeletonTrackingData( Sensor* /*sensor*/ )	{}
	};

	void							addListener( Listener* listener );
	bool							removeListener( Listener* listener );

private:
	SensorInternals*				mInternals;
	static const char*				mStatusNames[StatusEnumCount];
	static CaptureSettings			mColorCaptureSettings[ColorCaptureSettingsEnumCount];
	static CaptureSettings			mDepthCaptureSettings[DepthCaptureSettingsEnumCount];
};

typedef std::vector<Sensor*> Sensors;

}