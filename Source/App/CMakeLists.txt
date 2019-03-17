set(EXENAME App)

set(SourceDir ${PROJECT_SOURCE_DIR}/Source)

#外部库目录
include_directories(
 ${SourceDir}/OrkRender/libraries
 ${SourceDir}/OrkRender
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

#筛选器
source_group(MainApp FILES ${MainAppSource})
source_group(ResourceFiles\\meshes FILES ${MeshFiles})
source_group(ResourceFiles\\methods FILES ${MethodsFiles})
source_group(ResourceFiles\\scenes FILES ${ScenesFiles})
source_group(ResourceFiles\\shaders FILES ${ShadersFiles})
source_group(ResourceFiles\\textures FILES ${TexturesFiles})

add_executable(${EXENAME} 
	${MainAppSource}
	${MeshFiles}
	${MethodsFiles}
	${ScenesFiles}
	${ShadersFiles}
	${TexturesFiles}
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(ExternLib "orkd")
else()
	set(ExternLib "ork")
endif()

#Added whole archive due to the "plug-in usage"
target_link_libraries(${EXENAME} ${ExternLib})

