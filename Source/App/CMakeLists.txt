set(EXENAME App)

set(SourceDir ${PROJECT_SOURCE_DIR}/Source)


include_directories(
	${SOURCE_DIR}
	${SOURCE_DIR}/Engine
	${SOURCE_DIR}/Engine/Renderer
	${SourceDir}/Engine/Renderer/libraries
	${SourceDir}/Engine/LargescaleScene
	${SourceDir}/Engine/LargescaleScene/common
	${PROJECT_SOURCE_DIR}/SDKs/AntTweakBar/include
	${PROJECT_SOURCE_DIR}/SDKs/glew/include
	${PROJECT_SOURCE_DIR}/SDKs/GLFW/include
	${PROJECT_SOURCE_DIR}/SDKs/tiff-4.0.4/libtiff
)


add_definitions(
"-D DISABLE_MSVC_WARNINGS"
"-D _CRT_SECURE_NO_WARNINGS"
)


file(GLOB MeshFiles ResourceFiles/meshes/*.*)
file(GLOB MethodsFiles ResourceFiles/methods/*.*)
file(GLOB ScenesFiles ResourceFiles/scenes/*.*)
file(GLOB ShadersFiles ResourceFiles/shaders/*.*)
file(GLOB TexturesFiles ResourceFiles/textures/*.*)


list(APPEND 
MainAppSource

 MainApp/Main.h
 MainApp/Main.cpp
 MainApp/Application.h
 MainApp/Application.cpp
 MainApp/WindowsApplication.h
 MainApp/WindowsApplication.cpp
 MainApp/RenderExampleGLFW.cpp
)

list(APPEND
LargescaleSceneSource
	LargescaleSceneApp/examples/EarthDemo.cpp
	LargescaleSceneApp/examples/core/Core.cpp
	LargescaleSceneApp/examples/forest/trees1/Trees1.cpp
	LargescaleSceneApp/examples/ocean/ocean1/ocean1.cpp
	LargescaleSceneApp/examples/ocean/ocean2/ocean2.cpp
)


source_group(MainApp FILES ${MainAppSource})
Source_group(LargescaleSceneApp FILES ${LargescaleSceneSource})
source_group(ResourceFiles\\meshes FILES ${MeshFiles})
source_group(ResourceFiles\\methods FILES ${MethodsFiles})
source_group(ResourceFiles\\scenes FILES ${ScenesFiles})
source_group(ResourceFiles\\shaders FILES ${ShadersFiles})
source_group(ResourceFiles\\textures FILES ${TexturesFiles})

add_definitions(
 -D TIXML_USE_STL
)

add_executable(${EXENAME}
	${MainAppSource}
	${LargescaleSceneSource}
	${MeshFiles}
	${MethodsFiles}
	${ScenesFiles}
	${ShadersFiles}
	${TexturesFiles}
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(ExternLib Sould LargescaleScened tiffd zlibd)
else()
	set(ExternLib Soul LargescaleScene tiff zlib)
endif()

#Added whole archive due to the "plug-in usage"
target_link_libraries(${EXENAME} ${ExternLib})

