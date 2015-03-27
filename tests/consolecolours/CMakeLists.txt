# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for Config creation test
project(test_names_to_ctrl_seqs)

set(test_names_to_ctrl_seqs_src
    ../../code/ConsoleColours.cpp
    names_to_controlsequences.cpp)

#if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions -std=c++0x)
#endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_names_to_ctrl_seqs ${test_names_to_ctrl_seqs_src})

# --- add it as a test
add_test(NAME func_nameToControlSequence
         COMMAND $<TARGET_FILE:test_names_to_ctrl_seqs>)
