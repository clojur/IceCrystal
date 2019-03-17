set(LibName ork)

set(SDKsDir ${PROJECT_SOURCE_DIR}/SDKs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${SDKsDir}/glew/include)


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
	common/macro.h
	common/Platform.h
	common/pmath.h
	common/SoulInit.h
	common/SoulInit.cpp
)
set(commonFiles ${common})
source_group(common FILES ${commonFiles})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(ExternLib  opengl32 glu freeglutd glew32d glfw3  pthreadVSE2)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(ExternLib opengl32 glu freeglut glew32 glfw3  pthreadVSE2)
else()
	set(ExternLib opengl32 glu freeglut glew32 glfw3  pthreadVSE2)
endif()


if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

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
