
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ArtificialNerualNetwork VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Neuron)
add_subdirectory(Network)

add_subdirectory(Test)