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