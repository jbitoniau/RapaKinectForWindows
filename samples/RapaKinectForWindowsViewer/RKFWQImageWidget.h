#pragma once

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )
#endif
	#include <QFrame>
	#include <QPainter>
#ifdef _MSC_VER
	#pragma warning(pop)
#endif



#include "RKFWImage.h"
#include "RKFWImageConverter.h"

namespace RKFW
{

class QRGB32ImageMaker;

class QImageWidget: public QFrame
{ 
	Q_OBJECT

public:
	QImageWidget( QWidget* parent );
	virtual ~QImageWidget();

	void				setImage( const RKFW::Image& image );
	void				clearImage();

protected:
	virtual void		paintEvent( QPaintEvent* paintEvent );

private:
	QRGB32ImageMaker*	mQImageMaker;
};


/*
	QRGB32ImageMaker

	A helper object that produces a QImage with the QT RGB32 format 
	from a RKFW::Image and performs any necessary conversion under the hood.	

	Note: with QT's RGB32 format, if you read the QImage buffer *byte after byte* 
	then the color components appear in this order: blue, green, red, 0xFF (no alpha).
	This format is strictly equivalent to the RKFW::ImageFormat::BGRX32.
*/
class QRGB32ImageMaker
{
public:
	QRGB32ImageMaker( int width, int height );
	~QRGB32ImageMaker();
	
	bool				update( const Image& image );
	const QImage&		getQImage() const { return *mQImage; }

private:
	QImage*				mQImage;
	ImageConverter*		mImageConverter;
};

}