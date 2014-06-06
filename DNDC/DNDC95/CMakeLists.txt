
file( GLOB_RECURSE HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB_RECURSE SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" )

set( TARGET_NAME DNDC95 )
set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
set( PRIVATE_SOURCES ${SOURCES} )

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2)

add_library( ${TARGET_NAME} SHARED ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} )

install( TARGETS ${TARGET_NAME}
    DESTINATION "." )
    
install( FILES "C:/dev/leaf-models-public/DNDC/build-32-bit/DNDC95/Debug/${TARGET_NAME}.pdb"
    "C:/dev/leaf-models-public/DNDC/install-32-bit/${TARGET_NAME}.dll"
    "C:/dev/leaf-models-public/DNDC/install-32-bit/${TARGET_NAME}.lib"
    DESTINATION "." )

    install( FILES "C:/dev/leaf-models-public/DNDC/install-32-bit/${TARGET_NAME}.pdb"
    "C:/dev/leaf-models-public/DNDC/install-32-bit/${TARGET_NAME}.dll"
    "C:/dev/leaf-models-public/DNDC/install-32-bit/${TARGET_NAME}.lib"
    DESTINATION "C:/dev/leaf/external/DNDC/DNDC_SITE/Win32/vc11" )

    install( FILES "C:/dev/leaf-models-public/DNDC/DNDC95/DNDCgo.h"
    "C:/dev/leaf-models-public/DNDC/DNDC95/StdAfx.h"
#   "C:/dev/leaf-models-public/DNDC/DNDC95/Source_main.h"
    DESTINATION "C:/dev/leaf/external/DNDC/DNDC_SITE/Win32/vc11" )