#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )
#endif
	#include <QApplication>
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include "RKFWQSensorManagerWidget.h"
#include "RKFWSensorManager.h"

#include "RKFWQFaceTrackerTestWidget.h"

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>

int main(int argc, char** argv)
{		
	QApplication app( argc, argv );

	RKFW::SensorManager* sensorManager = new RKFW::SensorManager();

	//QWidget* mainWidget = new QWidget();
	QWidget* mainWidget = NULL;
	RKFW::QSensorManagerWidget* sensorManagerWidget = new RKFW::QSensorManagerWidget(mainWidget, sensorManager);
	RKFW::QFaceTrackerTestWidget* faceTrackerWidget = new RKFW::QFaceTrackerTestWidget(mainWidget, sensorManager);
	RKFW::QFaceTrackerTestWidget* faceTrackerWidget2 = new RKFW::QFaceTrackerTestWidget(mainWidget, sensorManager);
	
//	mainWidget->show();
//	mainWidget->resize(100, 100);
	sensorManagerWidget->resize(320, 480);
	sensorManagerWidget->show();
	faceTrackerWidget->resize(320, 400);
	faceTrackerWidget->show();
	faceTrackerWidget2->resize(320, 400);
	faceTrackerWidget2->show();
	
	int ret = app.exec();

	delete mainWidget;
	mainWidget = NULL;

	delete sensorManager;
	sensorManager = NULL;

	return ret;
}