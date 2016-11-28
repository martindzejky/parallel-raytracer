# ParallelRaytracer

This is a CPU OpenGL raytracer - C++, OpenMP, MPI, OpenGL. It's a school project.

## Building

Download and install [Conan](https://www.conan.io/downloads), it's used to manage the project dependencies. If you don't have [CMake](https://cmake.org/download/), install that as well.

Also make sure you have [g++](https://gcc.gnu.org/install/), [mpich](http://www.mpich.org/downloads/), and OpenGL installed.

Go into the project directory and run:

```
conan install --build missing
mkdir build
cd build
cmake ..
cmake --build .
cd ..
./build/bin/Raytracer
```
