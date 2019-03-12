set(EXENAME App)

set(SourceDir ${PROJECT_SOURCE_DIR}/Source)
include_directories(
 ${SourceDir}/OrkRender/libraries
 ${SourceDir}/OrkRender
 ${PROJECT_SOURCE_DIR}/SDKs/glew/include
  ${PROJECT_SOURCE_DIR}/SDKs/GLFW/include
)

add_definitions("-D DISABLE_MSVC_WARNINGS")

link_directories()

file(GLOB SOURCE_FILES *.cpp *.h)

add_executable(${EXENAME} ${SOURCE_FILES})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(ExternLib "orkd")
else()
	set(ExternLib "ork")
endif()

#Added whole archive due to the "plug-in usage"
target_link_libraries(${EXENAME} ${ExternLib})
