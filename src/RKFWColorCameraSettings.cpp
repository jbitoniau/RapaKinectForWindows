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
#include "RKFWColorCameraSettings.h"

namespace RKFW
{

ColorCameraSettings::ColorCameraSettings()		
	: mAutoExposureInt(1),			// true
	  mAutoWhiteBalanceInt(1),		// true
	  mBacklightCompensationMode(BacklightCompensationMode_AverageBrightness),
	  
	  mBrightness(0.2156),
	  mMinBrightness(0),
	  mMaxBrightness(1),
	
	  mContrast(1),
	  mMinContrast(0.5),
	  mMaxContrast(2),
	  
	  mExposureTime(4000),
	  mMinExposureTime(1),
	  mMaxExposureTime(4000),
	  
	  mFrameInterval(0),
	  mMinFrameInterval(0),
	  mMaxFrameInterval(4000),

	  mGain(1),
	  mMinGain(1),
	  mMaxGain(16),

	  mGamma(2.2),
	  mMinGamma(1),
	  mMaxGamma(2.8),

	  mHue(0),
	  mMinHue(-22),
	  mMaxHue(22),
	  
	  mSaturation(1),
	  mMinSaturation(0),
	  mMaxSaturation(2),
	  
	  mSharpness(0.5),
	  mMinSharpness(0),
	  mMaxSharpness(1),
	  
	  mWhiteBalance(2700),
	  mMinWhiteBalance(2700),
	  mMaxWhiteBalance(6500),
	  
	  mPowerLineFrequency(PowerLineFrequency_Disabled)
{
}

bool ColorCameraSettings::operator==( const ColorCameraSettings& other ) const
{
	return	mAutoExposureInt==other.mAutoExposureInt &&
			
			mAutoWhiteBalanceInt==other.mAutoWhiteBalanceInt &&
			
			mBacklightCompensationMode==other.mBacklightCompensationMode &&
			
			mBrightness==other.mBrightness &&
			mMinBrightness==other.mMinBrightness &&
			mMaxBrightness==other.mMaxBrightness &&
			
			mContrast==other.mContrast &&
			mMinContrast==other.mMinContrast &&
			mMaxContrast==other.mMaxContrast &&

			mExposureTime==other.mExposureTime &&
			mMinExposureTime==other.mMinExposureTime &&
			mMaxExposureTime==other.mMaxExposureTime &&
			
			mFrameInterval==other.mFrameInterval &&
			mMinFrameInterval==other.mMinFrameInterval &&
			mMaxFrameInterval==other.mMaxFrameInterval &&
			
			mGain==other.mGain &&
			mMinGain==other.mMinGain &&
			mMaxGain==other.mMaxGain &&
			
			mGamma==other.mGamma &&
			mMinGamma==other.mMinGamma &&
			mMaxGamma==other.mMaxGamma &&
			
			mHue==other.mHue &&
			mMinHue==other.mMinHue &&
			mMaxHue==other.	mMaxHue &&
			
			mSaturation==other.mSaturation &&
			mMinSaturation==other.mMinSaturation &&
			mMaxSaturation==other.mMaxSaturation &&
			
			mSharpness==other.mSharpness &&
			mMinSharpness==other.mMinSharpness &&
			mMaxSharpness==other.mMaxSharpness &&
			
			mWhiteBalance==other.mWhiteBalance &&
			mMinWhiteBalance==other.mMinWhiteBalance &&
			mMaxWhiteBalance==other.mMaxWhiteBalance &&	

			mPowerLineFrequency==other.mPowerLineFrequency;
}

bool ColorCameraSettings::operator!=( const ColorCameraSettings& other ) const
{
	return !(*this==other);
}


}