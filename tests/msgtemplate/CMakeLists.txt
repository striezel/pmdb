# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for tests of SimpleBBCode class
project(msgtemplate_loadequality_test)

set(msgtemplate_loadequality_test_src
    ../../MsgTemplate.cpp
    loadFromFile_loadFromString_equality.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(msgtemplate_loadequality_test ${msgtemplate_loadequality_test_src})

# --- add it as a test
add_test(NAME class_MsgTemplate_loadequality
         COMMAND $<TARGET_FILE:msgtemplate_loadequality_test>)
