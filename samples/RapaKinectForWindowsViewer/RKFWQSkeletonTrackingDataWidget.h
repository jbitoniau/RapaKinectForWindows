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
	#include <QTextEdit>
	#include <QCheckbox>
	#include <QLayout>
	#include <QApplication>
	#include <QMessageBox>
	#include <QLabel>
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include "RKFWSensor.h"

namespace RKFW
{

class QSkeletonTrackingDataWidget :	public QFrame,
									public Sensor::Listener
{ 
	Q_OBJECT

public:
	QSkeletonTrackingDataWidget( QWidget* parent, Sensor* sensor );
	virtual ~QSkeletonTrackingDataWidget();

	Sensor*				getSensor() const { return mSensor; }

protected:
	virtual void		onInitialize( Sensor* sensor );
	virtual void		onShutdown( Sensor* sensor );
	virtual	void		onCapturedSkeletonTrackingData( Sensor* sensor );

	void				updateSkeletonTrackingDataTextEdit();

private slots:
	void				startButtonPressed();
	void				stopButtonPressed();

private:
	Sensor*				mSensor;
	
	QCheckBox*			mSeatedModeCheckBox;
	QPushButton*		mStartButton;
	QPushButton*		mStopButton;
	QTextEdit*			mSkeletonTrackingDataTextEdit;
};

}
