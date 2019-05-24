set(LibName Proland)

set(SDKsDir ${PROJECT_SOURCE_DIR}/SDKs)
include_directories(
${SOURCE_DIR}
${SOURCE_DIR}/OrkRender
${SOURCE_DIR}/OrkRender/ork
${SOURCE_DIR}/OrkRender/ork/common
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/common
${SDKsDir}
${SDKsDir}/pthread/include
)

macro(MakeGroup subDir)
	file(GLOB CodeFiles ${subDir}/*.h ${subDir}/*.cpp)
	set(${subDir}Files
			${CodeFiles}
	)
	source_group(${subDir} FILES ${${subDir}Files})
endmacro()

MakeGroup(math)
MakeGroup(particles)
MakeGroup(particles/screen)
MakeGroup(particles/terrain)
MakeGroup(producer)
MakeGroup(terrain)
MakeGroup(ui)
#MakeGroup(ui/twbar)
MakeGroup(util)

list(APPEND
	common
	common/ProlandMacro.h
)
set(commonFiles ${common})
source_group(common FILES ${commonFiles})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(ExternLib  opengl32 glu freeglutd  glfw3  pthreadVSE2 orkd)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(ExternLib opengl32 glu freeglut  glfw3  pthreadVSE2 ork)
else()
	set(ExternLib opengl32 glu freeglut  glfw3  pthreadVSE2 ork)
endif()

add_definitions(
-D PROLAND_EXPORTS
-D DISABLE_MSVC_WARNINGS
-D TIXML_USE_STL
-D HAVE_STRUCT_TIMESPEC
)


#depend extern library
macro(DependLibrary libName)
set(${libName}Include ${SDKsDir}/${libName}/include)
set(${libName}Lib ${SDKsDir}/${libName}/lib/${CMAKE_BUILD_TYPE})

include_directories(${${libName}Include})
link_directories(${${libName}Lib})
endmacro()

DependLibrary(freeglut)
DependLibrary(glfw)
DependLibrary(pthread)
DependLibrary(opengl)

add_library(${LibName} SHARED
 ${commonFiles}
 ${mathFiles}
 ${particlesFiles}
 ${particles/screenFiles}
 ${particles/terrainFiles}
 ${producerFiles}
 ${terrainFiles}
 ${uiFiles}
 ${utilFiles}
 #${ui/twbarFiles}
)

if(WIN32)
	set_target_properties(${LibName}  
		PROPERTIES
		LINK_FLAGS /SAFESEH:NO
	)
endif()

target_link_libraries(${LibName}  ${ExternLib})
