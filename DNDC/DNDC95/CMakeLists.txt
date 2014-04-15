
file( GLOB_RECURSE HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB_RECURSE SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" )

option( DNDC_BASE_MODE "#Define BASE"       OFF )
option( DNDC_BATCH_MODE  "#Define DEGUI"    OFF )
option( DNDC_DLL_MODE "#Define DLL_MODE"    ON )

if( DNDC_BASE_MODE )
    set( TARGET_NAME DNDCgo )
    add_definitions( -DBASE )
elseif( DNDC_BATCH_MODE )
    set( TARGET_NAME DNDCgo_batch )
    add_definitions( -DDEGUI )
elseif( DNDC_DLL_MODE )
    set( TARGET_NAME DNDCgo_site )
    add_definitions( -DBASE -DDLL_MODE )
endif()

set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
set( PRIVATE_SOURCES ${SOURCES} )

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2)

add_library( ${TARGET_NAME} SHARED ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} )

install( TARGETS ${TARGET_NAME}
    DESTINATION "../install-32-bit" )

if( DNDC_BATCH_MODE OR DNDC_DLL_MODE )
    install( TARGETS ${TARGET_NAME}
        DESTINATION "C:/dev/leaf/install-32-bit/bin/" )
endif()