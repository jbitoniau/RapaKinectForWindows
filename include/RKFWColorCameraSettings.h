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

namespace RKFW
{

/*
	ColorCameraSettings

	Represents the various settings of the color camera (for example: exposure, saturation, 
	white balance, etc...). 
	These can be changed live when the camera is capturing.
	
	The class has been designed so instances can be passed relatively easily to Unity as is.

	Here is the reference:
	http://msdn.microsoft.com/en-us/library/nuisensor.inuicolorcamerasettings.aspx
*/
class ColorCameraSettings
{
public:
	ColorCameraSettings();

	bool operator==( const ColorCameraSettings& other ) const;
	bool operator!=( const ColorCameraSettings& other ) const;

	enum BacklightCompensationModeEnum	// Must be kept in sync with NUI_BACKLIGHT_COMPENSATION_MODE
	{
		BacklightCompensationMode_AverageBrightness = 0,
		BacklightCompensationMode_CenterPriority = 1,
		BacklightCompensationMode_LowlightsPriority = 2,
		BacklightCompensationMode_CenterOnly = 4
	};

	enum PowerLineFrequencyEnum			// Must be kept in sync with NUI_POWER_LINE_FREQUENCY 
	{	
		PowerLineFrequency_Disabled = 0,
		PowerLineFrequency_50Hz = 1,
		PowerLineFrequency_60Hz = 2
	};

	// For the communication with Unity, We use a union for bool values so they are stored on an int. 
	// Warning: this is only true on a 32bits build!
	union	 
	{	
		bool	mAutoExposure;
		int	 	mAutoExposureInt;
	};
	union
	{
		bool	mAutoWhiteBalance;
		int		mAutoWhiteBalanceInt;
	};
	BacklightCompensationModeEnum	mBacklightCompensationMode;
	
	double		mBrightness;
	double		mMinBrightness;
	double		mMaxBrightness;
	
	double		mContrast;
	double		mMinContrast;
	double		mMaxContrast;

	double		mExposureTime;
	double		mMinExposureTime;
	double		mMaxExposureTime;
	
	double		mFrameInterval;
	double		mMinFrameInterval;
	double		mMaxFrameInterval;
	
	double		mGain;
	double		mMinGain;
	double		mMaxGain;
	
	double		mGamma;
	double		mMinGamma;
	double		mMaxGamma;
	
	double		mHue;
	double		mMinHue;
	double		mMaxHue;
	
	double		mSaturation;
	double		mMinSaturation;
	double		mMaxSaturation;
	
	double		mSharpness;
	double		mMinSharpness;
	double		mMaxSharpness;
	
	int			mWhiteBalance;
	int			mMinWhiteBalance;
	int			mMaxWhiteBalance;

	PowerLineFrequencyEnum	mPowerLineFrequency;
};

}