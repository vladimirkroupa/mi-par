cmake_minimum_required(VERSION 2.8)

project(kgm)

set(CMAKE_CXX_FLAGS "-Wall")

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_executable(kgm UndirectedGraph.cpp Edge.cpp DFSSolver.cpp SquareMatrix.cpp SpanningTree.cpp main.cpp Packer.cpp)

target_link_libraries(kgm ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
  set_target_properties(kgm PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(kgm PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
