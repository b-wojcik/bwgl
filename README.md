*BWGL* is a small, header only C++ rendering framework built on **OpenGL 4.4**, which provides reusable abstractions for common tasks such as window handling, shader and texture caching, camera system and model loading.
<br/> <br/>
It is designed to provide the foundational systems needed to get an OpenGL project up and running without having to build them from scratch.

## CMake using FetchContent
You can add *BWGL* to your CMake project to be built together.

Add this code to the `CMakeLists.txt` file:
```cmake_code
cmake_minimum_required(VERSION 3.16)

include(FetchContent)

# BWGL
FetchContent_Declare(
    bwgl
    GIT_REPOSITORY https://github.com/b-wojcik/bwgl.git
    GIT_TAG main
)

FetchContent_MakeAvailable(bwgl)
```
