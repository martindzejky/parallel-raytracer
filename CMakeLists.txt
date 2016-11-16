project(Raytracer)


# meta
cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-6")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")


# dependencies
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIR})
link_directories(${OpenGL_LIBRARY_DIRS})

# conan dependencies
include(conanbuildinfo.cmake)
conan_basic_setup()
set(CMAKE_SKIP_RPATH 0)


# project files
set(SOURCE_FILES
    Error.cpp
    FullscreenQuad.cpp
    Input.cpp
    Main.cpp
    Shaders.cpp
    Raytracer.cpp
    Texture.cpp
    Window.cpp
    )


# compile & link project
add_executable(Raytracer ${SOURCE_FILES})
target_link_libraries(Raytracer ${CONAN_LIBS} ${OPENGL_LIBRARY})
