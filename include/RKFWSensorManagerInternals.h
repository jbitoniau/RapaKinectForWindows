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
