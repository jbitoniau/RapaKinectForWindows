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
#include "RKFWQSensorWidget.h"

#include <assert.h>
#include <sstream>

namespace RKFW
{

QSensorWidget::QSensorWidget( QWidget* parent, Sensor* device )
	: QFrame(parent),
	  mSensor(device),
	  mIdLabel(NULL),
	  mStatusLabel(NULL),
	  mAngleLabel(NULL),
	  mAngleSpinBox(NULL),
	  mAccelerationLabel(NULL),
	  mColorCameraCheckBox(NULL),
	  mDepthCameraCheckBox(NULL),
	  mSkeletonTrackingCheckBox(NULL),
	  mDepthRadioButton(NULL),
	  mDepthAndPlayerIndexRadioButton(NULL),
  	  mInitializeButton(NULL),
  	  mShutdownButton(NULL),	
	  mColorCameraWidget(NULL),
	  mDepthCameraWidget(NULL)
{
	QString connectionId = QString::fromUtf16( reinterpret_cast<const ushort*>(mSensor->getConnectionId().c_str()) );
	setWindowTitle( connectionId );

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout( mainLayout);

	mIdLabel = new QLabel( QString("Connection id: %0").arg( connectionId ), this );
	mainLayout->addWidget( mIdLabel );
	mIdLabel->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	
	mStatusLabel = new QLabel("Status: ", this );
	mainLayout->addWidget( mStatusLabel );
	mStatusLabel->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	
	QHBoxLayout* angleLayout = new QHBoxLayout();
	angleLayout->addWidget( new QLabel( "Elevation angle (in deg)", this ) );
	
	mAngleSpinBox = new QSpinBox( this );
	mAngleSpinBox->setMinimum( Sensor::getMinElevationAngleInDeg() );
	mAngleSpinBox->setMaximum( Sensor::getMaxElevationAngleInDeg() );
	mAngleSpinBox->setKeyboardTracking(false);
	bool ret = connect( mAngleSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( angleValueChanged(int) ) );
	assert(ret);
	angleLayout->addWidget( mAngleSpinBox );
	
	mAngleLabel = new QLabel("current value: ", this);
	angleLayout->addWidget( mAngleLabel );
	angleLayout->addStretch();
	
	mainLayout->addLayout( angleLayout );
	
	mAccelerationLabel = new QLabel("Acceleration (in g's): ", this);
	mAccelerationLabel->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	mainLayout->addWidget( mAccelerationLabel );
	
	// Initialize/shutdown
	QHBoxLayout* initFlagsLayout = new QHBoxLayout();
	
	mInitializeButton = new QPushButton( "Initialize", this );
	mInitializeButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	initFlagsLayout->addWidget( mInitializeButton );
	ret = connect( mInitializeButton, SIGNAL( pressed() ), this, SLOT( initializeSensor() ) );
	assert(ret);

	mShutdownButton = new QPushButton( "Shutdown", this );
	mShutdownButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	initFlagsLayout->addWidget( mShutdownButton );
	ret = connect( mShutdownButton, SIGNAL( pressed() ), this, SLOT( shutdownSensor() ) );
	assert(ret);

	mColorCameraCheckBox = new QCheckBox( "Color camera", this );
	mColorCameraCheckBox->setChecked( true );
	initFlagsLayout->addWidget( mColorCameraCheckBox );

	mDepthCameraCheckBox = new QCheckBox( "Depth camera", this );
	mDepthCameraCheckBox->setChecked( true );
	initFlagsLayout->addWidget( mDepthCameraCheckBox );

	mSkeletonTrackingCheckBox = new QCheckBox( "Skeleton tracking", this );
	mSkeletonTrackingCheckBox->setChecked( true ); 
	initFlagsLayout->addWidget( mSkeletonTrackingCheckBox );
	
	initFlagsLayout->addStretch();

	mainLayout->addLayout( initFlagsLayout );

	mColorCameraWidget = new QCameraWidget( this, mSensor, QCameraWidget::ColorCamera );
	mColorCameraWidget->setFrameShape( QFrame::Box );
	mColorCameraWidget->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
	mainLayout->addWidget( mColorCameraWidget, 4 );

	mDepthCameraWidget = new QCameraWidget( this, mSensor, QCameraWidget::DepthCamera );
	mDepthCameraWidget->setFrameShape( QFrame::Box );
	mDepthCameraWidget->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
	mainLayout->addWidget( mDepthCameraWidget, 4 );
	
	mSkeletonTrackingDataWidget = new QSkeletonTrackingDataWidget( this, mSensor );
	mSkeletonTrackingDataWidget->setFrameShape( QFrame::Box );
	mainLayout->addWidget( mSkeletonTrackingDataWidget, 1 );

	mTimer = new QTimer(this);
	ret = connect( mTimer, SIGNAL( timeout() ), this, SLOT( onTimerTimeOut() ) );
	assert( ret );
	mTimer->setInterval(500);
	mTimer->start();
}

QSensorWidget::~QSensorWidget()
{	
}

void QSensorWidget::onTimerTimeOut()
{
	const char* statusName = mSensor->getStatusName();
	mStatusLabel->setText( QString("Status: %1").arg(statusName) );

	mAngleLabel->setText( QString("current: %0").arg( mSensor->getElevationAngleInDeg() ) );

	Vector3f acc = mSensor->getAccelerationInGs();
	std::stringstream stream;
	stream.precision(3);
	stream << std::fixed;
	stream << "Acceleration (in g's): " << acc.x() << " " << acc.y() << " " << acc.z();
	mAccelerationLabel->setText( stream.str().c_str() );
}

void QSensorWidget::initializeSensor()
{
	unsigned int flagsAsInt = 0;
	if ( mColorCameraCheckBox->checkState()==Qt::Checked )
		flagsAsInt |= Sensor::Subsystem_ColorCamera;
	if ( mDepthCameraCheckBox->checkState()==Qt::Checked )
		flagsAsInt |= Sensor::Subsystem_DepthCamera;
	if ( mSkeletonTrackingCheckBox->checkState()==Qt::Checked )
		flagsAsInt |= Sensor::Subsystem_SkeletonTracking;
	Sensor::SubsystemEnum flags = static_cast<Sensor::SubsystemEnum>(flagsAsInt);
	if ( !mSensor->initialize( flags ) )
	{
		QMessageBox msgBox;
		msgBox.setText("Operation failed");
		msgBox.exec();
	}
}	
	
void QSensorWidget::shutdownSensor()
{
	mSensor->shutdown();	
}

void QSensorWidget::angleValueChanged( int value )
{
	mSensor->setElevationAngleInDeg(value);
}

}
