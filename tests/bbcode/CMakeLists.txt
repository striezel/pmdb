# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for Config creation test
project(simple_bbcode_test)

set(simple_bbcode_test_src
    ../../MsgTemplate.cpp
    ../../bbcode/BBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    simple_bbcode_test.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(simple_bbcode_test ${simple_bbcode_test_src})

# --- add it as a test
add_test(NAME test_simple_bbcode
         COMMAND $<TARGET_FILE:simple_bbcode_test>)
