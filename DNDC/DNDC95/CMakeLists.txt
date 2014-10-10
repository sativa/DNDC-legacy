file( GLOB_RECURSE HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB_RECURSE SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" )

set( TARGET_NAME DNDC95 )
set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
set( PRIVATE_SOURCES ${SOURCES} )

add_definitions(-D_AFXDLL)
add_definitions(-DDNDC95_DLL )
set(CMAKE_MFC_FLAG 2)

add_library( ${TARGET_NAME} SHARED ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} )

install( TARGETS ${TARGET_NAME}
    DESTINATION "." )

if( CMAKE_CL_64 )

    install( FILES "C:/dev/leaf-models-public/DNDC/build-64-bit/DNDC95/RelWithDebInfo/${TARGET_NAME}.pdb"
    DESTINATION "C:/dev/leaf-models-public/DNDC/install-64-bit" )

    install( FILES "C:/dev/leaf-models-public/DNDC/install-64-bit/${TARGET_NAME}.pdb"
    "C:/dev/leaf-models-public/DNDC/install-64-bit/${TARGET_NAME}.dll"
    "C:/dev/leaf-models-public/DNDC/install-64-bit/${TARGET_NAME}.lib"
    DESTINATION "C:/dev/leaf/external/DNDC_DLL/DNDC95/x64/vc11" )

    install( 
    FILES "C:/dev/leaf-models-public/DNDC/DNDC95/DNDC_Interface.h"
    DESTINATION "C:/dev/leaf/external/DNDC_DLL/DNDC95/x64/vc11" )
    
    return()
endif()

install( FILES "C:/dev/leaf-models-public/DNDC/build-32-bit/DNDC95/RelWithDebInfo/${TARGET_NAME}.pdb"
DESTINATION "C:/dev/leaf-models-public/DNDC/install-32-bit" )

install( FILES "C:/dev/leaf-models-public/DNDC/install-32-bit/${TARGET_NAME}.pdb"
"C:/dev/leaf-models-public/DNDC/install-32-bit/${TARGET_NAME}.dll"
"C:/dev/leaf-models-public/DNDC/install-32-bit/${TARGET_NAME}.lib"
DESTINATION "C:/dev/leaf/external/DNDC_DLL/DNDC95/Win32/vc11" )

install( 
FILES "C:/dev/leaf-models-public/DNDC/DNDC95/DNDC_Interface.h"
DESTINATION "C:/dev/leaf/external/DNDC_DLL/DNDC95/Win32/vc11" )
    