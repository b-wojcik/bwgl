cmake_minimum_required(VERSION 3.16)

project("bwgl" LANGUAGES C CXX)

include(FetchContent)

# ------------------------------------------------------------
# GLFW
# ------------------------------------------------------------

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

# ------------------------------------------------------------
# GLAD 2 — OpenGL 4.4 Core
# ------------------------------------------------------------

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glfw glad)

glad_add_library(
    glad_gl_core_44
    STATIC
    REPRODUCIBLE
    API gl:core=4.4
)

# ------------------------------------------------------------
# Application
# ------------------------------------------------------------

add_executable(
    bwgl
    src/main.cpp
    src/bwgl/bwgl.hpp
    src/bwgl/core/window.hpp
    src/bwgl/core/input.hpp
)

target_compile_features(
    bwgl
    PRIVATE
        cxx_std_17
)

target_link_libraries(
    bwgl
    PRIVATE
        glfw
        glad_gl_core_44
)