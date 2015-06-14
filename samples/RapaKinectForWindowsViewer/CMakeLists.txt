CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

PROJECT( "RapaKinectForWindowsViewer" )

IF( MSVC )
	INCLUDE( RapaConfigureVisualStudio )
ENDIF()

SET(	SOURCES
		RKFWQImageWidget.h
		RKFWQImageWidget.cpp
		RKFWQCameraWidget.h
		RKFWQCameraWidget.cpp
		RKFWQColorCameraSettingsWidget.h
		RKFWQColorCameraSettingsWidget.cpp
		RKFWQSkeletonTrackingDataWidget.h
		RKFWQSkeletonTrackingDataWidget.cpp
		RKFWQSensorWidget.h
		RKFWQSensorWidget.cpp
		RKFWQSensorManagerWidget.h
		RKFWQSensorManagerWidget.cpp
		RKFWQFaceTrackerTestWidget.h
		RKFWQFaceTrackerTestWidget.cpp
		Main.cpp 
	)

SOURCE_GROUP("" FILES ${SOURCES} )		# Avoid "Header Files" and "Source Files" virtual folders in VisualStudio

INCLUDE( RapaFindQt )
IF( RAPA_QT_FOUND )
	
	INCLUDE_DIRECTORIES( ${RapaKinectForWindows_SOURCE_DIR} )

	ADD_EXECUTABLE( ${PROJECT_NAME} WIN32 ${SOURCES} )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} RapaKinectForWindows )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${RAPA_QT_LINK_LIBRARIES} )

	#
	# Install
	#
	INSTALL( TARGETS  ${PROJECT_NAME}
			CONFIGURATIONS Debug
			RUNTIME DESTINATION "bin/debug"
			LIBRARY DESTINATION "lib"
			ARCHIVE DESTINATION "lib"	)
	INSTALL( TARGETS  ${PROJECT_NAME}
			CONFIGURATIONS Release
			RUNTIME DESTINATION "bin/release"
			LIBRARY DESTINATION "lib"
			ARCHIVE DESTINATION "lib"	)
	INSTALL( FILES ${RAPA_QT_DEBUG_DLLS} CONFIGURATIONS Debug DESTINATION "bin/debug")
	INSTALL( FILES ${RAPA_QT_DEBUG_PLATFORMS_DLLS} CONFIGURATIONS Debug DESTINATION "bin/debug/platforms")
	INSTALL( FILES ${RAPA_QT_RELEASE_DLLS} CONFIGURATIONS Release DESTINATION "bin/release")
	INSTALL( FILES ${RAPA_QT_RELEASE_PLATFORMS_DLLS} CONFIGURATIONS Release DESTINATION "bin/release/platforms")
	
ELSE()
	MESSAGE("QT not found")
ENDIF()
	

	
