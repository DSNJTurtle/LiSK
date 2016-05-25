cmake_minimum_required( VERSION 3.3 )
project( "LiSK" VERSION 1.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 11 )

find_path( CLN_INCLUDE_DIR "cln.h" DOC "Directory of cln.h" )
find_library( CLN_LIB "cln" DOC "Path to libcln" )

add_executable( example "example.cpp" )
include_directories( "../LiSK" )

include_directories( SYSTEM "${CLN_INCLUDE_DIR}" )
target_link_libraries( example "${CLN_LIB}" )
