# We might support earlier versions, too, but they are not tested.
cmake_minimum_required (VERSION 3.8)

# binary for Config creation test
project(test_names_to_ctrl_seqs)

set(test_names_to_ctrl_seqs_src
    ../../code/ConsoleColours.cpp
    names_to_controlsequences.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -O3 -fexceptions)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_names_to_ctrl_seqs ${test_names_to_ctrl_seqs_src})

# --- add it as a test
add_test(NAME func_nameToControlSequence
         COMMAND $<TARGET_FILE:test_names_to_ctrl_seqs>)
