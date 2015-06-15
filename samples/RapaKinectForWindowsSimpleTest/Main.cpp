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
#include "RKFWSensorManager.h"

#include <stdio.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>

int main()
{
	RKFW::SensorManager* sensorManager = new RKFW::SensorManager();
	sensorManager->update();

	const RKFW::Sensors& sensors = sensorManager->getSensors();
	for ( std::size_t i=0; i<sensors.size(); ++i )
	{
		printf("%d\n", i );
		//	testDevice( deviceManager, i );
	}

	RKFW::Sensor* sensor = NULL;
	if ( sensorManager->getSensors().size()>0 )
	{
		sensor = sensorManager->getSensors()[0];
		/*bool ret = */sensor->startColorCapture( RKFW::Sensor::ColorCaptureSettings_Color_BGRX32_1280x960_12FPS );
		//printf("ret\n");
	}
	
	for ( int i=0; i<2000; ++i )
	{
		sensorManager->update();


		printf(".");
		::Sleep( 200 );
	}


	printf("Done!");
	delete sensorManager;
	sensorManager = NULL;

	getchar();
	
	return 0;
}