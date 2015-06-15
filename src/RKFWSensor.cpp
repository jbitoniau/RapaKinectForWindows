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
#include "RKFWSensor.h"

#include "RKFWSensorInternals.h"

namespace RKFW
{

const char* Sensor::mStatusNames[StatusEnumCount] = 
	{
		"OK",
		"initializing",
		"not connected",
		"not genuine",
		"not supported",
		"insufficient bandwidth",
		"not powered",
		"not ready",
		"Unknown"
	};

CaptureSettings Sensor::mColorCaptureSettings[ColorCaptureSettingsEnumCount] = 
	{ 
		CaptureSettings( CaptureSettings::ImageContent_Color,	ImageFormat( 1280, 960, ImageFormat::Bayer),		12, NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS * 2.f ),
		CaptureSettings( CaptureSettings::ImageContent_Color,	ImageFormat(  640, 480, ImageFormat::Bayer),		30, NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS ),
		CaptureSettings( CaptureSettings::ImageContent_Color,	ImageFormat(  640, 480, ImageFormat::UYVY),			15, NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS ),
		CaptureSettings( CaptureSettings::ImageContent_Color,	ImageFormat( 1280, 960, ImageFormat::BGRX32),		12, NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS * 2.f ),
		CaptureSettings( CaptureSettings::ImageContent_Color,	ImageFormat(  640, 480, ImageFormat::BGRX32),		30, NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS ),
		CaptureSettings( CaptureSettings::ImageContent_Color,	ImageFormat(  640, 480, ImageFormat::BGRX32),		15, NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS ), 
		CaptureSettings( CaptureSettings::ImageContent_Infrared, ImageFormat(  640, 480, ImageFormat::Grayscale16),	30, NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS )
	};

CaptureSettings Sensor::mDepthCaptureSettings[DepthCaptureSettingsEnumCount] = 
	{ 
		CaptureSettings( CaptureSettings::ImageContent_Depth,	ImageFormat( 640, 480, ImageFormat::Grayscale16), 30, NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS * 2.f ),
		CaptureSettings( CaptureSettings::ImageContent_Depth,	ImageFormat( 320, 240, ImageFormat::Grayscale16), 30, NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS ),
		CaptureSettings( CaptureSettings::ImageContent_Depth,	ImageFormat(  80,  60, ImageFormat::Grayscale16), 30, NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS / 4.f )
	};

Sensor::Sensor( unsigned int index )
	: mInternals(NULL)
{
	mInternals = new SensorInternals(this, index);
}

Sensor::Sensor( const std::wstring instanceId )
	: mInternals(NULL)
{
	mInternals = new SensorInternals(this, instanceId);
}

Sensor::~Sensor()
{
	delete mInternals;
	mInternals = NULL;
}

bool Sensor::isValid() const
{
	return mInternals->isValid();
}

Sensor::StatusEnum Sensor::getStatus() const
{
	return mInternals->getStatus();
}

const char* Sensor::getStatusName() const
{
	return getStatusName( getStatus() );
}

const char* Sensor::getStatusName( StatusEnum status )
{
	return mStatusNames[status];
}

void Sensor::update() 
{
	return mInternals->update();
}

std::wstring Sensor::getConnectionId() const
{
	return mInternals->getConnectionId();
}

int Sensor::getMinElevationAngleInDeg()
{
	return SensorInternals::getMinElevationAngleInDeg();
}

int Sensor::getMaxElevationAngleInDeg()
{
	return SensorInternals::getMaxElevationAngleInDeg();
}

int	Sensor::getElevationAngleInDeg() const
{
	return mInternals->getElevationAngleInDeg();
}

bool Sensor::setElevationAngleInDeg( int angleInDegrees )
{
	return mInternals->setElevationAngleInDeg( angleInDegrees );
}

Vector3f Sensor::getAccelerationInGs() const
{
	return mInternals->getAccelerationInGs();
}

/*
	Initialize and shutdown
*/
bool Sensor::initialize( SubsystemEnum flags )
{
	return mInternals->initialize( flags );
}

void Sensor::shutdown()
{
	return mInternals->shutdown();
}

/*
	Color camere settings
*/
const ColorCameraSettings& Sensor::getColorCameraSettings() const
{
	return mInternals->getColorCameraSettings();
}

bool Sensor::setColorCameraSettings( const ColorCameraSettings& settings )
{
	return mInternals->setColorCameraSettings( settings );
}

bool Sensor::resetColorCameraSettings()
{
	return mInternals->resetColorCameraSettings();
}

/*
	Color
*/
const CaptureSettings& Sensor::getColorCaptureSettings( ColorCaptureSettingsEnum settingsEnum )
{
	return mColorCaptureSettings[settingsEnum];
}

bool Sensor::startColorCapture( ColorCaptureSettingsEnum settingsEnum )
{
	return mInternals->startColorCapture( settingsEnum );
}

const CapturedImage* Sensor::getColorCapturedImage() const
{
	return mInternals->getColorCapturedImage();
}

Sensor::ColorCaptureSettingsEnum Sensor::getStartedColorCaptureSettings() const
{
	return mInternals->getStartedColorCaptureSettings();
}

/*
	Depth
*/
const CaptureSettings& Sensor::getDepthCaptureSettings( DepthCaptureSettingsEnum settingsEnum )
{
	return mDepthCaptureSettings[settingsEnum];
}

bool Sensor::startDepthCapture( DepthCaptureSettingsEnum settingsEnum, bool includePlayerIndexInDepth, bool nearMode )
{
	return mInternals->startDepthCapture( settingsEnum, includePlayerIndexInDepth, nearMode );
}

const CapturedImage* Sensor::getDepthCapturedImage() const
{
	return mInternals->getDepthCapturedImage();
}

Sensor::DepthCaptureSettingsEnum Sensor::getStartedDepthCaptureSettings() const
{
	return mInternals->getStartedDepthCaptureSettings();
}

/*
	Skeletons
*/
bool Sensor::startSkeletonTracking( bool seatedMode )
{
	return mInternals->startSkeletonTracking(seatedMode);
}

bool Sensor::stopSkeletonTracking()
{
	return mInternals->stopSkeletonTracking();
}

const SkeletonTrackingData* Sensor::getSkeletonTrackingData() const
{
	return mInternals->getSkeletonTrackingData();
}

/*
	Listeners
*/
void Sensor::addListener( Listener* listener )
{
	return mInternals->addListener( listener );
}

bool Sensor::removeListener( Listener* listener )
{
	return mInternals->removeListener( listener );
}

	 

}