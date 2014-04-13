set( HEADERS
    "CreateInputFile.h"
    "Dndc_shell.h"
    "Dndc_tool.h"
    "Source_main.h"
    "DNDCgo.h"
    "Resource.h"
    "StdAfx.h" )

set( SOURCES
    "CreateInputFiles.cpp"
    "Dndc_balance.cpp"
    "Dndc_balloon.cpp"
    "Dndc_CH4.cpp"
    "Dndc_CH4peat.cpp"
    "Dndc_crop0.cpp"
    "Dndc_data.cpp"
    "Dndc_Eh.cpp"
    "Dndc_farming.cpp"
    "Dndc_hydrology.cpp"
    "Dndc_main.cpp"
    "Dndc_manure0"
    "Dndc_manure1"
    "Dndc_manure2"
    "Dndc_manure3"
    "Dndc_manure5"
    "Dndc_manure6"
    "Dndc_manure7"
    "Dndc_manure9"
    "Dndc_manure10"
    "Dndc_ManureComponent.cpp"
    "Dndc_ManureFeedlot.cpp"
    "Dndc_ManureTreatment.cpp"
    "Dndc_mapping.cpp"
    "Dndc_output.cpp"
    "Dndc_P.cpp"
    "Dndc_paint.cpp"
    "Dndc_regiondaily.cpp"
    "Dndc_RegionOutput.cpp"
    "Dndc_shell.cpp"
    "Dndc_SoilChemistry.cpp"
    "Dndc_SoilPhysics.cpp"
    "Dndc_summary.cpp"
    "Dndc_tool.cpp"
    "Dndc_VOCs.cpp"
    "Dndc_WT.cpp"
    "DNDCgo.cpp"
    "StdAfx.cpp" )

#file( GLOB_RECURSE HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
#file( GLOB_RECURSE SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cxx" )

set( TARGET_NAME DNDCgo )
set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
set( PRIVATE_SOURCES ${SOURCES} )

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2)

add_library( ${TARGET_NAME} SHARED ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} )

install( 
    TARGETS ${TARGET_NAME}
    DESTINATION ../install-32-bit )