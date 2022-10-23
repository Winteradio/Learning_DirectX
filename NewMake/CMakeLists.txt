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

SET( DX_DIR ${CMAKE_SOURCE_DIR}/${SRC_DIR}/DX )
SET( DXCAMERA_DIR ${DX_DIR}/DXCamera )
SET( DXD3D_DIR ${DX_DIR}/DXD3D )
SET( DXENGINE_DIR ${DX_DIR}/DXEngine )
SET( DXLIGHT_DIR ${DX_DIR}/DXLight )
SET( DXMODEL_DIR ${DX_DIR}/DXModel )
SET( DXPHYSICS_DIR ${DX_DIR}/DXPhysics )
SET( DXSHADER_DIR ${DX_DIR}/DXShader )

SET( DM_DIR ${DXMODEL_DIR} )
SET( MDPARTICLE_DIR ${DM_DIR}/MDParticle )
SET( MDBOX_DIR ${DM_DIR}/MDBox )

SET( SYSTEM_DIR ${CMAKE_SOURCE_DIR}/${SRC_DIR}/System )

SET( INPUT_DIR ${CMAKE_SOURCE_DIR}/${SRC_DIR}/Input )
SET( LOG_DIR ${CMAKE_SOURCE_DIR}/${SRC_DIR}/Log )
SET( TIMER_DIR ${CMAKE_SOURCE_DIR}/${SRC_DIR}/Timer )

## Source Files
SET( SRCS
	${SRC_DIR}/main.cpp

	${SYSTEM_DIR}/System.h ${SYSTEM_DIR}/System.cpp

	${LOG_DIR}/Log.h ${LOG_DIR}/Log.cpp

	${DX_DIR}/DXCommon.h
	${DXCAMERA_DIR}/DXCamera.h ${DXCAMERA_DIR}/DXCamera.cpp
	${DXD3D_DIR}/DXD3D.h ${DXD3D_DIR}/DXD3D.cpp
	${DXENGINE_DIR}/DXEngine.h ${DXENGINE_DIR}/DXEngine.cpp
	${DXLIGHT_DIR}/DXLight.h ${DXLIGHT_DIR}/DXLight.cpp
 	${DXMODEL_DIR}/DXModel.h ${DXMODEL_DIR}/DXModel.cpp
	${DXPHYSICS_DIR}/DXPhysics.h ${DXPHYSICS_DIR}/DXPhysics.cpp
	${DXSHADER_DIR}/DXShader.h ${DXSHADER_DIR}/DXShader.cpp

	${MDPARTICLE_DIR}/MDParticle.h ${MDPARTICLE_DIR}/MDParticle.cpp
	${MDBOX_DIR}/MDBox.h ${MDBOX_DIR}/MDBox.cpp
	)

## Header Files
SET( INCS
	${SYSTEM_DIR}

	${LOG_DIR}
	${TIMER_DIR}

	${DX_DIR}
	${DXCAMERA_DIR}
	${DXD3D_DIR}
	${DXENGINE_DIR}
	${DXLIGHT_DIR}
	${DXMODEL_DIR}
	${DXPHYSICS_DIR}
	${DXSHADER_DIR}

	${MDPARTICLE_DIR}
	${MDBOX_DIR}
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

MESSAGE( STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Set for Compiling and Dependency ------- #
MESSAGE( STATUS "Setting for Compile and Dependency ")

TARGET_COMPILE_DEFINITIONS( ${PROJECT_NAME} PUBLIC
	WINDOW_NAME = "${WINDOW_NAME}"
	WINDOW_WIDTH = ${WINDOW_WIDTH}
	WINDOW_HEIGHT = ${WINDOW_HEIGHT}
	)

## ADD_DEPENDENCIES( ${PROJECT_NAME} ${DEP_LIST} )

MESSAGE( STATUS "Setting Done ")
# ----------------------------------------------- #