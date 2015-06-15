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
