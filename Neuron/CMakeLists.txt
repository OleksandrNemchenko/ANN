
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(avn_neuron VERSION 0.1.0 LANGUAGES CXX)

add_library(avn_neuron)

target_include_directories(avn_neuron
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_sources(avn_neuron
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/avn/neuron/neuron_base.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/avn/neuron/neuron_input.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/avn/neuron/neuron_sum.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/avn/neuron/sigmoid.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/avn/neuron/gaussian.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/avn/neuron/variable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gaussian.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sigmoid.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neuron_base.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neuron_sum.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/variable.cpp
        )
