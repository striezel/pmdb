# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for tests of SimpleBBCode class
project(simple_bbcode_test)

set(simple_bbcode_test_src
    ../../bbcode/SimpleBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    simple_bbcode_test.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(simple_bbcode_test ${simple_bbcode_test_src})

# --- add it as a test
add_test(NAME class_SimpleBBCode
         COMMAND $<TARGET_FILE:simple_bbcode_test>)


# binary for tests of CustomizedSimpleBBCode class
project(customized_simple_bbcode_test)

set(customized_simple_bbcode_test_src
    ../../bbcode/CustomizedSimpleBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    customized_simple_bbcode_test.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(customized_simple_bbcode_test ${customized_simple_bbcode_test_src})

# --- add it as a test
add_test(NAME class_CustomizedSimpleBBCode
         COMMAND $<TARGET_FILE:customized_simple_bbcode_test>)
