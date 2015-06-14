#include "RKFWQColorCameraSettingsWidget.h"

#include <assert.h>
#include <sstream>

namespace RKFW
{

QColorCameraSettingsWidget::QColorCameraSettingsWidget( QWidget* parent, Sensor* device )
	: QFrame(parent),
	  mSensor(device),
	  mDontProcessSignals(false),
	  mResetButton(NULL),
	  mAutoExposureCheckBox(NULL),
	  mBacklightComboBox(NULL),
	  mBrightnessSpinBox(NULL),
	  mExposureTimeSpinBox(NULL),
	  mFrameIntervalSpinBox(NULL),
	  mGainSpinBox(NULL),
	  mPowerLineComboBox(NULL),
	  mContrastSpinBox(NULL),
	  mGammaSpinBox(NULL),
 	  mHueSpinBox(NULL),
	  mSaturationSpinBox(NULL),
	  mSharpnessSpinBox(NULL),
	  mAutoWhiteCheckBox(NULL),
	  mWhiteBalanceSpinBox(NULL)
{
	QHBoxLayout* subLayout = NULL;
	bool ret = false;
	const double singleStep = 0.01;

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout( mainLayout );

	mResetButton = new QPushButton("Reset to default", this );
	ret = connect( mResetButton, SIGNAL( pressed() ), this, SLOT( resetButtonPressed() ) );
	assert(ret);
	mainLayout->addWidget( mResetButton );
	
	mAutoExposureCheckBox = new QCheckBox( "Auto exposure", this );
	mAutoExposureCheckBox->setSizePolicy( QSizePolicy::Fixed , QSizePolicy::Fixed );
	ret = connect( mAutoExposureCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( checkBoxStateChanged(int) ) );
	assert(ret);
	mainLayout->addWidget( mAutoExposureCheckBox );
	
	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Backlight compensation mode", this ) );
	mBacklightComboBox = new QComboBox(this);
	mBacklightComboBox->addItem( "Average brightness" );
	mBacklightComboBox->addItem( "Center priority" );
	mBacklightComboBox->addItem( "Low lights priority" );
	mBacklightComboBox->addItem( "Center only" );
	subLayout->addWidget( mBacklightComboBox );
	subLayout->addStretch();

	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Brightness", this ) );
	mBrightnessSpinBox = new QDoubleSpinBox( this );
	ret = connect( mBrightnessSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mBrightnessSpinBox->setSingleStep( singleStep );
	subLayout->addWidget( mBrightnessSpinBox );
	subLayout->addStretch();
	
	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Exposure time", this ) );
	mExposureTimeSpinBox = new QDoubleSpinBox( this );
	ret = connect( mExposureTimeSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mExposureTimeSpinBox->setSingleStep( 100 );
	subLayout->addWidget( mExposureTimeSpinBox );
	subLayout->addStretch();
	
	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Frame interval", this ) );
	mFrameIntervalSpinBox = new QDoubleSpinBox( this );
	ret = connect( mFrameIntervalSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mFrameIntervalSpinBox->setSingleStep( 100 );
	subLayout->addWidget( mFrameIntervalSpinBox );
	subLayout->addStretch();

	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Gain", this ) );
	mGainSpinBox = new QDoubleSpinBox( this );
	ret = connect( mGainSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mGainSpinBox->setSingleStep( singleStep );
	subLayout->addWidget( mGainSpinBox );
	subLayout->addStretch();

	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Power line frequency", this ) );
	mPowerLineComboBox = new QComboBox(this);
	mPowerLineComboBox->addItem( "Disabled" );
	mPowerLineComboBox->addItem( "50 Hz" );
	mPowerLineComboBox->addItem( "60 Hz" );
	subLayout->addWidget( mPowerLineComboBox );
	subLayout->addStretch();

	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Contrast", this ) );
	mContrastSpinBox = new QDoubleSpinBox( this );
	ret = connect( mContrastSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mContrastSpinBox->setSingleStep( singleStep );
	subLayout->addWidget( mContrastSpinBox );
	subLayout->addStretch();

	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Gamma", this ) );
	mGammaSpinBox = new QDoubleSpinBox( this );
	ret = connect( mGammaSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mGammaSpinBox->setSingleStep( singleStep );
	subLayout->addWidget( mGammaSpinBox );
	subLayout->addStretch();

	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Hue", this ) );
	mHueSpinBox = new QDoubleSpinBox( this );
	ret = connect( mHueSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mHueSpinBox->setSingleStep( 1 );
	subLayout->addWidget( mHueSpinBox );
	subLayout->addStretch();

	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Saturation", this ) );
	mSaturationSpinBox = new QDoubleSpinBox( this );
	ret = connect( mSaturationSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mSaturationSpinBox->setSingleStep( singleStep );
	subLayout->addWidget( mSaturationSpinBox );
	subLayout->addStretch();

	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "Sharpness", this ) );
	mSharpnessSpinBox = new QDoubleSpinBox( this );
	ret = connect( mSharpnessSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( doubleSpinBoxValueChanged(double) ) );
	assert(ret);
	mSharpnessSpinBox->setSingleStep( singleStep );
	subLayout->addWidget( mSharpnessSpinBox );
	subLayout->addStretch();

	mAutoWhiteCheckBox = new QCheckBox( "Auto white balance", this );
	mAutoWhiteCheckBox->setSizePolicy( QSizePolicy::Fixed , QSizePolicy::Fixed );
	ret = connect( mAutoWhiteCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( checkBoxStateChanged(int) ) );
	assert(ret);
	mainLayout->addWidget( mAutoWhiteCheckBox );
	
	subLayout = new QHBoxLayout();
	mainLayout->addLayout( subLayout );
	subLayout->addWidget( new QLabel( "White balance", this ) );
	mWhiteBalanceSpinBox = new QSpinBox( this );
	ret = connect( mWhiteBalanceSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( spinBoxValueChanged(int) ) );
	assert(ret);
	mWhiteBalanceSpinBox->setSingleStep( 100 );
	subLayout->addWidget( mWhiteBalanceSpinBox );
	subLayout->addStretch();
 
	updateWidgets();
	mSensor->addListener( this );
}

QColorCameraSettingsWidget::~QColorCameraSettingsWidget()
{
	mSensor->removeListener( this );
}

void QColorCameraSettingsWidget::onColorCameraSettingsChanged( Sensor* /*sensor*/ )
{
	updateWidgets();
}

void QColorCameraSettingsWidget::resetButtonPressed()
{
	if ( !mSensor->resetColorCameraSettings() )
	{
		QMessageBox msgBox;
		msgBox.setText("Operation failed");
		msgBox.exec();
	}
}

void QColorCameraSettingsWidget::updateWidgets()
{
	// We want to update the content of the widgets but without them 
	// generate any events when we proceed
	mDontProcessSignals = true;

	const ColorCameraSettings& settings = mSensor->getColorCameraSettings();
	
	mAutoExposureCheckBox->setChecked( settings.mAutoExposure );
	mBacklightComboBox->setCurrentIndex( settings.mBacklightCompensationMode );
	
	mBrightnessSpinBox->setMinimum( settings.mMinBrightness );		
	mBrightnessSpinBox->setMaximum( settings.mMaxBrightness );
	mBrightnessSpinBox->setValue( settings.mBrightness );

	mExposureTimeSpinBox->setMinimum( settings.mMinExposureTime );		
	mExposureTimeSpinBox->setMaximum( settings.mMaxExposureTime );
	mExposureTimeSpinBox->setValue( settings.mExposureTime );

	mFrameIntervalSpinBox->setMinimum( settings.mMinFrameInterval );		
	mFrameIntervalSpinBox->setMaximum( settings.mMaxFrameInterval );
	mFrameIntervalSpinBox->setValue( settings.mFrameInterval );

	mGainSpinBox->setMinimum( settings.mMinGain );		
	mGainSpinBox->setMaximum( settings.mMaxGain );
	mGainSpinBox->setValue( settings.mGain );

	mPowerLineComboBox->setCurrentIndex( settings.mPowerLineFrequency );

	mContrastSpinBox->setMinimum( settings.mMinContrast );		
	mContrastSpinBox->setMaximum( settings.mMaxContrast );
	mContrastSpinBox->setValue( settings.mContrast );

	mGammaSpinBox->setMinimum( settings.mMinGamma );		
	mGammaSpinBox->setMaximum( settings.mMaxGamma );
	mGammaSpinBox->setValue( settings.mGamma );

	mHueSpinBox->setMinimum( settings.mMinHue );		
	mHueSpinBox->setMaximum( settings.mMaxHue );
	mHueSpinBox->setValue( settings.mHue  );

	mSaturationSpinBox->setMinimum( settings.mMinSaturation );		
	mSaturationSpinBox->setMaximum( settings.mMaxSaturation );
	mSaturationSpinBox->setValue( settings.mSaturation );

	mSharpnessSpinBox->setMinimum( settings.mMinSharpness );		
	mSharpnessSpinBox->setMaximum( settings.mMaxSharpness );
	mSharpnessSpinBox->setValue( settings.mSharpness );

	mAutoWhiteCheckBox->setChecked( settings.mAutoWhiteBalance );

	mWhiteBalanceSpinBox->setMinimum( settings.mMinWhiteBalance );		
	mWhiteBalanceSpinBox->setMaximum( settings.mMaxWhiteBalance );
	mWhiteBalanceSpinBox->setValue( settings.mWhiteBalance );

	// Auto-exposure drives the following paramaters
	mBacklightComboBox->setEnabled( settings.mAutoExposure );
	mBrightnessSpinBox->setEnabled( settings.mAutoExposure );
	mExposureTimeSpinBox->setEnabled( !settings.mAutoExposure );
	mFrameIntervalSpinBox->setEnabled( !settings.mAutoExposure );
	mGainSpinBox->setEnabled( !settings.mAutoExposure );
	mPowerLineComboBox->setEnabled( !settings.mAutoExposure );

	// Auto-white balance drives the following:
	mWhiteBalanceSpinBox->setEnabled( !settings.mAutoWhiteBalance );
	
	mDontProcessSignals = false;
}

void QColorCameraSettingsWidget::checkBoxStateChanged( int state )
{
	if ( mDontProcessSignals )
		return;

	ColorCameraSettings settings = mSensor->getColorCameraSettings();

	bool value = (state != Qt::Unchecked);
	if ( sender()==mAutoExposureCheckBox )
		settings.mAutoExposure = value;
	else if ( sender()==mAutoWhiteCheckBox )
		settings.mAutoWhiteBalance = value;

	bool ret = mSensor->setColorCameraSettings( settings );
	if ( !ret )
	{
		QMessageBox msgBox;
		msgBox.setText("Operation failed");
		msgBox.exec();
	}

	// The checkboxes can have an effect on the enabled/disabled states of the widgets
	updateWidgets();
}

void QColorCameraSettingsWidget::doubleSpinBoxValueChanged( double value )
{
	if ( mDontProcessSignals )
		return;

	ColorCameraSettings settings = mSensor->getColorCameraSettings();

	if ( sender()==mBrightnessSpinBox )
		settings.mBrightness = value;
	else if ( sender()==mExposureTimeSpinBox )
		settings.mExposureTime = value;
	else if ( sender()==mFrameIntervalSpinBox )
		settings.mFrameInterval = value;
	else if ( sender()==mGainSpinBox )
		settings.mGain = value;

	else if ( sender()==mContrastSpinBox )
		settings.mContrast = value;
	else if ( sender()==mGammaSpinBox )
		settings.mGamma = value;
	else if ( sender()==mHueSpinBox )
		settings.mHue = value;
	else if ( sender()==mSaturationSpinBox )
		settings.mSaturation = value;
	else if ( sender()==mSharpnessSpinBox )
		settings.mSharpness = value;
	else if ( sender()==mExposureTimeSpinBox )
		settings.mBrightness = value;

	bool ret = mSensor->setColorCameraSettings( settings );
	if ( !ret )
	{
		QMessageBox msgBox;
		msgBox.setText("Operation failed");
		msgBox.exec();
	}
}

void QColorCameraSettingsWidget::spinBoxValueChanged( int value )
{
	if ( mDontProcessSignals )
		return;

	ColorCameraSettings settings = mSensor->getColorCameraSettings();

	if ( sender()==mWhiteBalanceSpinBox )
		settings.mWhiteBalance = value;
	
	bool ret = mSensor->setColorCameraSettings( settings );
	if ( !ret )
	{
		QMessageBox msgBox;
		msgBox.setText("Operation failed");
		msgBox.exec();
	}
}

}
