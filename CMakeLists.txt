#-------------------------------------
# CMakeFile to compile 
# SudokuBreaker (c) 2011 by Tim Bolender
#
# By default, Release mode is chosen
#-------------------------------------

CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( SudokuBreaker )

#
# Set Important Variables
#

# Set Program Version
SET( SudokuBreaker_VERSION_MAJOR 1 )
SET( SudokuBreaker_VERSION_MINOR 37 )
SET( SudokuBreaker_VERSION_PATCH 0 )
SET( SudokuBreaker_VERSION "${SudokuBreaker_VERSION_MAJOR}.${SudokuBreaker_VERSION_MINOR}" )
SET( SudokuBreaker_VERSION_FULL "${SudokuBreaker_VERSION_MAJOR}.${SudokuBreaker_VERSION_MINOR}.${SudokuBreaker_VERSION_PATCH}" )

# If No Build Type Was Set We Assume Release
IF( NOT CMAKE_BUILD_TYPE )
	SET( CMAKE_BUILD_TYPE Release )
ENDIF( NOT CMAKE_BUILD_TYPE )
MESSAGE( STATUS "Program will be build in ${CMAKE_BUILD_TYPE} mode!" )

# Set Package Settings
INCLUDE( InstallRequiredSystemLibraries )
SET( CPACK_PACKAGE_NAME SudokuBreaker )
SET( CPACK_PACKAGE_VERSION "${SudokuBreaker_VERSION_FULL}" )
SET( CPACK_PACKAGE_VERSION_MAJOR "${SudokuBreaker_VERSION_MAJOR}" )
SET( CPACK_PACKAGE_VERSION_MINOR "${SudokuBreaker_VERSION_MINOR}" )
SET( CPACK_PACKAGE_VERSION_PATCH "${SudokuBreaker_VERSION_PATCH}" )
SET( CPACK_GENERATOR "ZIP" )
SET( SRC_ARCHIVE_NAME "${CMAKE_PROJECT_NAME}-${SudokuBreaker_VERSION_MAJOR}.${SudokuBreaker_VERSION_MINOR}.${SudokuBreaker_VERSION_PATCH}-Source" )
ADD_CUSTOM_TARGET( dist
    COMMAND git archive --prefix=${SRC_ARCHIVE_NAME}/ HEAD
        | bzip2 > ${CMAKE_BINARY_DIR}/${SRC_ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )

#
# Add Executable
#

# Add Src and Include Directory to Path
SET( SRC_DIR ${CMAKE_SOURCE_DIR}/src )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/include )

# Get Src Files for Executable
INCLUDE( ${CMAKE_SOURCE_DIR}/src/Sources.cmake )

# Create Executable
ADD_EXECUTABLE( sudokubreaker WIN32 ${SRCs} )

# Install Executable
INSTALL( TARGETS sudokubreaker DESTINATION "." )

# Create Complete Packages
INCLUDE( CPack )
