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
#define static_assert(e, msg)	// Define static_assert for old version of VisualStudio so compilation of NuiApi works
#endif
#include <NuiApi.h>

#include "RKFWSensor.h"
#include "RKFWSensorManager.h"

namespace RKFW
{

class SensorManagerInternals
{
public:
	SensorManagerInternals( SensorManager* parentSensorManager );
	virtual ~SensorManagerInternals();

	void						update();
	const Sensors&				getSensors() const	{ return mSensors; }

	void						addListener( SensorManager::Listener* listener );
	bool						removeListener( SensorManager::Listener* listener );

protected:
	bool						createSensorsFromIndices();
	bool						addSensor( Sensor* sensor );
	bool						createSensorFromIndex( int index );
	bool						createSensorFromId( const std::wstring& sensorId );
	bool						deleteSensor( const std::wstring& sensorId );
	void						updateSensorList();
	static void CALLBACK		sensorStatusProc( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void* pUserData );

private:
	SensorManager*				mParentSensorManager;
	CRITICAL_SECTION			mCriticalSection;
	Sensors						mSensors;
	std::vector<std::wstring>	mRecentlyAttachedSensorIds;
	std::vector<std::wstring>	mRecentlyDetachedSensorIds;
	bool						mCreateSensorsFromIndices;

	typedef	std::vector<SensorManager::Listener*> Listeners; 
	Listeners					mListeners;
};

}
