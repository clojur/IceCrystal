set(EXENAME App)

set(SourceDir ${PROJECT_SOURCE_DIR}/Source)

#外部库目录
include_directories(
 ${SOURCE_DIR}
 ${SourceDir}/OrkRender/libraries
 ${SourceDir}/OrkRender
 ${SourceDir}/Proland
  ${SourceDir}/Proland/common
 ${PROJECT_SOURCE_DIR}/SDKs/glew/include
  ${PROJECT_SOURCE_DIR}/SDKs/GLFW/include
)

#预处理定义
add_definitions(
"-D DISABLE_MSVC_WARNINGS"
"-D _CRT_SECURE_NO_WARNINGS"
)

#资源文件
file(GLOB MeshFiles ResourceFiles/meshes/*.*)
file(GLOB MethodsFiles ResourceFiles/methods/*.*)
file(GLOB ScenesFiles ResourceFiles/scenes/*.*)
file(GLOB ShadersFiles ResourceFiles/shaders/*.*)
file(GLOB TexturesFiles ResourceFiles/textures/*.*)


#源文件
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
ProlandSource
 ProlandApp/examples/core/Core.cpp
 ProlandApp/examples/forest/trees1/Trees1.cpp
)


#筛选器
source_group(MainApp FILES ${MainAppSource})
Source_group(ProlandApp FILES ${ProlandSource})
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
	${ProlandSource}
	${MeshFiles}
	${MethodsFiles}
	${ScenesFiles}
	${ShadersFiles}
	${TexturesFiles}
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(ExternLib "orkd" "prolandd")
else()
	set(ExternLib "ork" "proland")
endif()

#Added whole archive due to the "plug-in usage"
target_link_libraries(${EXENAME} ${ExternLib})

