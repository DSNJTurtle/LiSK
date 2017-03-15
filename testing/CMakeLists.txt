### Minimum required version
cmake_minimum_required( VERSION 3.3 )

### Project name
project( "LiSKTest" VERSION 1.0 LANGUAGES CXX )

### Compilation specific stuff
set( CMAKE_CXX_STANDARD 11 )

### Add CLN and GiNaC
find_path( CLN_INCLUDE_DIR "cln/cln.h" DOC "Directory of cln.h" )
find_library( CLN_LIB "cln" DOC "Path to libcln" )
find_path( GINAC_INCLUDE_DIR "ginac/ginac.h" DOC "Directory of ginac.h" )
find_library( GINAC_LIB "ginac" DOC "Path to libginac" )


### Google Test
### Taken from 'googletest/googletest/README.md' or rather
### from http://crascit.com/2015/07/25/cmake-gtest/
### My comments are denoted by "###"
########################################################
# Download and unpack googletest at configure time.
### Copy CMakeLists.txt.in to googletest-download/CMakeLists.txt
configure_file(CMakeLists.txt.in
               googletest-download/CMakeLists.txt)
### Run the new CMakeLists.txt file so that directories and Makefiles
### are created
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
### Exectue the Makefile, which then download google test and creates
### Makefiles for this stage. During the actual build of the main 
### project the google test framework is compiled
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
 
# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
 
# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)
 
# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories(
  	${gtest_SOURCE_DIR}/include
	${gmock_SOURCE_DIR}/include
	)
endif()

########################################################


### Executable
add_executable( test_dprec testing_dprec.cpp )
add_executable( test_arprec testing_arprec.cpp )
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

### Suppress compiler warnings due to GiNaC's use of depreciated conntainers
target_compile_options(test_dprec PUBLIC -Wno-deprecated-declarations)
target_compile_options(test_arprec PUBLIC -Wno-deprecated-declarations)


### Include directories
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../LiSK )
include_directories( SYSTEM ${CLN_INCLUDE_DIR} ${GINAC_INCLUDE_DIR} )


### Link directories
target_link_libraries( test_dprec ${CLN_LIB} ${GINAC_LIB} gtest_main )
target_link_libraries( test_arprec ${CLN_LIB} ${GINAC_LIB} gtest_main )

### Tests
enable_testing()

add_test( NAME runUnitTests_dprec  COMMAND test_dprec WORKING_DIRECTORY
 ${EXECUTABLE_OUTPUT_PATH} )
add_test( NAME runUnitTest_arprec COMMAND test_arprec WORKING_DIRECTORY
 ${EXECUTABLE_OUTPUT_PATH} )
