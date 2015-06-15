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
#include "RKFWQSkeletonTrackingDataWidget.h"

#include <assert.h>
#include <sstream>

namespace RKFW
{

QSkeletonTrackingDataWidget::QSkeletonTrackingDataWidget( QWidget* parent, Sensor* device )
	: QFrame(parent),
	  mSensor(device),
	  mSeatedModeCheckBox(NULL),
	  mStartButton(NULL),
	  mStopButton(NULL),
	  mSkeletonTrackingDataTextEdit(NULL)
{
	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout( mainLayout);

	QLabel* topLabel = NULL;
	topLabel = new QLabel( "Skeleton tracking", this );
	topLabel->setSizePolicy( QSizePolicy::Fixed , QSizePolicy::Fixed );
    mainLayout->addWidget( topLabel );

	mSkeletonTrackingDataTextEdit = new QTextEdit( this );
	mainLayout->addWidget( mSkeletonTrackingDataTextEdit );
	updateSkeletonTrackingDataTextEdit();

	QHBoxLayout* bottomBarLayout = new QHBoxLayout();
	mainLayout->addLayout( bottomBarLayout );

	mSeatedModeCheckBox = new QCheckBox( "Seated mode", this );
	mSeatedModeCheckBox->setChecked( true );		
	mSeatedModeCheckBox->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	bottomBarLayout->addWidget( mSeatedModeCheckBox );

	mStartButton = new QPushButton( "Start", this );
	mStartButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	bottomBarLayout->addWidget( mStartButton );
	bool ret = connect( mStartButton, SIGNAL( pressed() ), this, SLOT( startButtonPressed() ) );
	assert(ret);

	mStopButton = new QPushButton( "Stop", this );
	mStopButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	bottomBarLayout->addWidget( mStopButton );
	ret = connect( mStopButton, SIGNAL( pressed() ), this, SLOT( stopButtonPressed() ) );
	assert(ret);

	bottomBarLayout->addStretch();

	mSensor->addListener( this );
}

QSkeletonTrackingDataWidget::~QSkeletonTrackingDataWidget()
{
	mSensor->removeListener( this );
}

void QSkeletonTrackingDataWidget::startButtonPressed()
{
	bool seatedMode = ( mSeatedModeCheckBox->checkState()==Qt::Checked );
	if ( !mSensor->startSkeletonTracking( seatedMode ) )
	{
		QMessageBox msgBox;
		msgBox.setText("Operation failed");
		msgBox.exec();
	}
}

void QSkeletonTrackingDataWidget::stopButtonPressed()
{
	if ( !mSensor->stopSkeletonTracking() )
	{
		QMessageBox msgBox;
		msgBox.setText("Operation failed");
		msgBox.exec();
	}
}

void QSkeletonTrackingDataWidget::onInitialize( Sensor* /*sensor*/ )
{
	updateSkeletonTrackingDataTextEdit();
}

void QSkeletonTrackingDataWidget::onShutdown( Sensor* /*sensor*/ )
{
	updateSkeletonTrackingDataTextEdit();	
}

void QSkeletonTrackingDataWidget::onCapturedSkeletonTrackingData( Sensor* /*sensor*/ )
{
	updateSkeletonTrackingDataTextEdit();	
}

void QSkeletonTrackingDataWidget::updateSkeletonTrackingDataTextEdit()
{
	const SkeletonTrackingData* data = mSensor->getSkeletonTrackingData();
	
	if ( data )
		mSkeletonTrackingDataTextEdit->setText( data->toString("\n").c_str() );
	else
		mSkeletonTrackingDataTextEdit->setText( "No skeleton tracking data" );
}

}
