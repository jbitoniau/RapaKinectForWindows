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
#include "RKFWQImageWidget.h"

#include <assert.h>

namespace RKFW
{

/*
	QImageWidget
*/
QImageWidget::QImageWidget( QWidget* parent )
	: QFrame(parent),
	  mQImageMaker(NULL)
{
}

QImageWidget::~QImageWidget()
{
	delete mQImageMaker;
	mQImageMaker = NULL;
}

void QImageWidget::paintEvent( QPaintEvent* /*paintEvent*/ )
{
	QPainter painter(this);
	
	QColor backgroundColor( 180, 180, 180 );
	painter.setBrush(backgroundColor);
	painter.drawRect( 0, 0, width()-1, height()-1 );

	if ( mQImageMaker )
		painter.drawImage( QPointF(0,0), mQImageMaker->getQImage() );
}
	
void QImageWidget::setImage( const RKFW::Image& image )
{
	unsigned int width = image.getFormat().getWidth();
	unsigned int height = image.getFormat().getHeight();

	if ( !mQImageMaker || mQImageMaker->getQImage().width()!=static_cast<int>(width) || mQImageMaker->getQImage().height()!=static_cast<int>(height) )
	{
		delete mQImageMaker;
		mQImageMaker = new QRGB32ImageMaker( width, height );
	}
	mQImageMaker->update( image );

	update();
}

void QImageWidget::clearImage()
{
	delete mQImageMaker;
	mQImageMaker = NULL;

	update();
}

/*
	QRGB32ImageMaker
*/
QRGB32ImageMaker::QRGB32ImageMaker( int width, int height )
	: mQImage(NULL),
	  mImageConverter(NULL)
{
	ImageFormat outFormat( width, height, ImageFormat::BGRX32 );
	mImageConverter = new ImageConverter( outFormat );
	
	// Get a grip onto the data of the image that serves as output of the ImageConverter
	uchar* data = reinterpret_cast<uchar*>( mImageConverter->getImage().getBuffer().getBytes() );

	// Create a QImage pointing *directly* onto thsi data
	mQImage = new QImage( data, width, height, QImage::Format_RGB32 );
}

QRGB32ImageMaker::~QRGB32ImageMaker()
{
	delete mQImage;
	mQImage = NULL;

	delete mImageConverter;
	mImageConverter = NULL;
}

bool QRGB32ImageMaker::update( const Image& image )
{
	// Here we just have to update the ImageConverter
	// It internally updates the Image it contains.
	// Without having to do anything, this updates the QImage because
	// it shares its data with the the Image we just updated
	return mImageConverter->update( image );
}

}

