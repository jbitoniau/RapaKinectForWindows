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
