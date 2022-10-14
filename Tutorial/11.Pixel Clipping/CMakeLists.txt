# ------ Set Variables -------------------------- #
MESSAGE( STATUS "Setting Variables")

## Make Project Name
SET( PROJECT_NAME "Example" )
## Set Compiler
SET( CMAKE_CXX_COMPILER g++ )
SET( CMAKE_C_COMPILER gcc )
SET( CMAKE_BUILD_TYPE Debug )
## Set CXX Compiler Version
SET( CMAKE_SET_STANDARD 17 )
## Set Window Options
SET( WINDOW_NAME "Hello ~" )
SET( WINDOW_WIDTH 640 )
SET( WINDOW_HEIGHT 480 )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Options for Projects --------------- #
MESSAGE( STATUS "Setting Options for Project")

## CMake minimun version required
CMAKE_MINIMUM_REQUIRED( VERSION 3.11 )
## Project name
PROJECT( ${PROJECT_NAME} )
## Set compile options

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Source Files ----------------------- #
MESSAGE( STATUS "Setting Source Filesb")

## Source Directories
SET( SRC_DIR src )


###################################################
### If you make new Source or Header file, ########
### Rewrite Under the Code                 ########
### Just SET Directory for New files       ########
###################################################

SET( LOG_DIR ${CMAKE_SOURCE_DIR}/${SRC_DIR}/Log )
SET( DXWINDOW_DIR ${CMAKE_SOURCE_DIR}/${SRC_DIR}/DXWindow )
SET( DXENGINE_DIR ${CMAKE_SOURCE_DIR}/${SRC_DIR}/DXEngine )
SET( SHADER_DIR ${CMAKE_SOURCE_DIR}/shader )

## Source Files
SET( SRCS
	${SRC_DIR}/main.cpp
	${LOG_DIR}/Log.h ${LOG_DIR}/Log.cpp
	${DXWINDOW_DIR}/DXWindow.h ${DXWINDOW_DIR}/DXWindow.cpp
	${DXENGINE_DIR}/DXCommon.h ${DXENGINE_DIR}/DXVertex.h
	${DXENGINE_DIR}/DXEngine.h ${DXENGINE_DIR}/DXEngine.cpp
	)

## Header Files
SET( INCS
	${LOG_DIR}
	${DXWINDOW_DIR}
	${DXENGINE_DIR}
	)

###################################################
###################################################

ADD_EXECUTABLE( ${PROJECT_NAME} WIN32 ${SRCS} )

MESSAGE( STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Set Options for Target Files ----------- #
MESSAGE( STATUS "Setting Options for Target Files")

## Include CMake's Dependency file
INCLUDE( Dependency.cmake)

## Include Header Directories for Target Files
TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${INCS} )

## Link Settings for Target files
TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR} )
TARGET_LINK_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC ${DEP_LIBS} )

MESSAGE( STATUS " Setting Done ")
# ----------------------------------------------- #



# ------ Set for Compiling and Dependency ------- #
MESSAGE( STATUS " Setting for Compile and Dependency ")

TARGET_COMPILE_DEFINITIONS( ${PROJECT_NAME} PUBLIC
	WINDOW_NAME = "${WINDOW_NAME}"
	WINDOW_WIDTH = ${WINDOW_WIDTH}
	WINDOW_HEIGHT = ${WINDOW_HEIGHT}
	)

## ADD_DEPENDENCIES( ${PROJECT_NAME} ${DEP_LIST} )

MESSAGE( STATUS " Setting Done ")
# ----------------------------------------------- #