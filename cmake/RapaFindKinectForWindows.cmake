#
# A function to search for XInput header and lib in a given SDK root path (Windows Platform SDK or DirectX SDK).
# This function looks for the x64 lib if compiling in 64 bits, x86 otherwise.
# The function sets the following variables:
#  KINECTSDK_FOUND 
#  KinectSDK_INCLUDE_DIR 
#  KinectSDK_LIBRARY 
# 
#  KINECTDEVELOPPERTOOLKIT_FOUND
#  KinectDevelopperToolkit_INCLUDE_DIR 
#  KinectDevelopperToolkit_LIBRARY
#
FUNCTION( FINDKINECTFORWINDOWS KINECTSDK_ROOT_PATH DEVELOPERTOOLKIT_ROOT_PATH )

	#
	# Kinect SDK
	#

	# Determine the header path
	MESSAGE("Searching for Kinect For Windows in ${KINECTSDK_ROOT_PATH}")
	FIND_PATH(	KinectSDK_INCLUDE_DIR NAMES NuiApi.h
				PATHS "${KINECTSDK_ROOT_PATH}/inc" 
				NO_DEFAULT_PATH  )
		
	# Determine the library path depending on the architecture (x86 or x64)
	IF ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		FIND_LIBRARY(	KinectSDK_LIBRARY NAMES Kinect10 
						PATHS "${KINECTSDK_ROOT_PATH}/lib/amd64" 
						NO_DEFAULT_PATH )
	ELSE()
		FIND_LIBRARY(	KinectSDK_LIBRARY NAMES Kinect10 
						PATHS "${KINECTSDK_ROOT_PATH}/lib/x86" 
						NO_DEFAULT_PATH )
	ENDIF()

	#
	# Kinect Developer Toolkit (for face tracking)
	#

	# Determine the header path
	MESSAGE("Searching for Kinect Developer Toolkit in ${DEVELOPERTOOLKIT_ROOT_PATH}")
	FIND_PATH(	KinectDevelopperToolkit_INCLUDE_DIR NAMES FaceTrackLib.h
				PATHS "${DEVELOPERTOOLKIT_ROOT_PATH}/inc" 
				NO_DEFAULT_PATH  )
		
	# Determine the library path depending on the architecture (x86 or x64)
	IF ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		FIND_LIBRARY(	KinectDevelopperToolkit_LIBRARY NAMES FaceTrackLib 
						PATHS "${DEVELOPERTOOLKIT_ROOT_PATH}/Lib/amd64" 
						NO_DEFAULT_PATH )
	ELSE()
		FIND_LIBRARY(	KinectDevelopperToolkit_LIBRARY NAMES FaceTrackLib 
						PATHS "${DEVELOPERTOOLKIT_ROOT_PATH}/Lib/x86" 
						NO_DEFAULT_PATH )
	ENDIF()
			
	INCLUDE( FindPackageHandleStandardArgs )
	FIND_PACKAGE_HANDLE_STANDARD_ARGS( KinectSDK DEFAULT_MSG KinectSDK_LIBRARY KinectSDK_INCLUDE_DIR )
	FIND_PACKAGE_HANDLE_STANDARD_ARGS( KinectDevelopperToolkit DEFAULT_MSG KinectDevelopperToolkit_LIBRARY KinectDevelopperToolkit_INCLUDE_DIR )
	
	# Promote the XXX_FOUND variable to parent scope (otherwise it disappears at the function return).
	# Not needed for XXX_INCLUDE_DIR and XXX_LIBRARY as they are put in the cache which makes them global
	SET( KINECTSDK_FOUND ${KINECTSDK_FOUND} PARENT_SCOPE )	
	SET( KINECTDEVELOPPERTOOLKIT_FOUND ${KINECTDEVELOPPERTOOLKIT_FOUND} PARENT_SCOPE )	

ENDFUNCTION()

#
# Main entry point
#
IF( CMAKE_SYSTEM_NAME MATCHES Windows )

	SET( KinectSDK_SEARCH_PATH  
	     "C:/Program Files/Microsoft SDKs/Kinect/v1.8"
		 CACHE STRING 
		 "The path of the Kinect For Windows SDK" )

	SET( KinectDeveloperToolkit_SEARCH_PATH  
	     "C:/Program Files/Microsoft SDKs/Kinect/Developer Toolkit v1.8.0"
		 CACHE STRING 
		 "The path of the Kinect For Windows Developer Toolkit" )

	FINDKINECTFORWINDOWS( ${KinectSDK_SEARCH_PATH} ${KinectDeveloperToolkit_SEARCH_PATH} )

ENDIF()

	