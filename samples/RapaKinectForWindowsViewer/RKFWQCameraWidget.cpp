#include "RKFWQCameraWidget.h"

#include <assert.h>
#include <sstream>

namespace RKFW
{

QCameraWidget::QCameraWidget( QWidget* parent, Sensor* device, CameraType cameraType )
	: QFrame(parent),
	  mSensor(device),
	  mCameraType(cameraType),
	  mImageWidget(NULL),
	  mCaptureSettingsCombo(NULL),
	  mDepthNearModeCheckBox(NULL),
	  mIncludePlayerIndexCheckBox(NULL),
	  mStartButton(NULL),
	  mImageNumberLabel(NULL),
	  mColorCameraSettingsWidget(NULL),
	  mBGRXDepthImageWithPlayerIndex(NULL)
{
	QString title = QString::fromUtf16( reinterpret_cast<const ushort*>(mSensor->getConnectionId().c_str()) );
	setWindowTitle( title );

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout( mainLayout);

	QLabel* topLabel = NULL;
	if ( mCameraType==ColorCamera )
		topLabel  = new QLabel( "Color camera", this );
	else if ( mCameraType==DepthCamera )
		topLabel  = new QLabel( "Depth camera", this );
	else
	{
		assert(false);
	}
	topLabel->setSizePolicy (QSizePolicy::Fixed , QSizePolicy::Fixed);
    
	mainLayout->addWidget( topLabel );

	QHBoxLayout* imageLayout = new QHBoxLayout();
	mainLayout->addLayout( imageLayout );
	mImageWidget = new QImageWidget( this );
	mImageWidget->setSizePolicy( QSizePolicy::MinimumExpanding , QSizePolicy::MinimumExpanding );
	mImageWidget->setFrameShape( QFrame::Box);
	imageLayout->addWidget( mImageWidget );

	if ( mCameraType==ColorCamera )
	{
		mColorCameraSettingsWidget = new QColorCameraSettingsWidget( this, mSensor );
		mColorCameraSettingsWidget->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
		imageLayout->addWidget( mColorCameraSettingsWidget );
	}

	QHBoxLayout* bottomBarLayout = new QHBoxLayout();
	mainLayout->addLayout( bottomBarLayout );

	mCaptureSettingsCombo = new QComboBox( this );
	bottomBarLayout->addWidget( mCaptureSettingsCombo );
	
	if ( mCameraType==ColorCamera )
	{
		for ( std::size_t i=0; i<Sensor::ColorCaptureSettingsEnumCount; ++i )
		{
			const CaptureSettings& captureSettings = Sensor::getColorCaptureSettings( static_cast<Sensor::ColorCaptureSettingsEnum>(i) );
			QString text = QString( captureSettings.toString().c_str() );
			mCaptureSettingsCombo->addItem( text );
		}
	}
	else if ( mCameraType==DepthCamera )
	{
		for ( std::size_t i=0; i<Sensor::DepthCaptureSettingsEnumCount; ++i )
		{
			const CaptureSettings& captureSettings = Sensor::getDepthCaptureSettings( static_cast<Sensor::DepthCaptureSettingsEnum>(i) );
			QString text = QString( captureSettings.toString().c_str() );
			mCaptureSettingsCombo->addItem( text );
		}
	}
	else
	{
		assert(false);
	}

	if ( mCameraType==DepthCamera )
	{
		mDepthNearModeCheckBox = new QCheckBox( "Near mode", this );
		mDepthNearModeCheckBox->setChecked( true );
		mDepthNearModeCheckBox->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
		bottomBarLayout->addWidget( mDepthNearModeCheckBox );

		mIncludePlayerIndexCheckBox = new QCheckBox( "Include player index", this );
		mIncludePlayerIndexCheckBox->setChecked( true );
		mIncludePlayerIndexCheckBox->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
		bottomBarLayout->addWidget( mIncludePlayerIndexCheckBox );
	}

	mStartButton = new QPushButton( "Start", this );
	mStartButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	bottomBarLayout->addWidget( mStartButton );
	bool ret = connect( mStartButton, SIGNAL( pressed() ), this, SLOT( startButtonPressed() ) );
	assert(ret);

	mImageNumberLabel = new QLabel( "#:0 t:0", this );
	mImageNumberLabel->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	mImageNumberLabel->setMinimumWidth( 35 );
	bottomBarLayout->addWidget( mImageNumberLabel );

	mSensor->addListener( this );
}

QCameraWidget::~QCameraWidget()
{
	mSensor->removeListener( this );

	delete mBGRXDepthImageWithPlayerIndex;
	mBGRXDepthImageWithPlayerIndex = NULL;			
}

void QCameraWidget::startButtonPressed()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();
	
	int index = mCaptureSettingsCombo->currentIndex();
	if ( mCameraType==ColorCamera )
	{
		Sensor::ColorCaptureSettingsEnum captureSettings = static_cast<Sensor::ColorCaptureSettingsEnum>(index);
		if ( !mSensor->startColorCapture( captureSettings ) )
		{
			QMessageBox msgBox;
			msgBox.setText("Operation failed");
			msgBox.exec();
		}
	}
	else if ( mCameraType==DepthCamera )
	{
		Sensor::DepthCaptureSettingsEnum captureSettings = static_cast<Sensor::DepthCaptureSettingsEnum>(index);
		bool includePlayerIndex = mIncludePlayerIndexCheckBox->checkState() == Qt::Checked;
		bool nearMode = mDepthNearModeCheckBox->checkState() == Qt::Checked;
		if ( !mSensor->startDepthCapture( captureSettings, includePlayerIndex, nearMode ) )
		{
			QMessageBox msgBox;
			msgBox.setText("Operation failed");
			msgBox.exec();
		}
	}
	else
	{
		assert(false);
	}
	QApplication::restoreOverrideCursor();
}

void QCameraWidget::onInitialize( Sensor* /*sensor*/ )
{
	mImageWidget->clearImage();
}

void QCameraWidget::onShutdown( Sensor* /*sensor*/ )
{
	mImageWidget->clearImage();
}

void QCameraWidget::onCapturedColorImage( Sensor* sensor )
{
	assert( sensor==mSensor );
	if ( mCameraType!=ColorCamera )
		return;

	const CapturedImage* capturedImage = sensor->getColorCapturedImage();
	if ( capturedImage )
	{
		mImageWidget->setImage( capturedImage->getImage() );
		std::stringstream stream;
		stream.precision(2);
		stream << std::fixed;
		stream << "#:" << capturedImage->getSequenceNumber() << " ";
		stream << "t:" << capturedImage->getTimestampInSec();
		mImageNumberLabel->setText( stream.str().c_str() );
	}
}

/*
	A hand-made image conversion that interprets the "player index" bits of the Grayscale16 image
	returned by the depth camera of the Kinect sensor.
*/
bool QCameraWidget::convertDepthImageWithPlayerIndexToBGRX32Image( const Image& grayscale16Image, Image& bgrx32Image )
{
	static const unsigned int playerColors[7]=  
		{ 0xFF800000, 0xFF808000, 0xFF008000, 0xFF008080, 0xFF000080, 0xFF800080, 0xFFFFFFFF };

	// Pre-checks
	if ( grayscale16Image.getFormat().getEncoding()!=ImageFormat::Grayscale16 )
		return false;
	if ( bgrx32Image.getFormat().getEncoding()!=ImageFormat::BGRX32 )
		return false;

	unsigned int width = grayscale16Image.getFormat().getWidth();
	unsigned int height = grayscale16Image.getFormat().getHeight();
	if ( bgrx32Image.getFormat().getWidth()!=width || bgrx32Image.getFormat().getHeight()!=height )
		 return false;

	unsigned int sourceNumBytes = grayscale16Image.getBuffer().getSizeInBytes();
	
	const unsigned short int* source = reinterpret_cast<const unsigned short int*>( grayscale16Image.getBuffer().getBytes() );
	unsigned char* dest = bgrx32Image.getBuffer().getBytes();

	// Uncomment this line to read teh depth value at the center of the image
	//unsigned short int centerDepth = source[ (height/2)*width + (width/2) ];

	for ( unsigned int i=0; i<sourceNumBytes/2; ++i )
	{
		unsigned short int gray = *source;
		unsigned short int playerIndex = gray & 0x0007;
		source++;

		unsigned char rescaledGray = static_cast<unsigned char>( gray / 256 ); 
		if ( playerIndex==0 )
		{
			dest[0] = rescaledGray; 
			dest[1] = rescaledGray; 
			dest[2] = rescaledGray; 
			dest[3] = 255; 
		}
		else
		{
			unsigned int* bgrx = reinterpret_cast<unsigned int*>( dest );
			*bgrx = playerColors[playerIndex-1];
		}
		dest+=4;
	}
	return true;
}

void QCameraWidget::onCapturedDepthImage( Sensor* sensor )
{
	assert( sensor==mSensor );
	if ( mCameraType!=DepthCamera )
		return;

	const CapturedImage* capturedImage = sensor->getDepthCapturedImage();
	if ( capturedImage )
	{
		const Image& depthImage = capturedImage->getImage();
		const ImageFormat& depthImageFormat = depthImage.getFormat();
		if ( !mBGRXDepthImageWithPlayerIndex || 
			 mBGRXDepthImageWithPlayerIndex->getFormat().getWidth()!=depthImageFormat.getWidth() || 
			 mBGRXDepthImageWithPlayerIndex->getFormat().getHeight()!=depthImageFormat.getHeight() )
		{
			delete mBGRXDepthImageWithPlayerIndex;
			mBGRXDepthImageWithPlayerIndex = new Image( ImageFormat(depthImageFormat.getWidth(), depthImageFormat.getHeight(), ImageFormat::BGRX32 ) );
		}
		convertDepthImageWithPlayerIndexToBGRX32Image( depthImage, *mBGRXDepthImageWithPlayerIndex );
		mImageWidget->setImage( *mBGRXDepthImageWithPlayerIndex );

		std::stringstream stream;
		stream.precision(2);
		stream << std::fixed;
		stream << "#:" << capturedImage->getSequenceNumber() << " ";
		stream << "t:" << capturedImage->getTimestampInSec();
		mImageNumberLabel->setText( stream.str().c_str() );
	}
}

}
