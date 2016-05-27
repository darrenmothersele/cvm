cmake_minimum_required(VERSION 3.5)
project(cy2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)
set(TARGET_LINKS ${TARGET_LINKS} glfw ${GLFW_LIBRARIES})

set(SOURCE_FILES main.cpp)
add_executable(cy2 ${SOURCE_FILES})
target_link_libraries(cy2 ${TARGET_LINKS})
