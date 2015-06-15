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
#include "RKFWQSensorManagerWidget.h"

#include <assert.h>

#include "RKFWQSensorWidget.h"

namespace RKFW
{

/*
	Notes:
	- To be more reusable this widget should not be in charge of updating the SensorManager 
*/
QSensorManagerWidget::QSensorManagerWidget( QWidget* parent, RKFW::SensorManager* sensorManager )
	:	QFrame(parent), 
		mSensorManager(sensorManager),
		mSensorListWidget(NULL),
		mStartStopButton(NULL)
{
	setWindowTitle( "Kinect Sensor Manager" );

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout( mainLayout);

	mSensorListWidget = new QListWidget( this );
	mainLayout->addWidget( mSensorListWidget );

	mTimer = new QTimer(this);
	bool ret = connect( mTimer, SIGNAL( timeout() ), this, SLOT( onTimerTimeOut() ) );
	assert( ret );
	mTimer->setInterval(0);
	mTimer->start();

	mSensorManager->addListener(this);
}

QSensorManagerWidget::~QSensorManagerWidget()
{
	mSensorManager->removeListener(this);
}

void QSensorManagerWidget::onTimerTimeOut()
{
	mSensorManager->update();
}

void QSensorManagerWidget::startStopButtonPressed()
{
}

void QSensorManagerWidget::onSensorAdded( SensorManager* /*sensorManager*/, Sensor* sensor )
{
	QSensorWidget* sensorWidget = new QSensorWidget( this, sensor );
	sensorWidget->setWindowFlags( Qt::Window );
	sensorWidget->show();
	sensorWidget->resize( 960, 960 );

	updateSensorListWidget();
}

void QSensorManagerWidget::onSensorRemoving( SensorManager* /*sensorManager*/, Sensor* sensor )
{
	QList<QSensorWidget*> sensorWidgets = findChildren<QSensorWidget*>();
	for ( int i=0; i<sensorWidgets.size(); ++i )  
	{
		QSensorWidget* sensorWidget = sensorWidgets.at(i);
		if ( sensorWidget->getSensor()==sensor )
		{
			delete sensorWidget;
			break;
		}
	}
}

void QSensorManagerWidget::onSensorRemoved( SensorManager* /*sensorManager*/, Sensor* /*sensor*/ )
{
	updateSensorListWidget();
}

void QSensorManagerWidget::updateSensorListWidget()
{
	mSensorListWidget->clear();

	RKFW::Sensors sensors = mSensorManager->getSensors();
	for ( std::size_t i=0; i<sensors.size(); ++i )
	{
		std::wstring connectionId = sensors[i]->getConnectionId();
		QString id = QString::fromUtf16( reinterpret_cast<const ushort*>(connectionId.c_str()) );
		mSensorListWidget->addItem( id );
	}
}

}

