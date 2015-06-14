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
	#include <QListWidget>
	#include <QPushButton>
	#include <QLayout>
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include "RKFWSensorManager.h"

namespace RKFW
{

class QSensorManagerWidget : public QFrame,
							 public SensorManager::Listener
{ 
	Q_OBJECT

public:
	QSensorManagerWidget( QWidget* parent, SensorManager* sensorManager );
	virtual ~QSensorManagerWidget();

protected:
	virtual void				onSensorAdded( SensorManager* sensorManager, Sensor* sensor );
	virtual void				onSensorRemoving( SensorManager* sensorManager, Sensor* sensor );
	virtual void				onSensorRemoved( SensorManager* sensorManager, Sensor* sensor );

	void						updateSensorListWidget();

private slots:
	void						onTimerTimeOut();
	void						startStopButtonPressed();

private:
	SensorManager*				mSensorManager;
	QTimer*						mTimer;
	QListWidget*				mSensorListWidget;
	QPushButton*				mStartStopButton;
};

}
