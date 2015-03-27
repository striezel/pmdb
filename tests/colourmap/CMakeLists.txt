# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for Config creation test
project(colourmap_test_load)

set(colourmap_test_load_src
    ../../code/ColourMap.cpp
    ../../code/ConsoleColours.cpp
    colourmap_load.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(colourmap_test_load ${colourmap_test_load_src})

# --- add it as a test
add_test(NAME class_ColourMap_load
         COMMAND $<TARGET_FILE:colourmap_test_load> ${CMAKE_CURRENT_SOURCE_DIR})
