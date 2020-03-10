
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(test_neuron VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_neuron)

target_sources(test_neuron
        PRIVATE
        include/tests.h

        main.cpp
        src/test_neuron.cpp
        )

target_include_directories(test_neuron
        PRIVATE
        include
        )

target_link_libraries(test_neuron
        PRIVATE
        avn_neuron
        )

target_compile_definitions(test_neuron
        PRIVATE
        )