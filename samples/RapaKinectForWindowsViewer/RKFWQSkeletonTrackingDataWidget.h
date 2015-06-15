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
