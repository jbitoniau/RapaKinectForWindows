CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

PROJECT( "RapaKinectForWindows" )

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake" )

IF( CMAKE_SYSTEM_NAME MATCHES "Windows" )
	
	IF( MSVC )
		INCLUDE( RapaConfigureVisualStudio )
	ENDIF()

	INCLUDE( RapaFindKinectForWindows )
	IF( KINECTSDK_FOUND AND KINECTDEVELOPPERTOOLKIT_FOUND )
		
		#MESSAGE( "Using Kinect for Windows SDK from ${KinectSDK_INCLUDE_DIR}" )
		#MESSAGE( "Using Kinect for Windows Developer Toolkit from ${KinectDevelopperToolkit_INCLUDE_DIR}" )
			
		INCLUDE_DIRECTORIES( ${KinectSDK_INCLUDE_DIR} )
		INCLUDE_DIRECTORIES( ${KinectDevelopperToolkit_INCLUDE_DIR} )
		INCLUDE_DIRECTORIES( include )
		
		SET	(	HEADERS
				include/RKFWVector3.h
				include/RKFWCriticalSectionEnterer.h
				include/RKFWCOMObjectSharedPtr.h
				include/RKFWMemoryBuffer.h
				include/RKFWImageFormat.h
				include/RKFWImage.h
				include/RKFWImageConverter.h
				include/RKFWCapturedImage.h
				include/RKFWCaptureSettings.h
				include/RKFWSkeletonTrackingData.h
				include/RKFWColorCameraSettings.h
				include/RKFWSensorInternals.h
				include/RKFWSensor.h
				include/RKFWSensorManagerInternals.h
				include/RKFWSensorManager.h
				include/RKFWFaceTrackerResult.h
				include/RKFWFaceTrackerInternals.h
				include/RKFWFaceTracker.h
			)	
		
		SET	(	SOURCES
				${HEADERS}
				src/RKFWCriticalSectionEnterer.cpp
				src/RKFWCOMObjectSharedPtr.cpp
				src/RKFWMemoryBuffer.cpp
				src/RKFWImageFormat.cpp
				src/RKFWImage.cpp
				src/RKFWImageConverter.cpp
				src/RKFWCapturedImage.cpp
				src/RKFWCaptureSettings.cpp
				src/RKFWSkeletonTrackingData.cpp
				src/RKFWColorCameraSettings.cpp
				src/RKFWSensorInternals.cpp
				src/RKFWSensor.cpp
				src/RKFWSensorManagerInternals.cpp
				src/RKFWSensorManager.cpp
				src/RKFWFaceTrackerResult.cpp
				src/RKFWFaceTrackerInternals.cpp
				src/RKFWFaceTracker.cpp		
			)	
	
		SOURCE_GROUP("" FILES ${SOURCES} )		# Avoid "Header Files" and "Source Files" virtual folders in VisualStudio

		SET(CMAKE_DEBUG_POSTFIX "d")
		ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SOURCES} )
		TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${KinectSDK_LIBRARY} ${KinectDevelopperToolkit_LIBRARY} ) 
	
		#
		# Install
		#
		INSTALL(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION lib
				RUNTIME DESTINATION bin )
				#INCLUDES DESTINATION include )		# If uncommented, the ${PROJECT_NAME} target contains INCLUDE_DIRECTORIES information. Importing the target automatically adds this directory to the INCLUDE_DIRECTORIES.
		SET( TARGET_NAMESPACE Rapa:: )
		INSTALL( FILES ${HEADERS} DESTINATION include COMPONENT Devel )		
		EXPORT( EXPORT ${PROJECT_NAME}Targets FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake" NAMESPACE ${TARGET_NAMESPACE} )
		CONFIGURE_FILE( cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" @ONLY )
		SET( ConfigPackageLocation lib/cmake/${PROJECT_NAME} )
		INSTALL(EXPORT ${PROJECT_NAME}Targets
				FILE ${PROJECT_NAME}Targets.cmake
				NAMESPACE ${TARGET_NAMESPACE}
				DESTINATION ${ConfigPackageLocation} )
		INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" DESTINATION ${ConfigPackageLocation} COMPONENT Devel )
	
		ADD_SUBDIRECTORY( samples )
		
	ELSE()
		MESSAGE("Kinect For Windows not found")
	ENDIF()
ELSE()
	MESSAGE("${PROJECT_NAME} is Windows only")
ENDIF()









 