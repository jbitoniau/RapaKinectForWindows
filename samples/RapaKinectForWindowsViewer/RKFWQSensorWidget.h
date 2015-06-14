#pragma once

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )
#endif
	#include <QTimer>
	#include <QFrame>
	#include <QSpinBox>
	#include <QPushButton>
	#include <QLabel>
	#include <QCheckbox>
	#include <QRadioButton>
	#include <QLayout>
	#include <QApplication>
	#include <QMessageBox>	
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include "RKFWQCameraWidget.h"
#include "RKFWQSkeletonTrackingDataWidget.h"
#include "RKFWSensor.h"

namespace RKFW
{

class QSensorWidget :	public QFrame
{ 
	Q_OBJECT

public:
	QSensorWidget( QWidget* parent, Sensor* sensor );
	virtual ~QSensorWidget();

	Sensor*						getSensor() const { return mSensor; }

private slots:
	void						onTimerTimeOut();
	void						initializeSensor();
	void						shutdownSensor();
	void						angleValueChanged( int value );

private:
	Sensor*						mSensor;

	QTimer*						mTimer;
	
	QLabel*						mIdLabel;
	QLabel*						mStatusLabel;
	QLabel*						mAngleLabel;
	QSpinBox*					mAngleSpinBox;
	QLabel*						mAccelerationLabel;
	QCheckBox*					mColorCameraCheckBox;
	QCheckBox*					mDepthCameraCheckBox;
	QCheckBox*					mSkeletonTrackingCheckBox;
	QRadioButton*				mDepthRadioButton;
	QRadioButton*				mDepthAndPlayerIndexRadioButton;
	QPushButton*				mInitializeButton;
	QPushButton*				mShutdownButton;
	
	QCameraWidget*					mColorCameraWidget;
	QCameraWidget*					mDepthCameraWidget;
	QSkeletonTrackingDataWidget*	mSkeletonTrackingDataWidget;
};

}
