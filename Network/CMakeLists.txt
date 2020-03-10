
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(avn_neuron_network VERSION 0.1.0 LANGUAGES CXX)

add_library(avn_neuron_network)

target_include_directories(avn_neuron_network
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_sources(avn_neuron_network
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/avn/neuron/network.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network.cpp
        )

target_link_libraries(avn_neuron_network
        PUBLIC
        avn_neuron
        )
