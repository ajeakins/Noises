CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/" )

if(APPLE)
	# Add path for homebrew, TODO: this could be more generic...
	SET( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/local/Cellar/qt/5.10.1/ )
endif()

PROJECT( noises )

FIND_PACKAGE( Qt5 REQUIRED COMPONENTS Widgets )
FIND_PACKAGE( portaudio REQUIRED )

SET( CMAKE_BUILD_TYPE Release )
SET( CMAKE_AUTOMOC ON )
SET( CMAKE_INCLUDE_CURRENT_DIR ON )

FILE( GLOB_RECURSE noises_SOURCES *.cpp )

SET( noises_RESOURCES data/resources.qrc )

INCLUDE_DIRECTORIES(
	${Qt5Widgets_INCLUDE_DIRS}
	${PORTAUDIO_INCLUDE_DIRS} )

SET( CMAKE_CXX_FLAGS ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} )

ADD_DEFINITIONS(
	${QT_DEFINITIONS}
	-Wall
	-Wextra
	-Wpedantic
	-Werror
	-std=c++11
	-O2 )

QT5_ADD_RESOURCES( noises_RESOURCES_RCC ${noises_RESOURCES} )

ADD_EXECUTABLE( noises
	${noises_SOURCES}
	${noises_HEADERS_MOC}
	${noises_RESOURCES_RCC} )

TARGET_LINK_LIBRARIES( noises
	${QT_LIBRARIES}
	${Qt5Widgets_LIBRARIES}
	${PORTAUDIO_LIBRARIES}
	sndfile
	samplerate )
