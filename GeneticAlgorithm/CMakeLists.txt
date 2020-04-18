
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(avn_genetic_algorithm VERSION 0.1.0 LANGUAGES CXX)

add_library(avn_genetic_algorithm)

target_include_directories(avn_genetic_algorithm
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_sources(avn_genetic_algorithm
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/avn/neuron/genetic-algorithm.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/genetic-algorithm.cpp
        )

target_link_libraries(avn_genetic_algorithm
        PUBLIC
        avn_neuron
        )
