cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(obj-renderer)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/ext/project_path.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/ext/project_path.hpp")

set(glm_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glm/cmake/glm)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

file(GLOB SOURCE_FILES src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC src/)
target_include_directories(${PROJECT_NAME} PUBLIC ext/)
target_include_directories(${PROJECT_NAME} PUBLIC ext/gl3w/)
target_include_directories(${PROJECT_NAME} PUBLIC ext/glm/)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})

if (OPENGL_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
endif()

if (NOT GLFW_FOUND)
    message(FATAL_ERROR "Can't find GLFW." )
endif()