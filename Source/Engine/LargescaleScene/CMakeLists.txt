set(LibName LargescaleScene)

set(SDKsDir ${PROJECT_SOURCE_DIR}/SDKs)
include_directories(
${SOURCE_DIR}
${SOURCE_DIR}/Engine
${SOURCE_DIR}/Engine/Renderer
${SOURCE_DIR}/Engine/Renderer/Soul
${SOURCE_DIR}/Engine/Renderer/Soul/common
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/common
${SDKsDir}
${SDKsDir}/pthread/include
${SDKsDir}/tiff-4.0.4/libtiff
${SDKsDir}/AntTweakBar/include
${PROJECT_SOURCE_DIR}/libs/tiff
)

macro(MakeGroup subDir)
	file(GLOB CodeFiles ${subDir}/*.h ${subDir}/*.cpp)
	set(${subDir}Files
			${CodeFiles}
	)
	source_group(${subDir} FILES ${${subDir}Files})
endmacro()

macro(MakeSubGroup parentPath myName )
	file(GLOB 
		CodeFiles
		${parentPath}/${myName}/*.h 
		${parentPath}/${myName}/*.cpp
	)
	set(${parentPath}_${myName}Files ${CodeFiles})
	source_group(${parentPath}\\${myName} FILES ${${parentPath}_${myName}Files})
endmacro()

macro(MakeSubSubGroup FirstPath SecondPath myPath )
	file(GLOB 
		CodeFiles
		${FirstPath}/${SecondPath}/${myPath}/*.h 
		${FirstPath}/${SecondPath}/${myPath}/*.cpp
	)
	set(${FirstPath}_${SecondPath}_${myPath}Files ${CodeFiles})
	source_group(${FirstPath}\\${SecondPath}\\${myPath} FILES ${${FirstPath}_${SecondPath}_${myPath}Files})
endmacro()

MakeGroup(math)

MakeGroup(particles)
MakeSubGroup(particles screen)
MakeSubGroup(particles terrain)

MakeGroup(producer)

MakeGroup(terrain)

MakeGroup(ui)
MakeSubGroup(ui twbar)

MakeGroup(util)

MakeGroup(plugin)

MakeGroup(plants)

MakeGroup(preprocess)
MakeSubGroup(preprocess atmo)

MakeGroup(ocean)

MakeGroup(edit)

MakeGroup(graph)
MakeSubGroup(graph graph)
MakeSubSubGroup(graph graph producer)
MakeSubGroup(graph ortho)
MakeSubGroup(graph dem)

MakeGroup(rivers)
MakeSubGroup(rivers graph)


list(APPEND
	common
	common/LargescaleSceneMacro.h
)
set(commonFiles ${common})
source_group(common FILES ${commonFiles})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(ExternLib  
	opengl32 
	glu 
	freeglutd  
	glfw3  
	pthreadVSE2 
	Sould 
	tiffd 
	zlibd
	AntTweakBard
)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(ExternLib 
	opengl32 
	glu 
	freeglut 
	glfw3  
	pthreadVSE2 
	Soul 
	tiff 
	zlib
	AntTweakBar
)
else()
	set(ExternLib 
	opengl32 
	glu 
	freeglut  
	glfw3  
	pthreadVSE2 
	Soul 
	tiff 
	zlib
	AntTweakBar
)
endif()

add_definitions(
-D LargescaleScene_EXPORTS
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
  ${particles_screenFiles}
  ${particles_terrainFiles}
  ${graphFiles}
  ${graph_graphFiles}
  ${graph_graph_producerFiles}
  ${graph_demFiles}
  ${graph_orthoFiles}
  ${riversFiles}
  ${rivers_graphFiles}
  ${ui_twbarFiles}
  ${producerFiles}
  ${terrainFiles}
  ${uiFiles}
  ${utilFiles}
  ${pluginFiles}
  ${plantsFiles}
  ${preprocess_atmoFiles}
  ${oceanFiles}
  ${editFiles}
)

if(WIN32)
	set_target_properties(${LibName}  
		PROPERTIES
		LINK_FLAGS /SAFESEH:NO
	)
endif()

target_link_libraries(${LibName}  ${ExternLib})

set_property(TARGET ${LibName} PROPERTY FOLDER "Engine")
