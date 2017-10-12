cmake_minimum_required(VERSION 3.8)
project(mpi_barrier)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES ./main.c)

add_executable(mpi_barrier ${SOURCE_FILES})