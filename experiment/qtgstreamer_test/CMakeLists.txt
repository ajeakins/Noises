cmake_minimum_required( VERSION 2.8 )

PROJECT( qtgstreamer_test )

FIND_PACKAGE( Qt4 REQUIRED )
FIND_PACKAGE( QtGStreamer REQUIRED )

SET( qtgstreamer_test_SOURCES 
	main.cpp 
	mediaapp.cpp
	player.cpp )

SET( qtgstreamer_test_HEADERS 
	mediaapp.h 
	player.h )

QT4_WRAP_CPP( qtgstreamer_test_HEADERS_MOC ${qtgstreamer_test_HEADERS})

INCLUDE_DIRECTORIES( ${QTGSTREAMER_INCLUDE_DIR} )
INCLUDE( ${QT_USE_FILE} } )
ADD_DEFINITIONS( ${QT_DEFINITIONS} ${QTGSTREAMER_DEFINITIONS} )

ADD_EXECUTABLE( qtgstreamer_test ${qtgstreamer_test_SOURCES} ${qtgstreamer_test_HEADERS_MOC})
TARGET_LINK_LIBRARIES( qtgstreamer_test  ${QT_LIBRARIES} ${QTGSTREAMER_LIBRARIES} ${QTGSTREAMER_UI_LIBRARIES} )


