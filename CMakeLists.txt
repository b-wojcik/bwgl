cmake_minimum_required(VERSION 3.16)

project(bwgl
    VERSION 1.0.0
    LANGUAGES C CXX
)

include(FetchContent)

# GLFW
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

FetchContent_MakeAvailable(glfw)

# GLAD 2 - OpenGL 4.4 Core
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)

glad_add_library(
    glad_gl_core_44
    STATIC
    REPRODUCIBLE
    API gl:core=4.4
)

# GLM
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glm)

# BWGL
add_library(bwgl INTERFACE)
add_library(bwgl::bwgl ALIAS bwgl)

target_compile_features(
    bwgl
    INTERFACE
        cxx_std_20
)

target_include_directories(
    bwgl
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    bwgl
    INTERFACE
        glfw
        glad_gl_core_44
        glm::glm
)
