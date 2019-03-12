cmake_minimum_required(VERSION 3.2)

project(OrkFrame)

IF(APPLE)
	SET(CMAKE_CXX_COMPILER "clang") 
elseif (UNIX)
	SET(CMAKE_CXX_COMPILER "g++")         #设置编译器（可选的有gcc，g++）
ENDIF ()

IF (UNIX)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )
ENDIF(UNIX)

set(CMAKE_BUILD_TYPE Debug)
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 
SET( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_subdirectory(Source)