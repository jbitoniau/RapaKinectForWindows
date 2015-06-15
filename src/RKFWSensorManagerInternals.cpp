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
#include "RKFWSensorManagerInternals.h"

#include <assert.h>
#include <algorithm>

#include "RKFWCriticalSectionEnterer.h"

namespace RKFW
{

SensorManagerInternals::SensorManagerInternals( SensorManager* parentSensorManager )
	: mParentSensorManager(parentSensorManager),
	  mCriticalSection(),
	  mSensors(),
	  mRecentlyAttachedSensorIds(),
	  mRecentlyDetachedSensorIds(),
	  mCreateSensorsFromIndices(true),
	  mListeners()
{
	InitializeCriticalSection( &mCriticalSection );
}

SensorManagerInternals::~SensorManagerInternals()
{
	Sensors sensors = mSensors;	// The copy is on purpose here
	for ( std::size_t i=0; i<sensors.size(); i++ )
		deleteSensor( sensors[i]->getConnectionId() );
	assert( mSensors.empty() );

	// http://social.msdn.microsoft.com/Forums/en-US/kinectsdknuiapi/thread/ee5b3708-a279-48cf-bd1e-8dbc72dc2e23
	// Note I suppose this removes all the registered callbacks including the ones that may have been registered
	// outside the scope of this library :(
	NuiSetDeviceStatusCallback( NULL, NULL );

	DeleteCriticalSection( &mCriticalSection );
}

void SensorManagerInternals::update()
{
	if ( mCreateSensorsFromIndices )
	{
		NuiSetDeviceStatusCallback( &sensorStatusProc, this );
	
		// Enumerate and create sensors. After this initially enumeration, 
		// the device status callback will take over to update the list of sensors
		createSensorsFromIndices();

		mCreateSensorsFromIndices = false;
	}

	updateSensorList();

	for ( Sensors::iterator itr=mSensors.begin(); itr!=mSensors.end(); ++itr )
		(*itr)->update();
}

bool SensorManagerInternals::createSensorsFromIndices()
{
	int numSensors = 0;
	HRESULT hr = NuiGetSensorCount( &numSensors );
	if ( FAILED(hr) )
		return false;
	for ( int i=0; i<numSensors; ++i )
		createSensorFromIndex(i);
	return true;
}

bool SensorManagerInternals::addSensor( Sensor* sensor )
{
	if ( !sensor->isValid() )
		return false;

	// Check for sensor id collision
	std::wstring sensorId = sensor->getConnectionId();
	for ( std::size_t i=0; i<mSensors.size(); i++ )
		if ( mSensors[i]->getConnectionId()==sensorId ) 
			return false;

	// Notify 
	for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
		(*itr)->onSensorAdding( mParentSensorManager );

	mSensors.push_back( sensor );

	// Notify 
	for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
		(*itr)->onSensorAdded( mParentSensorManager, sensor );

	return true;
}

bool SensorManagerInternals::createSensorFromIndex( int index )
{
	if ( index<0 )
		return false;
	Sensor* sensor = new Sensor( index );
	if ( !addSensor( sensor ) )
	{
		delete sensor;
		return false;
	}
	return true;
}

bool SensorManagerInternals::createSensorFromId( const std::wstring& sensorId )
{	
	// Pre-check that we're not trying to create a Sensor with the same id as an existing one
	for ( std::size_t i=0; i<mSensors.size(); i++ )
		if ( mSensors[i]->getConnectionId()==sensorId ) 
			return false;
			
	Sensor* sensor = new Sensor( sensorId );
	if ( !addSensor( sensor ) )
	{
		delete sensor;
		return false;
	}
	return true;
}

bool SensorManagerInternals::deleteSensor( const std::wstring& sensorId )
{	
	Sensors::iterator itr = mSensors.begin();
	while ( itr!=mSensors.end() )
	{
		if ( (*itr)->getConnectionId()==sensorId )
			break;
		else
			*itr++;
	}
	if ( itr==mSensors.end() )
		return false;
	
	Sensor* sensor = *itr;
	
	// Notify 
	for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
		(*itr)->onSensorRemoving( mParentSensorManager, sensor );

	mSensors.erase( itr );
	delete sensor;
	
	// Notify
	for ( Listeners::const_iterator itr=mListeners.begin(); itr!=mListeners.end(); ++itr )
		(*itr)->onSensorRemoved( mParentSensorManager, sensor );
	
	return true;
}

void SensorManagerInternals::updateSensorList()
{
	std::vector<std::wstring> recentlyAttachedSensorIds;
	std::vector<std::wstring> recentlyDetachedSensorIds;
	{
		CriticalSectionEnterer criticalSectionRAII( mCriticalSection );				
		recentlyAttachedSensorIds = mRecentlyAttachedSensorIds;
		mRecentlyAttachedSensorIds.clear();
		recentlyDetachedSensorIds = mRecentlyDetachedSensorIds;
		mRecentlyDetachedSensorIds.clear();
	}

	for ( std::size_t i=0; i<recentlyAttachedSensorIds.size(); ++i )
	{
		const std::wstring sensorId = recentlyAttachedSensorIds[i];
		bool ret = createSensorFromId( sensorId );
		assert( ret );		
	}

	for ( std::size_t i=0; i<recentlyDetachedSensorIds.size(); ++i )
	{
		const std::wstring sensorId = recentlyDetachedSensorIds[i];
		/*bool ret =*/ deleteSensor( sensorId );
		//assert( ret );		// JBM: commented out because it seems the SensorStatusProc is called multiple times for the same disconnect 
	}
}

void CALLBACK SensorManagerInternals::sensorStatusProc( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* /*uniqueDeviceName*/, void* pUserData )
{
	//wprintf(L"\n%d -- %s -- %s\n", hrStatus, instanceName, uniqueDeviceName );
	switch ( hrStatus )
	{
		case S_NUI_INITIALIZING:	
			printf("S_NUI_INITIALIZING\n");break;
		case E_NUI_NOTCONNECTED:	
			printf("E_NUI_NOTCONNECTED\n");break;
		case E_NUI_NOTGENUINE:	
			printf("E_NUI_NOTGENUINE\n");break;
		case E_NUI_NOTSUPPORTED:	
			printf("E_NUI_NOTSUPPORTED\n");break;
		case E_NUI_INSUFFICIENTBANDWIDTH:	
			printf("E_NUI_INSUFFICIENTBANDWIDTH\n");break;
		case E_NUI_NOTPOWERED:	
			printf("E_NUI_NOTPOWERED\n");break;
		case E_NUI_NOTREADY:	
			printf("E_NUI_NOTREADY\n");break;
		case S_OK:	
			printf("S_OK\n");break;
	}
	
	SensorManagerInternals* internals = reinterpret_cast<SensorManagerInternals*>( pUserData );
	assert( internals );
	
	CriticalSectionEnterer criticalSectionRAII( internals->mCriticalSection );				

	// After investigation, the status passed by the system to the callback works as follow:
	// - when the sensor is plugged-in, there's a call with S_NUI_INITIALIZING then another one 
	//   with the status indicating success (S_OK) or the nature of the problem (E_NUI_INSUFFICIENTBANDWIDTH, etc...)
	// - when the sensor is disconnected, the call is done with a status set to E_NUI_NOTPOWERED (under Parallels Desktop)
	//   or E_NUI_DISCONNECTED (under Boot Camp)
	// The following implementation only notifies the manager of a new sensor when it fully works. That means the
	// manager can't tell the client code if a problematic sensor is plugged in and what's the problem.
    if ( hrStatus==S_OK )      
    {          
		std::wstring attachedSensorId = instanceName;
		std::vector<std::wstring>& attachedSensorIds = internals->mRecentlyAttachedSensorIds;
		if ( std::find( attachedSensorIds.begin(), attachedSensorIds.end(), attachedSensorId )==attachedSensorIds.end() )
			attachedSensorIds.push_back( attachedSensorId );
	}      
    else if ( hrStatus==E_NUI_NOTPOWERED || hrStatus==E_NUI_NOTCONNECTED )
    {          
		std::wstring detachedSensorId = instanceName;
		std::vector<std::wstring> & detachedSensorIds = internals->mRecentlyDetachedSensorIds;
		if ( std::find( detachedSensorIds.begin(), detachedSensorIds.end(), detachedSensorId )==detachedSensorIds.end() )
			detachedSensorIds.push_back( detachedSensorId );
    }
}

/*
void SensorManagerInternals::wideCharStringToMultiByteString( const wchar_t* wideCharString, std::string& multiByteString )
{
	assert( wideCharString );
	size_t size = wcslen(wideCharString)+1;
	char* buffer = new char[size];
	size_t numCharConverted = 0;
	int ret = wcstombs_s( &numCharConverted, buffer, size, wideCharString, size );
	if ( ret==0 )
		multiByteString = buffer;
	delete[] buffer;
}*/

void SensorManagerInternals::addListener( SensorManager::Listener* listener )
{
	assert(listener);
	mListeners.push_back(listener);
}

bool SensorManagerInternals::removeListener( SensorManager::Listener* listener )
{
	Listeners::iterator itr = std::find( mListeners.begin(), mListeners.end(), listener );
	if ( itr==mListeners.end() )
		return false;
	mListeners.erase( itr );
	return true;
}

}