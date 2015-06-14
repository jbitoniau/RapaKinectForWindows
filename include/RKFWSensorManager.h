#pragma once

#include "RKFWSensor.h"

namespace RKFW
{


class SensorManagerInternals;

/*
	SensorManager

	The SensorManager is responsible for enumerating the attached Kinect Sensors 
	and maintain this list up to date when a Sensor arrives or is removed. 
	
	The client code needs to call the update method regularly so the SensorManager
	handles these changes. The update of the SensorManager also propagates to 
	the Sensors themselves.

	The client code can be notified via listeners of any changes in the lost of 
	connected Sensors.
*/
class SensorManager
{
public:
	SensorManager();
	virtual ~SensorManager();

	void					update();
	const Sensors&			getSensors() const;

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void		onSensorAdding( SensorManager* /*sensorManager*/ ) {}
		virtual void		onSensorAdded( SensorManager* /*sensorManager*/, Sensor* /*sensor*/ ) {}
		virtual void		onSensorRemoving( SensorManager* /*sensorManager*/, Sensor* /*sensor*/ ) {}
		virtual void		onSensorRemoved( SensorManager* /*sensorManager*/, Sensor* /*sensor*/ ) {}	// The sensor object is already deleted, do not call methods on it
	};

	void					addListener( Listener* listener );
	bool					removeListener( Listener* listener );

private:
	SensorManagerInternals* mInternals;
};

}
