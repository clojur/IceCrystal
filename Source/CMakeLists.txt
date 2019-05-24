
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(NoesisLib ${PROJECT_SOURCE_DIR}/SDKs/NoesisGUI/Bin/windows_x86)
set(GLFWLib ${PROJECT_SOURCE_DIR}/SDKs/glfw/lib/${CMAKE_BUILD_TYPE})
set(GLuLib ${PROJECT_SOURCE_DIR}/SDKs/opengl/lib/${CMAKE_BUILD_TYPE}/GLU.DLL)
set(openglLib ${PROJECT_SOURCE_DIR}/SDKs/opengl/lib/${CMAKE_BUILD_TYPE}/opengl.DLL)
set(pThreadLib ${PROJECT_SOURCE_DIR}/SDKs/pthread/lib/${CMAKE_BUILD_TYPE}/pthreadVSE2.dll)


if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(orkLib  ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/orkd.dll)
	set(prolandLib  ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/Prolandd.dll)
else()
     set(orkLib  ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/ork.dll)
	 set(prolandLib  ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/Proland.dll)
endif()

set(OutDir ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})

add_custom_command(OUTPUT  COPY_RES
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${NoesisLib} ${OutDir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLFWLib} ${OutDir}
  COMMAND ${CMAKE_COMMAND} -E copy ${GLuLib} ${OutDir}
  COMMAND ${CMAKE_COMMAND} -E copy ${openglLib} ${OutDir}
  COMMAND ${CMAKE_COMMAND} -E copy ${pThreadLib} ${OutDir}
  COMMAND ${CMAKE_COMMAND} -E copy ${orkLib} ${OutDir}
  COMMAND ${CMAKE_COMMAND} -E copy ${prolandLib} ${OutDir}
)

add_custom_target(CopyTask ALL DEPENDS COPY_RES)

add_subdirectory(OrkRender)
add_subdirectory(App)
add_subdirectory(Proland)