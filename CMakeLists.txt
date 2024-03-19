cmake_minimum_required(VERSION 3.7...3.28)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(ParametricVisions)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)

#-------------------------------------------------------------------------------
# https://github.com/adishavit/argh/releases/tag/v1.3.1
include_directories(SYSTEM include/argh-1.3.1/)

#-------------------------------------------------------------------------------
# https://glad.dav1d.de/
add_subdirectory(include/glad)
set(LIBRARIES ${LIBRARIES} glad)

#-------------------------------------------------------------------------------
# https://www.glfw.org/

# Turn off building their docs/tests/examples.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(include/glfw-3.3.2)
set(LIBRARIES ${LIBRARIES} glfw)

#-------------------------------------------------------------------------------
# https://github.com/gurki/vivid/releases/tag/v2.2.1
include_directories(SYSTEM include/vivid-2.2.1/include)
include_directories(SYSTEM include/vivid-2.2.1/dependencies/glm)

#-------------------------------------------------------------------------------
# https://glm.g-truc.net/0.9.9/index.html
include_directories(SYSTEM include/glm-0.9.9.7/)


include_directories(SYSTEM include/imgui-docking-1.90.5)


find_package(OpenGL REQUIRED)
set(LIBRARIES ${LIBRARIES} ${OPENGL_gl_LIBRARY})


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	list(APPEND _589_689_CMAKE_CXX_FLAGS ${_589_689_CMAKE_CXX_FLAGS} "-Wall" "-pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using GCC
	list(APPEND _589_689_CMAKE_CXX_FLAGS ${_589_689_CMAKE_CXX_FLAGS} "-fdiagnostics-color=always" "-Wall" "-pedantic" "-Wconversion")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	# using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_compile_options("/permissive-")
	add_compile_options("/Zc:preprocessor")
	# Remove the default warning level.
	# https://stackoverflow.com/questions/58708772/cmake-project-in-visual-studio-gives-flag-override-warnings-command-line-warnin
	string(REGEX REPLACE "/W[3|4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	 # The above preprocessor warns about certain things in windows headers.
	 # Turn those off: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c5105?view=vs-2019
	add_compile_options("/wd5105")

	set(_589_689_CMAKE_CXX_FLAGS ${_589_689_CMAKE_CXX_FLAGS} /W3)

endif()

if(UNIX)
	set(LIBRARIES ${LIBRARIES} pthread GL dl)
endif(UNIX)
if (WIN32)
endif()


add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD=ON)
# include_directories(SYSTEM include/imgui include/imgui/examples)
# include_directories(src)


# Compile our main application
file(GLOB SOURCES
    src/*
	include/imgui-docking-1.90.5/*.cpp
)
set(INCLUDES ${INCLUDES} src)

set(APP_NAME "visions")


# Copy all the shaders and tell the build system to re-run CMAKE if one of them changes
file(GLOB files src/shaders/*)
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	configure_file(${file} shaders/${name})
endforeach()

add_executable(${APP_NAME} ${SOURCES})
target_include_directories(${APP_NAME} PRIVATE ${INCLUDES})
target_link_libraries(${APP_NAME} ${LIBRARIES})
target_compile_definitions(${APP_NAME} PRIVATE ${DEFINITIONS})
target_compile_options(${APP_NAME} PRIVATE ${_589_689_CMAKE_CXX_FLAGS})
set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH "./" BUILD_RPATH "./")
