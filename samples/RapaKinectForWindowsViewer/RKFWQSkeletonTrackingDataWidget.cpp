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
