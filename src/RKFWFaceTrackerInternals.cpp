#include "RKFWFaceTrackerInternals.h"

#include <assert.h>


/*
	Notes:
	- The start() method creates IFTImages for storing color and depth information. It allocates their buffers.
	  This means that when submitting new image data (via the update() method), copies of the color/depth data 
	  is done into those IFTImages. It might be possible to avoid this extra copy, by creating IFTImages that wrap
	  external image data (coming from the Sensor CapturedImage for example). The user of the API would 
	  have to ensure this external image data is valid as long as he uses the FaceTracker's update() method.
*/

namespace RKFW
{

FaceTrackerInternals::FaceTrackerInternals()
	: mIsStarted(false),
	  mColorImage(NULL),
	  mDepthImage(NULL),
	  mFaceTracker(NULL),
	  mFaceTrackerResult(NULL),
	  mResult(),
	  mFaceModelNeutralVertices(),
	  mFaceModelNeutralTriangles(),
	  mFaceModelFittedVertices()
{
}

FaceTrackerInternals::~FaceTrackerInternals()
{
	if ( isStarted() )
		stop();

	assert( !mFaceTrackerResult.get() );
	assert( !mFaceTracker.get() );
	assert( !mDepthImage.get() );
	assert( !mColorImage.get() );
}

bool FaceTrackerInternals::getIFTImageFormat( ImageFormat::Encoding encoding, FTIMAGEFORMAT& iftImageFormat )
{
	iftImageFormat = FTIMAGEFORMAT_INVALID;
	switch ( encoding )
	{
		case ImageFormat::Grayscale16 : 
				// The format FTIMAGEFORMAT_UINT16_D16 doesn't seem to be supported by the FaceTracker
				// The StartTracking() method in that case returns FT_ERROR_INVALID_INPUT_IMAGE.
				// So we stick with Kinect-specific FTIMAGEFORMAT_UINT16_D13P3 but that's OK
				iftImageFormat = FTIMAGEFORMAT_UINT16_D13P3;
				return true;

		case ImageFormat::BGRX32 : 
				iftImageFormat = FTIMAGEFORMAT_UINT8_B8G8R8X8;
				return true;
	}
	return false;
}

// This method copy as much data as it can from the image into the IFTImage, 
// considering the image have the proper dimension and zero stride 
void FaceTrackerInternals::copyImageToIFTImage( const Image& image, IFTImage& iftImage )
{
	const unsigned char* sourceData = image.getBuffer().getBytes();
	unsigned int sourceDataSize = image.getBuffer().getSizeInBytes();
	
	unsigned char* destData = iftImage.GetBuffer();
	if ( !destData )
		return;
	unsigned int destDataSize = iftImage.GetBufferSize();
	
	unsigned int numBytes = std::min( sourceDataSize, destDataSize );
	memcpy( destData, sourceData, numBytes );				
}


bool FaceTrackerInternals::start( ImageFormat colorImageFormat, float colorImageFocalLengthInPixels, 
								  ImageFormat depthImageFormat, float depthImageFocalLengthInPixels )
{
	if ( isStarted() )
		return true;

	// Check that the color and depth pixel formats are supported
	FTIMAGEFORMAT nuiColorImageFormat = FTIMAGEFORMAT_INVALID;
	if ( !getIFTImageFormat( colorImageFormat.getEncoding(), nuiColorImageFormat ) )
		return false;

	FTIMAGEFORMAT nuiDepthImageFormat = FTIMAGEFORMAT_INVALID;
	if ( !getIFTImageFormat( depthImageFormat.getEncoding(), nuiDepthImageFormat ) )
		return false;

	// Create the face tracker
	IFTFaceTracker* faceTrackerRaw = FTCreateFaceTracker();
	COMObjectSharedPtr<IFTFaceTracker> faceTracker( faceTrackerRaw );
	if ( !faceTracker.get() )
		return false;
	
	// Get color and depth camera configs
	FT_CAMERA_CONFIG colorCameraConfig;
	colorCameraConfig.Width = colorImageFormat.getWidth();
	colorCameraConfig.Height = colorImageFormat.getHeight();
	colorCameraConfig.FocalLength = colorImageFocalLengthInPixels;

	FT_CAMERA_CONFIG depthCameraConfig;
	depthCameraConfig.Width = depthImageFormat.getWidth();			
	depthCameraConfig.Height = depthImageFormat.getHeight();
	depthCameraConfig.FocalLength = depthImageFocalLengthInPixels;			
    
	// Create and allocate color image
	HRESULT hr = NULL;
	IFTImage* colorImageRaw = FTCreateImage();
	COMObjectSharedPtr<IFTImage> colorImage( colorImageRaw );
	if ( !colorImage.get() )
		return false;
	
	hr = colorImage->Allocate( colorCameraConfig.Width, colorCameraConfig.Height, FTIMAGEFORMAT_UINT8_B8G8R8X8/*nuiColorImageFormat*/ );
	if ( FAILED(hr) )
		return false;
	
	// Create and allocate depth image
	IFTImage* depthImageRaw = FTCreateImage();
	COMObjectSharedPtr<IFTImage> depthImage( depthImageRaw );
	if ( !depthImage.get() )
		return false;
	
	hr = depthImage->Allocate( depthCameraConfig.Width, depthCameraConfig.Height, nuiDepthImageFormat );
	if ( FAILED(hr) )
		return false;
    
	// Initialize the face tracker
	FTRegisterDepthToColor depthToColorMappingFunc = NULL;
	PCWSTR faceModelPath = NULL;
	hr = faceTracker->Initialize( &colorCameraConfig, &depthCameraConfig, depthToColorMappingFunc, faceModelPath ); 
	if ( FAILED(hr) )
		return false;
	
	// Create a face tracker result to hold the tracking data concerning a single face
	IFTResult* faceTrackerResultRaw = NULL;
	hr = faceTracker->CreateFTResult( &faceTrackerResultRaw );
	COMObjectSharedPtr<IFTResult> faceTrackerResult( faceTrackerResultRaw );
    if ( FAILED(hr) || !faceTrackerResult.get() )
		return false;

	// Get face model data
	FaceTracker::Vertices neutralVertices;
	FaceTracker::Triangles neutralTriangles;
	if ( !getFaceModelNeutralGeometry( faceTracker.get(), neutralVertices, neutralTriangles ) )
		return false;
	
	// Update members from local variables now that all is good    
	mColorImage = colorImage;
	mDepthImage = depthImage;
	mFaceTracker = faceTracker;
	mFaceTrackerResult = faceTrackerResult;

	mIsStarted = true;
	mResult = FaceTrackerResult();
	assert( !mResult.mIsTracking );

	mFaceModelNeutralVertices = neutralVertices;
	mFaceModelNeutralTriangles = neutralTriangles;
	mFaceModelFittedVertices.clear();
	
	return true;
}

// If tracking is lost, previous Result remains untouched (except the mIsTracking indicator)
// This also applies to the face model fitted vertices
bool FaceTrackerInternals::update( const Image& colorImage, const Image& depthImage )
{
	if ( !isStarted() )
		return false;

	HRESULT hr = NULL;

	// Updates the content of the color/depth IFTImages with the provided images
	copyImageToIFTImage( colorImage, *mColorImage.get() );
	copyImageToIFTImage( depthImage, *mDepthImage.get() );

	// Proceeds with face tracking
	FLOAT zoomFactor = 1.0f; 
	const POINT* viewOffset = NULL;
	FT_SENSOR_DATA sensorData( mColorImage.get(), mDepthImage.get(), zoomFactor, viewOffset );
	if ( !mResult.mIsTracking )
	{
		const RECT* regionOfInterest = NULL;
		const FT_VECTOR3D* headPoints = NULL;
		hr = mFaceTracker->StartTracking( &sensorData, regionOfInterest, headPoints, mFaceTrackerResult.get() );

/*	if ( hr==FT_FACILITY )
	{
		int i =10;
	} 
	else if ( hr==FT_ERROR_INVALID_MODELS )
	{
		int i =10;
	} 
	else if ( hr==FT_ERROR_INVALID_INPUT_IMAGE )
	{
		int i =10;
	} 
	else if ( hr==FT_ERROR_AAM_FAILED )
	{
		int i =10;
	} 
	else if ( hr==FT_ERROR_NN_FAILED )
	{
		int i =10;
	} 
	else if ( hr==FT_ERROR_UNINITIALIZED )
	{
		int i =10;
	} 
	else if ( hr==FT_ERROR_INVALID_MODEL_PATH )
	{
		int i =10;
	} 
	else if ( hr==FT_ERROR_EVAL_FAILED )
	{
		int i =10;
	}
	else if ( hr==FT_ERROR_INVALID_CAMERA_CONFIG )
	{
		int i =10;
	}
	else if ( hr==FT_ERROR_INVALID_3DHINT )
	{
		int i =10;
	}
	else if ( hr==FT_ERROR_HEAD_SEARCH_FAILED )
	{
		int i =10;
	}
	else if ( hr==FT_ERROR_USER_LOST )
	{
		int i =10;
	}
	else if ( hr==FT_ERROR_KINECT_DLL_FAILED )
	{
		int i =10;
	}
	else if ( hr==FT_ERROR_KINECT_NOT_CONNECTED )
	{
		int i =10;
	}*/     
		if ( SUCCEEDED(hr) )
		{
			if ( SUCCEEDED( mFaceTrackerResult->GetStatus() ) )
				mResult.mIsTracking = true;
			else 
				mResult.mIsTracking = false;
		}
	}
	else
	{
		const FT_VECTOR3D* headPoints = NULL;
		hr = mFaceTracker->ContinueTracking( &sensorData, headPoints, mFaceTrackerResult.get() );
		if ( FAILED(hr) || FAILED(mFaceTrackerResult->GetStatus()) )
			mResult.mIsTracking = false;
	}

	if ( mResult.mIsTracking )
	{
		// Update face transformation
		FLOAT scale = 1.f;
		FLOAT rotation[3] = { 0.f, 0.f, 0.f };
		FLOAT translation[3] = { 0.f, 0.f, 0.f };
		hr = mFaceTrackerResult->Get3DPose( &scale, rotation, translation );	
		if ( SUCCEEDED(hr) )
		{
			mResult.mFaceScale = scale;
			mResult.mFaceRotation = Vector3f( rotation[0], rotation[1], rotation[2] );
			mResult.mFaceTranslation = Vector3f( translation[0], translation[1], translation[2] );
		}

		// Update fitted vertices 
		bool ret = getFaceModelFittedVertices( mFaceTracker.get(), mFaceTrackerResult.get(), mFaceModelFittedVertices );
		assert( ret );
	}
	return true;
}

void FaceTrackerInternals::stop()
{
	if ( !isStarted() )
		return;

	mFaceTrackerResult.reset();
	mFaceTracker.reset();
	mDepthImage.reset(); 
	mColorImage.reset(); 
		
	mIsStarted = false;
	mResult = FaceTrackerResult();
	assert(!mResult.mIsTracking);

	mFaceModelNeutralVertices.clear();
	mFaceModelNeutralTriangles.clear();
	mFaceModelFittedVertices.clear();
}

bool FaceTrackerInternals::getFaceModelNeutralGeometry( IFTFaceTracker* faceTracker, FaceTracker::Vertices& vertices, FaceTracker::Triangles& triangles )
{
	assert( faceTracker );

	// Access the model
	HRESULT hr = 0;
	IFTModel* faceModel = NULL;
	hr = faceTracker->GetFaceModel(&faceModel);
	if ( FAILED(hr) )
		return false;
	
	// Get the triangles
	FT_TRIANGLE* faceTrackerTriangles = NULL;
	UINT faceTrackerTriangleCount = 0;
	hr = faceModel->GetTriangles( &faceTrackerTriangles, &faceTrackerTriangleCount );	// I guess this doesn't alloc anything but just returns a pointer to the face tracker internal storage
	if ( FAILED(hr) )
	{
		faceModel->Release();
		faceModel = NULL;	
		return false; 
	}
	
	triangles.resize( faceTrackerTriangleCount * 3 );
	for ( UINT i=0, j=0; i<faceTrackerTriangleCount; ++i, j+=3 )
	{
		triangles[j] = faceTrackerTriangles[i].i;
		triangles[j+1] = faceTrackerTriangles[i].j;
		triangles[j+2] = faceTrackerTriangles[i].k;
	}
		
	// Create empty shape units. Here we need to prepare the right number of SU otherwise the Get3DShape method fails.
	FLOAT shapeUnits[maxShapeUnitCount];
	for ( int i=0; i<maxShapeUnitCount; ++i )
		shapeUnits[i] = 0.f;
	UINT shapeUnitCount = faceModel->GetSUCount();
	assert( shapeUnitCount<=maxShapeUnitCount );

	// Artificially create an array of a single AU which is zero. Tthat's because the Get3DShape() requires a valid AU pointer.
	UINT animationUnitCount= 0;
	FLOAT animationUnits[1] = { 0 };

	// Set face global transform to identity
	FLOAT scale = 1.f;
	FLOAT rotation[3] = { 0.f, 0.f, 0.f };
	FLOAT translation[3] = { 0.f, 0.f, 0.f };

	// Get the 3D shape (i.e. the vertices)
	int vertexCount = faceModel->GetVertexCount();
	assert( sizeof(FT_VECTOR3D)==sizeof(Vector3f) );
	assert( vertexCount>0 );
	vertices.resize( vertexCount );
	Vector3f* firstVertex = &(vertices[0]);
	FT_VECTOR3D* faceTrackerVertices = reinterpret_cast<FT_VECTOR3D*>( firstVertex );

	hr = faceModel->Get3DShape( shapeUnits, shapeUnitCount, animationUnits, animationUnitCount, scale, rotation, translation, faceTrackerVertices, vertexCount );
	if ( FAILED(hr) )
	{
		faceModel->Release();
		faceModel = NULL;	
		return false;
	}

	faceModel->Release();
	faceModel = NULL;	
		
	return true;				
}

bool FaceTrackerInternals::getFaceModelFittedVertices( IFTFaceTracker* faceTracker, IFTResult* faceTrackerResult, FaceTracker::Vertices& vertices )
{
	assert( faceTracker );
	assert( faceTrackerResult );

	// Access the model
	HRESULT hr = 0;
	IFTModel* faceModel = NULL;
	hr = faceTracker->GetFaceModel(&faceModel);
	if ( FAILED(hr) )
		return false;
	
	// Shape units
	FLOAT* headScale = NULL;				
	FLOAT* shapeUnits = NULL;
	UINT shapeUnitCount = 0;
	BOOL shapeUnitsConverged = false;
	hr = faceTracker->GetShapeUnits( headScale, &shapeUnits, &shapeUnitCount, &shapeUnitsConverged );
	if ( FAILED(hr) )
	{
		faceModel->Release();
		faceModel = NULL;	
		return false;
	}

	// Create empty shape units. Here we need to prepare the right number of SU otherwise the Get3DShape method fails.
	/*FLOAT shapeUnits[maxShapeUnitCount];
	for ( int i=0; i<maxShapeUnitCount; ++i )
		shapeUnits[i] = 0.f;
	UINT shapeUnitCount = faceModel->GetSUCount();
	assert( shapeUnitCount<=maxShapeUnitCount );
	*/

	// Animation units
	FLOAT* animationUnits = NULL;
	UINT animationUnitCount = 0;
	hr = faceTrackerResult->GetAUCoefficients( &animationUnits, &animationUnitCount );
	if ( FAILED(hr) )
	{
		faceModel->Release();
		faceModel = NULL;	
		return false;
	}

	// Set face global transform to identity
	FLOAT scale = 1.f;
	FLOAT rotation[3] = { 0.f, 0.f, 0.f };
	FLOAT translation[3] = { 0.f, 0.f, 0.f };
	
	// Get the 3D shape (i.e. the vertices)
	int vertexCount = faceModel->GetVertexCount();
	assert( sizeof(FT_VECTOR3D)==sizeof(Vector3f) );
	assert( vertexCount>0 );
	vertices.resize( vertexCount );
	Vector3f* firstVertex = &(vertices[0]);
	FT_VECTOR3D* faceTrackerVertices = reinterpret_cast<FT_VECTOR3D*>( firstVertex );

	hr = faceModel->Get3DShape( shapeUnits, shapeUnitCount, animationUnits, animationUnitCount, scale, rotation, translation, faceTrackerVertices, vertexCount );
	if ( FAILED(hr) )
	{
		faceModel->Release();
		faceModel = NULL;	
		return false;
	}

	faceModel->Release();
	faceModel = NULL;	

	return true;
}

}
