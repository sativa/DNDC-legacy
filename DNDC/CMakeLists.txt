cmake_minimum_required( VERSION 2.8.8 )

project( DNDC )

set( VC_VERSION 11 )

option( BASE_MODE "Build DNDC .dll for use with DNDC GUI" OFF )
option( BATCH_MODE "Build DNDC in batch mode with NO gui" OFF )
option( DLL_MODE "Build base DNDC as .dll only" ON )

add_subdirectory( DNDC95 )
