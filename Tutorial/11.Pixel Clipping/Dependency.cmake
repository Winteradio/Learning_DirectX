# ------ Set for ExternalProject ---------------- #
MESSAGE( STATUS "Setting for ExternalProject")

INCLUDE( ExternalProject )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Variables for Dependency ----------- #
MESSAGE( STATUS "Setting Variables for Dependency")

SET( DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install )
SET( DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include )
SET( DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib )

SET( DEP_LIST ${DEP_LIST} )
SET( DEP_LIBS ${DEP_LIBS} )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



#### DIRECTX11 ---------------------------------- #
MESSAGE( STATUS "Directx11 - Linking : DIRECTX SDK")

SET( WINDOWSDK_INCLUDE_DIR
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\shared"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\um"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\winrt
	")

SET( D11_INCLUDE_DIR
	"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include"
	)
SET( D11_LIB_DIR
	"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64"
	)
SET( D11_LIBS d3d11.lib d3dx11.lib d3dx10.lib dxgi.lib dxguid.lib )

TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${WINDOWSDK_INCLUDE_DIR} ${D11_INCLUDE_DIR} )
TARGET_LINK_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${D11_LIB_DIR} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC ${D11_LIBS} )

MESSAGE( STATUS "Directx11 - Done")
#### DIRECTX11 ---------------------------------- #