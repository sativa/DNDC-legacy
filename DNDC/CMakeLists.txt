cmake_minimum_required( VERSION 2.8.8 )

project( DNDC )

set( CMAKE_BUILD_TYPE "Debug" )

set( INSTALL_LIBDIR lib )
set( INSTALL_BINDIR bin )
set( INSTALL_INCDIR include )
set( INSTALL_SHRDIR share )



add_subdirectory( DNDC95 )
