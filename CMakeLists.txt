cmake_minimum_required(VERSION 3.10)
project(Microrecif)

set(CMAKE_CXX_STANDARD 17)

# Find gtkmm-4.0 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(.)

# Link directories
link_directories(${GTKMM_LIBRARY_DIRS})

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
        simulation.h
        gui.cc
        gui.h
        graphic.cc
        graphic.h
        graphic_gui.h)

# Link against gtkmm libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})