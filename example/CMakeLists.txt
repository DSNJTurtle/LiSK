### Minimum required version
cmake_minimum_required( VERSION 3.3 )

### Project name
project( "LiSK" VERSION 1.0 LANGUAGES CXX )

### Special commands for compilation
set( CMAKE_CXX_STANDARD 11 )

### Add CLN library
### Header file and library should automatically be
### found if present in environment
find_path( CLN_INCLUDE_DIR "cln/cln.h" DOC "Directory of cln.h" )
find_library( CLN_LIB "cln" DOC "Path to libcln" )

### Executable
add_executable( example "example.cpp" )
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

### Include directories
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../LiSK )
include_directories( SYSTEM ${CLN_INCLUDE_DIR} )

### Linking
target_link_libraries( example ${CLN_LIB} )