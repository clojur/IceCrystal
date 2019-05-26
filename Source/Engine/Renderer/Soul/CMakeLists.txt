set(LibName Soul)

set(SDKsDir ${PROJECT_SOURCE_DIR}/SDKs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

macro(MakeGroup subDir)
	file(GLOB CodeFiles ${subDir}/*.h ${subDir}/*.cpp)
	set(${subDir}Files
			${CodeFiles}
	)
	source_group(${subDir} FILES ${${subDir}Files})
endmacro()

MakeGroup(core)
MakeGroup(math)
MakeGroup(render)
MakeGroup(resource)
MakeGroup(scenegraph)
MakeGroup(taskgraph)
MakeGroup(ui)
MakeGroup(util)
MakeGroup(stbi)
MakeGroup(tinyxml)

list(APPEND
	common
	common/SoulMacro.h
	common/Platform.h
	common/pmath.h
	common/RenderContext.h
	common/RenderContext.cpp
	common/glew.c
)
set(commonFiles ${common})
source_group(common FILES ${commonFiles})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(ExternLib  opengl32 glu freeglutd  glfw3  pthreadVSE2)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(ExternLib opengl32 glu freeglut  glfw3  pthreadVSE2)
else()
	set(ExternLib opengl32 glu freeglut  glfw3  pthreadVSE2)
endif()

add_definitions(-DSoul_EXPORTS)

add_library(${LibName} SHARED
 ${commonFiles}
 ${coreFiles}
 ${mathFiles}
 ${renderFiles}
 ${resourceFiles}
 ${scenegraphFiles}
 ${taskgraphFiles}
 ${uiFiles}
 ${utilFiles}
 ${stbiFiles}
 ${tinyxmlFiles}
)

if(WIN32)
	set_target_properties(${LibName}  
		PROPERTIES
		LINK_FLAGS /SAFESEH:NO
	)
endif()

target_link_libraries(${LibName}  ${ExternLib})

set_property(TARGET ${LibName} PROPERTY FOLDER "Engine")
