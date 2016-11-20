FROM ubuntu

# dependencies
RUN apt-get update -y
RUN apt-get install -y sudo apt-utils software-properties-common
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test
RUN apt-get update -y
RUN apt-get install -y cmake build-essential gcc-6 g++-6 mpich python python-pip git
RUN apt-get install -y xorg-dev mesa-common-dev libglu1-mesa-dev libglew-dev
RUN pip install conan

# project files
ADD *.hpp /project/
ADD *.cpp /project/
ADD *.glsl /project/
ADD CMakeLists.txt /project/
ADD conanfile.txt /project/

# build
WORKDIR /project
RUN conan install --build missing
WORKDIR /project/build
RUN cmake -D CMAKE_CXX_COMPILER=`which mpicxx` ..
RUN cmake --build .
WORKDIR /project

# mpi ports
RUN export MPIR_CVAR_CH3_PORT_RANGE=10000:10010
EXPOSE 10000-10010

# default run command
CMD mpirun ./build/bin/Raytracer
