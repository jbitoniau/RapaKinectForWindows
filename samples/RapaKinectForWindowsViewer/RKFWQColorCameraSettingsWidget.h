#pragma once

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )
#endif
	#include <QFrame>
	#include <QPushButton>
	#include <QCheckbox>
	#include <QDoubleSpinBox>
	#include <QComboBox>
	#include <QSpinBox>
	#include <QLayout>
	#include <QMessageBox>
	#include <QLabel>
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include "RKFWSensor.h"

namespace RKFW
{

class QColorCameraSettingsWidget :	public QFrame,
									public Sensor::Listener
{ 
	Q_OBJECT

public:
	QColorCameraSettingsWidget( QWidget* parent, Sensor* sensor );
	virtual ~QColorCameraSettingsWidget();

	Sensor*					getSensor() const { return mSensor; }

protected:
	virtual void			onColorCameraSettingsChanged( Sensor* sensor );

private slots:
	void					resetButtonPressed();
	void					checkBoxStateChanged( int value );
	void					doubleSpinBoxValueChanged( double value );
	void					spinBoxValueChanged( int value );
	
private:
	void					updateWidgets();
	
	Sensor*					mSensor;

	bool					mDontProcessSignals;
	
	QPushButton*			mResetButton;
	QCheckBox*				mAutoExposureCheckBox;
	QComboBox*				mBacklightComboBox;
	QDoubleSpinBox*			mBrightnessSpinBox;
	QDoubleSpinBox*			mExposureTimeSpinBox;
	QDoubleSpinBox*			mFrameIntervalSpinBox;
	QDoubleSpinBox*			mGainSpinBox;
	
	QComboBox*				mPowerLineComboBox;
	QDoubleSpinBox*			mContrastSpinBox;
	QDoubleSpinBox*			mGammaSpinBox;
	QDoubleSpinBox*			mHueSpinBox;
	QDoubleSpinBox*			mSaturationSpinBox;
	QDoubleSpinBox*			mSharpnessSpinBox;
	
	QCheckBox*				mAutoWhiteCheckBox;
	QSpinBox*				mWhiteBalanceSpinBox;
};

}

