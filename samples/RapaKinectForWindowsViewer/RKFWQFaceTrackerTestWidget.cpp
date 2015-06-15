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
#include "RKFWQFaceTrackerTestWidget.h"

#include <assert.h>

namespace RKFW
{

QFaceTrackerTestWidget::QFaceTrackerTestWidget( QWidget* parent, SensorManager* sensorManager )
	: QFrame(parent),
	  mSensorManager(sensorManager),
	  mSensorIndexSpinBox(NULL),
	  mStartStopButton(NULL),
	  mFaceTrackerResultTextEdit(NULL),
	  mSensor(NULL),
	  mFaceTracker(NULL)
{
	setWindowTitle( "Test Face Tracker");

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout( mainLayout);

	QHBoxLayout* sublayout = new QHBoxLayout();
	mainLayout->addLayout( sublayout );
	sublayout->addWidget( new QLabel("Kinect sensor index", this ) );
	mSensorIndexSpinBox = new QSpinBox(this);
	mSensorIndexSpinBox->setMinimum(0);
	mSensorIndexSpinBox->setMaximum(5);
	sublayout->addWidget( mSensorIndexSpinBox );
	
	mStartStopButton = new QPushButton( "Start", this );
	mStartStopButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	mainLayout->addWidget( mStartStopButton );
	bool ret = connect( mStartStopButton, SIGNAL( pressed() ), this, SLOT( startStopButtonPressed() ) );
	assert(ret);

	mFaceTrackerResultTextEdit = new QTextEdit(this);
	mainLayout->addWidget( mFaceTrackerResultTextEdit );

	mFaceTracker = new FaceTracker();
}

QFaceTrackerTestWidget::~QFaceTrackerTestWidget()
{
	delete mFaceTracker;
	mFaceTracker = NULL;
}

void QFaceTrackerTestWidget::startStopButtonPressed()
{
	if ( mFaceTracker->isStarted() )
	{
		mSensor->removeListener(this);
		mSensor = NULL;

		mFaceTracker->stop();
		mStartStopButton->setText( "Start" );
	}
	else
	{
		int sensorIndex = mSensorIndexSpinBox->value();
		if ( sensorIndex<static_cast<int>(mSensorManager->getSensors().size()) )
		{
			Sensor* sensor = mSensorManager->getSensors()[sensorIndex];

			Sensor::ColorCaptureSettingsEnum colorSettingsEnum = sensor->getStartedColorCaptureSettings();
			Sensor::DepthCaptureSettingsEnum depthSettingsEnum = sensor->getStartedDepthCaptureSettings();

			if ( colorSettingsEnum!=Sensor::ColorCaptureSettingsEnumCount && 
				 depthSettingsEnum!=Sensor::DepthCaptureSettingsEnumCount )
			{
				CaptureSettings colorSettings = Sensor::getColorCaptureSettings(colorSettingsEnum);
				CaptureSettings depthSettings = Sensor::getDepthCaptureSettings(depthSettingsEnum);
				if ( mFaceTracker->start( colorSettings.getImageFormat(), colorSettings.getFocalDistanceInPixels(),
										  depthSettings.getImageFormat(), depthSettings.getFocalDistanceInPixels() ) )
				{
					mSensor = sensor;
					mSensor->addListener( this );

					mStartStopButton->setText( "Stop" );
				}
				else
				{
					QMessageBox msgBox;
					msgBox.setText("Failed to start FaceTracker");
					msgBox.exec();
				}
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText("Color and depth camera not started");
				msgBox.exec();
			}
		}
		else
		{	
			QMessageBox msgBox;
			msgBox.setText("No Kinect sensor at this index");
			msgBox.exec();
		}
	}
}

void QFaceTrackerTestWidget::onCapturedDepthImage( Sensor* sensor )
{
	assert( mSensor==sensor );
	assert( mFaceTracker->isStarted() );
	assert( sensor->getColorCapturedImage() );
	assert( sensor->getDepthCapturedImage() );
	
	const Image& colorImage = sensor->getColorCapturedImage()->getImage();
	const Image& depthImage = sensor->getDepthCapturedImage()->getImage();
	mFaceTracker->update( colorImage, depthImage );
	
	std::string text = mFaceTracker->getResult().toString( "\n" );
	mFaceTrackerResultTextEdit->setText( text.c_str() );
}


}