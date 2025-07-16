cmake_minimum_required (VERSION 3.8...3.31)

# binary for colourmap load test
project(colourmap_test_load)

set(colourmap_test_load_src
    ../../code/ColourMap.cpp
    ../../code/ConsoleColours.cpp
    colourmap_load.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (ENABLE_SANITIZER)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

add_executable(colourmap_test_load ${colourmap_test_load_src})

# --- add it as a test
add_test(NAME class_ColourMap_load
         COMMAND $<TARGET_FILE:colourmap_test_load> ${CMAKE_CURRENT_SOURCE_DIR})
