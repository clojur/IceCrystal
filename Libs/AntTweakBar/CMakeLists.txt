cmake_minimum_required(VERSION 3.12)
project(AntTweakBar)



# Project includes
set(ATB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/SDKs/AntTweakBar/include)

# Standard Packages
set(OPENGL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/SDKs/opengl/include)

set(GLFW_DIR ${CMAKE_SOURCE_DIR}/SDKs/glfw/)

include_directories( 
   ${ATB_INCLUDE_DIR} 
   ${OPENGL_INCLUDE_DIR} 
   ${GLFW_DIR}/include
)

link_directories(
${CMAKE_SOURCE_DIR}/SDKs/opengl/lib
${GLFW_DIR}/lib
)

set( SOURCES 
    ../../SDKs/AntTweakBar/src/LoadOGL.cpp
	../../SDKs/AntTweakBar/src/LoadOGLCore.cpp
    ../../SDKs/AntTweakBar/src/TwColors.cpp
    ../../SDKs/AntTweakBar/src/TwMgr.cpp
    ../../SDKs/AntTweakBar/src/TwPrecomp.cpp
    ../../SDKs/AntTweakBar/src/TwEventWin32.c
    ../../SDKs/AntTweakBar/src/TwOpenGL.cpp
    ../../SDKs/AntTweakBar/src/TwBar.cpp
    ../../SDKs/AntTweakBar/src/TwFonts.cpp
    ../../SDKs/AntTweakBar/src/TwEventGLFW.c
    ../../SDKs/AntTweakBar/src/TwOpenGLCore.cpp
    ../../SDKs/AntTweakBar/src/TwDirect3D9.cpp
    ../../SDKs/AntTweakBar/src/TwDirect3D10.cpp
)


add_library(AntTweakBar STATIC 
${SOURCES}
)

message(Extra libs ${EXTRA_LIBS})
target_link_libraries(AntTweakBar  
   opengl32
   glut32
   glfw3
   ${EXTRA_LIBS})

set_property(TARGET AntTweakBar PROPERTY FOLDER "libs")