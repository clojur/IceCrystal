cmake_minimum_required(VERSION 3.12)

project(IceCrystal)

set(CMAKE_BUILD_TYPE "Debug")

IF(WIN32)
	IF(CMAKE_CL_64)
		set(PLATFORM "x64")
		add_compile_options(-std=c++11 /MP)
	ELSE(CMAKE_CL_64)
		set(PLATFORM "x86")
		add_compile_options(-std=c++11 /MP)
	ENDIF(CMAKE_CL_64)
ENDIF(WIN32)

IF(APPLE)
	SET(CMAKE_CXX_COMPILER "clang") 
elseif (UNIX)
	SET(CMAKE_CXX_COMPILER "g++")         #设置编译器（可选的有gcc，g++）
ENDIF ()

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()#添加debug后缀d


IF (UNIX)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )
ENDIF(UNIX)

set_property(GLOBAL PROPERTY USE_FOLDERS On)#可用解决方案文件夹

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${PLATFORM}/bin) 
set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/${PLATFORM}/lib)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/${PLATFORM}/lib)

add_subdirectory(Source)
add_subdirectory(Libs)