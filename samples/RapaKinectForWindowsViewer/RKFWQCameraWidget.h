#pragma once


#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )
#endif
	#include <QLayout>
	#include <QApplication>
	#include <QMessageBox>
	#include <QFrame>
	#include <QPushButton>
	#include <QComboBox>
	#include <QLabel>
	#include <QCheckbox>
	#include <QRadioButton>
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include "RKFWSensor.h"
#include "RKFWQImageWidget.h"
#include "RKFWQColorCameraSettingsWidget.h"

namespace RKFW
{

class QCameraWidget :	public QFrame,
						public Sensor::Listener
{ 
	Q_OBJECT

public:
	enum CameraType
	{
		ColorCamera,
		DepthCamera
	};

	QCameraWidget( QWidget* parent, Sensor* sensor, CameraType cameraType );
	virtual ~QCameraWidget();

	Sensor*					getSensor() const { return mSensor; }

protected:
	virtual void			onInitialize( Sensor* sensor );
	virtual void			onShutdown( Sensor* sensor );
	virtual void			onCapturedColorImage( Sensor* sensor );
	virtual	void			onCapturedDepthImage( Sensor* sensor );
	
private slots:
	void					startButtonPressed();

private:
	static bool				convertDepthImageWithPlayerIndexToBGRX32Image( const Image& grayscale16Image, Image& bgrx32Image );

	Sensor*					mSensor;
	CameraType				mCameraType;
	
	QImageWidget*			mImageWidget;
	QComboBox*				mCaptureSettingsCombo;
	QCheckBox*				mDepthNearModeCheckBox;
	QCheckBox*				mIncludePlayerIndexCheckBox;
	QPushButton*			mStartButton;
	QLabel*					mImageNumberLabel;

	QColorCameraSettingsWidget* mColorCameraSettingsWidget;			// Only for Color Camera
	Image*						mBGRXDepthImageWithPlayerIndex;		// Only for DepthCamera
};

}
