cmake_minimum_required(VERSION 3.10)

project(MRenderer)

set(CMAKE_CXX_STANDARD 17)
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

message("${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE REMOVE_CMAKE "CMakeFiles/*")

file(GLOB_RECURSE HEADER_FILES
  "${PROJECT_SOURCE_DIR}/*.h"
)


add_executable(MRenderer ${HEADER_FILES} ${SOURCE_FILES})

cmake_policy(SET CMP0072 NEW)
set(CMAKE_BUILD_TYPE Debug)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Linux specific
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

TARGET_LINK_LIBRARIES(MRenderer ${GTK3_LIBRARIES})

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

message("${INCLUDE_DIR}/GLFW")

include_directories(
  "${PROJECT_SOURCE_DIR}"
  "${INCLUDE_DIR}"
  "${INCLUDE_DIR}/glfw/include"
  "${INCLUDE_DIR}/imgui"
)

add_subdirectory(include/glad)

target_link_libraries(
  MRenderer
  glfw
  ${OPENGL_LIBRARIES}
  ${X11_LIBRARIES}
  ${CMAKE_DL_LIBS}
  glad
  assimp
)

target_include_directories(MRenderer PUBLIC "${PROJECT_SOURCE_DIR}")

add_custom_command(
         TARGET MRenderer POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/MaroRenderer/Engine/Shaders $<TARGET_FILE_DIR:MRenderer>/Engine/Shaders
         COMMENT "Copying shaders" VERBATIM
         )

add_custom_command(
          TARGET MRenderer POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory              
              ${CMAKE_SOURCE_DIR}/MaroRenderer/Assets $<TARGET_FILE_DIR:MRenderer>/Assets
          COMMENT "Copying assets" VERBATIM
          )

