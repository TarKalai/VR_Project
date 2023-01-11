#Template for INFOH502
#Heavily inspired from https://github.com/Polytonic/Glitter/blob/master/CMakeLists.txt
include(FetchContent)

#project setup
project(infoh502-cpp)
#set the minimal version of cmake and the minimal version of the c++ language
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)     
set(CMAKE_VERBOSE_MAKEFILE ON)

#define some variable
set(COMPILE_SRC ON CACHE BOOL "Compile Source Project")
set(COMPILE_SRC2 ON CACHE BOOL "Compile Source Normal")



find_package(OpenGL REQUIRED)

# # for glew
# add_subdirectory(3rdParty/glew/bin/Release/Win32/)

# add_library( glew STATIC )
# find_package(GLEW REQUIRED)
# set(GLEW_INCLUDE "3rdParty/glew/bin/Release/Win32/")
# target_link_libraries(YourTarget GLEW::GLEW)
# # include_directories(${GLEW_INCLUDE_DIRS})
# # link_libraries(${GLEW_LIBRARIES})

#for glad library
add_library( glad STATIC 3rdParty/glad/src/glad.c)
set(GLAD_INCLUDE "3rdParty/glad/include")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE})

#for GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)


add_subdirectory(3rdParty/glfw)
add_subdirectory(3rdParty/bullet3)
add_subdirectory(3rdParty/assimp)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG        v1.88
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    message("Fetching imgui")
    FetchContent_Populate(imgui)

    add_library(imgui
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)

    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${glfw_SOURCE_DIR}/include)

    target_link_libraries(imgui PRIVATE glfw)
endif ()


include_directories(3rdParty/glad/include/
                    3rdParty/glfw/include/
                    3rdParty/glm/
                    3rdParty/stb/
                    3rdParty/bullet3/src/
                    3rdParty/assimp/include/)


if(COMPILE_SRC)
	add_subdirectory(src)
endif()

if(COMPILE_SRC2)
	add_subdirectory(src_normal)
endif()