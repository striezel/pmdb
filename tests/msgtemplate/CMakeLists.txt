# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for checking equality of templates loaded from file and
# loaded from string
project(test_msgtemplate_loadequality)

set(test_msgtemplate_loadequality_src
    ../../code/MsgTemplate.cpp
    loadFromFile_loadFromString_equality.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_msgtemplate_loadequality ${test_msgtemplate_loadequality_src})

# --- add it as a test
add_test(NAME class_MsgTemplate_loadequality
         COMMAND $<TARGET_FILE:test_msgtemplate_loadequality>)



# binary for checking some basic MsgTemplate replacement functionality
project(test_MsgTemplate_replacements)

set(test_MsgTemplate_replacements_src
    ../../code/MsgTemplate.cpp
    replacement_tests.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_MsgTemplate_replacements ${test_MsgTemplate_replacements_src})

# --- add it as a test
add_test(NAME class_MsgTemplate_replacements
         COMMAND $<TARGET_FILE:test_MsgTemplate_replacements>)
