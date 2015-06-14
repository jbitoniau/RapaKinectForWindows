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

