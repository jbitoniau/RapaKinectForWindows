#pragma once

#include <string>

namespace RKFW
{

/*
	ImageFormat
	
	The ImageFormat defines the fundamental characteristics of an Image:  
	- its size in pixels 
	- its encoding

	The encoding defines how the pixels of the image are laid out in the memory buffer owned by 
	the Image object. More precisely, it defines which color model.
	is used and how the components of the tuples representing the color are stored at the byte level.

	The Image/ImageFormat system only support
	- uncompressed data (no variable-length spatial/temporal compression like JPG, H264, etc...)
	- non-paletized image
	- pixel-oriented or "interleaved" data storage (as opposed to planar-oriented data) 
	
	The concept of stride/padding is not supported.

	Some references:
	http://en.wikipedia.org/wiki/Color_model
	http://software.intel.com/sites/products/documentation/hpc/ipp/ippi/ippi_ch6/ch6_pixel_and_planar_image_formats.html
*/
class ImageFormat
{
public:
	enum Encoding
	{
		Grayscale16,	// The depth camera of the Kinect produces images with this encoding.
						// The depth value is stored in bits 3 to 15 and is expressed in mm.
						// (so you have to shift the raw value on the right by 3 to get the mm value)
						// Depending on the includePlayerIndex parameter of startDepthCapture(), 
						// bits 0 to 2 are either 0 or contain the so-called Player-index (only 
						// available if initialize() was call with Skeleton sub-system).
						// Note: maybe this should be called D13X3 (when no player index) and D13P3
		
		BGRX32,			// 32-bit RGB values where the components appear in the following order (when 
						// read byte after byte): blue, green, red, unused (always 0)

		UYVY,			// 4 bytes per 2-pixel macroblock. The byte sequence: is U0, Y0, V0, Y1.
						// Where Y0 and Y1 represent the luma component of each pixel 
						// and U0 and V0 represent the chroma component of both pixels.
						// See http://www.fourcc.org/yuv.php#UYVY

		Bayer,			// http://en.wikipedia.org/wiki/Bayer_filter  
						// G, R, G, R, G, R...
						// B, G, B, G, B, G... 
						// G, R, G, R, G, R...
				
		EncodingCount	
	};

	ImageFormat();
	ImageFormat( unsigned int width, unsigned int height, Encoding encoding );

	unsigned int			getWidth() const			{ return mWidth; }
	unsigned int			getHeight() const			{ return mHeight; }
	Encoding				getEncoding() const			{ return mEncoding; }
	const char*				getEncodingName() const		{ return getEncodingName( getEncoding() ); }
	static const char*		getEncodingName( Encoding encoding );
	
	unsigned int			getNumBitsPerPixel() const		{ return getNumBitsPerPixel( getEncoding() ); }
	static unsigned int		getNumBitsPerPixel( Encoding encoding );
	unsigned int			getNumBytesPerLine() const		{ return getNumBitsPerPixel()*getWidth()/8; }
	unsigned int			getDataSizeInBytes() const;

	bool					operator==( const ImageFormat& other ) const;
	bool					operator!=( const ImageFormat& other ) const;

	std::string				toString() const;

private:
	
	static const char*		mEncodingNames[EncodingCount];
	static unsigned int		mEncodingBitsPerPixel[EncodingCount];
	
	unsigned int			mWidth;
	unsigned int			mHeight;
	Encoding				mEncoding;
};

}
