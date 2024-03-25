cmake_minimum_required(VERSION 3.27)
project(Microrecif)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Microrecif
        constantes.h
        lifeform.cc
        lifeform.h
        message.cc
        message.h
        projet.cc
        shape.cc
        shape.h
        simulation.cc
        simulation.h)
