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