#pragma once

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )
#endif
	#include <QFrame>
	#include <QTextEdit>
	#include <QSpinBox>
	#include <QTextEdit>
	#include <QPushButton>
	#include <QLayout>
	#include <QMessageBox>
	#include <QLabel>
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include "RKFWSensorManager.h"
#include "RKFWFaceTracker.h"

namespace RKFW
{

class QFaceTrackerTestWidget:	public QFrame,
								public Sensor::Listener
{ 
	Q_OBJECT

public:
	QFaceTrackerTestWidget( QWidget* parent, SensorManager* sensorManager );
	virtual ~QFaceTrackerTestWidget();

private slots:
	void					startStopButtonPressed();

protected:
	virtual void			onCapturedDepthImage( Sensor* sensor );

private:
	SensorManager*			mSensorManager;
	QSpinBox*				mSensorIndexSpinBox;
	QPushButton*			mStartStopButton;
	QTextEdit*				mFaceTrackerResultTextEdit;

	Sensor*					mSensor;
	FaceTracker*			mFaceTracker;
};

}