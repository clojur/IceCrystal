include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SDKDir ${PROJECT_SOURCE_DIR}/SDks)

#depend extern library
macro(DependLibrary libName)
set(${libName}Include ${SDKDir}/${libName}/include)
set(${libName}Lib ${SDKDir}/${libName}/lib/${CMAKE_BUILD_TYPE})

include_directories(${${libName}Include})
link_directories(${${libName}Lib})
endmacro()

DependLibrary(freeglut)
DependLibrary(glfw)
DependLibrary(pthread)
DependLibrary(opengl)
#pre define macro
add_definitions(
    "-D DUSE_SHARED_PTR"
    "-D USEFREEGLUT"
    "-D _CRT_SECURE_NO_WARNINGS"
    "-D HAVE_STRUCT_TIMESPEC"
    "-D TIXML_USE_STL"
	"-D DISABLE_MSVC_WARNINGS"
	"-D GLEW_STATIC"
)       
add_subdirectory(ork)